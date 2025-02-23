package ru.itskekoff.j2c.translator.processor.cpp.impl.condition;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.JumpInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

public class ConditionProcessor extends BaseProcessor {
    public ConditionProcessor() {
        super(IFEQ, IFNE, IFLT, IFGE,
                IFGT, IFLE, IF_ICMPEQ, IF_ICMPNE,
                IF_ICMPLT, IF_ICMPGE, IF_ICMPGT, IF_ICMPLE,
                IF_ACMPEQ, IF_ACMPNE);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insnNode, MethodNode method) {
        if (insnNode instanceof JumpInsnNode jumpNode) {
            int instruction = insnNode.getOpcode();
            int stackPointer = context.getStackPointer().peek();

            switch (instruction) {
                case IFEQ -> context.output().pushMethodLine("if (cstack%s.i == 0) goto %s;"
                        .formatted(stackPointer - 1, context.getLabelPool().getName(jumpNode.label.getLabel())));
                case IFNE -> context.output().pushMethodLine("if (cstack%s.i != 0) goto %s;"
                        .formatted(stackPointer - 1, context.getLabelPool().getName(jumpNode.label.getLabel())));
                case IFLT -> context.output().pushMethodLine("if (cstack%s.i < 0) goto %s;"
                        .formatted(stackPointer - 1, context.getLabelPool().getName(jumpNode.label.getLabel())));
                case IFLE -> context.output().pushMethodLine("if (cstack%s.i <= 0) goto %s;"
                        .formatted(stackPointer - 1, context.getLabelPool().getName(jumpNode.label.getLabel())));
                case IFGT -> context.output().pushMethodLine("if (cstack%s.i > 0) goto %s;"
                        .formatted(stackPointer - 1, context.getLabelPool().getName(jumpNode.label.getLabel())));
                case IFGE -> context.output().pushMethodLine("if (cstack%s.i >= 0) goto %s;"
                        .formatted(stackPointer - 1, context.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ICMPEQ -> context.output().pushMethodLine("if (cstack%s.i == cstack%s.i) goto %s;"
                        .formatted(stackPointer - 2, stackPointer - 1, context.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ICMPNE -> context.output().pushMethodLine("if (cstack%s.i != cstack%s.i) goto %s;"
                        .formatted(stackPointer - 2, stackPointer - 1, context.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ICMPLT -> context.output().pushMethodLine("if (cstack%s.i < cstack%s.i) goto %s;"
                        .formatted(stackPointer - 2, stackPointer - 1, context.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ICMPLE -> context.output().pushMethodLine("if (cstack%s.i <= cstack%s.i) goto %s;"
                        .formatted(stackPointer - 2, stackPointer - 1, context.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ICMPGT -> context.output().pushMethodLine("if (cstack%s.i > cstack%s.i) goto %s;"
                        .formatted(stackPointer - 2, stackPointer - 1, context.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ICMPGE -> context.output().pushMethodLine("if (cstack%s.i >= cstack%s.i) goto %s;"
                        .formatted(stackPointer - 2, stackPointer - 1, context.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ACMPEQ ->
                        context.output().pushMethodLine("if (env->IsSameObject(cstack%s.l, cstack%s.l)) goto %s;"
                                .formatted(stackPointer - 2, stackPointer - 1, context.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ACMPNE ->
                        context.output().pushMethodLine("if (!env->IsSameObject(cstack%s.l, cstack%s.l)) goto %s;"
                                .formatted(stackPointer - 2, stackPointer - 1, context.getLabelPool().getName(jumpNode.label.getLabel())));
                default -> throw new IllegalStateException("Unsupported instruction.");
            }

        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return switch (insnNode.getOpcode()) {
            case IFEQ, IFNE, IFLT, IFGE, IFGT, IFLE -> currentPointer - 1;
            case IF_ICMPEQ, IF_ICMPNE, IF_ICMPLT, IF_ICMPLE, IF_ICMPGT, IF_ICMPGE, IF_ACMPEQ, IF_ACMPNE ->
                    currentPointer - 2;
            default -> throw new IllegalStateException("Unexpected value: " + insnNode.getOpcode());
        };
    }
}
