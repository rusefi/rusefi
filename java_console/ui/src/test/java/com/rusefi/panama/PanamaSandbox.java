package com.rusefi.panama;

import java.io.File;
import java.io.IOException;

public class PanamaSandbox {

    public static void main(String[] args) throws IOException {

        PanamaClient.uploadFile("https://rusefi.com/panama-test/a.php",
            new File("build.gradle"),
            "My name is John X",
            239239);
    }
}
