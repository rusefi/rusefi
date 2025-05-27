package com.rusefi.autoupdate;

import com.rusefi.core.net.JarFileUtil;

public class ConsoleExeFileLocator {
    String getConsoleExeFileName() {
        return JarFileUtil.getJarFileNamePrefix() + "_console.exe";
    }
}
