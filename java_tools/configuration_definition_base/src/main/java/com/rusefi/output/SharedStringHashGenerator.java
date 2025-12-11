package com.rusefi.output;

import com.rusefi.VariableRegistry;
import com.rusefi.util.LazyFile;

import java.io.IOException;

/**
 * Reads integration/rusefi_config_shared.txt and generates hash defines for all string defines.
 * is used on configuration wizard code (both fw and ts plugin)
 * For each NAME defined as "value" it emits:
 *   // value
 *   #define NAME_HASH <hash(value)>
 */
public class SharedStringHashGenerator {
    public static final String INPUT_FILE = "integration/rusefi_config_shared.txt";
    public static final String OUTPUT_FILE = "controllers/generated/generated_rusefi_config_hashes.h";

    public static String generateContent(VariableRegistry variableRegistry) {
        StringBuilder sb = new StringBuilder();

        for (String key : variableRegistry.getKeys()) {
            String value = variableRegistry.get(key);
            if (VariableRegistry.isQuoted(value, '"')) {
                String unquoted = VariableRegistry.unquote(value);
                int hash = HashUtil.hash(unquoted);
                sb.append("// ").append(unquoted).append("\n");
                sb.append("#define ").append(key).append("_HASH ").append(hash).append("\n");
            }
        }

        return sb.toString();
    }

    public static void run(LazyFile.LazyFileFactory fileFactory) throws IOException {
        VariableRegistry vr = new VariableRegistry();
        try {
            vr.readPrependValues(INPUT_FILE, false);
        } catch (IllegalStateException e) {
            if (e.getCause() instanceof java.io.FileNotFoundException) {
                return;
            }
            throw e;
        }

        String content = generateContent(vr);

        try (LazyFile lf = fileFactory.create(OUTPUT_FILE)) {
            lf.write(content);
        }
    }
}
