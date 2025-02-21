package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.InsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class MathProcessor extends BaseProcessor {

    public MathProcessor() {
        super(IADD, LADD, FADD,
                DADD, ISUB, LSUB,
                FSUB, DSUB, IMUL,
                LMUL, FMUL, DMUL,
                INEG, LNEG, FNEG,
                DNEG, ISHL, LSHL,
                ISHR, LSHR, IUSHR,
                LUSHR, IAND, LAND,
                IOR, LOR, IXOR,
                LXOR, LCMP, DCMPG, FCMPG,
                DCMPL, FCMPL, FREM,
                DREM, IREM, LREM,
                FDIV, DDIV, IDIV,
                LDIV
        );
    }

    @Override
    public void translate(ClassContext writer, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof InsnNode) {
            switch (insn.getOpcode()) {
                case IADD -> writer.output().pushMethodLine("cstack%s.i = cstack%s.i + cstack%s.i;"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1)
                );
                case LADD -> writer.output().pushMethodLine("cstack%s.j = cstack%s.j + cstack%s.j;"
                        .formatted(writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 2)
                );
                case FCMPG ->  {
                    writer.output().pushString("\n{ jfloat value1 = cstack%s.f; jfloat value2 = cstack%s.f; cstack%s.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1)); }"
                            .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1, writer.getStackPointer().peek() - 2));
//                    writer.output().pushMethodBlock(
//                        "jfloat value1 = cstack%s.f;".formatted(writer.getStackPointer().peek() - 2),
//                        "jfloat value2 = cstack%s.f;".formatted(writer.getStackPointer().peek() - 1),
//                        "cstack%s.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));".formatted(writer.getStackPointer().peek() - 2)
//                );
                }
                case FCMPL -> writer.output().pushString("\n{ jfloat value1 = cstack%s.f; jfloat value2 = cstack%s.f; cstack%s.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : -1)); }"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1, writer.getStackPointer().peek() - 2));// -2, -1, -2,
//                case FCMPL -> writer.output().pushMethodBlock(
//                        "jfloat value1 = cstack%s.f;".formatted(writer.getStackPointer().peek() - 2),
//                        "jfloat value2 = cstack%s.f;".formatted(writer.getStackPointer().peek() - 1),
//                        "cstack%s.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : -1));".formatted(writer.getStackPointer().peek() - 2)
//                );
                case FADD -> writer.output().pushMethodLine("cstack%s.f = cstack%s.f + cstack%s.f;"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1)
                );
                case DADD -> writer.output().pushMethodLine("cstack%s.d = cstack%s.d + cstack%s.d;"
                        .formatted(writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 2)
                );
                case ISUB -> writer.output().pushMethodLine("cstack%s.i = cstack%s.i - cstack%s.i;"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1)
                );
                case LSUB -> writer.output().pushMethodLine("cstack%s.j = cstack%s.j - cstack%s.j;"
                        .formatted(writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 2)
                );
                case FSUB -> writer.output().pushMethodLine("cstack%s.f = cstack%s.f - cstack%s.f;"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1)
                );
                case DSUB -> writer.output().pushMethodLine("cstack%s.d = cstack%s.d - cstack%s.d;"
                        .formatted(writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 2)
                );
                case IMUL -> writer.output().pushMethodLine("cstack%s.i = cstack%s.i * cstack%s.i;"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1)
                );
                case LMUL -> writer.output().pushMethodLine("cstack%s.j = cstack%s.j * cstack%s.j;"
                        .formatted(writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 2)
                );
                case FMUL -> writer.output().pushMethodLine("cstack%s.f = cstack%s.f * cstack%s.f;"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1)
                );
                case DMUL -> writer.output().pushMethodLine("cstack%s.d = cstack%s.d * cstack%s.d;"
                        .formatted(writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 2)
                );
                case INEG -> writer.output().pushMethodLine("cstack%s.i = -cstack%s.i;"
                        .formatted(writer.getStackPointer().peek() - 1, writer.getStackPointer().peek() - 1)
                );
                case LNEG -> writer.output().pushMethodLine("cstack%s.j = -cstack%s.j;"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2)
                );
                case FNEG -> writer.output().pushMethodLine("cstack%s.f = -cstack%s.f;"
                        .formatted(writer.getStackPointer().peek() - 1, writer.getStackPointer().peek() - 1)
                );
                case DNEG -> writer.output().pushMethodLine("cstack%s.d = -cstack%s.d;"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2)
                );
                case ISHL -> writer.output().pushMethodLine("cstack%s.i = cstack%s.i << (0x1f & cstack%s.i);"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1));
                case LSHL -> writer.output().pushMethodLine("cstack%s.j = cstack%s.j << (0x3f & cstack%s.j);"
                        .formatted(writer.getStackPointer().peek() - 3, writer.getStackPointer().peek() - 3, writer.getStackPointer().peek() - 1)
                );
                case ISHR -> writer.output().pushMethodLine("cstack%s.i = cstack%s.i >> (0x1f & cstack%s.i);"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1)
                );
                case LSHR -> writer.output().pushMethodLine("cstack%s.j = cstack%s.j >> (0x3f & cstack%s.j);"
                        .formatted(writer.getStackPointer().peek() - 3, writer.getStackPointer().peek() - 3, writer.getStackPointer().peek() - 1)
                );
                case IUSHR -> writer.output().pushMethodLine("cstack%s.i = (jint) (((uint32_t) cstack%s.i) >> (((uint32_t) cstack%s.i) & 0x1f));"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1)
                );
                case LUSHR -> writer.output().pushMethodLine("cstack%s.j = (jlong) (((uint64_t) cstack%s.j) >> (((uint64_t) cstack%s.j) & 0x3f));"
                        .formatted(writer.getStackPointer().peek() - 3, writer.getStackPointer().peek() - 3, writer.getStackPointer().peek() - 1)
                );
                case IAND -> writer.output().pushMethodLine("cstack%s.i = cstack%s.i & cstack%s.i;"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1)
                );
                case LAND -> writer.output().pushMethodLine("cstack%s.j = cstack%s.j & cstack%s.j;"
                        .formatted(writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 2)
                );
                case IOR -> writer.output().pushMethodLine("cstack%s.i = cstack%s.i | cstack%s.i;"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1)
                );
                case LOR -> writer.output().pushMethodLine("cstack%s.j = cstack%s.j | cstack%s.j;"
                        .formatted(writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 2)
                );
                case IXOR -> writer.output().pushMethodLine("cstack%s.i = cstack%s.i ^ cstack%s.i;"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1)
                );
                case LXOR -> writer.output().pushMethodLine("cstack%s.j = cstack%s.j ^ cstack%s.j;"
                        .formatted(writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 2)
                );
                case DCMPL -> writer.output().pushMethodBlock(
                        "jdouble value1 = cstack%s.d;"
                                .formatted(writer.getStackPointer().peek() - 4),
                        "jdouble value2 = cstack%s.d;"
                                .formatted(writer.getStackPointer().peek() - 2),
                        "cstack%s.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : -1));"
                                .formatted(writer.getStackPointer().peek() - 4)
                );
                case DCMPG -> writer.output().pushMethodBlock(
                        "jdouble value1 = cstack%s.d;".formatted(writer.getStackPointer().peek() - 4),
                        "jdouble value2 = cstack%s.d;".formatted(writer.getStackPointer().peek() - 2),
                        "cstack%s.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));".formatted(writer.getStackPointer().peek() - 4)
                );
                case LCMP -> writer.output().pushMethodLine("cstack%s.i = (cstack%s.j == cstack%s.j) ? 0 : (cstack%s.j > cstack%s.j ? 1 : -1);"
                        .formatted(writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 4,
                                writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 2)
                );
                case LREM -> writer.output().pushMethodLine("cstack%s.j = (cstack%s.j %% cstack%s.j);"
                        .formatted(writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 2
                        )
                );
                case IREM -> {
                    int divisorIndex = writer.getStackPointer().peek() - 1;
                    int dividendIndex = writer.getStackPointer().peek() - 2;
                    writer.output().pushMethodLine("cstack%s.i = cstack%s.i %% cstack%s.i;".formatted(dividendIndex, dividendIndex, divisorIndex));
                }
                case FREM -> writer.output().pushMethodLine("cstack%s.f = std::fmod(cstack%s.f, cstack%s.f);"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1)
                );
                case DREM -> writer.output().pushMethodLine("cstack%s.d = std::fmod(cstack%s.d, cstack%s.d);"
                        .formatted(writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 2)
                );
                case FDIV -> writer.output().pushMethodLine("cstack%s.f = cstack%s.f / cstack%s.f;"
                        .formatted(writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 2, writer.getStackPointer().peek() - 1)
                );
                case IDIV -> {
                    int divisorIndex = writer.getStackPointer().peek() - 1;
                    int dividendIndex = writer.getStackPointer().peek() - 2;

                    writer.output().pushMethodLine("cstack%s.i = cstack%s.i / cstack%s.i;"
                            .formatted(dividendIndex, dividendIndex, divisorIndex)
                    );
                }

                case LDIV -> writer.output().pushMethodLine("cstack%s.j = cstack%s.j / cstack%s.j;"
                        .formatted(writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 2)
                );
                case DDIV -> writer.output().pushMethodLine("cstack%s.d = cstack%s.d / cstack%s.d;"
                        .formatted(writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 4, writer.getStackPointer().peek() - 2)
                );
            }
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return switch (insnNode.getOpcode()) {
            case IADD, FADD, FSUB, FCMPG, FCMPL, IXOR, IOR, IAND,
                 LUSHR, IUSHR, LSHR, LSHL, ISHR, ISHL, FREM, IREM,
                 FDIV, IDIV, FMUL, IMUL, ISUB -> currentPointer - 1;
            case DCMPG, DCMPL, LCMP -> currentPointer - 3;
            case LXOR, LOR, LAND, DREM, LREM, DDIV, LDIV, DMUL,
                 LMUL, DSUB, LSUB, DADD, LADD -> currentPointer - 2;
            case LNEG, INEG, DNEG, FNEG -> currentPointer;
            default -> throw new IllegalStateException("Unexpected value: " + insnNode.getOpcode());
        };
    }
}