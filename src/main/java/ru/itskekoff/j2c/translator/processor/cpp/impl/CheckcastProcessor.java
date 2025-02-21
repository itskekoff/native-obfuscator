package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodNode;
import org.objectweb.asm.tree.TypeInsnNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class CheckcastProcessor extends BaseProcessor {
    public CheckcastProcessor() {
        super(CHECKCAST);
    }

    @Override
    public void translate(ClassContext writer, AbstractInsnNode insn, MethodNode method) {

        if (writer.isNotClinit(method)) {

            int classId = writer.output().pushJavaClass(((TypeInsnNode) insn).desc);

            writer.output().pushMethodLine("if (cstack%s.l != nullptr && !env->IsInstanceOf(cstack%s.l, classes[%s].applyDecryption())) {}"
                    .formatted(writer.getStackPointer().peek() - 1, writer.getStackPointer().peek() - 1, classId));

        } else {

            writer.output().pushMethodLine("if (cstack%s.l != nullptr && !env->IsInstanceOf(cstack%s.l, env->FindClass(\"%s\"))) {}"
                    .formatted(writer.getStackPointer().peek() - 1, writer.getStackPointer().peek() - 1, ((TypeInsnNode) insn).desc));

        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer;
    }
}
