package ru.itskekoff.j2c.translator.processor.cpp.utils.translate;

import lombok.Getter;
import org.objectweb.asm.Label;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.tree.ClassNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceNode;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceSnippetGenerator;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceTable;
import ru.itskekoff.j2c.translator.utils.BaseUtils;

import java.util.*;
import java.util.concurrent.CopyOnWriteArrayList;

@Getter
public class MethodContext {
    private final LabelPool labelPool = new LabelPool();
    private final StackPointer stackPointer = new StackPointer();
    private final List<Integer> locals = new ArrayList<>();
    private final ClassNode classNode;
    private final ContextBuilder contextBuilder;

    public MethodContext(ClassNode classNode) {
        this.classNode = classNode;
        this.contextBuilder = new ContextBuilder(this.classNode);
    }

    public ContextBuilder output() {
        return this.contextBuilder;
    }

    public boolean notClinit(MethodNode methodNode) {
        return !methodNode.name.contains("$Clinit");
    }

    @SuppressWarnings("all")
    public static class ContextBuilder {
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

            //not found, we should allocate

            ReferenceNode referenceNode = new ReferenceNode(klassName, "NULL", "NULL", true, index);


            this.classReferenceBuilder.append("    classes[%s] = (jclass)std::stoll(request(std::format(\"http://localhost:6555/decrypt?value={}&seed=%s&rtdsc={}\", ((__int64)env->NewGlobalRef(env->FindClass(\"%s\")) ^ %s), rtdsc)));\n"
                    .formatted(index,
                            referenceNode.getSeed(),
                            klassName,
                            referenceNode.getClinit()));

            classes.add(referenceNode);

            return referenceNode;
        }

        private ReferenceNode referenceNodeAllocationTemplate(MethodContext context, String className, String name, String signature, boolean isStatic, List<ReferenceNode> references, StringBuilder referenceStringBuilder, String arrayName, String castType, int index, MethodNode methodNode) {
            for (ReferenceNode referenceNode : references) {
                if (referenceNode.getClassName().equals(className)) {
                    if (referenceNode.getName().equals(name)) {
                        if (referenceNode.getSignature().equals(signature)) {
                            if (referenceNode.isStaticVal() == isStatic) {
                                return referenceNode;
                            }
                        }
                    }
                }
            }

            //not found, we should allocate

            ReferenceNode referenceNode = new ReferenceNode(className, name, signature, isStatic, index);

            referenceStringBuilder.append("    %ss[%s] = (%s)std::stoll(request(std::format(\"http://localhost:6555/decrypt?value={}&seed=%s&rtdsc={}\", ((__int64)env->Get%s%sID(%s, \"%s\", \"%s\") ^ %s), rtdsc)));\n"
                    .formatted(
                            arrayName,
                            referenceNode.getId(),
                            castType,
                            referenceNode.getSeed(),
                            referenceNode.isStatic(),
                            Character.toUpperCase(arrayName.charAt(0)) + arrayName.substring(1),
                            ReferenceSnippetGenerator.generateJavaClassReference(context, methodNode, className),
                            referenceNode.getName(),
                            referenceNode.getSignature(),
                            referenceNode.getClinit(),
                            arrayName,
                            castType
                    ));

            getMethods().add(referenceNode);

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
            return "%s\n%s".formatted("/* METHOD REFERENCE TABLE */", this.fieldReferenceBuilder.toString());
        }

        public String getMethodReferences() {
            return "%s\n%s".formatted("/* METHOD REFERENCE TABLE */", this.methodReferenceBuilder.toString());
        }
    }

    public static class StackPointer {
        private int pointer;

        public StackPointer() {
            this.pointer = 0;
        }

        public StackPointer push(int... count) {
            pointer += (count.length > 0) ? count[0] : 1;
            return this;
        }

        public StackPointer pop(int... count) {
            pointer -= (count.length > 0) ? count[0] : 1;
            return this;
        }

        public int peek() {
            return this.pointer;
        }

        public void set(int pointer) {
            this.pointer = pointer;
        }
    }

    public static class LabelPool {

        private final WeakHashMap<Label, Long> labels = new WeakHashMap<>();
        private long currentIndex = 0;

        public String getName(Label label) {
            return "L" + this.labels.computeIfAbsent(label, _ -> ++currentIndex);
        }

        public void clear() {
            this.labels.clear();
            this.currentIndex = 0;
        }
    }
}