package ru.itskekoff.j2c.translator.processor.cpp.impl.array;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.InsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;


public class ArrayLoadStoreProcessor extends BaseProcessor {
    private int storeIndex = 0;
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
    public void translate(ClassContext writer, AbstractInsnNode insn, MethodNode method) {
        String tempAddition = "temp_%d".formatted(storeIndex++);
        if (insn instanceof InsnNode) {
            switch (insn.getOpcode()) {
                case IALOAD -> {
                    writer.output().pushMethodLine("env->GetIntArrayRegion((jintArray) cstack%s.l, cstack%s.i, 1, &cstack%s.i);"
                            .formatted(
                                    writer.getStackPointer().peek() - 2,
                                    writer.getStackPointer().peek() - 1,
                                    writer.getStackPointer().peek() - 2
                            )
                    );
                }
                case LALOAD -> {
                    writer.output().pushMethodLine("env->GetLongArrayRegion((jlongArray) cstack%s.l, cstack%s.i, 1, &cstack%s.j);"
                            .formatted(
                                    writer.getStackPointer().peek() - 2,
                                    writer.getStackPointer().peek() - 1,
                                    writer.getStackPointer().peek() - 2
                            )
                    );
                }
                case FALOAD -> {
                    writer.output().pushMethodLine("env->GetFloatArrayRegion((jfloatArray) cstack%s.l, cstack%s.i, 1, &cstack%s.f);"
                            .formatted(
                                    writer.getStackPointer().peek() - 2,
                                    writer.getStackPointer().peek() - 1,
                                    writer.getStackPointer().peek() - 2
                            )
                    );
                }
                case DALOAD -> {
                    writer.output().pushMethodLine("env->GetDoubleArrayRegion((jdoubleArray) cstack%s.l, cstack%s.i, 1, &cstack%s.d);"
                            .formatted(
                                    writer.getStackPointer().peek() - 2,
                                    writer.getStackPointer().peek() - 1,
                                    writer.getStackPointer().peek() - 2
                            )
                    );
                }
                case AALOAD -> {
                    writer.output().pushMethodLine("cstack%s.l = env->GetObjectArrayElement((jobjectArray) cstack%s.l, cstack%s.i);"
                            .formatted(
                                    writer.getStackPointer().peek() - 2,
                                    writer.getStackPointer().peek() - 2,
                                    writer.getStackPointer().peek() - 1
                            )
                    );
                }
                case BALOAD -> {
                    writer.output().pushMethodLine("cstack%s.i = (jint) baload(env, (jarray) cstack%s.l, cstack%s.i);"
                            .formatted(
                                    writer.getStackPointer().peek() - 2,
                                    writer.getStackPointer().peek() - 2,
                                    writer.getStackPointer().peek() - 1
                            )
                    );
                }
                case CALOAD -> {
                    writer.output().pushMethodLine("jchar %s = 0;".formatted(tempAddition));
                    writer.output().pushMethodLine("env->GetCharArrayRegion((jcharArray) cstack%s.l, cstack%s.i, 1, &%s);\ncstack%s.i = (jint) %s;"
                            .formatted(
                                    writer.getStackPointer().peek() - 2,
                                    writer.getStackPointer().peek() - 1,
                                    tempAddition,
                                    writer.getStackPointer().peek() - 2,
                                    tempAddition
                            )
                    );
                }
                case SALOAD -> {
                    writer.output().pushMethodLine("jshort %s = 0;".formatted(tempAddition));
                    writer.output().pushMethodLine("env->GetShortArrayRegion((jshortArray) cstack%s.l, cstack%s.i, 1, &%s);\ncstack%s.i = (jint) %s;"
                            .formatted(
                                    writer.getStackPointer().peek() - 2,
                                    writer.getStackPointer().peek() - 1,
                                    tempAddition,
                                    writer.getStackPointer().peek() - 2,
                                    tempAddition
                            )
                    );
                }
                case IASTORE -> {
                    writer.output().pushMethodLine("env->SetIntArrayRegion((jintArray) cstack%s.l, cstack%s.i, 1, &cstack%s.i);"
                            .formatted(
                                    writer.getStackPointer().peek() - 3,
                                    writer.getStackPointer().peek() - 2,
                                    writer.getStackPointer().peek() - 1
                            )
                    );
                }
                case LASTORE -> {
                    writer.output().pushMethodLine("env->SetLongArrayRegion((jlongArray) cstack%s.l, cstack%s.i, 1, &cstack%s.j);"
                            .formatted(
                                    writer.getStackPointer().peek() - 4,
                                    writer.getStackPointer().peek() - 3,
                                    writer.getStackPointer().peek() - 2
                            )
                    );
                }
                case FASTORE -> {
                    writer.output().pushMethodLine("env->SetFloatArrayRegion((jfloatArray) cstack%s.l, cstack%s.i, 1, &cstack%s.f);"
                            .formatted(
                                    writer.getStackPointer().peek() - 3,
                                    writer.getStackPointer().peek() - 2,
                                    writer.getStackPointer().peek() - 1
                            )
                    );
                }
                case DASTORE -> {
                    writer.output().pushMethodLine("env->SetDoubleArrayRegion((jdoubleArray) cstack%s.l, cstack%s.i, 1, &cstack%s.d);"
                            .formatted(
                                    writer.getStackPointer().peek() - 4,
                                    writer.getStackPointer().peek() - 3,
                                    writer.getStackPointer().peek() - 2
                            )
                    );
                }
                case AASTORE -> {
                    writer.output().pushMethodLine("env->SetObjectArrayElement((jobjectArray) cstack%s.l, cstack%s.i, cstack%s.l);"
                            .formatted(
                                    writer.getStackPointer().peek() - 3,
                                    writer.getStackPointer().peek() - 2,
                                    writer.getStackPointer().peek() - 1
                            )
                    );
                }
                case BASTORE -> {
                    writer.output().pushMethodLine("bastore(env, (jarray) cstack%s.l, cstack%s.i, cstack%s.i);"
                            .formatted(
                                    writer.getStackPointer().peek() - 3,
                                    writer.getStackPointer().peek() - 2,
                                    writer.getStackPointer().peek() - 1
                            )
                    );
                }
                case CASTORE -> {
                    writer.output().pushMethodLine("jchar %s = (jchar) cstack%s.i;".formatted(tempAddition, writer.getStackPointer().peek() - 1));
                    writer.output().pushMethodLine("env->SetCharArrayRegion((jcharArray) cstack%s.l, cstack%s.i, 1, &%s);"
                            .formatted(
                                    writer.getStackPointer().peek() - 3,
                                    writer.getStackPointer().peek() - 2,
                                    tempAddition
                            )
                    );
                }
                case SASTORE -> {
                    writer.output().pushMethodLine("jshort %s = (jshort) cstack%s.i;".formatted(tempAddition, writer.getStackPointer().peek() - 1));
                    writer.output().pushMethodLine("env->SetShortArrayRegion((jshortArray) cstack%s.l, cstack%s.i, 1, &%s);"
                            .formatted(writer.getStackPointer().peek() - 3,
                                    writer.getStackPointer().peek() - 2,
                                    tempAddition
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
