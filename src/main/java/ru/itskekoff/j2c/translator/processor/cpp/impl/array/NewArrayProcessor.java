package ru.itskekoff.j2c.translator.processor.cpp.impl.array;

import org.objectweb.asm.Opcodes;
import org.objectweb.asm.tree.*;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

public class NewArrayProcessor extends BaseProcessor {
    public NewArrayProcessor() {
        super(NEWARRAY, ANEWARRAY);
    }

    @Override
    public void translate(ClassContext writer, AbstractInsnNode insnNode, MethodNode method) {
        if (insnNode instanceof TypeInsnNode type) {
            if (insnNode.getOpcode() == ANEWARRAY) {

                if(writer.isNotClinit(method))
                writer.output().pushMethodLine("if (cstack%s.i < 0) throw_re(env, \"java/lang/NegativeArraySizeException\", \"ARRAYLENGTH negative\", __LINE__); else { cstack%s.l = env->NewObjectArray(cstack%s.i, classes[%s].applyDecryption(), nullptr); }"
                        .formatted(
                                writer.getStackPointer().peek() - 1,
                                writer.getStackPointer().peek() - 1,
                                writer.getStackPointer().peek() - 1,
                                writer.output().pushJavaClass(type.desc)
                        )
                );
                else
                    writer.output().pushMethodLine("if (cstack%s.i < 0) throw_re(env, \"java/lang/NegativeArraySizeException\", \"ARRAYLENGTH negative\", __LINE__); else { cstack%s.l = env->NewObjectArray(cstack%s.i, env->FindClass(\"%s\"), nullptr); }"
                        .formatted(
                                writer.getStackPointer().peek() - 1,
                                writer.getStackPointer().peek() - 1,
                                writer.getStackPointer().peek() - 1,
                                type.desc
                        )
                );

            }
        } else if (insnNode instanceof IntInsnNode intInsnNode) {
            if (insnNode.getOpcode() == NEWARRAY) {
                String arrayType = switch (intInsnNode.operand) {
                    case Opcodes.T_BOOLEAN -> "NewBooleanArray";
                    case Opcodes.T_CHAR -> "NewCharArray";
                    case Opcodes.T_FLOAT -> "NewFloatArray";
                    case Opcodes.T_DOUBLE -> "NewDoubleArray";
                    case Opcodes.T_BYTE -> "NewByteArray";
                    case Opcodes.T_SHORT -> "NewShortArray";
                    case Opcodes.T_INT -> "NewIntArray";
                    case Opcodes.T_LONG -> "NewLongArray";
                    default -> throw new IllegalArgumentException("Unsupported array type");
                };

                writer.output().pushMethodLine("    cstack%s.l = env->%s(cstack%s.i);"
                        .formatted(
                                writer.getStackPointer().peek() - 1,
                                arrayType,
                                writer.getStackPointer().peek() - 1
                        )
                );
//                writer.getStackPointer().pop();
            }
        }

    }
    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return switch (insnNode.getOpcode()) {
            case ANEWARRAY, NEWARRAY -> currentPointer;
            default -> throw new IllegalStateException("Unexpected value: " + insnNode.getOpcode());
        };
    }
}
