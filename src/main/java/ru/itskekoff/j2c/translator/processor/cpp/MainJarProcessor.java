package ru.itskekoff.j2c.translator.processor.cpp;

import lombok.Setter;
import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.tree.*;
import ru.itskekoff.j2c.annotations.NativeExclude;
import ru.itskekoff.j2c.annotations.NativeInclude;
import ru.itskekoff.j2c.annotations.vmp.VMProtect;
import ru.itskekoff.j2c.annotations.vmp.VMProtectType;
import ru.itskekoff.j2c.translator.TranslatorMain;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceTable;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.context.MethodContext;
import ru.itskekoff.j2c.translator.processor.instructions.InsnProcessorManager;
import ru.itskekoff.j2c.translator.utils.BaseUtils;
import ru.itskekoff.j2c.translator.utils.clazz.parser.ClassFilter;
import ru.itskekoff.j2c.translator.processor.cpp.utils.NativeLinker;
import ru.itskekoff.j2c.loader.Protection;
import ru.itskekoff.j2c.translator.utils.resource.ResourceUtils;
import software.coley.cafedude.InvalidClassException;
import software.coley.cafedude.classfile.ClassFile;
import software.coley.cafedude.io.ClassFileReader;
import software.coley.cafedude.io.ClassFileWriter;

import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.jar.Manifest;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import static org.objectweb.asm.Opcodes.ACC_STATIC;
import static ru.itskekoff.j2c.translator.processor.cpp.utils.translate.BaseProcessor.RETURN;

@Setter
public class MainJarProcessor {
    private ClassFilter classFilter;

    public void process(Path inputJarPath, Path outputJarPath) throws IOException {
        StringBuilder cppCode = new StringBuilder();
        MethodProcessor methodProcessor = new MethodProcessor();

        Path outputDir;
        String outputName;

        if (Files.isDirectory(outputJarPath)) {
            Files.deleteIfExists(outputJarPath);
        }
        outputDir = outputJarPath.getParent();
        outputName = outputJarPath.getFileName().toString();

        Files.createDirectories(outputDir);

        Path outputFile = outputDir.resolve(outputName);

        if (Files.exists(outputFile) && Files.isDirectory(outputFile)) {
            throw new IOException("Output path is a directory: " + outputFile);
        }

        if (!Files.exists(outputFile)) {
            Files.createFile(outputFile);
        }

        TranslatorMain.LOGGER.info("Output directory: {}", outputDir.toAbsolutePath());
        TranslatorMain.LOGGER.info("Output file: {}", outputFile.toAbsolutePath());

        Path cppDir = outputDir.resolve("cpp");
        Files.createDirectories(cppDir);

        List<String> resources = List.of("assets/vmp/VMProtectSDK64.dll", "assets/vmp/VMProtectSDK32.dll", "assets/vmp/VMProtect_Ext64.dll", "assets/vmp/VMProtect_Ext32.dll","assets/jni.h", "assets/vmp/VMProtectSDK.h", "assets/jni_md.h", "assets/xorstr.h", "assets/CMakeLists.txt");
        resources.forEach(name -> {
            try {
                BaseUtils.copyResource(name, cppDir);
            } catch (IOException e) {
                TranslatorMain.LOGGER.info("An error has occurred in copying resource {}", name, e);
            }
        });

        BaseUtils.copyResource("assets/vmp.exe", new File("%s/build/lib/".formatted(cppDir.toFile().getAbsolutePath())).toPath());
        BaseUtils.copyBin("assets/vmp/VMProtectSDK64", cppDir, "VMProtectSDK64.lib");
        BaseUtils.copyBin("assets/vmp/VMProtectSDK32", cppDir, "VMProtectSDK32.lib");

        Path dllMainPath = cppDir.resolve("dllmain.cpp");

        try (BufferedWriter mainWriter = Files.newBufferedWriter(dllMainPath);
             ZipOutputStream out = new ZipOutputStream(Files.newOutputStream(outputFile));
             JarFile jar = new JarFile(inputJarPath.toFile())) {

            mainWriter.write("\n%s".formatted(ResourceUtils.getStringFromResource("/assets/snippets/start.cpp")));
            processJarEntries(jar, out, cppCode, methodProcessor);
            mainWriter.append(cppCode.toString());
            mainWriter.write("\n%s".formatted(ResourceUtils.getStringFromResource("/assets/snippets/initialization.cpp")));
            Optional.ofNullable(jar.getManifest()).ifPresent(manifest -> writeManifest(out, manifest));
            addProtectionClass(out);
            try (BufferedWriter tableWriter = Files.newBufferedWriter(cppDir.resolve("table.h"))) {
                tableWriter.write(ResourceUtils.getStringFromResource("/assets/table.h"));
                tableWriter.append("\njclass classes[%s];".formatted(ReferenceTable.getClassIndex()));
                tableWriter.append("\njmethodID methods[%s];".formatted(ReferenceTable.getMethodIndex()));
                tableWriter.append("\njfieldID fields[%s];".formatted(ReferenceTable.getFieldIndex()));
            }

            mainWriter.close();

            try {
                executeCommand((new String[]{"cmake", "."}), cppDir.toFile());
                executeCommand((new String[]{"cmake", "--build", ".", "--config", "Release"}), cppDir.toFile());
                executeCommand((new String[]{cppDir.toFile()+"/build/lib/vmp.exe", "AntiAutistLeak.dll", "AALProtection.dll"}), new File(cppDir.toFile()+"/build/lib/"));
            } catch (Exception e) {
                System.err.println("Error during CMake execution: " + e.getMessage());
                e.printStackTrace();
            }

            out.putNextEntry(new ZipEntry("⚜richessssstafffs⚜/AntiAutistLeak.dll"));
            out.write(Files.readAllBytes(new File("%s/build/lib/AALProtection.dll".formatted(cppDir.toFile().getAbsolutePath())).toPath()));
        }
        TranslatorMain.LOGGER.info("Created output file (path={})", outputJarPath.toAbsolutePath());
    }

    private void executeCommand(String[] command, File directory) throws IOException, InterruptedException {
        Process process = Runtime.getRuntime().exec(command, null, directory);

        try (BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()))) {
            String line;
            while ((line = reader.readLine()) != null) {
                TranslatorMain.COMPILE_LOGGER.info(line);
            }
        }

        try (BufferedReader errorReader = new BufferedReader(new InputStreamReader(process.getErrorStream()))) {
            String line;
            while ((line = errorReader.readLine()) != null) {
                TranslatorMain.COMPILE_LOGGER.error(line);
            }
        }

        int exitCode = process.waitFor();
        if (exitCode != 0) {
            throw new IOException("Process exited with code: " + exitCode);
        }
    }

    private void processJarEntries(JarFile jar, ZipOutputStream out, StringBuilder cppCode, MethodProcessor methodProcessor) throws IOException {
        jar.stream()
                .filter(entry -> !entry.getName().equals("META-INF/MANIFEST.MF"))
                .forEach(entry -> processEntry(jar, entry, out, cppCode, methodProcessor));
    }

    private void processEntry(JarFile jar, ZipEntry entry, ZipOutputStream out, StringBuilder cppCode, MethodProcessor methodProcessor) {
        try {
            JarEntry jarEntry = (JarEntry) entry;
            if (!entry.getName().endsWith(".class")) {
                BaseUtils.writeEntry(jar, out, jarEntry);
                return;
            }

            byte[] classBytes = jar.getInputStream(entry).readAllBytes();
            if (!this.isClassFile(classBytes)) {
                BaseUtils.writeEntry(out, entry.getName(), classBytes);
                return;
            }

            ClassNode classNode = loadClassNode(classBytes);

            List<Class<?>> removeClasses = List.of(VMProtect.class, VMProtectType.class, NativeInclude.class, NativeExclude.class);
            if (removeClasses.stream().anyMatch(clazz -> Type.getInternalName(clazz).equals(classNode.name))) {
                return;
            }

            if (!this.classFilter.shouldProcess(classNode)) {
                BaseUtils.writeEntry(out, entry.getName(), classBytes);
                return;
            }

            TranslatorMain.LOGGER.info("Processing class {}", classNode.name);

            this.ensureClinitMethod(classNode);

            InsnProcessorManager.process(classNode);
            this.processClass(classNode, methodProcessor, cppCode);
            this.classFilter.cleanAnnotations(classNode);

            BaseUtils.writeEntry(out, entry.getName(), classToBytes(classNode, ClassWriter.COMPUTE_MAXS));
        } catch (Exception ex) {
            TranslatorMain.LOGGER.error("An error occurred in processing entry {}", entry.getName(), ex);
        }
    }

    private void processClass(ClassNode classNode, MethodProcessor methodProcessor, StringBuilder cppCode) {
        NativeLinker linker = new NativeLinker(classNode);
        MethodContext context = new MethodContext(classNode);
        classNode.methods.stream()
                .filter(this::shouldProcessMethod)
                .filter(method -> this.classFilter.shouldProcess(classNode, method))
                .forEach(method -> methodProcessor.process(context, method, linker));
        cppCode.append(context.output().toString());
    }

    private ClassNode loadClassNode(byte[] classBytes) throws InvalidClassException {
        return loadClass(classBytes, ClassReader.EXPAND_FRAMES);
    }

    public static ClassNode loadClass(byte[] bytes, int readerMode) throws InvalidClassException {
        return loadClass(bytes, readerMode, true);
    }

    public static ClassNode loadClass(byte[] bytes, int readerMode, boolean fix) throws InvalidClassException {
        ClassNode classNode;
        try {
            classNode = new ClassNode();
            ClassReader classReader = new ClassReader(bytes);
            classReader.accept(classNode, readerMode);
        } catch (Exception e) {
            classNode = fix ? loadClass(fixClass(bytes), readerMode, false) : null;
        }

        return classNode;
    }

    public static byte[] fixClass(byte[] bytes) throws InvalidClassException {
        ClassFileReader classFileReader = new ClassFileReader();
        ClassFile classFile = classFileReader.read(bytes);
        bytes = new ClassFileWriter().write(classFile);

        return bytes;
    }

    private void ensureClinitMethod(ClassNode classNode) {
        if (classNode.methods.stream().noneMatch(method -> method.name.equals("<clinit>"))) {
            MethodNode clinit = new MethodNode(ACC_STATIC, "<clinit>", "()V", null, null);
            clinit.instructions.add(new InsnNode(RETURN));
            classNode.methods.add(clinit);
        }
    }

    private boolean shouldProcessMethod(MethodNode method) {
        return !method.name.equals("<clinit>") &&
               !method.name.equals("<init>") &&
               !BaseUtils.hasFlag(method.access, Opcodes.ACC_ABSTRACT) &&
               (!method.name.contains("_proxy") && !method.name.contains("hello"));
    }

    private void writeManifest(ZipOutputStream out, Manifest manifest) {
        try {
            out.putNextEntry(new ZipEntry(JarFile.MANIFEST_NAME));
            manifest.write(out);
        } catch (IOException e) {
            throw new UncheckedIOException(e);
        }
    }

    private void addProtectionClass(ZipOutputStream out) {
        try {
            ClassNode protectionClass = addNode(Protection.class);
            out.putNextEntry(new ZipEntry(protectionClass.name + ".class"));
            out.write(classToBytes(protectionClass, ClassWriter.COMPUTE_FRAMES | ClassWriter.COMPUTE_MAXS));
        } catch (IOException e) {
            throw new UncheckedIOException(e);
        }
    }

    public static byte[] classToBytes(ClassNode classNode, int writerMode) {
        var classWriter = new ClassWriter(writerMode);
        classNode.accept(classWriter);
        return classWriter.toByteArray();
    }

    public static ClassNode addNode(Class<?> clazz) {
        try {
            ClassReader classReader = new ClassReader(clazz.getName());
            ClassNode classNode = new ClassNode();
            classReader.accept(classNode, 0);
            return classNode;
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private boolean isClassFile(byte[] bytes) {
        return BaseUtils.byteArrayToInt(Arrays.copyOfRange(bytes, 0, 4)) == 0xCAFEBABE;
    }

    public ClassFilter getFilter() {
        return this.classFilter;
    }
}