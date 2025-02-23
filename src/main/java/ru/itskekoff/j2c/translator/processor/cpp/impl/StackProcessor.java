package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.IntInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;

public class StackProcessor extends BaseProcessor {

    public StackProcessor() {
        super(BIPUSH, SIPUSH, NOP, POP, POP2);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insn, MethodNode method) {
        int instruction = insn.getOpcode();

        if ((instruction == BIPUSH || instruction == SIPUSH) && insn instanceof IntInsnNode intInsnNode) {
            String val = String.valueOf(intInsnNode.operand);
            context.output().pushMethodLine("cstack%d.i = (jint) %s;".formatted(context.getStackPointer().peek(), val));
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return switch (insnNode.getOpcode()) {
            case POP -> currentPointer - 1;
            case POP2 -> currentPointer - 2;
            case BIPUSH, SIPUSH -> currentPointer + 1;
            case NOP -> currentPointer;
            default -> throw new RuntimeException("PIZDEC??");
        };
    }
}
