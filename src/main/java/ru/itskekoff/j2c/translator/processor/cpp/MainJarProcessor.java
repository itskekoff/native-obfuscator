package ru.itskekoff.j2c.translator.processor.cpp;

import lombok.Setter;
import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.tree.*;
import ru.itskekoff.j2c.translator.TranslatorMain;
import ru.itskekoff.j2c.translator.build.ProcessManager;
import ru.itskekoff.j2c.translator.build.SetupManager;
import ru.itskekoff.j2c.translator.processor.cpp.reference.ReferenceTable;
import ru.itskekoff.j2c.translator.processor.cpp.utils.translate.MethodContext;
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

import java.io.BufferedWriter;
import java.io.File;
import java.io.IOException;
import java.io.UncheckedIOException;
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
        BaseUtils.copyResource("jni.h", cppDir);

        try (BufferedWriter mainWriter = Files.newBufferedWriter(cppDir.resolve("dllmain.cpp"));
             ZipOutputStream out = new ZipOutputStream(Files.newOutputStream(outputFile));
             JarFile jar = new JarFile(inputJarPath.toFile())) {

            mainWriter.write("\n%s".formatted(ResourceUtils.getStringFromResource("/assets/snippets/start.cpp")));
            processJarEntries(jar, out, cppCode, methodProcessor);

            mainWriter.append(cppCode);
            mainWriter.write("\n%s".formatted(ResourceUtils.getStringFromResource("/assets/snippets/initialization.cpp")));
            Optional.ofNullable(jar.getManifest()).ifPresent(manifest -> writeManifest(out, manifest));
            addProtectionClass(out);
            try (BufferedWriter tableWriter = Files.newBufferedWriter(cppDir.resolve("table.h"))) {
                tableWriter.write(ResourceUtils.getStringFromResource("/assets/table.h"));
                tableWriter.append("\nRBM classes[%s];".formatted(ReferenceTable.getClassIndex()));

                tableWriter.append("\njmethodID methods[%s];".formatted(ReferenceTable.getFieldIndex()));

            }
            String osName = "windows";
            String platformTypeName = "x86_64";
            String libName = "x64-windows.dll";
            String zigPath = System.getProperty("user.dir") + File.separator + "zig-windows-x86_64-0.9.1" + File.separator + "zig" + (SetupManager.isWindows() ? ".exe" : "");

            if (Files.exists(Paths.get(zigPath))) {
                TranslatorMain.COMPILE_LOGGER.info("Compiling C++ Objects");
                List<String> compileCommand = Arrays.asList(
                        zigPath,
                        "c++",
                        "-target", platformTypeName + "-" + osName + "-gnu",
                        "-I." + File.separator + "cpp",
                        "-fms-compatibility",
                        "-o." + File.separator + "build" + File.separator + "lib" + File.separator + libName,
                        "." + File.separator + "cpp" + File.separator + "dllmain.cpp"
                );
                ProcessManager.ProcessResult compileRunResult = ProcessManager.run(outputDir, 600000L, compileCommand);
                compileRunResult.check("zig build");
            } else {
                TranslatorMain.COMPILE_LOGGER.error("Zig compiler not found at path: {}", zigPath);
            }
        }
        TranslatorMain.LOGGER.info("Created output file (path={})", outputJarPath.toAbsolutePath());
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
            if (!this.shouldProcessClass(classNode)) {
                BaseUtils.writeEntry(out, entry.getName(), classBytes);
                return;
            }
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

    private boolean shouldProcessClass(ClassNode classNode) {
        return classFilter.shouldProcess(classNode) &&
               classNode.methods.stream()
                       .anyMatch(method -> ClassFilter.shouldProcess(method) && classFilter.shouldProcess(classNode, method));
    }
    private boolean isClassFile(byte[] bytes) {
        return BaseUtils.byteArrayToInt(Arrays.copyOfRange(bytes, 0, 4)) == 0xCAFEBABE;
    }

    public ClassFilter getFilter() {
        return this.classFilter;
    }
}