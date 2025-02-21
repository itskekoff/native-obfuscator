package ru.itskekoff.j2c.translator.build;


import com.google.gson.Gson;
import ru.itskekoff.j2c.translator.TranslatorMain;
import ru.itskekoff.j2c.translator.utils.system.FileUtils;
import ru.itskekoff.j2c.translator.utils.system.FileZipperUtils;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.Arrays;
import java.util.Map;

public class SetupManager {
    private static String OS;

    static {
        SetupManager.OS = System.getProperty("os.name").toLowerCase();
    }

    public static void init() {
        final String platformTypeName = getPlatformTypeName();
        String fileName = null;
        String dirName = null;
        if (platformTypeName != null && !"".equals(platformTypeName)) {
            if (isLinux()) {
                fileName = "zig-linux-" + platformTypeName + "-0.9.1.tar.xz";
                dirName = "zig-linux-" + platformTypeName + "-0.9.1";
            } else if (isMacOS()) {
                fileName = "zig-macos-" + platformTypeName + "-0.9.1.tar.xz";
                dirName = "zig-macos-" + platformTypeName + "-0.9.1";
            } else if (isWindows()) {
                fileName = "zig-windows-" + platformTypeName + "-0.9.1.zip";
                dirName = "zig-windows-" + platformTypeName + "-0.9.1";
            }
            downloadZigCompiler(fileName, dirName);
            return;
        }
        TranslatorMain.LOGGER.error("This system is not supported. Please contact the developer");
    }

    private static String getPlatformTypeName() {
        final String lowerCase;
        final String platform = lowerCase = System.getProperty("os.arch").toLowerCase();
        String platformTypeName = null;
        switch (lowerCase) {
            case "x86_64":
            case "amd64": {
                platformTypeName = "x86_64";
                break;
            }
            case "aarch64": {
                platformTypeName = "aarch64";
                break;
            }
            case "x86": {
                platformTypeName = "i386";
                break;
            }
            default: {
                platformTypeName = "";
                break;
            }
        }
        return platformTypeName;
    }

    public static boolean isLinux() {
        return SetupManager.OS.contains("linux");
    }

    public static boolean isMacOS() {
        return SetupManager.OS.contains("mac") && SetupManager.OS.indexOf("os") > 0;
    }

    public static boolean isWindows() {
        return SetupManager.OS.contains("windows");
    }

    public static void downloadZigCompiler(final String fileName, final String dirName) {
        if (Files.exists(Paths.get(dirName))) {
            TranslatorMain.LOGGER.info("Found compiler at {}", dirName);
            return;
        }

        try {
            final String currentDir = System.getProperty("user.dir");
            TranslatorMain.LOGGER.info("Downloading zig compiler...");
            // System.out.println("Ссылка на скачивание：https://ziglang.org/download/0.9.1/" + fileName);
            final InputStream in = new URL("https://ziglang.org/download/0.9.1/" + fileName).openStream();
            Files.copy(in, Paths.get(currentDir + File.separator + fileName), StandardCopyOption.REPLACE_EXISTING);
            TranslatorMain.LOGGER.info("Unpacking zig compiler...");
            unzipFile(currentDir, fileName, currentDir);
            deleteFile(currentDir, fileName + ".temp");
            deleteFile(currentDir, fileName);
            TranslatorMain.LOGGER.info("Compiler downloaded.");
            if (!isWindows()) {
                final String compilePath2 = currentDir + File.separator + dirName + File.separator + "zig";
                ProcessManager.run(Paths.get(currentDir), 160000L, Arrays.asList("chmod", "777", compilePath2));
                //  System.out.println("Successfully set running permission");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void deleteFile(final String path, final String file) {
        new File(path + File.separator + file).delete();
    }

    public static void unzipFile(final String path, final String file, final String destination) {
        try {
            FileZipperUtils.extract(Paths.get(path + File.separator + file), Paths.get(destination));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static String getZigGlobalCacheDirectory(final boolean clear) {
        final String platformTypeName = getPlatformTypeName();
        String dirName = null;
        if (platformTypeName != null && !"".equals(platformTypeName)) {
            if (isLinux()) {
                dirName = "zig-linux-" + platformTypeName + "-0.9.1";
            } else if (isMacOS()) {
                dirName = "zig-macos-" + platformTypeName + "-0.9.1";
            } else if (isWindows()) {
                dirName = "zig-windows-" + platformTypeName + "-0.9.1";
            }
        }
        final String currentDir = System.getProperty("user.dir");
        if (Files.exists(Paths.get(currentDir + File.separator + dirName))) {
            final String compilePath = currentDir + File.separator + dirName + File.separator + "zig" + (isWindows() ? ".exe" : "");
            if (Files.exists(Paths.get(compilePath))) {
                try {
                    final ProcessManager.ProcessResult compileRunresult = ProcessManager.run(Paths.get(currentDir + File.separator + dirName, new String[0]), 160000L, Arrays.asList(compilePath, "env"));
                    Gson gson = new Gson();
                    Map<String, String> map = gson.fromJson(compileRunresult.stdout, Map.class);
                    if (clear) {
                        FileUtils.clearDirectory(map.get("global_cache_dir"));
                    }
                    return map.get("global_cache_dir");
                } catch (Throwable e) {
                    e.printStackTrace();
                }
            }
        }
        TranslatorMain.LOGGER.error("Failed to get zig temporary file directory");
        return "";
    }
}
