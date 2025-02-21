package ru.itskekoff.j2c.translator.processor.cpp.impl.constant;

import org.objectweb.asm.Type;
import org.objectweb.asm.tree.AbstractInsnNode;
import org.objectweb.asm.tree.LdcInsnNode;
import org.objectweb.asm.tree.MethodNode;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.ClassContext;
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
    public void translate(ClassContext classContext, AbstractInsnNode insnNode, MethodNode method) {
        Object cst = ((LdcInsnNode) insnNode).cst;
        if (cst instanceof String) {
            classContext.output().pushMethodLine("cstack%s.l = env->NewStringUTF(\"%s\");".formatted(classContext.getStackPointer().peek(), escapeString(cst.toString())));
        } else if (cst instanceof Integer) {
            classContext.output().pushMethodLine("cstack%s.i = %s;".formatted(classContext.getStackPointer().peek(), getIntString((Integer) cst)));
        } else if (cst instanceof Long) {
            classContext.output().pushMethodLine("cstack%s.j = %s;".formatted(classContext.getStackPointer().peek(), getLongValue((Long) cst)));
        } else if (cst instanceof Float) {
            classContext.output().pushMethodLine("cstack%s.f = %s;".formatted(classContext.getStackPointer().peek(), getFloatValue((Float) cst)));
        } else if (cst instanceof Double) {
            classContext.output().pushMethodLine("cstack%s.d = %s;".formatted(classContext.getStackPointer().peek(), getDoubleValue((Double) cst)));
        } else if (cst instanceof Type) {
            if (classContext.isNotClinit(method))
                classContext.output().pushMethodLine("cstack%s.l = classes[%s].applyDecryption();"
                        .formatted(classContext.getStackPointer().peek(), classContext.output().pushJavaClass(((Type) cst).getClassName().replace(".", "/"))));
             else             classContext.output().pushMethodLine("cstack%s.l = env->FindClass(\"%s\");"
                    .formatted(classContext.getStackPointer().peek(), ((Type) cst).getClassName().replace(".", "/")));

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
