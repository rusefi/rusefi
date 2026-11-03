package com.rusefi.maintenance;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Collections;
import java.util.Set;

import static javax.management.ObjectName.quote;
import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

class KansasLimaTuningDetectorMigrationTest {
    private static final String FIELD = "isTuningDetectorEnabled";
    private static final String KANSAS = "rusEFI lts-25kansas.2026.08.02.proteus_f7.123";
    private static final String LIMA = "rusEFI lts-26lima.2026.09.02.proteus_f7.456";

    @TempDir
    Path tempDir;

    @Test
    void firmwareUpdateAndMsqImportEnableDetection() throws Exception {
        for (boolean importMsq : new boolean[]{false, true}) {
            for (String previous : new String[]{"no", "yes"}) {
                for (String destination : new String[]{"no", "yes"}) {
                    // Kansas's inverted "no" must not disable the corrected detector (rusEFI #9309).
                    checkMigration(KANSAS, LIMA, previous, destination, importMsq,
                        Collections.emptySet(), "yes");
                }
            }
        }
    }

    @Test
    void developmentReleaseSignatures() throws Exception {
        checkMigration(KANSAS, LIMA.replace("lts-26lima", "lts-26lima-dev"),
            "no", "yes", false, Collections.emptySet(), "yes");
        checkMigration(KANSAS.replace("lts-25kansas", "lts-25kansas-dev"), LIMA,
            "no", "yes", true, Collections.emptySet(), "yes");
    }

    @Test
    void otherTransitionsKeepTheSourceChoice() throws Exception {
        for (String source : new String[]{LIMA, "", "unknown lts-25kansas.foo",
            "rusEFI lts-25kansas.foo", KANSAS.replace("lts-25kansas", "master"),
            KANSAS.replace("lts-25kansas", "lts-25kansas-custom")}) {
            checkMigration(source, LIMA, "no", "yes", true, Collections.emptySet(), "no");
        }
        for (String destination : new String[]{KANSAS, "unknown lts-26lima.foo",
            LIMA.replace("lts-26lima", "master"), LIMA.replace("lts-26lima", "lts-26lima-custom")}) {
            checkMigration(KANSAS, destination, "no", "yes", false, Collections.emptySet(), "no");
        }
    }

    @Test
    void explicitExclusionPreservesDestination() throws Exception {
        checkMigration(KANSAS, LIMA, "yes", "no", true, Collections.singleton(FIELD), "no");
    }

    private void checkMigration(String sourceSignature, String destinationSignature,
                                String previous, String destination, boolean importMsq,
                                Set<String> ignoredFields, String expected) throws Exception {
        CalibrationsInfo source = calibration(sourceSignature, previous, false);
        CalibrationsInfo target = calibration(destinationSignature, destination, true);
        // The updater generates its source MSQ from the ECU image; import reads XML instead.
        Msq tune = source.generateMsq();
        if (importMsq) {
            Path msq = tempDir.resolve("source.msq");
            tune.writeXmlFile(msq.toString());
            tune = Msq.readTune(msq.toString());
        }
        assertEquals(quote(previous), tune.getConstantsAsMap().get(FIELD).getValue());
        byte[] original = target.getImage().getConfigurationImage().getContent().clone();

        CalibrationsHelper.MergeResult result = CalibrationsHelper.mergeCalibrationsWithPartialFailure(
            source.getIniFile(), tune, target, mock(UpdateOperationCallbacks.class), ignoredFields);

        assertTrue(result.failedFields.isEmpty(), result.failedFields.toString());
        CalibrationsInfo merged = result.mergedCalibrations.orElse(target);
        assertEquals(quote(expected), merged.generateMsq().getConstantsAsMap().get(FIELD).getValue());
        byte[] expectedBytes = original.clone();
        expectedBytes[4] = (byte) ((expectedBytes[4] & ~0x20) | (expected.equals("yes") ? 0x20 : 0));
        assertArrayEquals(expectedBytes, merged.getImage().getConfigurationImage().getContent());
        assertArrayEquals(original, target.getImage().getConfigurationImage().getContent());
        if (!expected.equals(destination)) {
            assertTrue(merged.getPagesToWrite().contains(0));
        }
    }

    private CalibrationsInfo calibration(String signature, String value, boolean destination) throws Exception {
        Path file = Files.createTempFile(tempDir, "calibration", ".ini");
        String iniText = "[MegaTune]\nsignature = \"" + signature + "\"\n"
            + "[Constants]\nnPages = 1\npageSize = 8\npageReadCommand = \"R\"\nburnCommand = \"B\"\n"
            + "page = 1\n" + FIELD + " = bits, U32, " + (destination ? "4, [5:5]" : "0, [3:3]")
            + ", \"no\", \"yes\"\n[OutputChannels]\nochBlockSize = 4\n";
        Files.write(file, iniText.getBytes(StandardCharsets.UTF_8));
        IniFileModel ini = IniFileReaderUtil.readIniFile(file.toString());
        ConfigurationImage image = new ConfigurationImage(new byte[]{0x12, 0x34, 0x56, 0x78,
            0x55, (byte) 0xaa, 0x5a, (byte) 0xa5});
        ConfigurationImageGetterSetter.setValue2(ini.getIniField(FIELD), image, FIELD, quote(value));
        return new CalibrationsInfo(ini, ConfigurationImageWithMeta.valueOf(ini, image));
    }
}
