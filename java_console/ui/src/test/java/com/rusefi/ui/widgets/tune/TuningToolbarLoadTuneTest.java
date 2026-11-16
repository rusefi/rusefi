package com.rusefi.ui.widgets.tune;

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
import static org.junit.jupiter.api.Assertions.assertTrue;

class TuningToolbarLoadTuneTest {
    @Test
    void connectedLoadAfrLambdaTableEncoding() throws Exception {
        TestTuneMigrationContext context = AfrMsqImportMigrationContext.loadAfrToLambda();
        IniFileModel targetIni = context.getUpdatedIniFile();
        ConfigurationImage targetImage = context.getUpdatedTune().asImage(targetIni);

        ConfigurationImage loaded = TuningToolbarWidget.applyLoadedTune(
            context.getPrevTune(), context.getPrevIniFile(), targetImage, targetIni, context.getCallbacks());

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
        ConfigurationImage base = new ConfigurationImage(new byte[]{5, (byte) 0xA5});

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
        ConfigurationImage base = new ConfigurationImage(new byte[]{5, (byte) 0xA5});
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
        ConfigurationImage base = new ConfigurationImage(new byte[]{5, (byte) 0xA5});
        TestCallbacks callbacks = new TestCallbacks();

        ConfigurationImage loaded = TuningToolbarWidget.applyLoadedTune(
            tune, standardSource, base, targetIni, callbacks);

        assertEquals(5, Byte.toUnsignedInt(loaded.getContent()[0]));
        assertEquals(0xA5, Byte.toUnsignedInt(loaded.getContent()[1]));
        assertTrue(callbacks.getContent().contains("ambiguous in the new enum"));
    }

    private static int rawValue(ConfigurationImage image, ArrayIniField field, int row, int column) {
        return Byte.toUnsignedInt(image.getContent()[field.getOffset(row, column)]);
    }

    private static Msq customPinTune(IniFileModel ini) {
        return MsqFactory.valueOf(new ConfigurationImage(new byte[]{47, 0}), ini);
    }

    private static IniFileModel pinIni(String listName, String options) throws IniParsingException {
        String text = "signature = \"connected-pin-test\"\n"
            + "nPages = 1\npageSize = 2\npageReadCommand = \"R\"\nochBlockSize = 1\n"
            + "#define " + listName + "=" + options + "\n"
            + "[Constants]\npage = 1\n"
            + "outputPin = bits, U16, 0, [0:7], $" + listName + "\n";
        RawIniFile lines = IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(lines, "synthetic.ini", new IniFileMetaInfoImpl(lines));
    }
}
