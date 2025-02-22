package ru.itskekoff.j2c.translator.processor.cpp.impl.array;

import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.MethodNode;
import org.objectweb.asm.tree.MultiANewArrayInsnNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

/**
 * @author itskekoff
 * @since 12:56 of 19.02.2025
 */
public class MultiNewArrayProcessor extends BaseProcessor {
    public MultiNewArrayProcessor() {
        super(MULTIANEWARRAY);
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insnNode, MethodNode method) {
        MultiANewArrayInsnNode multiANewArrayInsn = (MultiANewArrayInsnNode) insnNode;

        String desc = multiANewArrayInsn.desc;
        int dims = multiANewArrayInsn.dims;
        int stackIndex = context.getStackPointer().peek() - dims;

        boolean isPrimitive = desc.startsWith("[") && !desc.contains("L");

        // shit
        if (isPrimitive) {
            String sort = getPrimitiveSort(desc);

            context.output().pushMethodLine(
                    "cstack%s.l = create_multidim_array_value<%s>(env, %d, %d, \"%s\", __LINE__, %s);"
                            .formatted(stackIndex, sort, dims, dims, desc, generateDimsArray(dims, stackIndex))
            );
        } else {
            context.output().pushMethodLine(
                    "cstack%s.l = create_multidim_array(env, classloader, %d, %d, \"%s\", __LINE__, %s);"
                            .formatted(stackIndex, dims, dims, desc, generateDimsArray(dims, stackIndex))
            );
        }

    }

    private String generateDimsArray(int dims, int stackIndex) {
        StringBuilder dimsArray = new StringBuilder("{ ");
        for (int i = 0; i < dims; i++) {
            dimsArray.append("cstack").append(stackIndex + i).append(".i");
            if (i < dims - 1) {
                dimsArray.append(", ");
            }
        }
        dimsArray.append(" }");
        return dimsArray.toString();
    }

    private String getPrimitiveSort(String desc) {
        return switch (desc.substring(1)) {
            case "Z" -> "jbool";
            case "B" -> "jbyte";
            case "C" -> "jchar";
            case "S" -> "jshort";
            case "I" -> "jint";
            case "F" -> "jfloat";
            case "J" -> "jlong";
            case "D" -> "jdouble";
            default -> throw new IllegalArgumentException("Unsupported primitive type: " + desc);
        };
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        MultiANewArrayInsnNode arrayNode = (MultiANewArrayInsnNode) insnNode;
        return currentPointer - arrayNode.dims + 1;
    }
}
