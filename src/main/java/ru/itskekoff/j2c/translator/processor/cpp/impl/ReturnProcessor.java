package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.Type;
import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.InsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;
import ru.itskekoff.j2c.translator.processor.cpp.MethodProcessor;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.stack.StackPointer;

public class ReturnProcessor extends BaseProcessor {
    public ReturnProcessor() {
        super(RETURN, ARETURN, DRETURN,
                FRETURN, LRETURN, IRETURN
        );
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insnNode, MethodNode method) {
        if (insnNode instanceof InsnNode) {
            Type ret = Type.getReturnType(method.desc);
            String returnType = MethodProcessor.CPP_TYPES[ret.getSort()];

            switch (insnNode.getOpcode()) {
                case IRETURN, DRETURN, ARETURN, FRETURN, LRETURN -> {
                    String stackAccess = getStackAccess(context, insnNode.getOpcode());
                    context.output().pushMethodLine("return (%s) %s;".formatted(returnType, stackAccess));
                }
                case RETURN -> context.output().pushMethodLine("return;");
            }
        }
    }

    private String getStackAccess(MethodContext methodContext, int opcode) {
        StackPointer stackPointer = methodContext.getStackPointer();
        return switch (opcode) {
            case IRETURN, ARETURN, FRETURN ->
                    "cstack%s.%s".formatted(stackPointer.peek() - 1, getPrimitiveType(opcode));
            case DRETURN, LRETURN -> "cstack%s.%s".formatted(stackPointer.peek() - 2, getPrimitiveType(opcode));
            default -> throw new IllegalArgumentException("Unsupported opcode: " + opcode);
        };
    }

    private String getPrimitiveType(int opcode) {
        return switch (opcode) {
            case IRETURN -> "i";
            case DRETURN -> "d";
            case ARETURN -> "l";
            case FRETURN -> "f";
            case LRETURN -> "j";
            default -> throw new IllegalArgumentException("Unsupported opcode: " + opcode);
        };
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return switch (insnNode.getOpcode()) {
            case IRETURN, FRETURN, ARETURN -> currentPointer - 1;
            case DRETURN, LRETURN -> currentPointer - 2;
            case RETURN -> currentPointer;
            default -> throw new IllegalStateException("Unexpected value: " + insnNode.getOpcode());
        };
    }
}
