package ru.itskekoff.j2c.translator.processor.cpp.impl;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.InsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;
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
    public void translate(MethodContext context, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof InsnNode) {
            switch (insn.getOpcode()) {
                case IADD -> context.output().pushMethodLine("cstack%s.i = cstack%s.i + cstack%s.i;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1)
                );
                case LADD -> context.output().pushMethodLine("cstack%s.j = cstack%s.j + cstack%s.j;"
                        .formatted(context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 2)
                );
                case FCMPG -> context.output().pushString("\n{ jfloat value1 = cstack%s.f; jfloat value2 = cstack%s.f; cstack%s.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1)); }"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 2));
                case FCMPL -> context.output().pushString("\n{ jfloat value1 = cstack%s.f; jfloat value2 = cstack%s.f; cstack%s.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : -1)); }"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 2));// -2, -1, -2,
                case FADD -> context.output().pushMethodLine("cstack%s.f = cstack%s.f + cstack%s.f;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1)
                );
                case DADD -> context.output().pushMethodLine("cstack%s.d = cstack%s.d + cstack%s.d;"
                        .formatted(context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 2)
                );
                case ISUB -> context.output().pushMethodLine("cstack%s.i = cstack%s.i - cstack%s.i;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1)
                );
                case LSUB -> context.output().pushMethodLine("cstack%s.j = cstack%s.j - cstack%s.j;"
                        .formatted(context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 2)
                );
                case FSUB -> context.output().pushMethodLine("cstack%s.f = cstack%s.f - cstack%s.f;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1)
                );
                case DSUB -> context.output().pushMethodLine("cstack%s.d = cstack%s.d - cstack%s.d;"
                        .formatted(context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 2)
                );
                case IMUL -> context.output().pushMethodLine("cstack%s.i = cstack%s.i * cstack%s.i;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1)
                );
                case LMUL -> context.output().pushMethodLine("cstack%s.j = cstack%s.j * cstack%s.j;"
                        .formatted(context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 2)
                );
                case FMUL -> context.output().pushMethodLine("cstack%s.f = cstack%s.f * cstack%s.f;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1)
                );
                case DMUL -> context.output().pushMethodLine("cstack%s.d = cstack%s.d * cstack%s.d;"
                        .formatted(context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 2)
                );
                case INEG -> context.output().pushMethodLine("cstack%s.i = -cstack%s.i;"
                        .formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1)
                );
                case LNEG -> context.output().pushMethodLine("cstack%s.j = -cstack%s.j;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2)
                );
                case FNEG -> context.output().pushMethodLine("cstack%s.f = -cstack%s.f;"
                        .formatted(context.getStackPointer().peek() - 1, context.getStackPointer().peek() - 1)
                );
                case DNEG -> context.output().pushMethodLine("cstack%s.d = -cstack%s.d;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2)
                );
                case ISHL -> context.output().pushMethodLine("cstack%s.i = cstack%s.i << (0x1f & cstack%s.i);"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1));
                case LSHL -> context.output().pushMethodLine("cstack%s.j = cstack%s.j << (0x3f & cstack%s.j);"
                        .formatted(context.getStackPointer().peek() - 3, context.getStackPointer().peek() - 3, context.getStackPointer().peek() - 1)
                );
                case ISHR -> context.output().pushMethodLine("cstack%s.i = cstack%s.i >> (0x1f & cstack%s.i);"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1)
                );
                case LSHR -> context.output().pushMethodLine("cstack%s.j = cstack%s.j >> (0x3f & cstack%s.j);"
                        .formatted(context.getStackPointer().peek() - 3, context.getStackPointer().peek() - 3, context.getStackPointer().peek() - 1)
                );
                case IUSHR -> context.output().pushMethodLine("cstack%s.i = (jint) (((uint32_t) cstack%s.i) >> (((uint32_t) cstack%s.i) & 0x1f));"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1)
                );
                case LUSHR -> context.output().pushMethodLine("cstack%s.j = (jlong) (((uint64_t) cstack%s.j) >> (((uint64_t) cstack%s.j) & 0x3f));"
                        .formatted(context.getStackPointer().peek() - 3, context.getStackPointer().peek() - 3, context.getStackPointer().peek() - 1)
                );
                case IAND -> context.output().pushMethodLine("cstack%s.i = cstack%s.i & cstack%s.i;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1)
                );
                case LAND -> context.output().pushMethodLine("cstack%s.j = cstack%s.j & cstack%s.j;"
                        .formatted(context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 2)
                );
                case IOR -> context.output().pushMethodLine("cstack%s.i = cstack%s.i | cstack%s.i;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1)
                );
                case LOR -> context.output().pushMethodLine("cstack%s.j = cstack%s.j | cstack%s.j;"
                        .formatted(context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 2)
                );
                case IXOR -> context.output().pushMethodLine("cstack%s.i = cstack%s.i ^ cstack%s.i;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1)
                );
                case LXOR -> context.output().pushMethodLine("cstack%s.j = cstack%s.j ^ cstack%s.j;"
                        .formatted(context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 2)
                );
                case DCMPL -> context.output().pushMethodBlock(
                        "jdouble value1 = cstack%s.d;"
                                .formatted(context.getStackPointer().peek() - 4),
                        "jdouble value2 = cstack%s.d;"
                                .formatted(context.getStackPointer().peek() - 2),
                        "cstack%s.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : -1));"
                                .formatted(context.getStackPointer().peek() - 4)
                );
                case DCMPG -> context.output().pushMethodBlock(
                        "jdouble value1 = cstack%s.d;".formatted(context.getStackPointer().peek() - 4),
                        "jdouble value2 = cstack%s.d;".formatted(context.getStackPointer().peek() - 2),
                        "cstack%s.i = value1 > value2 ? 1 : ((value1 == value2) ? 0 : ((value1 < value2) ? -1 : 1));".formatted(context.getStackPointer().peek() - 4)
                );
                case LCMP -> context.output().pushMethodLine("cstack%s.i = (cstack%s.j == cstack%s.j) ? 0 : (cstack%s.j > cstack%s.j ? 1 : -1);"
                        .formatted(context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 4,
                                context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 2)
                );
                case LREM -> context.output().pushMethodLine("cstack%s.j = (cstack%s.j %% cstack%s.j);"
                        .formatted(context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 2
                        )
                );
                case IREM -> context.output().pushMethodLine("cstack%s.i = cstack%s.i %% cstack%s.i;".formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1));
                case FREM -> context.output().pushMethodLine("cstack%s.f = std::fmod(cstack%s.f, cstack%s.f);"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1)
                );
                case DREM -> context.output().pushMethodLine("cstack%s.d = std::fmod(cstack%s.d, cstack%s.d);"
                        .formatted(context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 2)
                );
                case FDIV -> context.output().pushMethodLine("cstack%s.f = cstack%s.f / cstack%s.f;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1)
                );
                case IDIV -> context.output().pushMethodLine("cstack%s.i = cstack%s.i / cstack%s.i;"
                        .formatted(context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 2, context.getStackPointer().peek() - 1)
                );

                case LDIV -> context.output().pushMethodLine("cstack%s.j = cstack%s.j / cstack%s.j;"
                        .formatted(context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 2)
                );
                case DDIV -> context.output().pushMethodLine("cstack%s.d = cstack%s.d / cstack%s.d;"
                        .formatted(context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 4, context.getStackPointer().peek() - 2)
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