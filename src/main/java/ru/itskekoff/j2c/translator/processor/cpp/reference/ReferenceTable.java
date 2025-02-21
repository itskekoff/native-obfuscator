package ru.itskekoff.j2c.translator.processor.cpp.reference;

import java.util.HashMap;
import java.util.Map;

public class ReferenceTable {
    private static final Map<String, Integer> classes = new HashMap<>();
    private static int classIndex = 0;
    private static int fieldIndex = 0;

    public static void pushClass(String klassName, int classIndex) {
        classes.putIfAbsent(klassName, classIndex);
    }

    public static Map<String, Integer> getClasses() {
        return classes;
    }

    public static int getFieldIndex() {
        return fieldIndex++;
    }

    public static int getClassIndex() {
        return classIndex++;
    }
}
