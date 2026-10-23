package com.rusefi.test;

import com.rusefi.ReaderProvider;
import com.rusefi.ldmp.LiveDataProcessor;
import com.rusefi.util.LazyFile;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.Reader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;

class WallFuelTelemetryTest {
    private static final Path FIRMWARE = Path.of(ConfigDefinitionTest.FIRMWARE);

    // [tag:wall_fuel_telemetry] Exercise the real definitions and YAML wiring, including SD/Lua exports.
    @Test
    @SuppressWarnings("unchecked") // LiveDataProcessor's YAML API returns raw maps.
    void wallFuelChannelsAndConsumers() throws IOException {
        List<LinkedHashMap> entries;
        try (Reader reader = Files.newBufferedReader(FIRMWARE.resolve("integration/LiveData.yaml"))) {
            entries = LiveDataProcessor.getStringObjectMap(reader);
        }
        entries.removeIf(entry -> !entry.get("name").equals("output_channels")
                && !entry.get("name").equals("wall_fuel_state"));
        for (LinkedHashMap entry : entries) {
            entry.put("input_folder", FIRMWARE.resolve((String) entry.get("folder")).toString());
            if (entry.get("prepend") != null) {
                entry.put("prepend", FIRMWARE.resolve((String) entry.get("prepend")).toString());
            }
        }

        Map<String, StringBuilder> files = new HashMap<>();
        LazyFile.LazyFileFactory capture = name -> {
            StringBuilder content = new StringBuilder();
            files.put(name, content);
            return new LazyFile() {
                public void write(String line) {
                    content.append(line);
                }

                public void close() {
                }
            };
        };
        LiveDataProcessor processor = new LiveDataProcessor("wall fuel test", ReaderProvider.REAL, capture, "");
        processor.handleYaml(entries);

        String channels = files.get(processor.getOutputsSectionFileName()).toString();
        String logs = files.get(processor.getDataLogFileName()).toString();
        String lookup = files.get(LiveDataProcessor.getOutputLookupFileName()).toString();
        String sd = files.get("console/binary/generated/log_fields_generated.h").toString();

        assertTrue(channels.matches("(?s).*wallFuelCorrection = scalar, F32, \\d+, \"mg\", 1000.0, 0.*"));
        assertTrue(channels.matches("(?s).*wallFuel = scalar, F32, \\d+, \"mg\", 1000.0, 0.*"));
        assertTrue(logs.contains("entry = wallFuelCorrection, \"Fuel Wall: correction\""));
        assertTrue(logs.contains("entry = wallFuel, \"Fuel Wall: amount\""));
        for (String output : new String[]{channels, logs, lookup, sd}) {
            assertFalse(output.contains("wallFuelCorrectionValue"));
            assertFalse(output.contains("wallFuelAmount"));
        }
        assertTrue(lookup.contains("return engine->injectionEvents.elements[0].getWallFuel().wallFuelCorrection * 1000.0f;"));
        assertTrue(lookup.contains("return engine->injectionEvents.elements[0].getWallFuel().wallFuel * 1000.0f;"));
        assertTrue(sd.contains("MLG_SOURCE(engine->injectionEvents.elements[0].getWallFuel().wallFuelCorrection), \"Fuel Wall: correction\", \"mg\", 3, \"none\", 1000.0f"));
        assertTrue(sd.contains("MLG_SOURCE(engine->injectionEvents.elements[0].getWallFuel().wallFuel), \"Fuel Wall: amount\", \"mg\", 3, \"none\", 1000.0f"));

        // TODO: fix
        //String header = files.get(Path.of("live_data_generated", "wall_fuel_state_generated.h").toString()).toString();
        //assertTrue(header.contains("Positive adds fuel, negative subtracts fuel."));
        //assertTrue(header.contains("Estimated fuel mass on the intake port wall"));

        String gauges = Files.readString(FIRMWARE.resolve("tunerstudio/gauge_declarations.ini"));
        assertTrue(gauges.contains("wallFuelAmountGauge = wallFuel, @@GAUGE_NAME_FUEL_WALL_AMOUNT@@, \"mg\""));
        assertTrue(gauges.contains("wallFuelCorrectionGauge = wallFuelCorrection, @@GAUGE_NAME_FUEL_WALL_CORRECTION@@, \"mg\""));
    }
}
