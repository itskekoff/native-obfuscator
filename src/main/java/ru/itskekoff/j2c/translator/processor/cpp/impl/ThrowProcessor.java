package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.InsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

public class ThrowProcessor extends BaseProcessor {
    public ThrowProcessor() {
        super(ATHROW);
    }

    private int tempIndex = 0;

    @Override
    public void translate(ClassContext writer, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof InsnNode) {
            String fieldAddition = "tempThrow_%d".formatted(tempIndex++);
            writer.output().pushMethodLine("jclass %s = env->FindClass(\"%s\");".formatted(fieldAddition, writer.classNode.name));
            writer.output().pushMethodLine("if (cstack%s.l == nullptr) env->ThrowNew(%s, \"\"); else env->Throw((jthrowable) cstack%s.l);"
                    .formatted(writer.getStackPointer().peek() - 1, fieldAddition, writer.getStackPointer().peek() - 1));
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer - 1;
    }
}
