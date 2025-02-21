package ru.itskekoff.j2c.translator.processor.cpp.impl.condition;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.JumpInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

public class ConditionProcessor extends BaseProcessor {
    public ConditionProcessor() {
        super(IFEQ, IFNE, IFLT, IFGE,
                IFGT, IFLE, IF_ICMPEQ, IF_ICMPNE,
                IF_ICMPLT, IF_ICMPGE, IF_ICMPGT, IF_ICMPLE,
                IF_ACMPEQ, IF_ACMPNE);
    }

    @Override
    public void translate(ClassContext classContext, AbstractInsnNode insnNode, MethodNode method) {
        if (insnNode instanceof JumpInsnNode jumpNode) {
            int instruction = insnNode.getOpcode();
            int stackPointer = classContext.getStackPointer().peek();

            switch (instruction) {
                case IFEQ -> classContext.output().pushMethodLine("if (cstack%s.i == 0) goto %s;"
                        .formatted(stackPointer - 1, classContext.getLabelPool().getName(jumpNode.label.getLabel())));
                case IFNE -> classContext.output().pushMethodLine("if (cstack%s.i != 0) goto %s;"
                        .formatted(stackPointer - 1, classContext.getLabelPool().getName(jumpNode.label.getLabel())));
                case IFLT -> classContext.output().pushMethodLine("if (cstack%s.i < 0) goto %s;"
                        .formatted(stackPointer - 1, classContext.getLabelPool().getName(jumpNode.label.getLabel())));
                case IFLE -> classContext.output().pushMethodLine("if (cstack%s.i <= 0) goto %s;"
                        .formatted(stackPointer - 1, classContext.getLabelPool().getName(jumpNode.label.getLabel())));
                case IFGT -> classContext.output().pushMethodLine("if (cstack%s.i > 0) goto %s;"
                        .formatted(stackPointer - 1, classContext.getLabelPool().getName(jumpNode.label.getLabel())));
                case IFGE -> classContext.output().pushMethodLine("if (cstack%s.i >= 0) goto %s;"
                        .formatted(stackPointer - 1, classContext.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ICMPEQ -> classContext.output().pushMethodLine("if (cstack%s.i == cstack%s.i) goto %s;"
                        .formatted(stackPointer - 2, stackPointer - 1, classContext.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ICMPNE -> classContext.output().pushMethodLine("if (cstack%s.i != cstack%s.i) goto %s;"
                        .formatted(stackPointer - 2, stackPointer - 1, classContext.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ICMPLT -> classContext.output().pushMethodLine("if (cstack%s.i < cstack%s.i) goto %s;"
                        .formatted(stackPointer - 2, stackPointer - 1, classContext.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ICMPLE -> classContext.output().pushMethodLine("if (cstack%s.i <= cstack%s.i) goto %s;"
                        .formatted(stackPointer - 2, stackPointer - 1, classContext.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ICMPGT -> classContext.output().pushMethodLine("if (cstack%s.i > cstack%s.i) goto %s;"
                        .formatted(stackPointer - 2, stackPointer - 1, classContext.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ICMPGE -> classContext.output().pushMethodLine("if (cstack%s.i >= cstack%s.i) goto %s;"
                        .formatted(stackPointer - 2, stackPointer - 1, classContext.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ACMPEQ ->
                        classContext.output().pushMethodLine("if (env->IsSameObject(cstack%s.l, cstack%s.l)) goto %s;"
                                .formatted(stackPointer - 2, stackPointer - 1, classContext.getLabelPool().getName(jumpNode.label.getLabel())));
                case IF_ACMPNE ->
                        classContext.output().pushMethodLine("if (!env->IsSameObject(cstack%s.l, cstack%s.l)) goto %s;"
                                .formatted(stackPointer - 2, stackPointer - 1, classContext.getLabelPool().getName(jumpNode.label.getLabel())));
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
