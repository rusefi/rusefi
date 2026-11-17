package com.rusefi.ldmp;

import com.rusefi.output.SdCardFieldsContent;
import org.junit.jupiter.api.Test;

import java.io.StringReader;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class SdCardFieldsAbiCompatibilityTest {
    @Test
    void emitsRetainedFieldConstructorsAndSkipsEnumsAndModuleOnlyEntries() throws Exception {
        String yaml = "Usages:\n"
                + "  - name: legacy\n"
                + "    java: Legacy.java\n"
                + "    folder: test\n"
                + "    constexpr: engine->legacy\n"
                + "  - name: module_only\n"
                + "    java: Module.java\n"
                + "    folder: test\n"
                + "    engineModule: TestModule\n";
        TestFileCaptor captor = new TestFileCaptor();
        LiveDataProcessor processor = new LiveDataProcessor("test", fileName -> new StringReader(
                fileName.contains("module_only")
                        ? "struct_no_prefix module_only_s\nfloat value;Module value;\nend_struct\n"
                         : "struct_no_prefix legacy_s\n"
                         + "custom sample_mode_e 4 bits, U32, @OFFSET@, [0:1], \"Off\", \"On\"\n"
                        + "float mass;Mass;\"kg\", 1000, -1, 0, 100, 2\n"
                        + "bit enabled\n"
                        + "sample_mode_e mode;Mode;\"\", 1, 0, 0, 2, 0\n"
                        + "end_struct\n"), captor, "");

        processor.handleYaml(LiveDataProcessor.getStringObjectMap(new StringReader(yaml)));

        String sd = captor.fileCapture.get(SdCardFieldsContent.SD_CARD_OUTPUT_FILE_NAME).sb.toString();
        assertTrue(sd.contains("{packedTime, GAUGE_NAME_TIME, \"sec\", 3}"), sd);
        assertTrue(sd.contains("{engine->legacy.mass, \"Mass\", \"kg\", 2}"), sd);
        assertTrue(sd.contains("{engine->legacy, 4, 0, \"enabled\", \"\"}"), sd);
        assertFalse(sd.contains("mode"), sd);
        assertFalse(sd.contains("MLG_SOURCE"), sd);
        assertFalse(sd.contains("1000.0f"), sd);
        assertFalse(sd.contains("module_only"), sd);
    }
}
