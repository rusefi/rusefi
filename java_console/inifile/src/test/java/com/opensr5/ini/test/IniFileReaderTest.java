package com.opensr5.ini.test;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.*;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.jetbrains.annotations.NotNull;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.util.Arrays;

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
    private static final double EPS = 0.00001;

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
    public void testTable() {
        String string =
                "[Constants]\n" +
                "page = 1\n" +
                "tpsTpsAccelTable = array, F32, 19744, [8x8], \"value\", 1, 0, 0, 30000, 2\n" +
                "tpsTpsAccelFromRpmBins = array, F32, 20000, [8], \"from\", 1, 0, 0, 30000, 2\n" +
                "tpsTpsAccelToRpmBins = array, F32, 20032, [8], \"to\", 1, 0, 0, 25500, 2\n\n " +
                "[TableEditor]\n" +
                "\ttable = tpsTpsAccelTbl,  tpsTpsAccelMap,  \"TPS/TPS Acceleration Extra Fuel(ms)\",\t1\n" +
                "\ttopicHelp = \"tpstpsHelp\"\n" +
                "\t\txBins\t\t= tpsTpsAccelFromRpmBins,  TPSValue\n" +
                "\t\tyBins\t\t= tpsTpsAccelToRpmBins,  TPSValue\n" +
                "\t\tzBins\t\t= tpsTpsAccelTable";
        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);
        assertEquals(3, model.getAllIniFields().size());
        assertEquals(3, model.getFieldsInUiOrder().size());
        assertTrue(model.getFieldsInUiOrder().containsKey("tpsTpsAccelToRpmBins"));
        assertFalse(model.getFieldsInUiOrder().containsKey("tpsTpsAccelTbl"));
        assertTrue(model.getFieldsInUiOrder().containsKey("tpsTpsAccelTable"));
    }

    @Test
    public void testConditional() {
        String string = "page = 1\n" +
                "[Constants]\n" +
                "#if LAMBDA\n" +
                "\tlambdaTable\t\t\t\t\t = array, U08, 18592, [16x16],\"deg\", 0.006802721088435374, 0, 0.6, 1.5, 2\n" +
                "#else\n" +
                "\tlambdaTable\t\t\t\t\t = array, U08, 18592, [16x16],\"deg\", 0.1, 0, 0, 25.0, 1\n" +
                "#endif\n";
        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(1, model.getAllIniFields().size());
        assertEquals(0, model.getFieldsInUiOrder().size()); // no UI for the field
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
    public void testDirectives() {
        String string = "page = 1\n" +
                "[Constants]\n" +
                "#if LAMBDA\n" +
                "\tname\t= bits,    U32,   \t744, [0:2], \"false\"\n" +
                "#else\n" +
                "\tname\t= bits,    U32,   \t744, [3:4], \"false\", \"true\"\n" +
                "#endif";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(1, model.getAllIniFields().size());

        EnumIniField field = (EnumIniField) model.getAllIniFields().get("name");
        assertEquals(0, field.getBitPosition());
        assertEquals(2, field.getBitSize0());
        assertEquals(1, field.getEnums().size());
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
        assertTrue(model.getDialogs().containsKey("Trigger Settings"));

        DialogModel dialog = model.getDialogs().get("Trigger Settings");
        assertNotNull(dialog);
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
        DialogModel dialog = model.getDialogs().get("Basic Settings");
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

        DialogModel dialog1 = model.getDialogs().get("Dialog One");
        assertNotNull(dialog1);
        assertEquals("helpOne", dialog1.getTopicHelp());

        DialogModel dialog2 = model.getDialogs().get("Dialog Two");
        assertNotNull(dialog2);
        assertEquals("helpTwo", dialog2.getTopicHelp());

        DialogModel dialog3 = model.getDialogs().get("Dialog Three");
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

        // Verify dialogs were created
        assertEquals(4, model.getDialogs().size());
        assertTrue(model.getDialogs().containsKey("MS IO box 1"));
        assertTrue(model.getDialogs().containsKey("MS IO box Container"));
        assertTrue(model.getDialogs().containsKey(""));
        assertTrue(model.getDialogs().containsKey("VE Table"));

        // Verify the msIoBox1 dialog has fields but no panels
        DialogModel msIoBoxDialog = model.getDialogs().get("MS IO box 1");
        assertNotNull(msIoBoxDialog);
        assertEquals(2, msIoBoxDialog.getFields().size());
        assertEquals(0, msIoBoxDialog.getPanels().size());

        // Verify the msIoBox container dialog has a panel reference
        DialogModel msIoBoxContainer = model.getDialogs().get("MS IO box Container");
        assertNotNull(msIoBoxContainer);
        assertEquals(0, msIoBoxContainer.getFields().size());
        assertEquals(1, msIoBoxContainer.getPanels().size());
        PanelModel panel = msIoBoxContainer.getPanels().get(0);
        assertEquals("msIoBox1", panel.getPanelName());
        assertNull(panel.getPlacement());

        // Verify the dialog with empty name is the veTableDialog with topicHelp
        DialogModel veDialog = model.getDialogs().get("");
        assertNotNull(veDialog);
        assertEquals("veTableDialogHelp", veDialog.getTopicHelp());
        assertEquals(1, veDialog.getFields().size());
        assertEquals(0, veDialog.getPanels().size());

        // Verify dialog with "VE Table" name also has topicHelp
        DialogModel veDialog3D = model.getDialogs().get("VE Table");
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
    public void testTableEditorBasic() {
        String string =
                "[Constants]\n" +
                "page = 1\n" +
                "tpsTpsAccelTable = array, F32, 19744, [8x8], \"value\", 1, 0, 0, 30000, 2\n" +
                "tpsTpsAccelFromRpmBins = array, F32, 20000, [8], \"from\", 1, 0, 0, 30000, 2\n" +
                "tpsTpsAccelToRpmBins = array, F32, 20032, [8], \"to\", 1, 0, 0, 25500, 2\n\n " +
                "[TableEditor]\n" +
                "\ttable = tpsTpsAccelTbl,  tpsTpsAccelMap,  \"TPS/TPS Acceleration Extra Fuel(ms)\",\t1\n" +
                "\ttopicHelp = \"tpstpsHelp\"\n" +
                "\t\txBins\t\t= tpsTpsAccelFromRpmBins,  TPSValue\n" +
                "\t\tyBins\t\t= tpsTpsAccelToRpmBins,  TPSValue\n" +
                "\t\tzBins\t\t= tpsTpsAccelTable";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(1, model.getTables().size());
        assertTrue(model.getTables().containsKey("tpsTpsAccelTbl"));

        // Verify table can be retrieved by both IDs
        TableModel table = model.getTable("tpsTpsAccelTbl");
        assertNotNull(table);
        assertEquals("tpsTpsAccelTbl", table.getTableId());
        assertEquals("tpsTpsAccelMap", table.getMap3dId());
        assertEquals("TPS/TPS Acceleration Extra Fuel(ms)", table.getTitle());
        assertEquals("tpstpsHelp", table.getTopicHelp());

        // Verify bins
        assertEquals("tpsTpsAccelFromRpmBins", table.getXBinsConstant());
        assertEquals("TPSValue", table.getXBinsChannel());
        assertFalse(table.isXBinsReadOnly());
        assertEquals("tpsTpsAccelToRpmBins", table.getYBinsConstant());
        assertEquals("TPSValue", table.getYBinsChannel());
        assertFalse(table.isYBinsReadOnly());
        assertEquals("tpsTpsAccelTable", table.getZBinsConstant());

    }

    @Test
    public void testTableEditorWithAllFeatures() {
        String string =
                "[Constants]\n" +
                "page = 1\n" +
                "ALSTimingRetardTable = array, F32, 19744, [8x8], \"value\", 1, 0, 0, 30000, 2\n" +
                "alsIgnRetardrpmBins = array, F32, 20000, [8], \"from\", 1, 0, 0, 30000, 2\n" +
                "alsIgnRetardLoadBins = array, F32, 20032, [8], \"to\", 1, 0, 0, 25500, 2\n\n " +
                "[TableEditor]\n" +
                "\ttable = IgnRetardTableALS, IgnRetardALS,  \"Ignition adjustment\",   1\n" +
                "\t  xyLabels = \"RPM\", \"Load\"\n" +
                "\t  xBins       = alsIgnRetardrpmBins,  RPMValue, readOnly\n" +
                "\t  yBins       = alsIgnRetardLoadBins,  TPSValue\n" +
                "\t  zBins       = ALSTimingRetardTable\n" +
                "\t  gridHeight  = 4.0\n" +
                "\t  gridOrient  = 250,   0, 340\n" +
                "\t  upDownLabel = \"(RICHER)\", \"(LEANER)\"";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        TableModel table = model.getTable("IgnRetardTableALS");
        assertNotNull(table);
        assertEquals("IgnRetardTableALS", table.getTableId());
        assertEquals("IgnRetardALS", table.getMap3dId());
        assertEquals("Ignition adjustment", table.getTitle());

        // Verify xyLabels
        assertEquals("RPM", table.getXLabel());
        assertEquals("Load", table.getYLabel());

        // Verify xBins with readOnly flag
        assertEquals("alsIgnRetardrpmBins", table.getXBinsConstant());
        assertEquals("RPMValue", table.getXBinsChannel());
        assertTrue(table.isXBinsReadOnly());

        // Verify yBins without readOnly flag
        assertEquals("alsIgnRetardLoadBins", table.getYBinsConstant());
        assertEquals("TPSValue", table.getYBinsChannel());
        assertFalse(table.isYBinsReadOnly());

        // Verify zBins
        assertEquals("ALSTimingRetardTable", table.getZBinsConstant());

        // Verify upDownLabel
        assertEquals("(RICHER)", table.getUpLabel());
        assertEquals("(LEANER)", table.getDownLabel());

        // Verify gridHeight
        assertNotNull(table.getGridHeight());
        assertEquals(4.0, table.getGridHeight(), EPS);

        // Verify gridOrient
        assertEquals("250,0,340", table.getGridOrient());
    }

    @Test
    public void testMultipleTables() {
        String string =
                "[Constants]\n" +
                "page = 1\n" +
                "boostTableOpenLoop = array, F32, 1000, [8x8], \"value\", 1, 0, 0, 100, 2\n" +
                "boostRpmBins = array, F32, 2000, [8], \"rpm\", 1, 0, 0, 8000, 0\n" +
                "boostOpenLoopLoadBins = array, F32, 3000, [8], \"load\", 1, 0, 0, 100, 1\n" +
                "boostTableClosedLoop = array, F32, 4000, [8x8], \"value\", 1, 0, 0, 300, 1\n" +
                "boostClosedLoopLoadBins = array, F32, 5000, [8], \"load\", 1, 0, 0, 100, 1\n" +
                "[TableEditor]\n" +
                "\ttable = boostTableTbl,  boostMapOpen,  \"Boost control duty cycle (open loop)\",\t1\n" +
                "\t\txBins\t\t= boostRpmBins,  RPMValue\n" +
                "\t\tyBins\t\t= boostOpenLoopLoadBins,  openLoopYAxis\n" +
                "\t\tzBins\t\t= boostTableOpenLoop\n\n" +
                "\ttable = boostClosedTbl,  boostMapClosed,  \"Boost control target / Closed Loop (kPa)\",\t1\n" +
                "\t\txBins\t\t= boostRpmBins,  RPMValue\n" +
                "\t\tyBins\t\t= boostClosedLoopLoadBins,  TPSValue\n" +
                "\t\tzBins\t\t= boostTableClosedLoop";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(2, model.getTables().size());

        // Verify first table
        TableModel table1 = model.getTable("boostTableTbl");
        assertNotNull(table1);
        assertEquals("boostTableTbl", table1.getTableId());
        assertEquals("boostMapOpen", table1.getMap3dId());
        assertEquals("Boost control duty cycle (open loop)", table1.getTitle());
        assertEquals("boostTableOpenLoop", table1.getZBinsConstant());

        // Verify second table
        TableModel table2 = model.getTable("boostClosedTbl");
        assertNotNull(table2);
        assertEquals("boostClosedTbl", table2.getTableId());
        assertEquals("boostMapClosed", table2.getMap3dId());
        assertEquals("Boost control target / Closed Loop (kPa)", table2.getTitle());
        assertEquals("boostTableClosedLoop", table2.getZBinsConstant());
    }

    @Test
    public void testTableWithoutOptionalFields() {
        String string =
                "[Constants]\n" +
                "page = 1\n" +
                "scriptTable1 = array, F32, 1000, [8x8], \"value\", 1, 0, 0, 100, 2\n" +
                "scriptTable1RpmBins = array, F32, 2000, [8], \"rpm\", 1, 0, 0, 8000, 0\n" +
                "scriptTable1LoadBins = array, F32, 3000, [8], \"load\", 1, 0, 0, 100, 1\n" +
                "[TableEditor]\n" +
                "\ttable = scriptTable1Tbl,  scriptTable1Map,  \"Script Table #1\",\t1\n" +
                "\t\txBins\t\t= scriptTable1RpmBins,  RPMValue\n" +
                "\t\tyBins\t\t= scriptTable1LoadBins,  fuelingLoad\n" +
                "\t\tzBins\t\t= scriptTable1";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        TableModel table = model.getTable("scriptTable1Tbl");
        assertNotNull(table);
        assertEquals("scriptTable1Tbl", table.getTableId());
        assertEquals("scriptTable1Map", table.getMap3dId());
        assertEquals("Script Table #1", table.getTitle());

        // Verify optional fields are null or empty
        assertNull(table.getTopicHelp());
        assertNull(table.getXLabel());
        assertNull(table.getYLabel());
        assertNull(table.getUpLabel());
        assertNull(table.getDownLabel());
        assertNull(table.getGridHeight());
        assertNull(table.getGridOrient());
    }

    @Test
    public void testEmptyTableEditor() {
        String string = "[TableEditor]\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(0, model.getTables().size());
    }

    @Test
    public void testGetDialogKeyByTitle() {
        String string = "[Constants]\n" +
                "page = 1\n" +
                "veTable = array, F32, 0, [16x16], \"value\", 1, 0, 0, 100, 2\n" +
                "field1 = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
                "[SettingContextHelp]\n" +
                "; SettingContextHelpEnd\n" +
                "\thelp = veTableDialogHelp, \"Volumetric Efficiency\"\n" +
                "\t\ttext = \"VE help text\"\n" +
                "\t\twebHelp = \"https://rusefi.com/s/fuel\"\n" +
                "\n" +
                "\tdialog = veTableDialog, \"VE Table\"\n" +
                "\t\ttopicHelp = \"veTableDialogHelp\"\n" +
                "\t\tfield = \"VE Table\", veTable\n" +
                "\n" +
                "\tdialog = engineChars, \"Base Engine Settings\"\n" +
                "\t\tfield = \"Field 1\", field1\n" +
                "\n" +
                "\tdialog = idleSettings, \"Idle Control\"\n" +
                "\t\tfield = \"Field 1\", field1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Test finding dialog key by title
        String veDialogKey = model.getDialogKeyByTitle("VE Table");
        assertEquals("veTableDialog", veDialogKey);

        String engineCharsKey = model.getDialogKeyByTitle("Base Engine Settings");
        assertEquals("engineChars", engineCharsKey);

        String idleSettingsKey = model.getDialogKeyByTitle("Idle Control");
        assertEquals("idleSettings", idleSettingsKey);

        // Test with non-existent title
        String nonExistent = model.getDialogKeyByTitle("Non Existent Dialog");
        assertNull(nonExistent);

        // Test with empty title
        String emptyTitle = model.getDialogKeyByTitle("");
        assertNull(emptyTitle);
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

    @Test
    public void testGaugeConfigurationWithComplexExpressions() {
        // Complex expressions with variables and operators like +, -, * are stored as expressions
        // and marked as needing runtime evaluation via needsOutputChannelEvaluation()
        String string = "[Constants]\n" +
                "page = 1\n" +
                "rpmHardLimit = scalar, U16, 100, \"rpm\", 1, 0, 0, 10000, 0\n" +
                "[GaugeConfigurations]\n" +
                "gaugeCategory = TestComplexExpressions\n" +
                // This gauge has complex expressions that will be stored for runtime evaluation
                "complexGauge = RPMValue, \"RPM\", \"RPM\", 0, {rpmHardLimit + 2000}, 0, {cranking_rpm}, {rpmHardLimit - 500}, {rpmHardLimit}, 0, 0\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // The gauge category should be created and contain the gauge
        assertEquals(1, model.getGaugeCategories().size());
        assertTrue(model.getGaugeCategories().containsKey("TestComplexExpressions"));

        GaugeCategoryModel category = model.getGaugeCategories().get("TestComplexExpressions");
        assertNotNull(category);
        assertEquals(1, category.getGauges().size());

        // The gauge with complex expressions should be in the model
        GaugeModel complexGauge = model.getGauge("complexGauge");
        assertNotNull(complexGauge);
        assertEquals("complexGauge", complexGauge.getName());
        assertEquals("RPMValue", complexGauge.getChannel());

        // The gauge should indicate it needs runtime evaluation
        assertTrue(complexGauge.needsOutputChannelEvaluation());

        // Numeric getters should return 0.0 for expression values
        assertEquals(0.0, complexGauge.getLowValue(), EPS);
        assertEquals(0.0, complexGauge.getHighValue(), EPS);  // Expression: {rpmHardLimit + 2000}
        assertEquals(0.0, complexGauge.getLowWarningValue(), EPS);  // Expression: {cranking_rpm}
        assertEquals(0.0, complexGauge.getHighWarningValue(), EPS);  // Expression: {rpmHardLimit - 500}
        assertEquals(0.0, complexGauge.getHighDangerValue(), EPS);  // Expression: {rpmHardLimit}

        // IniValue getters should preserve the expressions
        assertTrue(complexGauge.getHighValueValue().isExpression());
        assertEquals("{rpmHardLimit + 2000}", complexGauge.getHighValueValue().getStringValue());

        assertTrue(complexGauge.getLowWarningValueValue().isExpression());
        assertEquals("{cranking_rpm}", complexGauge.getLowWarningValueValue().getStringValue());

        assertTrue(complexGauge.getHighWarningValueValue().isExpression());
        assertEquals("{rpmHardLimit - 500}", complexGauge.getHighWarningValueValue().getStringValue());
    }

    @Test
    public void testDialogWithPanels() {
        String string = "[Constants]\n" +
                "page = 1\n" +
                "field1 = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
                "field2 = scalar, F32, 4, \"unit\", 1, 0, 0, 100, 1\n" +
                "[SettingContextHelp]\n" +
                "; SettingContextHelpEnd\n" +
                "\tdialog = subDialog1, \"Sub Dialog 1\"\n" +
                "\t\tfield = \"Field 1\", field1\n" +
                "\n" +
                "\tdialog = subDialog2, \"Sub Dialog 2\"\n" +
                "\t\tfield = \"Field 2\", field2\n" +
                "\n" +
                "\tdialog = mainDialog, \"Main Dialog\", yAxis\n" +
                "\t\tpanel = subDialog1\n" +
                "\t\tpanel = subDialog2, Center\n" +
                "\n" +
                "\tdialog = complexDialog, \"Complex Dialog\", border\n" +
                "\t\tpanel = subDialog1, West\n" +
                "\t\tpanel = subDialog2, Center, {field1 > 50}, {field2 < 100}\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Verify dialogs were created
        assertEquals(4, model.getDialogs().size());

        // Test mainDialog with panels
        DialogModel mainDialog = model.getDialogs().get("Main Dialog");
        assertNotNull(mainDialog);
        assertEquals(2, mainDialog.getPanels().size());
        assertEquals(0, mainDialog.getFields().size());

        // Check first panel (no placement)
        PanelModel panel1 = mainDialog.getPanels().get(0);
        assertEquals("subDialog1", panel1.getPanelName());
        assertNull(panel1.getPlacement());
        assertNull(panel1.getEnableExpression());
        assertNull(panel1.getVisibleExpression());

        // Check second panel (with placement)
        PanelModel panel2 = mainDialog.getPanels().get(1);
        assertEquals("subDialog2", panel2.getPanelName());
        assertEquals("Center", panel2.getPlacement());
        assertNull(panel2.getEnableExpression());
        assertNull(panel2.getVisibleExpression());

        // Test complexDialog with panels with expressions
        DialogModel complexDialog = model.getDialogs().get("Complex Dialog");
        assertNotNull(complexDialog);
        assertEquals(2, complexDialog.getPanels().size());

        // Check first panel (West placement, no expressions)
        PanelModel complexPanel1 = complexDialog.getPanels().get(0);
        assertEquals("subDialog1", complexPanel1.getPanelName());
        assertEquals("West", complexPanel1.getPlacement());
        assertNull(complexPanel1.getEnableExpression());
        assertNull(complexPanel1.getVisibleExpression());

        // Check second panel (Center placement, with enable and visible expressions)
        PanelModel complexPanel2 = complexDialog.getPanels().get(1);
        assertEquals("subDialog2", complexPanel2.getPanelName());
        assertEquals("Center", complexPanel2.getPlacement());
        assertEquals("{field1 > 50}", complexPanel2.getEnableExpression());
        assertEquals("{field2 < 100}", complexPanel2.getVisibleExpression());
    }

    @Test
    public void testDialogPanelFieldPath() {
        // This test demonstrates the hierarchy: Dialog -> Panel -> Field
        // A parent dialog references child dialogs via panels, which contain fields
        String string = "[Constants]\n" +
                "page = 1\n" +
                "injectorFlow = scalar, F32, 0, \"cc/min\", 1, 0, 0, 2000, 1\n" +
                "injectorPressure = scalar, F32, 4, \"kPa\", 1, 0, 0, 1000, 1\n" +
                "cylinderCount = scalar, U08, 8, \"cylinders\", 1, 0, 1, 12, 0\n" +
                "displacement = scalar, F32, 12, \"L\", 1, 0, 0, 10, 2\n" +
                "[SettingContextHelp]\n" +
                "; SettingContextHelpEnd\n" +
                "\n" +
                "dialog = injectorSettings, \"Injector Configuration\"\n" +
                "\t\tfield = \"Flow Rate\", injectorFlow\n" +
                "\t\tfield = \"Pressure\", injectorPressure\n" +
                "\n" +
                "\tdialog = engineSpecs, \"Engine Specifications\"\n" +
                "\t\tfield = \"Number of Cylinders\", cylinderCount\n" +
                "\t\tfield = \"Displacement\", displacement\n" +
                "\n" +
                "\tdialog = fuelSystemTab, \"Fuel System\", yAxis\n" +
                "\t\tpanel = injectorSettings\n" +
                "\t\tpanel = engineSpecs\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Verify all dialogs were created
        assertEquals(3, model.getDialogs().size());

        // Get the parent dialog (contains panels)
        DialogModel fuelSystemTab = model.getDialogs().get("Fuel System");
        assertNotNull(fuelSystemTab);
        assertEquals("fuelSystemTab", fuelSystemTab.getKey());
        assertEquals(0, fuelSystemTab.getFields().size()); // Parent has no direct fields
        assertEquals(2, fuelSystemTab.getPanels().size()); // Parent has 2 panels

        // Verify panel references
        PanelModel panel1 = fuelSystemTab.getPanels().get(0);
        assertEquals("injectorSettings", panel1.getPanelName());

        PanelModel panel2 = fuelSystemTab.getPanels().get(1);
        assertEquals("engineSpecs", panel2.getPanelName());

        // Get the first child dialog via panel reference
        DialogModel injectorSettings = model.getDialogs().get("Injector Configuration");
        assertNotNull(injectorSettings);
        assertEquals("injectorSettings", injectorSettings.getKey());
        assertEquals(2, injectorSettings.getFields().size()); // Child has fields
        assertEquals(0, injectorSettings.getPanels().size()); // Child has no panels

        // Verify fields in first child dialog
        DialogModel.Field field1 = injectorSettings.getFields().get(0);
        assertEquals("injectorFlow", field1.getKey());
        assertEquals("Flow Rate", field1.getUiName());

        DialogModel.Field field2 = injectorSettings.getFields().get(1);
        assertEquals("injectorPressure", field2.getKey());
        assertEquals("Pressure", field2.getUiName());

        // Get the second child dialog via panel reference
        DialogModel engineSpecs = model.getDialogs().get("Engine Specifications");
        assertNotNull(engineSpecs);
        assertEquals("engineSpecs", engineSpecs.getKey());
        assertEquals(2, engineSpecs.getFields().size());
        assertEquals(0, engineSpecs.getPanels().size());

        // Verify fields in second child dialog
        DialogModel.Field field3 = engineSpecs.getFields().get(0);
        assertEquals("cylinderCount", field3.getKey());
        assertEquals("Number of Cylinders", field3.getUiName());

        DialogModel.Field field4 = engineSpecs.getFields().get(1);
        assertEquals("displacement", field4.getKey());
        assertEquals("Displacement", field4.getUiName());

        // Demonstrate the complete path: fuelSystemTab -> injectorSettings -> injectorFlow
        // This simulates how a UI would navigate the hierarchy:
        // 1. Start with parent dialog
        assertEquals("fuelSystemTab", fuelSystemTab.getKey());

        // 2. Get first panel reference
        PanelModel firstPanel = fuelSystemTab.getPanels().get(0);
        assertEquals("injectorSettings", firstPanel.getPanelName());

        // 3. Resolve the child dialog using the simplified lookup method
        DialogModel childDialog = firstPanel.resolveDialog(model);
        assertNotNull(childDialog);
        assertEquals("injectorSettings", childDialog.getKey());

        // 4. Access fields in the child dialog
        assertEquals(2, childDialog.getFields().size());
        assertEquals("injectorFlow", childDialog.getFields().get(0).getKey());

        // Demonstrate simplified one-liner navigation
        DialogModel secondChild = fuelSystemTab.getPanels().get(1).resolveDialog(model);
        assertNotNull(secondChild);
        assertEquals("engineSpecs", secondChild.getKey());
        assertEquals("cylinderCount", secondChild.getFields().get(0).getKey());
    }

    @Test
    public void testGaugeConfigurationWithStringFunctionExpressions() {
        String string = "[Constants]\n" +
                "page = 1\n" +
                "gpPwmNote1 = string, ASCII, 200, 16\n" +
                "[GaugeConfigurations]\n" +
                "gaugeCategory = GPPWMGauges\n" +
                "gppwmGauge1 = gppwmOutput1, { stringValue(gpPwmNote1) }, \"%\", 0, 100, 0, 0, 100, 100, 1, 1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Test that gauge with stringValue() expression is parsed successfully
        GaugeModel gauge = model.getGauge("gppwmGauge1");
        assertNotNull(gauge);
        assertEquals("gppwmGauge1", gauge.getName());
        assertEquals("gppwmOutput1", gauge.getChannel());

        // Title should keep the expression as-is (evaluation happens at runtime)
        assertEquals("{ stringValue(gpPwmNote1) }", gauge.getTitle());

        // Units is a plain string (no expression)
        assertEquals("%", gauge.getUnits());

        // Numeric values
        assertEquals(0.0, gauge.getLowValue(), EPS);
        assertEquals(100.0, gauge.getHighValue(), EPS);
        assertEquals(1, gauge.getValueDecimalPlaces());
        assertEquals(1, gauge.getLabelDecimalPlaces());

        // Gauge should indicate it needs runtime evaluation due to stringValue() in title
        assertTrue(gauge.needsOutputChannelEvaluation());

        // Check that title is stored as an expression (contains function call)
        assertTrue(gauge.getTitleValue().isExpression());
        assertEquals("{ stringValue(gpPwmNote1) }", gauge.getTitleValue().getStringValue());

        // Units is stored as a plain string (no expression markers)
        assertTrue(gauge.getUnitsValue().isString());
        assertEquals("%", gauge.getUnitsValue().getStringValue());
    }
}
