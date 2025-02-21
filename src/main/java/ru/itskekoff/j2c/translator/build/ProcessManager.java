package ru.itskekoff.j2c.translator.build;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UncheckedIOException;
import java.nio.file.Path;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.function.Consumer;

public class ProcessManager {
    private static final ExecutorService executor = Executors.newFixedThreadPool(2);

    private static void readStream(InputStream is, Consumer<String> consumer) {
        executor.submit(() -> {
            try (InputStreamReader isr = new InputStreamReader(is);
                 BufferedReader reader = new BufferedReader(isr);) {
                int count;
                char[] buf = new char[1024];
                while ((count = reader.read(buf)) != -1) {
                    consumer.accept(String.copyValueOf(buf, 0, count));
                }
            } catch (IOException e) {
                throw new UncheckedIOException(e);
            }
        });
    }

    public static ProcessResult run(Path directory, long timeLimit, List<String> command) throws IOException {
        Process process = new ProcessBuilder(command).directory(directory.toFile()).start();
        long startTime = System.currentTimeMillis();
        ProcessResult result = new ProcessResult();
        result.commandLine = String.join((CharSequence) " ", command);
        StringBuilder stdoutBuilder = new StringBuilder();
        StringBuilder stderrBuilder = new StringBuilder();
        ProcessManager.readStream(process.getInputStream(), stdoutBuilder::append);
        ProcessManager.readStream(process.getErrorStream(), stderrBuilder::append);
        try {
            if (!process.waitFor(timeLimit, TimeUnit.MILLISECONDS)) {
                result.timeout = true;
                process.destroyForcibly();
            }
            process.waitFor();
        } catch (InterruptedException interruptedException) {
            // empty catch block
        }
        result.stdout = stdoutBuilder.toString();
        result.stderr = stderrBuilder.toString();
        result.execTime = System.currentTimeMillis() - startTime;
        result.exitCode = process.exitValue();
        return result;
    }

    public static class ProcessResult {
        public int exitCode;
        public long execTime;
        public boolean timeout;
        public String stdout = "";
        public String stderr = "";
        public String commandLine;

        public void check(String processName) {
            if (!this.timeout && this.exitCode == 0) {
                return;
            }
            if (this.timeout) {
                System.err.println(processName);
            } else if (this.commandLine.contains("zig") && this.commandLine.contains("jnic")) {
                System.err.println(processName);
                System.err.println("zig:" + SetupManager.getZigGlobalCacheDirectory(true));

            }
            System.err.println("stdout: \n" + this.stdout);
            System.err.println("stderr: \n" + this.stderr);
            throw new RuntimeException(processName);
        }
    }
}
