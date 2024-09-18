package com.rusefi.core.net;

import com.devexperts.logging.Logging;
import org.jetbrains.annotations.NotNull;

import java.net.URL;
import java.security.CodeSource;

public class JarFileUtil {
    private static final Logging log = Logging.getLogging(JarFileUtil.class);

    public static String getJarFileNamePrefix() {
        CodeSource codeSource = ConnectionAndMeta.class.getProtectionDomain().getCodeSource();
        if (codeSource == null)
            throw new IllegalStateException("No codeSource");
//        try {
            String jarFileName = getJarFileName(codeSource);
            int startOfSuffix = jarFileName.lastIndexOf('_');
            if (startOfSuffix == -1)
                throw new IllegalStateException("Unexpected jar file name: " + jarFileName);
            String jarFilePrefix = jarFileName.substring(0, startOfSuffix);
            log.info("jarFilePrefix = " + jarFilePrefix);
            return jarFilePrefix;
//        } catch (NullPointerException e) {
//            log.error("Unexpected " + e, e);
//        }
//        return "rusefi";
    }

    private static @NotNull String getJarFileName(CodeSource codeSource) {
        URL jarUrl = codeSource.getLocation();
        String jarPath = jarUrl.getPath();
        String jarFileName = jarPath.substring(jarPath.lastIndexOf('/') + 1);
        return jarFileName;
    }
}
