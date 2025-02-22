package ru.itskekoff.j2c.translator.processor.cpp.impl.constant;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class ConstProcessor extends BaseProcessor {

    public ConstProcessor() {
        super(ACONST_NULL, ICONST_M1, ICONST_0,
                ICONST_1, ICONST_2, ICONST_3,
                ICONST_4, ICONST_5, LCONST_0,
                LCONST_1, FCONST_0, FCONST_1,
                FCONST_2, DCONST_0, DCONST_1);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insn, MethodNode method) {
        switch (insn.getOpcode()) {
            case ACONST_NULL -> context.output().pushMethodLine("cstack%s.l = nullptr;".formatted(context.getStackPointer().peek()));
            case ICONST_M1 -> context.output().pushMethodLine("cstack%s.i = -1;".formatted(context.getStackPointer().peek()));
            case ICONST_0 -> context.output().pushMethodLine("cstack%s.i = 0;".formatted(context.getStackPointer().peek()));
            case ICONST_1 -> context.output().pushMethodLine("cstack%s.i = 1;".formatted(context.getStackPointer().peek()));
            case ICONST_2 -> context.output().pushMethodLine("cstack%s.i = 2;".formatted(context.getStackPointer().peek()));
            case ICONST_3 -> context.output().pushMethodLine("cstack%s.i = 3;".formatted(context.getStackPointer().peek()));
            case ICONST_4 -> context.output().pushMethodLine("cstack%s.i = 4;".formatted(context.getStackPointer().peek()));
            case ICONST_5 -> context.output().pushMethodLine("cstack%s.i = 5;".formatted(context.getStackPointer().peek()));
            case LCONST_0 -> context.output().pushMethodLine("cstack%s.j = 0;".formatted(context.getStackPointer().peek()));
            case LCONST_1 -> context.output().pushMethodLine("cstack%s.j = 1;".formatted(context.getStackPointer().peek()));
            case FCONST_0 -> context.output().pushMethodLine("cstack%s.i = 0.0f;".formatted(context.getStackPointer().peek()));
            case FCONST_1 -> context.output().pushMethodLine("cstack%s.i = 1.0f;".formatted(context.getStackPointer().peek()));
            case FCONST_2 -> context.output().pushMethodLine("cstack%s.i = 2.0f;".formatted(context.getStackPointer().peek()));
            case DCONST_0 -> context.output().pushMethodLine("cstack%s.d = 0.0;".formatted(context.getStackPointer().peek()));
            case DCONST_1 -> context.output().pushMethodLine("cstack%s.d = 1.0;".formatted(context.getStackPointer().peek()));
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return switch (insnNode.getOpcode()) {
            case ACONST_NULL, FCONST_2, FCONST_1, FCONST_0, ICONST_5,
                 ICONST_4, ICONST_3, ICONST_2, ICONST_1, ICONST_0,
                 ICONST_M1 -> currentPointer + 1;
            case LCONST_0, DCONST_1, DCONST_0,
                 LCONST_1 -> currentPointer + 2;
            default -> throw new IllegalStateException("Unexpected value: " + insnNode.getOpcode());
        };
    }
}
