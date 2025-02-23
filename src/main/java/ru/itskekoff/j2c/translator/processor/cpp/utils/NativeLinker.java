package ru.itskekoff.j2c.translator.processor.cpp.utils;

import lombok.Getter;
import org.objectweb.asm.tree.ClassNode;
import org.objectweb.asm.tree.MethodNode;

import java.util.Random;

public class NativeLinker {
    @Getter
    private final ClassNode classNode;
    private int count;
    @Getter
    private final StringBuilder stringBuilder;
    private final StringBuilder methods;

    public NativeLinker(ClassNode classNode) {
        this.classNode = classNode;
        this.stringBuilder = new StringBuilder();
        this.count = 0;
        stringBuilder.append("jclass linkingClass = env->FindClass(\"%s\");\n"
                .formatted(classNode.name));

        this.methods = new StringBuilder();
        methods.append("VMProtectBeginUltra(\"clinit_%s\");\n".formatted(
                String.valueOf(new Random().nextLong()*new Random().nextLong()+new Random().nextLong()+new Random().nextLong())
        ));
        methods.append("    JNINativeMethod jniMethods[] = {\n");
    }

    public void pushMethod(MethodNode methodNode, String nativeName) {
        count++;
        methods.append("        (char*)\"%s\", (char*)\"%s\", &Java_%s,\n"
                .formatted(methodNode.name, methodNode.desc, nativeName));
    }

    public void end() {
        if (count > 0) {
            methods.append("    };\n");
            methods.append("    env->RegisterNatives(env->FindClass(\"%s\"), jniMethods, sizeof(jniMethods) / sizeof(JNINativeMethod));\n"
                    .formatted(classNode.name));
            methods.append("    VMProtectEnd();\n");
        }
    }

    public StringBuilder getMethods() {
        return (count != 0 ? new StringBuilder(methods.toString()) : new StringBuilder());
    }

}
