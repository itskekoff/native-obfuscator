package ru.itskekoff.j2c.translator.processor.cpp.utils.translate;

import lombok.Getter;
import org.objectweb.asm.Label;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.tree.ClassNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceNode;
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
        private Map<String, Integer> classes = new HashMap<>();
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


        public int pushJavaClass(String klassName) {
            if (this.classes.containsKey(klassName)) {
                return this.classes.get(klassName);
            }
            int index = ReferenceTable.getClassIndex();
            this.classes.put(klassName, index);
            ReferenceTable.pushClass(klassName, index);

            this.classReferenceBuilder.append("    classes[%s] = RBM((jobject)((__int64)env->NewGlobalRef((jclass)(((__int64)env->FindClass(\"%s\"))))));"
                    .formatted(index, klassName));
            this.classReferenceBuilder.append("\n");
            return index;
        }

        public ReferenceNode allocateOrGetFieldNode(String className, String name, String signature, boolean isStatic) {

            for (ReferenceNode referenceNode : getFields()) {
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

            ReferenceNode referenceNode = new ReferenceNode(className, name, signature, isStatic, ReferenceTable.getFieldIndex());

            methodReferenceBuilder.append("methods[%s] = (jmethodID)std::stoll(request(std::format(\"http://localhost:6555/decrypt?value={}&seed=%s&rtdsc={}\", ((__int64)env->Get%sMethodID(env->FindClass(\"%s\"), \"%s\", \"%s\") ^ %s), rtdsc)));\n"
                    .formatted(referenceNode.getId(),
                            referenceNode.getSeed(),
                            referenceNode.isStatic(),
                            referenceNode.getClassName(),
                            referenceNode.getName(),
                            referenceNode.getSignature(),
                            referenceNode.getClinit()
                    ));

            getFields().add(referenceNode);

            return referenceNode;
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