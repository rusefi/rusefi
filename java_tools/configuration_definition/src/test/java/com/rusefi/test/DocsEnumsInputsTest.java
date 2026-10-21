package com.rusefi.test;

import com.rusefi.ldmp.LiveDataProcessor;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.Reader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * [tag:docs_enums_inputs] gen_live_documentation.sh only runs when one of DOCS_ENUMS_INPUTS in firmware/docs_enums.mk
 * changes. A live data .txt missing from that list is silently ignored by incremental builds: the ini, log fields
 * and lookups keep stale content until something else triggers regeneration.
 */
class DocsEnumsInputsTest {
    private static final Path FIRMWARE = Path.of(ConfigDefinitionTest.FIRMWARE);
    private static final Pattern MK_INPUT = Pattern.compile("\\$\\(PROJECT_DIR\\)/(\\S+)");
    private static final Pattern SCRIPT_INPUT = Pattern.compile("\\b(integration/[\\w/]+\\.(?:txt|yaml))\\b");

    @Test
    @SuppressWarnings("unchecked") // LiveDataProcessor's YAML API returns raw maps.
    void allGeneratorInputsAreMakeDependencies() throws IOException {
        Set<String> required = new TreeSet<>();

        List<LinkedHashMap> entries;
        try (Reader reader = Files.newBufferedReader(FIRMWARE.resolve("integration/LiveData.yaml"))) {
            entries = LiveDataProcessor.getStringObjectMap(reader);
        }
        for (LinkedHashMap entry : entries) {
            String folder = (String) entry.get("input_folder");
            if (folder == null)
                folder = (String) entry.get("folder");
            required.add(normalize(folder + "/" + entry.get("name") + ".txt"));
            String prepend = (String) entry.get("prepend");
            if (prepend != null)
                required.add(normalize(prepend));
        }

        Matcher scriptInputs = SCRIPT_INPUT.matcher(Files.readString(FIRMWARE.resolve("gen_live_documentation.sh")));
        while (scriptInputs.find())
            required.add(scriptInputs.group(1));

        Set<String> listed = new TreeSet<>();
        Matcher mkInputs = MK_INPUT.matcher(Files.readString(FIRMWARE.resolve("docs_enums.mk")));
        while (mkInputs.find())
            listed.add(normalize(mkInputs.group(1)));

        required.removeAll(listed);
        assertEquals(new TreeSet<>(), required, "add these to DOCS_ENUMS_INPUTS in firmware/docs_enums.mk");
    }

    private static String normalize(String path) {
        return Path.of(path).normalize().toString().replace('\\', '/');
    }
}
