package ru.itskekoff.j2c.translator.processor.cpp.impl.array;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.InsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class ArrayLoadStoreProcessor extends BaseProcessor {
    public ArrayLoadStoreProcessor() {
        super(IALOAD, LALOAD, FALOAD,
                DALOAD, AALOAD, BALOAD,
                CALOAD, SALOAD, IASTORE,
                LASTORE, FASTORE, DASTORE,
                AASTORE, BASTORE, CASTORE,
                SASTORE
        );
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insn, MethodNode method) {
        if (insn instanceof InsnNode) {
            switch (insn.getOpcode()) {
                case IALOAD -> {
                    context.output().pushMethodLine("env->GetIntArrayRegion((jintArray) cstack%s.l, cstack%s.i, 1, &cstack%s.i);"
                            .formatted(
                                    context.getStackPointer().peek() - 2,
                                    context.getStackPointer().peek() - 1,
                                    context.getStackPointer().peek() - 2
                            )
                    );
                }
                case LALOAD -> {
                    context.output().pushMethodLine("env->GetLongArrayRegion((jlongArray) cstack%s.l, cstack%s.i, 1, &cstack%s.j);"
                            .formatted(
                                    context.getStackPointer().peek() - 2,
                                    context.getStackPointer().peek() - 1,
                                    context.getStackPointer().peek() - 2
                            )
                    );
                }
                case FALOAD -> {
                    context.output().pushMethodLine("env->GetFloatArrayRegion((jfloatArray) cstack%s.l, cstack%s.i, 1, &cstack%s.f);"
                            .formatted(
                                    context.getStackPointer().peek() - 2,
                                    context.getStackPointer().peek() - 1,
                                    context.getStackPointer().peek() - 2
                            )
                    );
                }
                case DALOAD -> {
                    context.output().pushMethodLine("env->GetDoubleArrayRegion((jdoubleArray) cstack%s.l, cstack%s.i, 1, &cstack%s.d);"
                            .formatted(
                                    context.getStackPointer().peek() - 2,
                                    context.getStackPointer().peek() - 1,
                                    context.getStackPointer().peek() - 2
                            )
                    );
                }
                case AALOAD -> {
                    context.output().pushMethodLine("cstack%s.l = env->GetObjectArrayElement((jobjectArray) cstack%s.l, cstack%s.i);"
                            .formatted(
                                    context.getStackPointer().peek() - 2,
                                    context.getStackPointer().peek() - 2,
                                    context.getStackPointer().peek() - 1
                            )
                    );
                }
                case BALOAD -> {
                    context.output().pushMethodLine("cstack%s.i = (jint) baload(env, (jarray) cstack%s.l, cstack%s.i);"
                            .formatted(
                                    context.getStackPointer().peek() - 2,
                                    context.getStackPointer().peek() - 2,
                                    context.getStackPointer().peek() - 1
                            )
                    );
                }
                case CALOAD -> {
                    context.output().pushMethodLine("{ jchar temp = 0;");
                    context.output().pushMethodLine("env->GetCharArrayRegion((jcharArray) cstack%s.l, cstack%s.i, 1, &temp);\ncstack%s.i = (jint) temp; }"
                            .formatted(
                                    context.getStackPointer().peek() - 2,
                                    context.getStackPointer().peek() - 1,
                                    context.getStackPointer().peek() - 2
                            )
                    );
                }
                case SALOAD -> {
                    context.output().pushMethodLine("{ jshort temp = 0;");
                    context.output().pushMethodLine("env->GetShortArrayRegion((jshortArray) cstack%s.l, cstack%s.i, 1, &temp);\ncstack%s.i = (jint) temp; }"
                            .formatted(
                                    context.getStackPointer().peek() - 2,
                                    context.getStackPointer().peek() - 1,
                                    context.getStackPointer().peek() - 2
                            )
                    );
                }
                case IASTORE -> {
                    context.output().pushMethodLine("env->SetIntArrayRegion((jintArray) cstack%s.l, cstack%s.i, 1, &cstack%s.i);"
                            .formatted(
                                    context.getStackPointer().peek() - 3,
                                    context.getStackPointer().peek() - 2,
                                    context.getStackPointer().peek() - 1
                            )
                    );
                }
                case LASTORE -> {
                    context.output().pushMethodLine("env->SetLongArrayRegion((jlongArray) cstack%s.l, cstack%s.i, 1, &cstack%s.j);"
                            .formatted(
                                    context.getStackPointer().peek() - 4,
                                    context.getStackPointer().peek() - 3,
                                    context.getStackPointer().peek() - 2
                            )
                    );
                }
                case FASTORE -> {
                    context.output().pushMethodLine("env->SetFloatArrayRegion((jfloatArray) cstack%s.l, cstack%s.i, 1, &cstack%s.f);"
                            .formatted(
                                    context.getStackPointer().peek() - 3,
                                    context.getStackPointer().peek() - 2,
                                    context.getStackPointer().peek() - 1
                            )
                    );
                }
                case DASTORE -> {
                    context.output().pushMethodLine("env->SetDoubleArrayRegion((jdoubleArray) cstack%s.l, cstack%s.i, 1, &cstack%s.d);"
                            .formatted(
                                    context.getStackPointer().peek() - 4,
                                    context.getStackPointer().peek() - 3,
                                    context.getStackPointer().peek() - 2
                            )
                    );
                }
                case AASTORE -> {
                    context.output().pushMethodLine("env->SetObjectArrayElement((jobjectArray) cstack%s.l, cstack%s.i, cstack%s.l);"
                            .formatted(
                                    context.getStackPointer().peek() - 3,
                                    context.getStackPointer().peek() - 2,
                                    context.getStackPointer().peek() - 1
                            )
                    );
                }
                case BASTORE -> {
                    context.output().pushMethodLine("bastore(env, (jarray) cstack%s.l, cstack%s.i, cstack%s.i);"
                            .formatted(
                                    context.getStackPointer().peek() - 3,
                                    context.getStackPointer().peek() - 2,
                                    context.getStackPointer().peek() - 1
                            )
                    );
                }
                case CASTORE -> {
                    context.output().pushMethodLine("{ jchar temp = (jchar) cstack%s.i;".formatted(context.getStackPointer().peek() - 1));
                    context.output().pushMethodLine("env->SetCharArrayRegion((jcharArray) cstack%s.l, cstack%s.i, 1, &temp); }"
                            .formatted(
                                    context.getStackPointer().peek() - 3,
                                    context.getStackPointer().peek() - 2
                            )
                    );
                }
                case SASTORE -> {
                    context.output().pushMethodLine("{ jshort temp = (jshort) cstack%s.i;".formatted(context.getStackPointer().peek() - 1));
                    context.output().pushMethodLine("env->SetShortArrayRegion((jshortArray) cstack%s.l, cstack%s.i, 1, &temp); }"
                            .formatted(context.getStackPointer().peek() - 3,
                                    context.getStackPointer().peek() - 2
                            )
                    );
                }
            }
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        return switch (insnNode.getOpcode()) {
            case DALOAD, LALOAD -> currentPointer;
            case IALOAD, SALOAD, CALOAD, BALOAD, AALOAD, FALOAD -> currentPointer - 1;
            case IASTORE, SASTORE, CASTORE, BASTORE, AASTORE, FASTORE -> currentPointer - 3;
            case LASTORE, DASTORE -> currentPointer - 4;
            default -> throw new IllegalStateException("Unexpected value: " + insnNode.getOpcode());
        };
    }
}
