package com.rusefi.maintenance;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.config.FieldType;
import com.rusefi.binaryprotocol.IniNotFoundException;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.migrators.DefaultEnumIniFieldMigrator;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.MsqFactory;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.ArgumentMatchers.contains;

class OfflineTuneLoaderConnectedTest {
    @TempDir
    Path tempDir;

    @Test
    void connectedLoadDoesNotRequireConvertingTheMsqToASourceImage() throws IniParsingException {
        IniFileModel customIni = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel standardIni = pinIni("standard_pin_list", "0=\"NONE\",5=\"Output\"");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{47, 0}), customIni);

        OfflineTuneLoader.Result result = OfflineTuneLoader.loadConnectedTune(tune, "test-signature", standardIni);

        assertNotNull(result);
        assertSame(tune, result.msq);
        assertSame(standardIni, result.ini);
        assertNull(result.image);
    }

    @Test
    void failedOfflineConversionExplainsWhichIniToSelect() throws IniParsingException {
        IniFileModel customIni = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel standardIni = pinIni("standard_pin_list", "0=\"NONE\",5=\"Output\"");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{47, 0}), customIni);
        List<String> errors = new ArrayList<>();

        OfflineTuneLoader.loadResolvedTune(tune, "test-signature", standardIni, errors::add, signature -> null);

        assertEquals(1, errors.size());
        assertTrue(errors.get(0).contains("used to save this .msq tune"));
        assertTrue(errors.get(0).contains("not the destination ECU's .ini"));
    }

    @Test
    void missingAutomaticIniRecoversWithSelectedOriginalForOfflineLoad() throws IniParsingException {
        IniFileModel originalIni = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{47, 0}), originalIni);

        OfflineTuneLoader.Result result = OfflineTuneLoader.loadResolvedSourceIni(
            tune, "test-signature", ignored -> { }, signature -> originalIni, true,
            signature -> { throw new IniNotFoundException("not found"); });

        assertNotNull(result);
        assertSame(originalIni, result.ini);
        assertNotNull(result.image);
        assertEquals(47, Byte.toUnsignedInt(result.image.getContent()[0]));
    }

    @Test
    void missingAutomaticIniCancellationReportsSourceIniGuidance() throws IniParsingException {
        IniFileModel sourceIni = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{47, 0}), sourceIni);
        List<String> errors = new ArrayList<>();

        OfflineTuneLoader.Result result = OfflineTuneLoader.loadResolvedSourceIni(
            tune, "test-signature", errors::add, signature -> null, true,
            signature -> { throw new IniNotFoundException("not found"); });

        assertNull(result);
        assertTrue(errors.get(0).contains("used to save this .msq tune"));
        assertTrue(errors.get(0).contains("not the destination ECU's .ini"));
    }

    @Test
    void missingAutomaticIniRejectsSelectedIniWithDifferentSignature() throws IniParsingException {
        IniFileModel sourceIni = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel wrongIni = pinIni(
            "custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"", "different-signature");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{47, 0}), sourceIni);
        List<String> errors = new ArrayList<>();

        OfflineTuneLoader.Result result = OfflineTuneLoader.loadResolvedSourceIni(
            tune, "test-signature", errors::add, signature -> wrongIni, true,
            signature -> { throw new IniNotFoundException("not found"); });

        assertNull(result);
        assertTrue(errors.get(0).contains("does not match this tune"));
    }

    @Test
    void missingAutomaticIniRejectsSelectedIniMissingTuneFields() throws IniParsingException {
        IniFileModel originalIni = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel incompleteIni = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"",
            "test-signature", false);
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{47, 0}), originalIni);
        List<String> errors = new ArrayList<>();

        OfflineTuneLoader.Result result = OfflineTuneLoader.loadResolvedSourceIni(
            tune, "test-signature", errors::add, signature -> incompleteIni, true,
            signature -> { throw new IniNotFoundException("not found"); });

        assertNull(result);
        assertTrue(errors.get(0).contains("missing tune field outputPin"));
    }

    @Test
    void missingAutomaticIniAllowsConnectedLoadWithSelectedSourceSchema() throws IniParsingException {
        IniFileModel sourceIni = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{47, 0}), sourceIni);

        OfflineTuneLoader.Result result = OfflineTuneLoader.loadResolvedSourceIni(
            tune, "test-signature", ignored -> { }, signature -> sourceIni, false,
            signature -> { throw new IniNotFoundException("not found"); });

        assertNotNull(result);
        assertSame(sourceIni, result.ini);
        assertNull(result.image);
    }

    @Test
    void matchingDestinationSignatureLoadsWithoutSourceResolutionOrPicker() throws Exception, IniParsingException {
        IniFileModel destinationIni = pinIni("destination_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{47, 0}), destinationIni);
        Path file = tempDir.resolve("same-signature.msq");
        tune.writeXmlFile(file.toString());
        List<String> errors = new ArrayList<>();
        List<String> sourceRequests = new ArrayList<>();

        OfflineTuneLoader.Result result = OfflineTuneLoader.loadConnectedTuneFromFile(
            file.toString(), destinationIni, errors::add,
            signature -> {
                throw new AssertionError("matching destination signature must not prompt for a source INI");
            },
            signature -> {
                sourceRequests.add(signature);
                throw new IniNotFoundException("not found");
            });

        assertNotNull(result);
        assertNull(result.ini);
        assertEquals("test-signature", result.signature);
        assertTrue(sourceRequests.isEmpty());
        assertTrue(errors.isEmpty());
    }

    @Test
    void differentSignatureConnectedLoadResolvesTheGenuineSourceIni() throws Exception, IniParsingException {
        IniFileModel sourceIni = pinIni("source_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel destinationIni = pinIni(
            "destination_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"", "destination-signature");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{47, 0}), sourceIni);
        Path file = tempDir.resolve("different-signature.msq");
        tune.writeXmlFile(file.toString());
        List<String> sourceRequests = new ArrayList<>();

        OfflineTuneLoader.Result result = OfflineTuneLoader.loadConnectedTuneFromFile(
            file.toString(), destinationIni, ignored -> { },
            signature -> { throw new AssertionError("automatic source INI should resolve"); },
            signature -> {
                sourceRequests.add(signature);
                return sourceIni;
            });

        assertNotNull(result);
        assertSame(sourceIni, result.ini);
        assertEquals(List.of("test-signature"), sourceRequests);
    }

    @Test
    void destinationEnumLabelDoesNotRequireSourceMapping() {
        EnumIniField source = enumField(Map.of(0, "Other"));
        EnumIniField target = enumField(Map.of(0, "Other", 1, "Auxiliary pin"));

        Optional<String> migrated = DefaultEnumIniFieldMigrator.INSTANCE.tryMigrateValue(
            source, target, "\"Auxiliary pin\"", mock(UpdateOperationCallbacks.class));

        assertEquals(Optional.of("\"Auxiliary pin\""), migrated);
    }

    @Test
    void duplicateDestinationEnumLabelsAreNotResolvedByFirstMatch() {
        EnumIniField source = enumField(Map.of(0, "Other"));
        EnumIniField target = enumField(Map.of(0, "Auxiliary pin", 1, "Auxiliary pin"));

        Optional<String> migrated = DefaultEnumIniFieldMigrator.INSTANCE.tryMigrateValue(
            source, target, "\"Auxiliary pin\"", mock(UpdateOperationCallbacks.class));

        assertEquals(Optional.empty(), migrated);
    }

    @Test
    void unknownRawPinMarkerIsRejectedBeforeMatchingDestinationLabels() {
        String marker = EnumIniField.unknownOrdinalValue(33);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        EnumIniField source = new EnumIniField("outputPin", 0, FieldType.UINT16,
            new EnumIniField.EnumKeyValueMap(Map.of(0, "Other")), 0, 8, true);
        EnumIniField target = new EnumIniField("outputPin", 0, FieldType.UINT16,
            new EnumIniField.EnumKeyValueMap(Map.of(0, "Other", 1, marker)), 0, 8, true);

        Optional<String> migrated = DefaultEnumIniFieldMigrator.INSTANCE.tryMigrateValue(
            source, target, marker, callbacks);

        assertEquals(Optional.empty(), migrated);
        verify(callbacks).logLine(contains("this tune has an unknown pin"));
    }

    private static EnumIniField enumField(Map<Integer, String> labels) {
        return new EnumIniField("outputPin", 0, FieldType.INT16,
            new EnumIniField.EnumKeyValueMap(labels), 0, 8);
    }

    private static IniFileModel pinIni(String listName, String options) throws IniParsingException {
        return pinIni(listName, options, "test-signature", true);
    }

    private static IniFileModel pinIni(String listName, String options, String signature) throws IniParsingException {
        return pinIni(listName, options, signature, true);
    }

    private static IniFileModel pinIni(String listName, String options, String signature, boolean outputPresent)
            throws IniParsingException {
        String text = "signature = \"" + signature + "\"\n"
            + "nPages = 1\npageSize = 2\npageReadCommand = \"R\"\nochBlockSize = 1\n"
            + "#define " + listName + "=" + options + "\n"
            + "[Constants]\npage = 1\n"
            + (outputPresent ? "outputPin = bits, U16, 0, [0:8], $" + listName + "\n" : "");
        RawIniFile lines = IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(lines, "synthetic.ini", new IniFileMetaInfoImpl(lines));
    }
}
