package com.rusefi.core;

/**
 * Manual harness for {@link FindFileHelper} .srec file lookup.
 */
public class FindFileHelperSandbox {
    public static void main(String[] args) {
        FindFileHelper.INPUT_FILES_PATH = "C:\\stuff\\bundles\\rusefi_bundle_mre_f4\\rusefi.snapshot.mre_f4";
        FindFileHelper.findSrecFile(false);
    }
}
