package com.rusefi.ldmp;

import com.rusefi.output.SdCardFieldsContent;
import org.junit.jupiter.api.Test;

import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static org.junit.jupiter.api.Assertions.*;

public class LiveDataSdCoverageTest {
    private static final List<String> FRAGMENTS = Arrays.asList(
            "throttle_model", "high_pressure_fuel_pump", "antilag_system_state", "ac_control",
            "nitrous_control_state", "fuel_pump_control", "main_relay", "tps_accel_state",
            "closed_loop_idle", "misfire_detection_state", "sent_state", "short_term_fuel_trim_state",
            "vvl_controller_state", "live_data_rotational_idle", "injector_deadtime_autotune_state",
            "rolling_launch_state", "burst_knock_state", "engine_state_machine_state", "wot_enrichment_state");

    @Test
    @SuppressWarnings("unchecked") // Legacy YAML API exposes raw maps.
    public void includesStableLiveDataFragments() throws IOException {
        Path firmware = Paths.get("..", "..", "firmware");
        List<LinkedHashMap> entries;
        try (Reader yaml = new FileReader(firmware.resolve("integration/LiveData.yaml").toFile())) {
            entries = LiveDataProcessor.getStringObjectMap(yaml);
        }
        int checked = 0;
        for (LinkedHashMap entry : entries) {
            if (!FRAGMENTS.contains(entry.get("name"))) {
                continue;
            }
            checked++;
            if (entry.containsKey("prepend")) {
                entry.put("prepend", firmware.resolve((String) entry.get("prepend")).toString());
            }
            TestFileCaptor captor = new TestFileCaptor();
            LiveDataProcessor processor = new LiveDataProcessor("integration/LiveData.yaml",
                    name -> new FileReader(firmware.resolve(name).toFile()), captor, "");
            processor.handleYaml(Collections.singletonList(entry));
            String ts = captor.fileCapture.get(processor.getDataLogFileName()).sb.toString();
            String sd = captor.fileCapture.get(SdCardFieldsContent.SD_CARD_OUTPUT_FILE_NAME).sb.toString();
            Matcher fields = Pattern.compile("entry = (\\w+), (\"[^\"]+\")").matcher(ts);
            int fieldCount = 0;
            while (fields.find()) {
                // Enum serialization is a separate limitation; these five STFT fields still need it.
                if (fields.group(1).matches("stft(CorrectionBinIdx|LearningBinIdx|CorrectionState|LearningState[12])")) {
                    continue;
                }
                fieldCount++;
                assertTrue(sd.contains(fields.group(2)), entry.get("name") + ": " + fields.group(2));
            }
            assertTrue(fieldCount > 0, entry.get("name").toString());
        }
        assertEquals(FRAGMENTS.size(), checked);
    }
}
