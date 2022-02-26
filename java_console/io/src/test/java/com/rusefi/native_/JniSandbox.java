package com.rusefi.native_;

/**
 * Following VM option is needed to launch:
 * -Djava.library.path=../unit_tests/build
 */
public class JniSandbox {
    public static void main(String[] args) {
        System.loadLibrary("lib_rusefi_test");
        EngineLogic engineLogic = new EngineLogic();
        System.out.println(engineLogic.getVersion());
    }
}
