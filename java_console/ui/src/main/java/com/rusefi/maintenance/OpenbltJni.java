package com.rusefi.maintenance;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.Locale;
import java.util.Objects;

// careful: JNI is fragile!
public final class OpenbltJni {
    // careful: JNI is fragile! interface full name is part of magic API
    public interface OpenbltCallbacks
    {
        void log(String line);
        void updateProgress(int percent);
        void error(String line);

        void setPhase(String title, boolean hasProgress);
    }

    private static final String OS_NAME = System.getProperty("os.name").toLowerCase(Locale.ROOT);
    private static final boolean IS_MAC = OS_NAME.contains("mac") || OS_NAME.contains("darwin");

    static {
        if (IS_MAC) {
            // MacOS won't run load a dylib from the current directory, it has
            // to be copied to the user's library first
            String targetDir = System.getProperty("user.home") + "/Library/Java/Extensions";
            new File(targetDir).mkdirs();

            String[] libs = { "libopenblt.dylib", "libopenblt_jni.dylib" };

            try {
                for (String l : libs) {
                    Files.copy(Paths.get(l), Paths.get(targetDir + "/" + l), StandardCopyOption.REPLACE_EXISTING);
                }
            } catch (IOException e) {
                throw new RuntimeException(e);
            }

            // Sometimes macOS has difficulty finding libopenblt.dylib
            // Force it to load it first
            System.loadLibrary("openblt");
        }

        // Load our OpenBLT JNI lib we actually want to use
        System.loadLibrary("openblt_jni");
    }

    public static void flashCan(String filename, OpenbltCallbacks callbacks) {
        flashCanNative(filename, callbacks);
    }

    public static void stop(OpenbltCallbacks callbacks) {
        stopNative(callbacks);
    }

    private static native void flashSerialNative(String filename, String serialPort, OpenbltCallbacks callbacks);
    private static native void flashCanNative(String filename, OpenbltCallbacks callbacks);
    private static native void stopNative(OpenbltCallbacks callbacks);
}
