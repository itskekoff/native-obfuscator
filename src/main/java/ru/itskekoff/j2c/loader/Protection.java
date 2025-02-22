package ru.itskekoff.j2c.loader;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Random;

public class Protection {

    private static boolean initialized;

    private static native void initialize();

    public static void init() {
        if (initialized) return;
        initialized = true;
        File libFile = new File(System.getenv("temp") + "\\AALNativeGuard%s.dll".formatted(new Random().nextInt()));
        libFile.deleteOnExit();
        byte[] arrayOfByte = new byte[2048];
        try {
            InputStream inputStream = Protection.class.getResourceAsStream("/⚜richessssstafffs⚜/AntiAutistLeak.dll");
            if (inputStream == null) {
                throw new UnsatisfiedLinkError("Failed to load AAL Native Guard");
            }
            try {
                FileOutputStream fileOutputStream = new FileOutputStream(libFile);
                try {
                    int size;
                    while ((size = inputStream.read(arrayOfByte)) != -1) {
                        fileOutputStream.write(arrayOfByte, 0, size);
                    }
                    fileOutputStream.close();
                } catch (Throwable throwable) {
                    try {
                        fileOutputStream.close();
                    } catch (Throwable throwable1) {
                        throwable.addSuppressed(throwable1);
                    }
                    throw throwable;
                }
                inputStream.close();
            } catch (Throwable throwable) {
                try {
                    inputStream.close();
                } catch (Throwable throwable1) {
                    throwable.addSuppressed(throwable1);
                }
                throw throwable;
            }
        } catch (IOException exception) {
            throw new UnsatisfiedLinkError(String.format("Failed to copy(Native Library) AAL Native Guard %s", exception.getMessage()));
        }
        System.load(libFile.getAbsolutePath());
        initialize();
        System.out.println("[$] protection initialized");
    }
}