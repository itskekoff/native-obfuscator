package ru.itskekoff.j2c.translator.processor.cpp.impl.array;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.InsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

public class ArrayLengthProcessor extends BaseProcessor {
    public ArrayLengthProcessor() {
        super(ARRAYLENGTH);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof InsnNode) {
            if (insn.getOpcode() == ARRAYLENGTH) {
                context.output().pushMethodLine("cstack%s.i = (env)->GetArrayLength((jarray)cstack%s.l);"
                        .formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1));
            }
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return currentPointer;
    }
}
