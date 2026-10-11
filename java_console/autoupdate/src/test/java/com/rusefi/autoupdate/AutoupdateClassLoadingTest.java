package com.rusefi.autoupdate;

import com.rusefi.core.OsUtil;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.attribute.PosixFileAttributeView;
import java.nio.file.attribute.PosixFilePermission;
import java.util.EnumSet;
import java.util.concurrent.TimeUnit;
import java.util.jar.JarEntry;
import java.util.jar.JarInputStream;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class AutoupdateClassLoadingTest {
    private static final String OS_UTIL_CLASS = "com/rusefi/core/OsUtil.class";

    @TempDir
    Path temporaryDirectory;

    @Test
    void installedWindowsLauncherAfterUpdate() throws Exception {
        checkLauncherAfterUpdate("Windows 11");
    }

    @Test
    void installedLinuxLauncherAfterUpdate() throws Exception {
        checkLauncherAfterUpdate("Linux");
    }

    @Test
    void installedMacLauncherAfterUpdate() throws Exception {
        checkLauncherAfterUpdate("Mac OS X");
    }

    private void checkLauncherAfterUpdate(String osName) throws Exception {
        // A separate JVM prevents other tests from accidentally preloading OsUtil.
        // Unpack its JAR so the child can make the bytecode unavailable
        // after startup, modelling the running updater JAR being overwritten.
        Path originalClasses = Paths.get(OsUtil.class.getProtectionDomain().getCodeSource().getLocation().toURI());
        Path copiedClasses = temporaryDirectory.resolve("classes");
        try (InputStream input = Files.newInputStream(originalClasses);
             JarInputStream jar = new JarInputStream(input)) {
            JarEntry entry;
            while ((entry = jar.getNextJarEntry()) != null) {
                Path destination = copiedClasses.resolve(entry.getName());
                if (entry.isDirectory()) {
                    Files.createDirectories(destination);
                } else {
                    Files.createDirectories(destination.getParent());
                    Files.copy(jar, destination);
                }
            }
        }

        String[] classpath = System.getProperty("autoupdate.test.classpath").split(File.pathSeparator);
        boolean replaced = false;
        for (int i = 0; i < classpath.length; i++) {
            if (Paths.get(classpath[i]).toAbsolutePath().normalize().equals(originalClasses.toAbsolutePath().normalize())) {
                classpath[i] = copiedClasses.toString();
                replaced = true;
            }
        }
        assertTrue(replaced, "Isolate the shared_io classes on the child classpath");

        Path launcher = temporaryDirectory.resolve("installed launcher");
        Files.createFile(launcher);
        boolean supportsPosixPermissions = Files.getFileAttributeView(launcher, PosixFileAttributeView.class) != null;
        if (supportsPosixPermissions) {
            Files.setPosixFilePermissions(launcher, EnumSet.of(
                PosixFilePermission.OWNER_READ, PosixFilePermission.OWNER_WRITE));
        }
        Files.write(temporaryDirectory.resolve("installation.properties"),
            "console-exe-file=installed launcher\n".getBytes(StandardCharsets.UTF_8));
        Path output = temporaryDirectory.resolve("child.log");
        String javaName = OsUtil.isWindows() ? "java.exe" : "java";
        Process child = new ProcessBuilder(
            Paths.get(System.getProperty("java.home"), "bin", javaName).toString(),
            "-cp", String.join(File.pathSeparator, classpath),
            AutoupdateClassLoadingTest.class.getName(), osName,
            copiedClasses.resolve(OS_UTIL_CLASS).toString())
            .directory(temporaryDirectory.toFile())
            .redirectErrorStream(true)
            .redirectOutput(output.toFile())
            .start();
        try {
            assertTrue(child.waitFor(30, TimeUnit.SECONDS), "Child JVM timed out");
            String log = new String(Files.readAllBytes(output), StandardCharsets.UTF_8);
            assertEquals(0, child.exitValue(), log);
            if (supportsPosixPermissions) {
                assertEquals(!osName.contains("Windows"),
                    Files.getPosixFilePermissions(launcher).contains(PosixFilePermission.OWNER_EXECUTE),
                    "Repair Unix launcher permissions and leave Windows launchers unchanged");
            }
        } finally {
            child.destroyForcibly();
        }
    }

    public static void main(String[] args) throws IOException {
        System.setProperty("os.name", args[0]);
        // Initialize the updater while all its dependencies are still available.
        Autoupdate.isAutoUpdateEnabled(false, false);
        String launcher = new ConsoleExeFileLocator().getConsoleExeFileName();
        if (!"installed launcher".equals(launcher)) {
            throw new AssertionError("Expected the installation.properties launcher");
        }
        Files.delete(Paths.get(args[1]));
        Autoupdate.ensureLauncherIsExecutable(launcher);
    }
}
