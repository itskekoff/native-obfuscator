package ru.itskekoff.j2c.translator.processor.cpp.utils;


public class SnippetGenerator {

    public static String getMethodID(boolean isStatic, String name, String signature, String tempClassAddition) {
        return "env->Get%sMethodID(%s, \"%s\", \"%s\")"
                .formatted(isStatic ? "Static" : "",
                        tempClassAddition,
                        name,
                        signature);
    }

    public static String reinterpretArray(String type) {
        if (type.equals("Array")) return "Object";
        return type;
    }

}
