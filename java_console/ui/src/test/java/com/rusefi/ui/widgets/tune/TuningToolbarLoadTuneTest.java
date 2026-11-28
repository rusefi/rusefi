package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import com.rusefi.maintenance.TestCallbacks;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.afr_msq_import_migration.AfrMsqImportMigrationContext;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.MsqFactory;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class TuningToolbarLoadTuneTest {
    @Test
    void connectedLoadAfrLambdaTableEncoding() throws Exception {
        TestTuneMigrationContext context = AfrMsqImportMigrationContext.loadAfrToLambda();
        IniFileModel targetIni = context.getUpdatedIniFile();
        ConfigurationImage targetImage = context.getUpdatedTune().asImage(targetIni);
        // The migration fixture has no file header; connected loads require the source signature.
        Msq sourceTune = context.getPrevTune();
        sourceTune.getVersionInfo().setSignature(context.getPrevIniFile().getSignature());

        ConfigurationImage loaded = TuningToolbarWidget.applyLoadedTune(
            sourceTune, context.getPrevIniFile(), targetImage, targetIni, context.getCallbacks());

        ArrayIniField field = (ArrayIniField) targetIni.findIniField("lambdaTable").orElseThrow();
        assertEquals(147, rawValue(loaded, field, 0, 0));
        assertEquals(140, rawValue(loaded, field, 8, 4));
        assertEquals(126, rawValue(loaded, field, 11, 13));
    }

    @Test
    void connectedLoadMigratesCustomEnumLabelToSparseDestinationOrdinalAndPreservesNeighborBits()
            throws IniParsingException {
        IniFileModel customSource = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel standardSource = pinIni("standard_pin_list", "0=\"NONE\",5=\"Output\"");
        IniFileModel targetIni = pinIni("target_pin_list", "0=\"NONE\",19=\"Auxiliary pin\"");
        Msq tune = customPinTune(customSource);
        ConfigurationImage base = ConfigurationImageTestHelper.newLittleEndian(new byte[]{5, (byte) 0xA5});

        ConfigurationImage loaded = TuningToolbarWidget.applyLoadedTune(
            tune, standardSource, base, targetIni, new TestCallbacks());

        assertEquals(19, Byte.toUnsignedInt(loaded.getContent()[0]));
        assertEquals(0xA5, Byte.toUnsignedInt(loaded.getContent()[1]));
    }

    @Test
    void connectedLoadKeepsBasePinAndReportsWhenDestinationLabelIsAbsent() throws IniParsingException {
        IniFileModel customSource = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel standardSource = pinIni("standard_pin_list", "0=\"NONE\",5=\"Output\"");
        IniFileModel targetIni = pinIni("target_pin_list", "0=\"NONE\",19=\"Other pin\"");
        Msq tune = customPinTune(customSource);
        ConfigurationImage base = ConfigurationImageTestHelper.newLittleEndian(new byte[]{5, (byte) 0xA5});
        TestCallbacks callbacks = new TestCallbacks();

        ConfigurationImage loaded = TuningToolbarWidget.applyLoadedTune(
            tune, standardSource, base, targetIni, callbacks);

        assertEquals(5, Byte.toUnsignedInt(loaded.getContent()[0]));
        assertEquals(0xA5, Byte.toUnsignedInt(loaded.getContent()[1]));
        assertTrue(callbacks.getContent().contains("disappeared"));
    }

    @Test
    void connectedLoadKeepsBasePinAndReportsWhenDestinationLabelIsAmbiguous() throws IniParsingException {
        IniFileModel customSource = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel standardSource = pinIni("standard_pin_list", "0=\"NONE\",5=\"Output\"");
        IniFileModel targetIni = pinIni(
            "target_pin_list", "0=\"NONE\",19=\"Auxiliary pin\",27=\"Auxiliary pin\"");
        Msq tune = customPinTune(customSource);
        ConfigurationImage base = ConfigurationImageTestHelper.newLittleEndian(new byte[]{5, (byte) 0xA5});
        TestCallbacks callbacks = new TestCallbacks();

        ConfigurationImage loaded = TuningToolbarWidget.applyLoadedTune(
            tune, standardSource, base, targetIni, callbacks);

        assertEquals(5, Byte.toUnsignedInt(loaded.getContent()[0]));
        assertEquals(0xA5, Byte.toUnsignedInt(loaded.getContent()[1]));
        assertTrue(callbacks.getContent().contains("ambiguous in the new enum"));
    }

    @Test
    void sameSignatureImportMapsOnlyUniqueExactDestinationEnumLabels() throws IniParsingException {
        IniFileModel sourceIni = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel targetIni = pinIni("target_pin_list", "0=\"NONE\",19=\"Auxiliary pin\"");
        ConfigurationImage base = ConfigurationImageTestHelper.newLittleEndian(new byte[]{5, (byte) 0xA5});

        ConfigurationImage loaded = TuningToolbarWidget.applyLoadedTune(
            customPinTune(sourceIni), null, base, targetIni, new TestCallbacks());

        assertEquals(19, Byte.toUnsignedInt(loaded.getContent()[0]));
        assertEquals(0xA5, Byte.toUnsignedInt(loaded.getContent()[1]));
    }

    @Test
    void sameSignatureImportRetainsBaseForMissingAmbiguousAndUnknownEnumValues() throws IniParsingException {
        IniFileModel sourceIni = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        ConfigurationImage base = ConfigurationImageTestHelper.newLittleEndian(new byte[]{5, (byte) 0xA5});
        String marker = com.opensr5.ini.field.EnumIniField.unknownOrdinalValue(33);
        for (String options : new String[] {
                "0=\"NONE\",19=\"Other pin\"",
                "0=\"NONE\",19=\"Auxiliary pin\",27=\"Auxiliary pin\"",
                "0=\"NONE\",19=\"" + marker + "\""
        }) {
            IniFileModel targetIni = pinIni("target_pin_list", options);
            Msq tune = customPinTune(sourceIni);
            if (options.contains(marker)) {
                tune.findPage().constant.get(0).setValue(marker);
            }
            TestCallbacks callbacks = new TestCallbacks();

            ConfigurationImage loaded = TuningToolbarWidget.applyLoadedTune(
                tune, null, base, targetIni, callbacks);

            assertArrayEquals(base.getContent(), loaded.getContent());
            assertTrue(callbacks.getContent().contains(options.contains(marker)
                ? "unknown pin"
                : options.contains("27=") ? "ambiguous" : "pin is not available"));
        }
    }

    @Test
    void quotedMarkerLookingPinLabelIsImportedButInvalidPinIsRejected() throws IniParsingException {
        IniFileModel sourceIni = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        ConfigurationImage base = ConfigurationImageTestHelper.newLittleEndian(new byte[]{5, (byte) 0xA5});
        String marker = com.opensr5.ini.field.EnumIniField.unknownOrdinalValue(33);
        IniFileModel markerTarget = pinIni("target_pin_list", "0=\"NONE\",19=\"" + marker + "\"");
        Msq markerTune = customPinTune(sourceIni);
        markerTune.findPage().constant.get(0).setValue("\"" + marker + "\"");

        ConfigurationImage markerResult = TuningToolbarWidget.applyLoadedTune(
            markerTune, null, base, markerTarget, new TestCallbacks());
        assertEquals(19, Byte.toUnsignedInt(markerResult.getContent()[0]));

        IniFileModel pinSource = pinEnumIni("0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel invalidTarget = pinEnumIni("0=\"NONE\",19=\"INVALID\"");
        Msq invalidTune = customPinTune(pinSource);
        invalidTune.findPage().constant.get(0).setValue("\"INVALID\"");
        TestCallbacks callbacks = new TestCallbacks();
        ConfigurationImage invalidResult = TuningToolbarWidget.applyLoadedTune(
            invalidTune, null, base, invalidTarget, callbacks);
        assertArrayEquals(base.getContent(), invalidResult.getContent());
        assertTrue(callbacks.getContent().contains("unknown pin"));
    }

    @Test
    void sameSignatureImportValidatesUnitsAndArrayDimensionsAndKeepsFailedArraysAtomic()
            throws IniParsingException {
        IniFileModel sourceScalar = scalarIni("psi");
        IniFileModel targetScalar = scalarIni("kPa");
        Msq scalarTune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{42}), sourceScalar);
        ConfigurationImage scalarBase = ConfigurationImageTestHelper.newLittleEndian(new byte[]{9});

        ConfigurationImage scalarResult = TuningToolbarWidget.applyLoadedTune(
            scalarTune, null, scalarBase, targetScalar, new TestCallbacks());

        assertArrayEquals(scalarBase.getContent(), scalarResult.getContent());

        IniFileModel missingSourceUnits = scalarIni("");
        IniFileModel declaredDestinationUnits = scalarIni("psi");
        ConfigurationImage unqualifiedTune = ConfigurationImageTestHelper.newLittleEndian(new byte[]{42});
        ConfigurationImage unqualifiedBase = ConfigurationImageTestHelper.newLittleEndian(new byte[]{9});
        ConfigurationImage unqualifiedResult = TuningToolbarWidget.applyLoadedTune(
            MsqFactory.valueOf(unqualifiedTune, missingSourceUnits), null,
            unqualifiedBase, declaredDestinationUnits, new TestCallbacks());
        assertArrayEquals(unqualifiedBase.getContent(), unqualifiedResult.getContent());

        IniFileModel sourceArray = arrayIni("[2]", 2);
        IniFileModel differentDimensions = arrayIni("[1]", 2);
        Msq arrayTune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{3, 4}), sourceArray);
        ConfigurationImage arrayBase = ConfigurationImageTestHelper.newLittleEndian(new byte[]{8, 9});
        TestCallbacks dimensionCallbacks = new TestCallbacks();

        ConfigurationImage dimensionResult = TuningToolbarWidget.applyLoadedTune(
            arrayTune, null, arrayBase, differentDimensions, dimensionCallbacks);

        assertArrayEquals(arrayBase.getContent(), dimensionResult.getContent());
        assertTrue(dimensionCallbacks.getContent().contains("table size does not match"));

        arrayTune.findPage().constant.get(0).setValue("7 invalid");
        TestCallbacks invalidValueCallbacks = new TestCallbacks();
        ConfigurationImage atomicResult = TuningToolbarWidget.applyLoadedTune(
            arrayTune, null, arrayBase, sourceArray, invalidValueCallbacks);
        assertArrayEquals(arrayBase.getContent(), atomicResult.getContent());
        assertTrue(invalidValueCallbacks.getContent().contains("invalid value"));
    }

    @Test
    void sameSignatureImportRejectsUnrepresentableAndNonFiniteNumericValuesBeforeWriting()
            throws IniParsingException {
        IniFileModel scalarIni = scalarIni("");
        ConfigurationImage base = ConfigurationImageTestHelper.newLittleEndian(new byte[]{9});
        for (String value : new String[] {"256", "NaN"}) {
            Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{42}), scalarIni);
            tune.findPage().constant.get(0).setValue(value);
            TestCallbacks callbacks = new TestCallbacks();

            ConfigurationImage result = TuningToolbarWidget.applyLoadedTune(
                tune, null, base, scalarIni, callbacks);

            assertArrayEquals(base.getContent(), result.getContent());
            assertTrue(callbacks.getContent().contains("value is out of range"));
        }

        IniFileModel arrayIni = arrayIni("[2]", 2);
        for (String value : new String[] {"7 256", "7 NaN"}) {
            Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{3, 4}), arrayIni);
            tune.findPage().constant.get(0).setValue(value);
            TestCallbacks callbacks = new TestCallbacks();

            ConfigurationImage result = TuningToolbarWidget.applyLoadedTune(
                tune, null, ConfigurationImageTestHelper.newLittleEndian(new byte[]{8, 9}), arrayIni, callbacks);

            assertArrayEquals(new byte[]{8, 9}, result.getContent());
            assertTrue(callbacks.getContent().contains("value is out of range"));
        }
    }

    @Test
    void sameSignatureImportAcceptsAValueAtTheScaledStorageBoundary() throws IniParsingException {
        IniFileModel scaledIni = ini(
            "scaled = scalar, U08, 0, \"\", 0.5, 0, 0, 255, 0\n", 1);
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{0}), scaledIni);
        tune.findPage().constant.get(0).setValue("127.5");

        ConfigurationImage result = TuningToolbarWidget.applyLoadedTune(
            tune, null, ConfigurationImageTestHelper.newLittleEndian(new byte[]{0}), scaledIni, new TestCallbacks());

        assertEquals(255, Byte.toUnsignedInt(result.getContent()[0]));
    }

    @Test
    void sameSignatureImportRejectsTableDimensionsOnNonArrayFields() throws IniParsingException {
        IniFileModel scalarIni = scalarIni("");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{42}), scalarIni);
        tune.findPage().constant.get(0).setRows("1");
        tune.findPage().constant.get(0).setCols("1");
        ConfigurationImage base = ConfigurationImageTestHelper.newLittleEndian(new byte[]{9});
        TestCallbacks callbacks = new TestCallbacks();

        ConfigurationImage result = TuningToolbarWidget.applyLoadedTune(
            tune, null, base, scalarIni, callbacks);

        assertArrayEquals(base.getContent(), result.getContent());
        assertTrue(callbacks.getContent().contains("table size does not match"));
    }

    @Test
    void noSourceSchemaCannotBeUsedForDifferentSignature() throws IniParsingException {
        IniFileModel sourceIni = pinIni("custom_pin_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel targetIni = pinIni(
            "target_pin_list", "0=\"NONE\",19=\"Auxiliary pin\"", "different-signature");
        ConfigurationImage base = ConfigurationImageTestHelper.newLittleEndian(new byte[]{5, (byte) 0xA5});
        TestCallbacks callbacks = new TestCallbacks();

        ConfigurationImage loaded = TuningToolbarWidget.applyLoadedTune(
            customPinTune(sourceIni), null, base, targetIni, callbacks);

        assertArrayEquals(base.getContent(), loaded.getContent());
        assertTrue(callbacks.getContent().contains("matching source INI is required"));
    }

    @Test
    void sameSignatureImportDoesNotApplySecondaryFieldsWithoutACurrentPage() throws IniParsingException {
        IniFileModel sourceIni = ini("secondary = scalar, U08, 0, \"\", 1, 0, 0, 255, 0\n", 1);
        IniFileModel targetIni = secondaryIni();
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{42}), sourceIni);
        ConfigurationImage base = ConfigurationImageTestHelper.newLittleEndian(new byte[]{9});
        TestCallbacks callbacks = new TestCallbacks();

        ConfigurationImage loaded = TuningToolbarWidget.applyLoadedTune(
            tune, null, base, targetIni, callbacks);

        assertArrayEquals(base.getContent(), loaded.getContent());
        assertTrue(callbacks.getContent().contains("destination page is unavailable"));
    }

    private static int rawValue(ConfigurationImage image, ArrayIniField field, int row, int column) {
        return Byte.toUnsignedInt(image.getContent()[field.getOffset(row, column)]);
    }

    private static Msq customPinTune(IniFileModel ini) {
        return MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(new byte[]{47, 0}), ini);
    }

    private static IniFileModel pinIni(String listName, String options) throws IniParsingException {
        return pinIni(listName, options, "connected-pin-test");
    }

    private static IniFileModel pinIni(String listName, String options, String signature) throws IniParsingException {
        String text = "signature = \"" + signature + "\"\n"
            + "nPages = 1\npageSize = 2\npageReadCommand = \"R\"\nochBlockSize = 1\n"
            + "#define " + listName + "=" + options + "\n"
            + "[Constants]\npage = 1\n"
            + "outputPin = bits, U16, 0, [0:7], $" + listName + "\n";
        RawIniFile lines = IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(lines, "synthetic.ini", new IniFileMetaInfoImpl(lines));
    }

    private static IniFileModel pinEnumIni(String options) throws IniParsingException {
        String text = "signature = \"connected-pin-test\"\n"
            + "nPages = 1\npageSize = 2\npageReadCommand = \"R\"\nochBlockSize = 1\n"
            + "#define output_pin_e_list=" + options + "\n"
            + "[Constants]\npage = 1\n"
            + "outputPin = bits, U16, 0, [0:7], $output_pin_e_list\n";
        RawIniFile lines = IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(lines, "synthetic-pin.ini", new IniFileMetaInfoImpl(lines));
    }

    private static IniFileModel scalarIni(String units) throws IniParsingException {
        return ini("scalar = scalar, U08, 0, \"" + units + "\", 1, 0, 0, 255, 0\n", 1);
    }

    private static IniFileModel arrayIni(String dimensions, int pageSize) throws IniParsingException {
        return ini("curve = array, U08, 0, " + dimensions + ", \"\", 1, 0, 0, 255, 0\n", pageSize);
    }

    private static IniFileModel ini(String field, int pageSize) throws IniParsingException {
        String text = "signature = \"connected-pin-test\"\n"
            + "nPages = 1\npageSize = " + pageSize
            + "\npageReadCommand = \"R\"\nochBlockSize = 1\n"
            + "[Constants]\npage = 1\n" + field;
        RawIniFile lines = IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(lines, "synthetic.ini", new IniFileMetaInfoImpl(lines));
    }

    private static IniFileModel secondaryIni() throws IniParsingException {
        String text = "signature = \"connected-pin-test\"\n"
            + "nPages = 2\npageSize = 1, 1\n"
            + "pageReadCommand = \"R\", \"R\"\nochBlockSize = 1\n"
            + "[Constants]\npage = 1\n"
            + "main = scalar, U08, 0, \"\", 1, 0, 0, 255, 0\n"
            + "page = 2\nsecondary = scalar, U08, 0, \"\", 1, 0, 0, 255, 0\n";
        RawIniFile lines = IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(lines, "synthetic-secondary.ini", new IniFileMetaInfoImpl(lines));
    }
}
