package com.rusefi.native_;

import com.rusefi.FileLog;

import javax.xml.bind.SchemaOutputResolver;
import java.io.File;
import java.util.Arrays;

/**
 * Following VM option is needed to launch:
 * -Djava.library.path=../unit_tests/build
 */
public class JniSandbox {

    private static final String LIBNAME = "_rusefi_test";

    public static void main(String[] args) {
        loadLibrary();

        EngineLogic engineLogic = new EngineLogic();
        System.out.println(engineLogic.getVersion());
        engineLogic.setSensor("clt", 90);
        engineLogic.setConfiguration(new byte[4], 24, 14);
    }

    public static void loadLibrary() {
        String libPath = System.getProperty("java.library.path");
        System.out.println("Looking for libraries at " + libPath);
        String[] list = new File(libPath).list((dir, name) -> name.contains(LIBNAME));
        System.out.println("Matching files: " + Arrays.toString(list));
        if (list.length == 0) {
            if (FileLog.isWindows()) {
                System.out.println("Have you invoked `make -j4 SANITIZE=no build/_rusefi_test'?");
            } else {
                System.out.println("Have you invoked `make -j4 SANITIZE=no build/lib_rusefi_test`?");
            }
        }
        System.loadLibrary(LIBNAME);
    }
}
