package ru.itskekoff.j2c.translator.processor.cpp.reference;

import java.util.HashMap;
import java.util.Map;

/**
 * @author itskekoff
 * @since 14:18 of 21.02.2025
 */
public class ReferenceTable {
    private static final Map<String, Integer> classes = new HashMap<>();
    private static int classIndex = 0;

    public static void pushClass(String klassName, int classIndex) {
        classes.putIfAbsent(klassName, classIndex);
    }

    public static Map<String, Integer> getClasses() {
        return classes;
    }

    public static int getClassIndex() {
        return classIndex++;
    }
}
