package com.rusefi.maintenance;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.Arrays;
import java.util.Locale;

public final class OpenbltJni {
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
        System.loadLibrary("openblt_jni");

        if (IS_MAC) {
            // MacOS won't run load a dylib from the current directory, it has
            // to be copied to the user's library first
            String targetDir = "~/Library/Java/Extensions";
            new File(targetDir).mkdirs();

            String[] libs = { "libopenblt.dylib", "libopenblt_jni.dylib" };

            try {
                for (String l : libs) {
                    Files.copy(Paths.get(l), Paths.get(targetDir + "/" + l), StandardCopyOption.REPLACE_EXISTING);
                }
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

    public static native void flashSerial(String filename, String serialPort, OpenbltCallbacks callbacks);
    public static native void flashCan(String filename, OpenbltCallbacks callbacks);
    public static native void stop(OpenbltCallbacks callbacks);
}
