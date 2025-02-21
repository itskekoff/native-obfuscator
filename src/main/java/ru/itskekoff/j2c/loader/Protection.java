package ru.itskekoff.j2c.loader;

public class Protection {

    private static boolean initialized;

    private static native void initialize();

    public static void init() {
        if (initialized) return;
        initialized = true;

        String path;

            path = "C:\\%s\\native-obfuscator\\work\\output\\cpp\\build\\lib\\AntiAutistLeak.dll"
                    .formatted(
                            (System.getProperty("user.name").contains("kotopushka") ? "protection" : "IdeaProjects"
            ));

        System.load(path);
        initialize();
        System.out.println("[$] protection initialized");
    }
}