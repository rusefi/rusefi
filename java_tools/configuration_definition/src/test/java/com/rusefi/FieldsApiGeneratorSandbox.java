package com.rusefi;

import java.io.IOException;

/**
 * Manual harness that runs the fields-API header generator.
 */
public class FieldsApiGeneratorSandbox {
    public static void main(String[] args) throws IOException {
        RootHolder.ROOT = "../firmware/";

        FieldsApiGenerator.run("controllers/generated/generated_fields_api_header.h");
    }
}
