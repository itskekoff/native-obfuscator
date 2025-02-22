package ru.itskekoff.j2c.translator;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import picocli.CommandLine;
import ru.itskekoff.j2c.translator.build.SetupManager;
import ru.itskekoff.j2c.translator.configuration.TranslatorConfiguration;
import ru.itskekoff.j2c.translator.processor.cpp.MainJarProcessor;
import ru.itskekoff.j2c.translator.utils.clazz.parser.ClassFilter;

import java.io.*;
import java.nio.file.*;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.stream.Stream;

import static java.nio.file.Files.walk;

public class TranslatorMain {
    public static Logger LOGGER;
    public static Logger COMPILE_LOGGER;
    private static MainJarProcessor processor;

    public static void main(String[] args) {
        SetupManager.init();
        System.exit(new CommandLine(new NativeObfuscatorRunner())
                .setCaseInsensitiveEnumValuesAllowed(true)
                .execute(args));
    }

    public static MainJarProcessor getProcessor() {
        if (processor == null) {
            processor = new MainJarProcessor();
        }
        return processor;
    }

    @CommandLine.Command(name = "Jnic", mixinStandardHelpOptions = true, version = {"ZalupaCPP/v1.0.0"})
    private static class NativeObfuscatorRunner implements Callable<Integer> {
        @CommandLine.Parameters(index = "0", description = "Jar file to transpile")
        private File input;

        @CommandLine.Parameters(index = "1", description = "Output directory")
        private String output;

        @CommandLine.Option(names = {"-c", "--config"}, defaultValue = "config.yml",
                description = "Config file")
        private File config;

        @Override
        public Integer call() throws Exception {
            LOGGER.info("Reading configuration file (path={})", config.getAbsolutePath());

            TranslatorConfiguration.IMP.reload(Path.of("config.yml"));
            this.parseConfiguration();

            Path outputPath = prepareOutputPath(output, input);

            MainJarProcessor jarProcessor = getProcessor();
            jarProcessor.process(input.toPath(), outputPath);
            return 0;
        }

        private void parseConfiguration() {
            TranslatorConfiguration CONFIG = TranslatorConfiguration.IMP;
            ArrayList<String> whiteList = new ArrayList<>();
            if (CONFIG.MAIN.INCLUDES != null) {
                for (String match : CONFIG.MAIN.INCLUDES) {
                    StringBuilder stringBuilder = new StringBuilder();
                    if (StringUtils.isNotEmpty(match)) {
                        stringBuilder.append(match.replaceAll("\\.", "/"));
                    }
                    whiteList.add(stringBuilder.toString());
                }
            }
            ArrayList<String> blackList = new ArrayList<>();
            if (CONFIG.MAIN.EXCLUDES != null) {
                for (String match : CONFIG.MAIN.EXCLUDES) {
                    StringBuilder stringBuilder = new StringBuilder();
                    if (StringUtils.isNotEmpty(match)) {
                        stringBuilder.append(match.replaceAll("\\.", "/"));
                    }
                    blackList.add(stringBuilder.toString());
                }
            }
            getProcessor().setClassFilter(new ClassFilter(blackList, whiteList, TranslatorConfiguration.IMP.MAIN.USE_ANNOTATIONS));
        }

        private List<Path> collectLibraries(File librariesDirectory) throws IOException {
            List<Path> libs = new ArrayList<>();
            if (librariesDirectory != null && librariesDirectory.exists()) {
                try (Stream<Path> walker = walk(librariesDirectory.toPath(), FileVisitOption.FOLLOW_LINKS)) {
                    walker.filter(path -> path.toString().endsWith(".jar") || path.toString().endsWith(".zip")).forEach(libs::add);
                }
            }
            return libs;
        }

        private Path prepareOutputPath(String outputDir, File inputFile) {
            Path outputPath = Paths.get(outputDir);

            if (Files.isDirectory(outputPath)) {
                return outputPath.resolve(inputFile.getName());
            }
            return outputPath;
        }
    }

    static {
        System.setProperty("slf4j.internal.verbosity", "WARN");
        LOGGER = LoggerFactory.getLogger("Translator");
        COMPILE_LOGGER = LoggerFactory.getLogger("Compiler");
    }
}