package com.opensr5.ini.test;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.*;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.jetbrains.annotations.NotNull;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.util.Arrays;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/1/2017
 */
public class IniFileReaderTest {
    private static final String PAGE_READ = "    pageReadCommand     = \"X\",       \"X\",     \"X\"\n\n\n\n";
    private static final String CRC_READ = "    crc32CheckCommand     = \"X\",       \"X\",     \"X\"\n\n\n\n";
    private static final String SIGNATURE_UNIT_TEST = "  signature      = \"unit test\"\n";
    static final double EPS = 0.00001;

    @Test
    public void parseExpressions() {
        assertEquals(0.1, IniField.parseDouble("{1/10}"), EPS);
        assertEquals(0.1, IniField.parseDouble("{0.1}"), EPS);
        assertEquals(0.1, IniField.parseDouble("0.1"), EPS);
    }

    @Test
    public void testTotalPagesSize() {
        String string = "   nPages              = 3\n" +
                SIGNATURE_UNIT_TEST +
                PAGE_READ +
            CRC_READ +
            "ochBlockSize=1\n" +
                "   pageSize            = 288,   64,     288\n";


        IniFileMetaInfo meta = new IniFileMetaInfoImpl(fromString(string));

        /*
        assertEquals(3, meta.getnPages());
        assertEquals(IniFileMetaInfoImpl.DEFAULT_BLOCKING_FACTOR, meta.getBlockingFactor());
        assertEquals(640, meta.getTotalSize());
        */
        assertEquals("unit test", meta.getSignature());

        assertEquals(64, meta.getPageSize(1));
        assertEquals("X", meta.getPageReadCommand(1));
    }

    @NotNull
    private RawIniFile fromString(String string) {
        return IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
    }

    @Test
    public void testOutputChannels() throws IniMemberNotFound {
        String string = "[OutputChannels]\n" +
            "stftCorrection2 = scalar, F32, 1444,  \"%\", 100.0, -1.0\n" +
            "; total TS size = 1448\n" +
            "tpsFrom = scalar, F32, 1448\n";
        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        ScalarIniField stftCorrection2 = (ScalarIniField) model.getOutputChannel("stftCorrection2");
        assertEquals(1444, stftCorrection2.getOffset());
        assertEquals("%", stftCorrection2.getUnits());
        assertEquals(100.0, stftCorrection2.getMultiplier());  // multiplier from INI file
        assertEquals(-1.0, stftCorrection2.getSerializationOffset());  // serialization offset from INI file
        assertEquals(FieldType.FLOAT, stftCorrection2.getType());

        ScalarIniField tpsFrom = (ScalarIniField) model.getOutputChannel("tpsFrom");
        assertEquals(1448, tpsFrom.getOffset());
        assertEquals("", tpsFrom.getUnits());
        assertEquals(1.0, tpsFrom.getMultiplier());
        assertEquals(0.0, tpsFrom.getSerializationOffset());
        assertEquals(FieldType.FLOAT, stftCorrection2.getType());
    }

    @Test
    public void testReadDialogCommandsCurve() {
        String string =
            "   ; generates [SettingContextHelp]\n" +
                "[Constants]\n" +
                "page = 1\n" +
                "scriptCurve1Bins = array, F32, 4828, [16], \"x\", 1, 0, -10000, 10000, 3\n" +
                                "[SettingContextHelp]\n" +
                "; SettingContextHelpEnd\n" +
"\tdialog = engineTypeDialog1, \"Base T1\"\n" +
                "\t\tfield = \"!These buttons send a command to rusEFI controller to apply preset values\"\n" +
                "\t\tfield = \"!Once you send the command, please reconnect to rusEFI controller in order to read fresh values\"\n" +
                "\n" +
                "\t\tcommandButton = \"Reset to factory tune\", cmd_set_engine_type_default\n" +
                "\tdialog = engineTypeDialog2, \"Base T2\"\n" +
                "\t\tfield = \"!These buttons send a command to rusEFI controller to apply preset values\"\n" +
                "\t\tfield = \"!Once you send the command, please reconnect to rusEFI controller in order to read fresh values\"\n" +
                "\n" +
                "\t\tcommandButton = \"Reset to factory tune\", cmd_set_engine_type_default\n" +
                "" +
                "";
        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);
        assertEquals(1, model.getAllIniFields().size());
        assertEquals(2, model.getDialogs().size());
    }

    @Test
    public void testMultiPage() {
        String string =
            "[Constants]\n" +
            "page = 2\n" +
            "scriptCurve2Bins = array, F32, 4828, [16], \"x\", 1, 0, -10000, 10000, 3\n" +
            "scriptCurve2 = array, F32, 4892, [16], \"y\", 1, 0, -10000, 10000, 3\n " +
            "page = 1\n" +
            "scriptCurve1Bins = array, F32, 4828, [16], \"x\", 1, 0, -10000, 10000, 3\n" +
            "scriptCurve1 = array, F32, 4892, [16], \"y\", 1, 0, -10000, 10000, 3\n "
    ;
        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(2, model.getAllIniFields().size());
        assertEquals(2, model.getSecondaryIniFields().size());
        assertEquals(0, model.getFieldsInUiOrder().size());
    }

    @Test
    public void testProtocolMeta() {
        String string =
                "[Constants]\n" +
                        "   crc32CheckCommand   = \"k\\x00\\x00\\x00\\x00\\x00\\x00\"\n" +
                        "page = 1\n" +
                        "primingSquirtDurationMs\t\t\t= scalar, F32,\t96,\t\"*C\", 1.0, 0, -40, 200, 1\n" +
                        "";
        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);
        assertEquals(1, model.getAllIniFields().size());

        String crcProtocol = model.getProtocolMeta().get("crc32CheckCommand");
        assertEquals("k\\x00\\x00\\x00\\x00\\x00\\x00", crcProtocol);

        byte[] expected = {'k', 0, 0, 0, 0, 0, 0};

        assertTrue(Arrays.equals(expected, ProtocolCommand.parse(crcProtocol).getBytes()));
    }

    @Test
    public void testSetBits() {
        assertEquals(0xFE, EnumIniField.setBitRange(0xFF, 0, 0, 1));
        assertEquals(0xF0, EnumIniField.setBitRange(0xFF, 0, 0, 4));
        assertEquals(0x0F, EnumIniField.setBitRange(0xFF, 0, 4, 4));

        assertEquals(0xff, EnumIniField.setBitRange(0xFF, 3, 0, 2));
        assertEquals(0xF3, EnumIniField.setBitRange(0xFF, 3, 0, 4));
        assertEquals(0x3F, EnumIniField.setBitRange(0xFF, 3, 4, 4));

        assertEquals(0x400, EnumIniField.setBitRange(0, 1, 10, 1));
    }

    @Test
    public void testBitLogic() {
        assertEquals(4, ConfigurationImage.getBitRange(4, 0, 8));
        assertEquals(4, ConfigurationImage.getBitRange(4, 0, 3));
        assertEquals(0, ConfigurationImage.getBitRange(4, 0, 2));
        assertEquals(3, ConfigurationImage.getBitRange(7, 0, 2));

        assertEquals(true, ConfigurationImage.getBit(0xff, 0));

        assertEquals(true, ConfigurationImage.getBit(0xf0, 4));
        assertEquals(2, ConfigurationImage.getBitRange(0xf0, 3, 2));
    }

    static @NotNull IniFileModel readLines(RawIniFile lines) {
        IniFileMetaInfo metaInfo = mock(IniFileMetaInfo.class);
        return IniFileReaderUtil.readIniFile(lines, "", metaInfo);
    }

    @Test
    public void testGaugeCategoryModelImmutability() {
        String string = "[GaugeConfigurations]\n" +
                "gaugeCategory = TestCategory\n" +
                "testGauge = channel, \"Title\", \"units\", 0, 100, 0, 10, 90, 100, 1, 1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        GaugeCategoryModel category = model.getGaugeCategories().get("TestCategory");
        assertNotNull(category);

        // The list returned should be unmodifiable
        try {
            category.getGauges().clear();
            fail("Expected UnsupportedOperationException when modifying gauge list");
        } catch (UnsupportedOperationException e) {
            // Expected behavior - list should be immutable
        }
    }

    @Test
    public void testDialogWithTopicHelp() {
        String string = "[Constants]\n" +
                "page = 1\n" +
                "primingSquirtDurationMs = scalar, F32, 96, \"*C\", 1, 0, -40, 200, 1\n" +
                "[SettingContextHelp]\n" +
                "; SettingContextHelpEnd\n" +
                "dialog = triggerDialog, \"Trigger Settings\"\n" +
                "    topicHelp = \"triggerHelp\"\n" +
                "    field = \"Priming\", primingSquirtDurationMs\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Test that dialog was created with topicHelp
        assertEquals(1, model.getDialogs().size());
        assertTrue(model.getDialogs().containsKey("triggerDialog"));

        DialogModel dialog = model.getDialogs().get("triggerDialog");
        assertNotNull(dialog);
        assertEquals("triggerDialog", dialog.getKey());
        assertEquals("triggerHelp", dialog.getTopicHelp());

        // Test that topicHelp was also stored in the topicHelp map
        assertEquals(1, model.getTopicHelp().size());
        assertTrue(model.getTopicHelp().containsKey("triggerDialog"));
        assertEquals("triggerHelp", model.getTopicHelp().get("triggerDialog"));
    }

    @Test
    public void testDialogWithoutTopicHelp() {
        String string = "[Constants]\n" +
                "page = 1\n" +
                "primingSquirtDurationMs = scalar, F32, 96, \"*C\", 1, 0, -40, 200, 1\n" +
                "[SettingContextHelp]\n" +
                "; SettingContextHelpEnd\n" +
                "dialog = basicDialog, \"Basic Settings\"\n" +
                "    field = \"Priming\", primingSquirtDurationMs\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(1, model.getDialogs().size());
        DialogModel dialog = model.getDialogs().get("basicDialog");
        assertNotNull(dialog);
        assertNull(dialog.getTopicHelp());

        // topicHelp map should be empty
        assertEquals(0, model.getTopicHelp().size());
    }

    @Test
    public void testMultipleDialogsWithTopicHelp() {
        String string = "[Constants]\n" +
                "page = 1\n" +
                "field1 = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
                "field2 = scalar, F32, 4, \"unit\", 1, 0, 0, 100, 1\n" +
                "[SettingContextHelp]\n" +
                "; SettingContextHelpEnd\n" +
                "dialog = dialog1, \"Dialog One\"\n" +
                "    topicHelp = \"helpOne\"\n" +
                "    field = \"Field 1\", field1\n" +
                "dialog = dialog2, \"Dialog Two\"\n" +
                "    topicHelp = \"helpTwo\"\n" +
                "    field = \"Field 2\", field2\n" +
                "dialog = dialog3, \"Dialog Three\"\n" +
                "    field = \"Field 1\", field1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(3, model.getDialogs().size());

        DialogModel dialog1 = model.getDialogs().get("dialog1");
        assertNotNull(dialog1);
        assertEquals("helpOne", dialog1.getTopicHelp());

        DialogModel dialog2 = model.getDialogs().get("dialog2");
        assertNotNull(dialog2);
        assertEquals("helpTwo", dialog2.getTopicHelp());

        DialogModel dialog3 = model.getDialogs().get("dialog3");
        assertNotNull(dialog3);
        assertNull(dialog3.getTopicHelp());

        // Test topicHelp map
        assertEquals(2, model.getTopicHelp().size());
        assertEquals("helpOne", model.getTopicHelp().get("dialog1"));
        assertEquals("helpTwo", model.getTopicHelp().get("dialog2"));
    }

    @Test
    public void testContextHelpParsing() {
        String string = "[Constants]\n" +
                "page = 1\n" +
                "field1 = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
                "[SettingContextHelp]\n" +
                "    field1 = \"This is a simple tooltip\"\n" +
                "    help = boostPidHelp, \"Boost Control PID\"\n" +
                "        text = \"P = % per kPa error\"\n" +
                "        text = \"I = % per kPa-seconds\"\n" +
                "        text = \"D = % per kPa/sec\"\n" +
                "        webHelp = \"https://rusefi.com/s/boost\"\n" +
                "; SettingContextHelpEnd\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Test that simple tooltip was stored
        assertEquals(1, model.getTooltips().size());
        assertEquals("This is a simple tooltip", model.getTooltips().get("field1"));

        // Test that context help was parsed
        assertEquals(1, model.getContextHelp().size());
        assertTrue(model.getContextHelp().containsKey("boostPidHelp"));

        ContextHelpModel help = model.getContextHelp("boostPidHelp");
        assertNotNull(help);
        assertEquals("boostPidHelp", help.getReferenceName());
        assertEquals("Boost Control PID", help.getTitle());
        assertEquals(3, help.getTextLines().size());
        assertEquals("P = % per kPa error", help.getTextLines().get(0));
        assertEquals("I = % per kPa-seconds", help.getTextLines().get(1));
        assertEquals("D = % per kPa/sec", help.getTextLines().get(2));
        assertEquals("https://rusefi.com/s/boost", help.getWebHelp());
    }

    @Test
    public void testContextHelpWithoutWebHelp() {
        String string = "[SettingContextHelp]\n" +
                "    help = simpleHelp, \"Simple Help\"\n" +
                "        text = \"Line 1\"\n" +
                "        text = \"Line 2\"\n" +
                "; SettingContextHelpEnd\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        ContextHelpModel help = model.getContextHelp("simpleHelp");
        assertNotNull(help);
        assertEquals("simpleHelp", help.getReferenceName());
        assertEquals("Simple Help", help.getTitle());
        assertEquals(2, help.getTextLines().size());
        assertNull(help.getWebHelp());
    }

    @Test
    public void testMultipleContextHelpEntries() {
        String string = "[SettingContextHelp]\n" +
                "    help = help1, \"Help One\"\n" +
                "        text = \"Text for help 1\"\n" +
                "        webHelp = \"http://example.com/1\"\n" +
                "    help = help2, \"Help Two\"\n" +
                "        text = \"Text line 1 for help 2\"\n" +
                "        text = \"Text line 2 for help 2\"\n" +
                "    help = help3, \"Help Three\"\n" +
                "        text = \"Text for help 3\"\n" +
                "        webHelp = \"http://example.com/3\"\n" +
                "; SettingContextHelpEnd\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(3, model.getContextHelp().size());

        ContextHelpModel help1 = model.getContextHelp("help1");
        assertNotNull(help1);
        assertEquals("Help One", help1.getTitle());
        assertEquals(1, help1.getTextLines().size());
        assertEquals("http://example.com/1", help1.getWebHelp());

        ContextHelpModel help2 = model.getContextHelp("help2");
        assertNotNull(help2);
        assertEquals("Help Two", help2.getTitle());
        assertEquals(2, help2.getTextLines().size());
        assertNull(help2.getWebHelp());

        ContextHelpModel help3 = model.getContextHelp("help3");
        assertNotNull(help3);
        assertEquals("Help Three", help3.getTitle());
        assertEquals(1, help3.getTextLines().size());
        assertEquals("http://example.com/3", help3.getWebHelp());
    }

    @Test
    public void testMixedTooltipsAndContextHelp() {
        String string = "[SettingContextHelp]\n" +
                "    field1 = \"Tooltip 1\"\n" +
                "    field2 = \"Tooltip 2\"\n" +
                "    help = complexHelp, \"Complex Help\"\n" +
                "        text = \"Help text\"\n" +
                "    field3 = \"Tooltip 3\"\n" +
                "; SettingContextHelpEnd\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Test tooltips
        assertEquals(3, model.getTooltips().size());
        assertEquals("Tooltip 1", model.getTooltips().get("field1"));
        assertEquals("Tooltip 2", model.getTooltips().get("field2"));
        assertEquals("Tooltip 3", model.getTooltips().get("field3"));

        // Test context help
        assertEquals(1, model.getContextHelp().size());
        ContextHelpModel help = model.getContextHelp("complexHelp");
        assertNotNull(help);
        assertEquals("Complex Help", help.getTitle());
    }

    @Test
    public void testDialogTopicHelpReferencingContextHelp() {
        String string = "[Constants]\n" +
                "page = 1\n" +
                "veTable = array, F32, 0, [16x16], \"value\", 1, 0, 0, 100, 2\n" +
                "msIoBox0_id = scalar, U08, 0, \"\", 1, 0, 0, 255, 0\n" +
                "msIoBox0_vss = scalar, U08, 1, \"\", 1, 0, 0, 255, 0\n" +
                "[SettingContextHelp]\n" +
                "; SettingContextHelpEnd\n" +
                "\tdialog = msIoBox1, \"MS IO box 1\"\n" +
                "\t\tfield = \"Base CAN identifier\", msIoBox0_id\n" +
                "\t\tfield = \"VSS settings\", msIoBox0_vss\n" +
                "\n" +
                "\tdialog = msIoBox, \"MS IO box Container\"\n" +
                "\t\tpanel = msIoBox1\n" +
                "\n" +
                "\thelp = boostPidHelp, \"Boost Control PID\"\n" +
                "\t\ttext = \"P = % per kPa error\"\n" +
                "\t\ttext = \"I = % per kPa-seconds\"\n" +
                "\t\ttext = \"D = % per kPa/sec\"\n" +
                "\n" +
                "\thelp = veTableDialogHelp, \"Volumetric Efficiency\"\n" +
                "\t\ttext = \"Volumetric Efficiency is used to calculate fuel in Speed Density mode. VE incorporates pressure ratio as measured by MAP sensor in boost (no VE ever above 105%ish)\"\n" +
                "\t\twebHelp = \"https://rusefi.com/s/fuel\"\n" +
                "\n" +
                "\tdialog = veTableDialog, \"\"\n" +
                "\t\ttopicHelp = \"veTableDialogHelp\"\n" +
                "\t\tfield = \"VE Table\", veTable\n" +
                "\n" +
                "\tdialog = veTableDialog3D, \"VE Table\"\n" +
                "\t\ttopicHelp = \"veTableDialogHelp\"\n" +
                "\t\tfield = \"VE Table 3D\", veTable\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Verify dialogs were created (now stored by dialog key, not UI name)
        assertEquals(4, model.getDialogs().size());
        assertTrue(model.getDialogs().containsKey("msIoBox1"));
        assertTrue(model.getDialogs().containsKey("msIoBox"));
        assertTrue(model.getDialogs().containsKey("veTableDialog"));
        assertTrue(model.getDialogs().containsKey("veTableDialog3D"));

        // Verify the msIoBox1 dialog has fields but no panels
        DialogModel msIoBoxDialog = model.getDialogs().get("msIoBox1");
        assertNotNull(msIoBoxDialog);
        assertEquals(2, msIoBoxDialog.getFields().size());
        assertEquals(0, msIoBoxDialog.getPanels().size());

        // Verify the msIoBox container dialog has a panel reference
        DialogModel msIoBoxContainer = model.getDialogs().get("msIoBox");
        assertNotNull(msIoBoxContainer);
        assertEquals(0, msIoBoxContainer.getFields().size());
        assertEquals(1, msIoBoxContainer.getPanels().size());
        PanelModel panel = msIoBoxContainer.getPanels().get(0);
        assertEquals("msIoBox1", panel.getPanelName());
        assertNull(panel.getPlacement());

        // Verify the veTableDialog with topicHelp
        DialogModel veDialog = model.getDialogs().get("veTableDialog");
        assertNotNull(veDialog);
        assertEquals("veTableDialogHelp", veDialog.getTopicHelp());
        assertEquals(1, veDialog.getFields().size());
        assertEquals(0, veDialog.getPanels().size());

        // Verify dialog veTableDialog3D also has topicHelp
        DialogModel veDialog3D = model.getDialogs().get("veTableDialog3D");
        assertNotNull(veDialog3D);
        assertEquals("veTableDialogHelp", veDialog3D.getTopicHelp());

        // Verify the context help entries exist
        assertEquals(2, model.getContextHelp().size());

        // Check boostPidHelp
        ContextHelpModel boostHelp = model.getContextHelp("boostPidHelp");
        assertNotNull(boostHelp);
        assertEquals("boostPidHelp", boostHelp.getReferenceName());
        assertEquals("Boost Control PID", boostHelp.getTitle());
        assertEquals(3, boostHelp.getTextLines().size());
        assertEquals("P = % per kPa error", boostHelp.getTextLines().get(0));
        assertEquals("I = % per kPa-seconds", boostHelp.getTextLines().get(1));
        assertEquals("D = % per kPa/sec", boostHelp.getTextLines().get(2));
        assertNull(boostHelp.getWebHelp());

        // Check veTableDialogHelp
        ContextHelpModel veHelp = model.getContextHelp("veTableDialogHelp");
        assertNotNull(veHelp);
        assertEquals("veTableDialogHelp", veHelp.getReferenceName());
        assertEquals("Volumetric Efficiency", veHelp.getTitle());
        assertEquals(1, veHelp.getTextLines().size());
        assertEquals("Volumetric Efficiency is used to calculate fuel in Speed Density mode. VE incorporates pressure ratio as measured by MAP sensor in boost (no VE ever above 105%ish)",
                     veHelp.getTextLines().get(0));
        assertEquals("https://rusefi.com/s/fuel", veHelp.getWebHelp());

        // Demonstrate the lookup pattern: dialog -> topicHelp -> context help
        String helpReference = veDialog3D.getTopicHelp();
        assertNotNull(helpReference);
        ContextHelpModel fullHelp = model.getContextHelp(helpReference);
        assertNotNull(fullHelp);
        assertEquals("Volumetric Efficiency", fullHelp.getTitle());
        assertEquals("https://rusefi.com/s/fuel", fullHelp.getWebHelp());
    }

    @Test
    public void testTextFieldsInDialog() {
        String string = "[Constants]\n" +
                "page = 1\n" +
                "field1 = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
                "[SettingContextHelp]\n" +
                "; SettingContextHelpEnd\n" +
                "dialog = canReWidebandLegacyTools, \"Legacy\"\n" +
                "    field = \"!Please connect \"\n" +
                "    field = \"!Please do \"\n" +
                "    field = \"\"\n" +
                "    field = \"Just text\"\n" +
                "    field = \"Real Field\", field1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        DialogModel dialog = model.getDialogs().get("canReWidebandLegacyTools");
        assertNotNull(dialog);
        assertEquals(5, dialog.getFields().size());

        // Text-only fields (start with !) have key same as UI name
        assertEquals("!Please connect ", dialog.getFields().get(0).getUiName());
        assertEquals("!Please connect ", dialog.getFields().get(0).getKey());

        assertEquals("!Please do ", dialog.getFields().get(1).getUiName());
        assertEquals("!Please do ", dialog.getFields().get(1).getKey());

        assertEquals("", dialog.getFields().get(2).getUiName());
        assertEquals("", dialog.getFields().get(2).getKey());

        assertEquals("Just text", dialog.getFields().get(3).getUiName());
        assertEquals("Just text", dialog.getFields().get(3).getKey());

        assertEquals("Real Field", dialog.getFields().get(4).getUiName());
        assertEquals("field1", dialog.getFields().get(4).getKey());
    }


    @Test
    public void testExpressionOutputChannels() throws IniMemberNotFound {
        String string = "[OutputChannels]\n" +
            "; Scalar output channels\n" +
            "coolant = scalar, S16, 20, \"C\", 0.01, 0\n" +
            "intake = scalar, S16, 22, \"C\", 0.01, 0\n" +
            "; Expression output channels (virtual channels calculated from expressions)\n" +
            "coolantTemperature = { useMetricOnInterface ? coolant : (coolant * 1.8 + 32) }\n" +
            "intakeTemperature = { intake }\n" +
            "lambdaValue = { isLambdaDisplay ? lambda1 : (lambda1 * 14.7) }\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Test that scalar output channels are parsed
        ScalarIniField coolant = (ScalarIniField) model.getOutputChannel("coolant");
        assertEquals(20, coolant.getOffset());
        assertEquals("C", coolant.getUnits());

        ScalarIniField intake = (ScalarIniField) model.getOutputChannel("intake");
        assertEquals(22, intake.getOffset());

        // Test that expression output channels are stored separately
        Map<String, String> exprChannels = model.getExpressionOutputChannels();
        assertEquals(3, exprChannels.size());

        // Test that expression output channels can be looked up by name
        String coolantTempExpr = model.getExpressionOutputChannel("coolantTemperature");
        assertNotNull(coolantTempExpr);
        assertTrue(coolantTempExpr.contains("useMetricOnInterface"));
        assertTrue(coolantTempExpr.contains("coolant"));

        String intakeTempExpr = model.getExpressionOutputChannel("intakeTemperature");
        assertNotNull(intakeTempExpr);
        assertEquals("{ intake }", intakeTempExpr);

        String lambdaExpr = model.getExpressionOutputChannel("lambdaValue");
        assertNotNull(lambdaExpr);
        assertTrue(lambdaExpr.contains("isLambdaDisplay"));
        assertTrue(lambdaExpr.contains("lambda1"));

        // Verify expression channels are not in the scalar output channels
        assertThrows(IniMemberNotFound.class, () -> model.getOutputChannel("coolantTemperature"));
        assertThrows(IniMemberNotFound.class, () -> model.getOutputChannel("intakeTemperature"));
        assertThrows(IniMemberNotFound.class, () -> model.getOutputChannel("lambdaValue"));
    }

    @Test
    public void testGaugeConfigurationWithSimpleExpressions() {
        // Test with simple division expression that IniField.parseDouble can handle
        String string = "[GaugeConfigurations]\n" +
                "gaugeCategory = TestExpressions\n" +
                "simpleGauge = channel1, \"Simple Gauge\", \"unit\", 0, {10/2}, 0, 1, 4, 5, 1, 1\n" +
                "mathGauge = channel2, \"Math Gauge\", \"unit\", {1/10}, {100/10}, 0, 2, 8, 10, 2, 2\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(1, model.getGaugeCategories().size());
        assertTrue(model.getGaugeCategories().containsKey("TestExpressions"));

        GaugeCategoryModel category = model.getGaugeCategories().get("TestExpressions");
        assertNotNull(category);
        assertEquals(2, category.getGauges().size());

        GaugeModel simpleGauge = model.getGauge("simpleGauge");

        assertNotNull(simpleGauge);
        assertEquals("simpleGauge", simpleGauge.getName());
        assertEquals("channel1", simpleGauge.getChannel());
        assertEquals("Simple Gauge", simpleGauge.getTitle());
        assertEquals("unit", simpleGauge.getUnits());
        assertEquals(0.0, simpleGauge.getLowValue(), EPS);

        // Expression {10/2} evaluates to 5.0
        assertEquals(5.0, simpleGauge.getHighValue(), EPS);
        assertEquals(0.0, simpleGauge.getLowDangerValue(), EPS);
        assertEquals(1.0, simpleGauge.getLowWarningValue(), EPS);
        assertEquals(4.0, simpleGauge.getHighWarningValue(), EPS);
        assertEquals(5.0, simpleGauge.getHighDangerValue(), EPS);
        assertEquals(1, simpleGauge.getValueDecimalPlaces());
        assertEquals(1, simpleGauge.getLabelDecimalPlaces());

        // Test mathGauge with multiple division expressions
        GaugeModel mathGauge = model.getGauge("mathGauge");
        assertNotNull(mathGauge);
        assertEquals("mathGauge", mathGauge.getName());
        assertEquals("channel2", mathGauge.getChannel());
        assertEquals("Math Gauge", mathGauge.getTitle());
        assertEquals("unit", mathGauge.getUnits());
        // Expression {1/10} evaluates to 0.1
        assertEquals(0.1, mathGauge.getLowValue(), EPS);
        // Expression {100/10} evaluates to 10.0
        assertEquals(10.0, mathGauge.getHighValue(), EPS);
        assertEquals(0.0, mathGauge.getLowDangerValue(), EPS);
        assertEquals(2.0, mathGauge.getLowWarningValue(), EPS);
        assertEquals(8.0, mathGauge.getHighWarningValue(), EPS);
        assertEquals(10.0, mathGauge.getHighDangerValue(), EPS);
        assertEquals(2, mathGauge.getValueDecimalPlaces());
        assertEquals(2, mathGauge.getLabelDecimalPlaces());
    }


}
