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

    @Test
    public void testBitField() {
        String string = "page = 1\n" +
                "[Constants]\n" +
                "\tname\t= bits,    U32,   \t744, [3:5], \"false\", \"true\"";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(1, model.getAllIniFields().size());

        EnumIniField field = (EnumIniField) model.getAllIniFields().get("name");
        assertEquals(3, field.getBitPosition());
        assertEquals(2, field.getBitSize0());
        assertEquals(2, field.getEnums().size());
    }

    @Test
    public void testCurveField() {
        String string = "page = 1\n" +
                " \tname2\t\t\t= array, F32,\t108,\t[8],\t\"\", 1, 0, 0.0, 18000, 2\n" +
                "[Constants]\n" +
                " \tname\t\t\t= array, F32,\t108,\t[8],\t\"\", 1, 0, 0.0, 18000, 2\n" +
                "[PcVariables]\n" +
                " \tname3\t\t\t= array, F32,\t108,\t[8],\t\"\", 1, 0, 0.0, 18000, 2\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(1, model.getAllIniFields().size());
        ArrayIniField field = (ArrayIniField) model.getAllIniFields().get("name");
        assertNotNull(field);
        assertEquals(1, field.getCols());
        assertEquals(8, field.getRows());
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
    public void testGaugeConfiguration() {
        String string = "[GaugeConfigurations]\n" +
                "gaugeCategory = Engine\n" +
                "rpmGauge = RPMValue, \"Engine Speed\", \"RPM\", 0, 8000, 0, 0, 6000, 8000, 0, 0\n" +
                "cltGauge = coolant, \"Coolant Temp\", \"°C\", -40, 120, -40, 0, 100, 120, 1, 0\n" +
                "gaugeCategory = Sensors\n" +
                "tpsGauge = TPSValue, \"Throttle Position\", \"%\", 0, 100, 0, 0, 90, 100, 1, 1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Test gauge categories
        assertEquals(2, model.getGaugeCategories().size());
        assertTrue(model.getGaugeCategories().containsKey("Engine"));
        assertTrue(model.getGaugeCategories().containsKey("Sensors"));

        // Test Engine category
        GaugeCategoryModel engineCategory = model.getGaugeCategories().get("Engine");
        assertNotNull(engineCategory);
        assertEquals("Engine", engineCategory.getName());
        assertEquals(2, engineCategory.getGauges().size());

        // Test Sensors category
        GaugeCategoryModel sensorsCategory = model.getGaugeCategories().get("Sensors");
        assertNotNull(sensorsCategory);
        assertEquals("Sensors", sensorsCategory.getName());
        assertEquals(1, sensorsCategory.getGauges().size());

        // Test all gauges map
        assertEquals(3, model.getGauges().size());
        assertTrue(model.getGauges().containsKey("rpmGauge"));
        assertTrue(model.getGauges().containsKey("cltGauge"));
        assertTrue(model.getGauges().containsKey("tpsGauge"));

        // Test individual gauge retrieval
        GaugeModel rpmGauge = model.getGauge("rpmGauge");
        assertNotNull(rpmGauge);
        assertEquals("rpmGauge", rpmGauge.getName());
        assertEquals("RPMValue", rpmGauge.getChannel());
        assertEquals("Engine Speed", rpmGauge.getTitle());
        assertEquals("RPM", rpmGauge.getUnits());
        assertEquals(0.0, rpmGauge.getLowValue(), EPS);
        assertEquals(8000.0, rpmGauge.getHighValue(), EPS);
        assertEquals(0.0, rpmGauge.getLowDangerValue(), EPS);
        assertEquals(0.0, rpmGauge.getLowWarningValue(), EPS);
        assertEquals(6000.0, rpmGauge.getHighWarningValue(), EPS);
        assertEquals(8000.0, rpmGauge.getHighDangerValue(), EPS);
        assertEquals(0, rpmGauge.getValueDecimalPlaces());
        assertEquals(0, rpmGauge.getLabelDecimalPlaces());

        // Test coolant gauge with different decimal places
        GaugeModel cltGauge = model.getGauge("cltGauge");
        assertNotNull(cltGauge);
        assertEquals("cltGauge", cltGauge.getName());
        assertEquals("coolant", cltGauge.getChannel());
        assertEquals("Coolant Temp", cltGauge.getTitle());
        assertEquals("°C", cltGauge.getUnits());
        assertEquals(-40.0, cltGauge.getLowValue(), EPS);
        assertEquals(120.0, cltGauge.getHighValue(), EPS);
        assertEquals(-40.0, cltGauge.getLowDangerValue(), EPS);
        assertEquals(0.0, cltGauge.getLowWarningValue(), EPS);
        assertEquals(100.0, cltGauge.getHighWarningValue(), EPS);
        assertEquals(120.0, cltGauge.getHighDangerValue(), EPS);
        assertEquals(1, cltGauge.getValueDecimalPlaces());
        assertEquals(0, cltGauge.getLabelDecimalPlaces());

        // Test TPS gauge
        GaugeModel tpsGauge = model.getGauge("tpsGauge");
        assertNotNull(tpsGauge);
        assertEquals("tpsGauge", tpsGauge.getName());
        assertEquals("TPSValue", tpsGauge.getChannel());
        assertEquals("Throttle Position", tpsGauge.getTitle());
        assertEquals("%", tpsGauge.getUnits());
        assertEquals(1, tpsGauge.getValueDecimalPlaces());
        assertEquals(1, tpsGauge.getLabelDecimalPlaces());
    }

    @Test
    public void testGaugeConfigurationWithoutCategory() {
        String string = "[GaugeConfigurations]\n" +
                "rpmGauge = RPMValue, \"Engine Speed\", \"RPM\", 0, 8000, 0, 0, 6000, 8000, 0, 0\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Gauges without a category should still be in the all gauges map
        assertEquals(1, model.getGauges().size());
        assertTrue(model.getGauges().containsKey("rpmGauge"));

        GaugeModel rpmGauge = model.getGauge("rpmGauge");
        assertNotNull(rpmGauge);
        assertEquals("rpmGauge", rpmGauge.getName());
    }

    @Test
    public void testEmptyGaugeConfiguration() {
        String string = "[GaugeConfigurations]\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(0, model.getGaugeCategories().size());
        assertEquals(0, model.getGauges().size());
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
    public void testIncompleteGaugeConfiguration() {
        // Test gauge with only required fields: name, channel, title, units, lowValue, highValue
        String string = "[GaugeConfigurations]\n" +
                "egt1Gauge = egt1, \"EGT#1\", \"C\", 0, 2000\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Gauge should be parsed successfully
        assertEquals(1, model.getGauges().size());
        assertTrue(model.getGauges().containsKey("egt1Gauge"));

        GaugeModel gauge = model.getGauge("egt1Gauge");
        assertNotNull(gauge);
        assertEquals("egt1Gauge", gauge.getName());
        assertEquals("egt1", gauge.getChannel());
        assertEquals("EGT#1", gauge.getTitle());
        assertEquals("C", gauge.getUnits());
        assertEquals(0.0, gauge.getLowValue(), EPS);
        assertEquals(2000.0, gauge.getHighValue(), EPS);

        // Missing fields should default to sensible values
        assertEquals(0.0, gauge.getLowDangerValue(), EPS);  // defaults to lowValue
        assertEquals(0.0, gauge.getLowWarningValue(), EPS);  // defaults to lowValue
        assertEquals(2000.0, gauge.getHighWarningValue(), EPS);  // defaults to highValue
        assertEquals(2000.0, gauge.getHighDangerValue(), EPS);  // defaults to highValue
        assertEquals(0, gauge.getValueDecimalPlaces());  // defaults to 0
        assertEquals(0, gauge.getLabelDecimalPlaces());  // defaults to 0
    }

    @Test
    public void testPartialGaugeConfiguration() {
        // Test gauge with some optional fields provided
        String string = "[GaugeConfigurations]\n" +
                "testGauge = channel, \"Test\", \"units\", 0, 100, 5, 10, 90\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        GaugeModel gauge = model.getGauge("testGauge");
        assertNotNull(gauge);
        assertEquals(0.0, gauge.getLowValue(), EPS);
        assertEquals(100.0, gauge.getHighValue(), EPS);
        assertEquals(5.0, gauge.getLowDangerValue(), EPS);
        assertEquals(10.0, gauge.getLowWarningValue(), EPS);
        assertEquals(90.0, gauge.getHighWarningValue(), EPS);
        assertEquals(100.0, gauge.getHighDangerValue(), EPS);  // defaults to highValue
        assertEquals(0, gauge.getValueDecimalPlaces());  // defaults to 0
        assertEquals(0, gauge.getLabelDecimalPlaces());  // defaults to 0
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
                "\tdialog = msIoBox, \"\"\n" +
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
        // Note: Two dialogs have empty name "", so one overwrites the other in the map
        assertEquals(3, model.getDialogs().size());
        assertTrue(model.getDialogs().containsKey("MS IO box 1"));
        assertTrue(model.getDialogs().containsKey(""));
        assertTrue(model.getDialogs().containsKey("VE Table"));

        // Verify the dialog with empty name is the last one defined (veTableDialog with topicHelp)
        DialogModel veDialog = model.getDialogs().get("");
        assertNotNull(veDialog);
        assertEquals("veTableDialogHelp", veDialog.getTopicHelp());

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
}
