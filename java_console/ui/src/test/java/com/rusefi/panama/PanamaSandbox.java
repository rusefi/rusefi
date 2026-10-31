package com.rusefi.panama;

import java.io.File;
import java.io.IOException;

/**
 * Manual harness that uploads a file to the rusEFI Panama files API.
 */
public class PanamaSandbox {
    public static void main(String[] args) throws IOException {
        PanamaClient.uploadFile("https://rusefi.com/test" +
                PanamaClient.FILES_API,
            new File("build.gradle"),
            "My name is John X",
            239239);
    }
}
