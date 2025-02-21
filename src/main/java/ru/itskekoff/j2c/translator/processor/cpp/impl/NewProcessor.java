package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodNode;
import org.objectweb.asm.tree.TypeInsnNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class NewProcessor extends BaseProcessor {
    public NewProcessor() {
        super(NEW);
    }

    @Override
    public void translate(ClassContext writer, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof TypeInsnNode node) {

            if (writer.isNotClinit(method)) {
                writer.output().pushMethodLine("if (jobject obj = env->AllocObject(classes[%s].applyDecryption())) { cstack%s.l = obj; }"
                        .formatted(writer.output().pushJavaClass(node.desc), writer.getStackPointer().peek()));
            } else {
                writer.output().pushMethodLine("if (jobject obj = env->AllocObject(env->FindClass(\"%s\"))) { cstack%s.l = obj; }"
                        .formatted(node.desc, writer.getStackPointer().peek()));
            }

        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer + 1;
    }
}
