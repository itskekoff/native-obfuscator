package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.InsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

public class ConvertProcessor extends BaseProcessor {
    public ConvertProcessor() {
        super(I2L, I2F, I2D,
                L2I, L2F, L2D,
                F2I, F2L, F2D,
                D2I, D2L, D2F,
                I2B, I2C, I2S
        );
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof InsnNode) {
            switch (insn.getOpcode()) {
                case I2L -> context.output().pushMethodLine("cstack%s.j = cstack%s.i;".
                        formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1));
                case I2F -> context.output().pushMethodLine("cstack%s.f = (jfloat)  cstack%s.i;".
                        formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1));
                case I2D -> context.output().pushMethodLine("cstack%s.d = (jdouble) cstack%s.i;"
                        .formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1));
                case L2I -> context.output().pushMethodLine("cstack%s.i = (jint) cstack%s.j;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2));
                case L2F -> context.output().pushMethodLine("cstack%s.f = (jfloat) cstack%s.j;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2));
                case L2D -> context.output().pushMethodLine("cstack%s.d = (jdouble) cstack%s.j;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2));
                case F2I -> context.output().pushMethodLine("cstack%s.i = (jint) cstack%s.f;"
                        .formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1));
                case F2L -> context.output().pushMethodLine("cstack%s.j = (jlong) cstack%s.f;"
                        .formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1));
                case F2D -> context.output().pushMethodLine("cstack%s.d = (jdouble) cstack%s.f;"
                        .formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1));
                case D2I -> context.output().pushMethodLine("cstack%s.i = (jint) cstack%s.d;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2));
                case D2L -> context.output().pushMethodLine("cstack%s.j = (jlong) cstack%s.d;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2));
                case D2F -> context.output().pushMethodLine("cstack%s.f = (jfloat) cstack%s.d;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2));
                case I2B -> context.output().pushMethodLine("cstack%s.i = (jint) (jbyte) cstack%s.i;"
                        .formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1));
                case I2C -> context.output().pushMethodLine("cstack%s.i = (jint) (jchar) cstack%s.i;"
                        .formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1));
                case I2S -> context.output().pushMethodLine("cstack%s.i = (jint) (jshort) cstack%s.i;"
                        .formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1));
            }
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return switch (insnNode.getOpcode()) {
            case I2S, I2C, I2B, D2L, F2I, L2D, I2F -> currentPointer;
            case F2D, F2L, I2D, I2L -> currentPointer + 1;
            case D2F, D2I, L2F, L2I -> currentPointer -1;
            default -> throw new IllegalStateException("Unexpected value: " + insnNode.getOpcode());
        };
    }
}
