package ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context;

import lombok.Getter;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.tree.AnnotationNode;
import org.objectweb.asm.tree.ClassNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.annotations.vmp.VMProtectType;
import ru.itskekoff.j2c.translator.configuration.TranslatorConfiguration;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceNode;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceSnippetGenerator;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceTable;
import ru.itskekoff.j2c.translator.utils.BaseUtils;
import ru.itskekoff.j2c.translator.utils.clazz.parser.ClassFilter;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.Random;
import java.util.concurrent.CopyOnWriteArrayList;

import static ru.itskekoff.j2c.translator.utils.clazz.parser.ClassFilter.hasNativeAnnotation;

/**
 * @author itskekoff
 * @since 14:01 of 23.02.2025
 */
@SuppressWarnings("all")
public class ContextBuilder {
    private final StringBuilder output = new StringBuilder();
    private final StringBuilder classReferenceBuilder = new StringBuilder();
    private final StringBuilder methodReferenceBuilder = new StringBuilder();
    private final StringBuilder fieldReferenceBuilder = new StringBuilder();

    private List<ReferenceNode> classes = new CopyOnWriteArrayList<>();
    private @Getter List<ReferenceNode> methods = new CopyOnWriteArrayList<>();
    private @Getter List<ReferenceNode> fields = new CopyOnWriteArrayList<>();

    private final ClassNode classNode;

    public ContextBuilder(ClassNode classNode) {
        this.classNode = classNode;
        this.pushLine().pushString("// %s".formatted(classNode.name));
    }


    public void begin(MethodNode methodNode) {
        if (isMarkedForProtection(methodNode)) {
            findVMProtectAnnotation(methodNode).ifPresent(annotationNode -> {
                VMProtectType vmProtectType = extractVMProtectType(annotationNode);
                if (vmProtectType != null) {
                    String protectionBeginCode = generateVMProtectBeginCode(vmProtectType);
                    pushMethodLine(protectionBeginCode);
                }
            });
        }
    }

    public void end(MethodNode methodNode) {
        if (isMarkedForProtection(methodNode)) {
            findVMProtectAnnotation(methodNode).ifPresent(annotationNode -> {
                VMProtectType vmProtectType = extractVMProtectType(annotationNode);
                if (vmProtectType != null) {
                    pushMethodLine("VMProtectEnd();");
                }
            });
        }
    }

    public void pushMethod(MethodNode methodNode, String nativeName,
                           List<String> argNames, String[] CPP_TYPES, Type[] args,
                           boolean clinit, boolean isStatic) {
        this.pushLine().pushString("%s%s JNICALL  Java_%s(JNIEnv *env, "
                .formatted(clinit ? "extern \"C\" JNIEXPORT " : "",
                        CPP_TYPES[Type.getReturnType(methodNode.desc).getSort()],
                        nativeName)
        );

        this.pushString(BaseUtils.getFlag(methodNode.access, Opcodes.ACC_STATIC) ? "jclass clazz" : "jobject obj");

        for (int i = 0; i < args.length; ++i) {
            this.pushString(", %s arg%d".formatted(CPP_TYPES[args[i].getSort()], i));
        }
        this.pushString(") {");
    }


    public ReferenceNode pushJavaClass(String klassName) {

        for (ReferenceNode referenceNode : classes) {
            if (referenceNode.getClassName().equals(klassName)) {
                return referenceNode;
            }
        }

        int index = ReferenceTable.getClassIndex();
        ReferenceTable.pushClass(klassName, index);

        ReferenceNode referenceNode = new ReferenceNode(klassName, "NULL", "NULL", true, index);

        this.classReferenceBuilder.append("    classes[%s] = (jclass)std::stoll(request(std::format(\"%s?value={}&seed=%s&rtdsc={}\", ((__int64)env->NewGlobalRef(env->FindClass(\"%s\")) ^ %s), rtdsc)));\n"
                .formatted(index, TranslatorConfiguration.IMP.REFERENCE.REFERENCE_URL, referenceNode.getInitialSeed(), klassName, referenceNode.getClinitKey()));

        classes.add(referenceNode);

        return referenceNode;
    }

    private ReferenceNode referenceNodeAllocationTemplate(MethodContext context, String className, String name, String signature, boolean isStatic, List<ReferenceNode> references, StringBuilder referenceStringBuilder, String arrayName, String castType, int index, MethodNode methodNode) {
        Optional<ReferenceNode> existingNode = references.stream()
                .filter(node -> node.getClassName().equals(className))
                .filter(node -> node.getName().equals(name))
                .filter(node -> node.getSignature().equals(signature))
                .filter(node -> node.isStaticVal() == isStatic)
                .findFirst();
        if (existingNode.isPresent()) return existingNode.get();

        ReferenceNode referenceNode = new ReferenceNode(className, name, signature, isStatic, index);
        referenceStringBuilder.append("    %ss[%s] = (%s)std::stoll(request(std::format(\"%s?value={}&seed=%s&rtdsc={}\", ((__int64)env->Get%s%sID(%s, (\"%s\"), (\"%s\")) ^ %s), rtdsc)));\n"
                .formatted(arrayName, referenceNode.getId(), castType,
                        TranslatorConfiguration.IMP.REFERENCE.REFERENCE_URL,
                        referenceNode.getInitialSeed(), referenceNode.isStatic(), Character.toUpperCase(arrayName.charAt(0)) + arrayName.substring(1),
                        ReferenceSnippetGenerator.generateJavaClassReference(context, methodNode, className), referenceNode.getName(),
                        referenceNode.getSignature(), referenceNode.getClinitKey(), arrayName,
                        castType
                ));

        this.methods.add(referenceNode);

        return referenceNode;
    }

    public ReferenceNode allocateOrGetMethodNode(MethodContext context,String className, String name, String signature, boolean isStatic, MethodNode methodNode) {
        return referenceNodeAllocationTemplate(context, className, name, signature, isStatic, getMethods(), methodReferenceBuilder, "method", "jmethodID", ReferenceTable.getMethodIndex(), methodNode);
    }

    public ReferenceNode allocateOrGetFieldNode(MethodContext context,String className, String name, String signature, boolean isStatic, MethodNode methodNode) {
        return referenceNodeAllocationTemplate(context, className, name, signature, isStatic, getFields(), fieldReferenceBuilder, "field", "jfieldID", ReferenceTable.getFieldIndex(), methodNode);
    }

    public ContextBuilder pushLine() {
        output.append("\n");
        return this;
    }

    public ContextBuilder pushMethodLine(String str) {
        return this.pushLine().pushTab().pushString(str);
    }

    public ContextBuilder pushMethodBlock(String... lines) {
        this.pushLine().pushTab().pushString("{");
        Arrays.stream(lines).forEach(line -> this.pushLine().pushTab(2).pushString(line));
        this.pushLine().pushTab().pushString("}");
        return this;
    }

    public ContextBuilder pushLineWithTab(int tabAmount, String str) {
        return this.pushLine().pushTab(tabAmount).pushString(str);
    }

    public ContextBuilder pushString(String str) {
        output.append(str);
        return this;
    }

    public ContextBuilder pushTab(int amount) {
        output.append("    ".repeat(amount));
        return this;
    }

    public ContextBuilder pushTab() {
        return this.pushTab(1);
    }

    public String toString() {
        return output.toString();
    }


    public String getClassReferences() {
        return "%s\n%s".formatted("/* CLASS REFERENCE TABLE */", this.classReferenceBuilder.toString());
    }

    public String getFieldReferences() {
        return "%s\n%s".formatted("/* FIELD REFERENCE TABLE */", this.fieldReferenceBuilder.toString());
    }

    public String getMethodReferences() {
        return "%s\n%s".formatted("/* METHOD REFERENCE TABLE */", this.methodReferenceBuilder.toString());
    }

    private boolean isMarkedForProtection(MethodNode methodNode) {
        return hasNativeAnnotation(classNode) || hasNativeAnnotation(methodNode);
    }

    private Optional<AnnotationNode> findVMProtectAnnotation(MethodNode methodNode) {
        return Optional.ofNullable(methodNode.visibleAnnotations)
                .flatMap(annotations -> annotations.stream()
                        .filter(annotation -> annotation.desc.equals(ClassFilter.VMPROTECT_ANNOTATION_DESC))
                        .findFirst())
                .or(() -> Optional.ofNullable(methodNode.invisibleAnnotations)
                        .flatMap(annotations -> annotations.stream()
                                .filter(annotation -> annotation.desc.equals(ClassFilter.VMPROTECT_ANNOTATION_DESC))
                                .findFirst()));
    }

    private VMProtectType extractVMProtectType(AnnotationNode annotationNode) {
        if (annotationNode.values != null) {
            for (int i = 0; i < annotationNode.values.size(); i += 2) {
                if ("type".equals(annotationNode.values.get(i))) {
                    Object typeValue = annotationNode.values.get(i + 1);
                    return VMProtectType.valueOf((String) ((String[]) typeValue)[1]);
                }
            }
        }
        return null;
    }

    private String generateVMProtectBeginCode(VMProtectType vmProtectType) {
        String formattedType = Character.toUpperCase(vmProtectType.name().charAt(0)) +
                               vmProtectType.name().substring(1).toLowerCase();
        String randomKey = String.valueOf(new Random().nextLong() * new Random().nextLong() +
                                          new Random().nextLong() + new Random().nextLong());
        return "VMProtectBegin%s(\"%s\");".formatted(formattedType, randomKey);
    }
}
