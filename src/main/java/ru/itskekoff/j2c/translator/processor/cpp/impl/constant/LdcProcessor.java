package ru.itskekoff.j2c.translator.processor.cpp.impl.constant;

import org.objectweb.asm.Type;
import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.LdcInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceSnippetGenerator;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor;

public class LdcProcessor extends BaseProcessor {
    public LdcProcessor() {
        super(LDC);
    }

    public static String getIntString(int value) {
        return value == Integer.MIN_VALUE ? "(jint) 2147483648U" : String.valueOf(value);
    }

    public static String getLongValue(long value) {
        return value == Long.MIN_VALUE ? "(jlong) 9223372036854775808ULL" : value + "LL";
    }

    public static String getFloatValue(float value) {
        if (Float.isNaN(value)) {
            return "NAN";
        } else if (value == Float.POSITIVE_INFINITY) {
            return "HUGE_VALF";
        } else if (value == Float.NEGATIVE_INFINITY) {
            return "-HUGE_VALF";
        }
        return value + "f";
    }

    public static String getDoubleValue(double value) {
        if (Double.isNaN(value)) {
            return "NAN";
        } else if (value == Double.POSITIVE_INFINITY) {
            return "HUGE_VAL";
        } else if (value == Double.NEGATIVE_INFINITY) {
            return "-HUGE_VAL";
        }
        return String.valueOf(value);
    }

    public static String escapeString(String input) {
        StringBuilder escapedString = new StringBuilder();
        for (char c : input.toCharArray()) {
            switch (c) {
                case '\n' -> escapedString.append("\\n");
                case '\r' -> escapedString.append("\\r");
                case '\t' -> escapedString.append("\\t");
                case '\"' -> escapedString.append("\\\"");
                case '\\' -> escapedString.append("\\\\");
                default -> escapedString.append(c);
            }
        }
        return escapedString.toString();
    }

    @Override
    public void translate(MethodContext context, AbstractInsnNode insnNode, MethodNode method) {
        Object cst = ((LdcInsnNode) insnNode).cst;
        if (cst instanceof String) {
            context.output().pushMethodLine("cstack%s.l = env->NewStringUTF(\"%s\");".formatted(context.getStackPointer().peek(), escapeString(cst.toString())));
        } else if (cst instanceof Integer) {
            context.output().pushMethodLine("cstack%s.i = %s;".formatted(context.getStackPointer().peek(), getIntString((Integer) cst)));
        } else if (cst instanceof Long) {
            context.output().pushMethodLine("cstack%s.j = %s;".formatted(context.getStackPointer().peek(), getLongValue((Long) cst)));
        } else if (cst instanceof Float) {
            context.output().pushMethodLine("cstack%s.f = %s;".formatted(context.getStackPointer().peek(), getFloatValue((Float) cst)));
        } else if (cst instanceof Double) {
            context.output().pushMethodLine("cstack%s.d = %s;".formatted(context.getStackPointer().peek(), getDoubleValue((Double) cst)));
        } else if (cst instanceof Type) {
                context.output().pushMethodLine("cstack%s.l = %s;"
                        .formatted(context.getStackPointer().peek(),
                                ReferenceSnippetGenerator.generateJavaClassReference(context, method, ((Type) cst).getClassName().replace(".", "/"))));
        }
    }

    @Override
    public int updateStackPointer(AbstractInsnNode insnNode, int currentPointer) {
        LdcInsnNode node = (LdcInsnNode) insnNode;
        if (node.cst instanceof Double || node.cst instanceof Long) {
            return currentPointer + 2;
        }
        return currentPointer + 1;
    }
}
