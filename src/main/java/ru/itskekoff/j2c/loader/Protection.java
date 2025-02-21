package ru.itskekoff.j2c.loader;

public class Protection {

    private static boolean initialized;

    private static native void initialize();

    public static void init() {
        if (initialized) return;
        initialized = true;
        System.load("C:\\IdeaProjects\\native-obfuscator\\work\\output\\cpp\\build\\lib\\AntiAutistLeak.dll");
        initialize();
        System.out.println("[$] protection initialized");
    }
}