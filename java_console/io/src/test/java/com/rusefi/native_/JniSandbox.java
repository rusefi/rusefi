package com.rusefi.native_;

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
        System.out.println("Checking " + libPath);
        String[] list = new File(libPath).list((dir, name) -> name.contains(LIBNAME));
        System.out.println(" " + Arrays.toString(list));
        System.loadLibrary(LIBNAME);
    }
}
