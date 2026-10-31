package com.rusefi.tune_manifest;

import com.rusefi.AvailableHardware;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertSame;

public class TuneManifestHelperTest {
    @Test
    public void test() {
        assertEquals("docsimp123_-aonscomtunes", TuneManifestHelper.trimUrlToLocalName("https://docs.imp123_-aons.com/tunes/"));
    }

    @Test
    public void customParsingAndFiltering(@TempDir Path tempDir) throws Exception {
        Path manifest = tempDir.resolve("manifest.json");
        Files.write(manifest, ("["
            + "{\"MapID\":\"0001\",\"Notes\":\"one\",\"Url\":\"https://example.com/one.msq\",\"EngineSize\":\"1\",\"FileName\":\"One\",\"Variant\":\"skip\"},"
            + "{\"MapID\":\"0002\",\"Notes\":\"two\",\"Url\":\"https://example.com/two.msq\",\"EngineSize\":\"2\",\"FileName\":\"Two\",\"Variant\":\"keep\"}"
            + "]").getBytes(StandardCharsets.UTF_8));

        assertEquals(2, TuneManifestHelper.parseManifest(manifest.toString()).size());

        AvailableHardware hardware = new AvailableHardware(Collections.emptyList(), false, false, false);
        TuneManifestExtension extension = new TuneManifestExtension() {
            @Override
            public TuneModel parse(org.json.simple.JSONObject source, TuneModel standardModel) {
                return new ExtendedTuneModel(standardModel, (String) source.get("Variant"));
            }

            @Override
            public boolean includeTune(TuneModel tune, AvailableHardware receivedHardware) {
                assertSame(hardware, receivedHardware);
                return "keep".equals(((ExtendedTuneModel) tune).variant);
            }
        };
        List<TuneModel> tunes = TuneManifestHelper.parseManifest(manifest.toString(), extension, hardware);

        assertEquals(1, tunes.size());
        assertEquals("0002", tunes.get(0).getMapId());
    }

    private static class ExtendedTuneModel extends TuneModel {
        private final String variant;

        private ExtendedTuneModel(TuneModel standardModel, String variant) {
            super(standardModel.getNotes(), standardModel.getUrl(), standardModel.getSize(),
                standardModel.getMapId(), standardModel.getFileName());
            this.variant = variant;
        }
    }
}
