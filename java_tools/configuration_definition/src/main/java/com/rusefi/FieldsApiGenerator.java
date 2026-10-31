package com.rusefi;

import com.rusefi.output.HashUtil;
import com.rusefi.util.LazyFile;

import java.io.IOException;

public class FieldsApiGenerator {
    public static void run() throws IOException {
        VariableRegistry variableRegistry = new VariableRegistry();
        variableRegistry.readPrependValues("integration/fields_api.txt", false);

        StringBuilder sb = new StringBuilder();

        for (String key : variableRegistry.getKeys()) {
            String value = variableRegistry.get(key);
            int hash = HashUtil.hash(value);
            sb.append("// " + value + "\n");
            sb.append("#define " + key + "_HASH " + hash + "\n");

        }
        LazyFile lazyFile = LazyFile.REAL.create("controllers/generated/generated_fields_api_header.h");

        lazyFile.write(sb);
        lazyFile.close();
    }
}
