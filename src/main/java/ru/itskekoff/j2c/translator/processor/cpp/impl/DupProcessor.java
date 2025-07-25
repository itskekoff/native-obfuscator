package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.InsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class DupProcessor extends BaseProcessor {
    public DupProcessor() {
        super(DUP, DUP_X1, DUP_X2,
                DUP2, DUP2_X1, DUP2_X2
        );
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof InsnNode) {
            int topIndex = context.getStackPointer().peek();
            switch (insn.getOpcode()) {
                case DUP -> context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex, topIndex - 1));
                case DUP_X1 -> {
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex, topIndex - 1));
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex - 1, topIndex - 2));
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex - 2, topIndex));
                }
                case DUP_X2 -> {
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex, topIndex - 1));
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex - 1, topIndex - 2));
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex - 2, topIndex - 3));
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex - 3, topIndex));
                }
                case DUP2 -> {
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex, topIndex - 2));
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex + 1, topIndex - 1));
                }
                case DUP2_X1 -> {
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex, topIndex - 2));
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex + 1, topIndex - 1));
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex - 1, topIndex - 3));
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex - 2, topIndex + 1));
                }
                case DUP2_X2 -> {
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex, topIndex - 2));
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex + 1, topIndex - 1));
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex - 1, topIndex - 3));
                    context.output().pushMethodLine("cstack%s = cstack%s;".formatted(topIndex - 2, topIndex - 4));
                }
            }
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return switch (insnNode.getOpcode()) {
            case DUP, DUP_X1, DUP_X2 -> currentPointer + 1;
            case DUP2, DUP2_X1, DUP2_X2 -> currentPointer + 2;
            default -> throw new IllegalStateException("Unexpected value: " + insnNode.getOpcode());
        };
    }
}