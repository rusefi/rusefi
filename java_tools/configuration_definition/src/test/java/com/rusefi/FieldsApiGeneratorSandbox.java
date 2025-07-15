package com.rusefi;

import java.io.IOException;

public class FieldsApiGeneratorSandbox {
    public static void main(String[] args) throws IOException {
        RootHolder.ROOT = "../firmware/";

        FieldsApiGenerator.run();
    }
}
