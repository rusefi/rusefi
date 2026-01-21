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
    public void testSplitWithEmptyUnits() {
        {
            String[] s = IniFileReaderUtil.splitTokens("\tverboseCanBaseAddress\t\t\t= \"\", 1");
            assertArrayEquals(new String[] { "verboseCanBaseAddress", "", "1" }, s);
        }
        {
            String[] s = IniFileReaderUtil.splitTokens("\tverboseCanBaseAddress\t\t\t= scalar, U32,\t756,\t\"\", 1, 0, 0, 536870911, 0");
            assertArrayEquals(new String[] { "verboseCanBaseAddress", "scalar", "U32", "756", "", "1", "0", "0", "536870911", "0" }, s);
        }
    }

    @Test
    public void testSplit() {
        {
            String[] s = IniFileReaderUtil.splitTokens("1");
            assertArrayEquals(new String[] { "1" }, s);
        }
        {
            String[] s = IniFileReaderUtil.splitTokens("hello");
            assertArrayEquals(new String[] { "hello" }, s);
        }
        {
            String[] s = IniFileReaderUtil.splitTokens("\"hello\"");
            assertArrayEquals(new String[] { "hello" }, s);
        }
        {
            String[] s = IniFileReaderUtil.splitTokens("\"hello\",\"w\"");
            assertArrayEquals(new String[] { "hello", "w" }, s);
        }
    }

    @Test
    public void testBraces() {
        {
            String[] result = IniFileReaderUtil.splitTokens(
                "veLoadBins = array, U16, 17056, [16], {bitStringValue(fuelUnits, fuelAlgorithm) }, 1, 0, 0, 1000, 0"
            );
            assertArrayEquals(new String[] {
                "veLoadBins", "array", "U16", "17056", "[16]", "{bitStringValue(fuelUnits, fuelAlgorithm) }", "1", "0",
                "0", "1000", "0"
            }, result);
        }
    }

    @Test
    public void testRealLine() {
        String[] result = IniFileReaderUtil.splitTokens("\tdialog = engineChars,\t\"Base Engine Settings\"");
        assertArrayEquals(new String[] { "dialog", "engineChars", "Base Engine Settings" }, result);
        assertEquals(result.length, 3);
    }

    @Test
    public void testQuotedTokens() {
        {
            String[] result = IniFileReaderUtil.splitTokens("\"hel  lo\"");
            assertArrayEquals(new String[] { "hel  lo" }, result);
        }
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
    public void testCurve() {
        String string =
            "   ; generates [SettingContextHelp]\n" +
                "[Constants]\n" +
                "page = 1\n" +
                "scriptCurve1Bins = array, F32, 4828, [16], \"x\", 1, 0, -10000, 10000, 3\n" +
                "scriptCurve1 = array, F32, 4892, [16], \"y\", 1, 0, -10000, 10000, 3\n " +
                "[CurveEditor]\n" +
                "\tcurve = scriptCurve1, \"Script Curve #1\"\n" +
                "\t\tcolumnLabel = \"X\", \"Y\"\n" +
                "\t\txAxis\t\t=  0, 128, 10\n" +
                "\t\tyAxis\t\t= -155,  150, 10\n" +
                "\t\txBins\t\t= scriptCurve1Bins\n" +
                "\t\tyBins\t\t= scriptCurve1\n" +
                "\t\tshowTextValues = true\n";
        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);
        // technically these are constants?!
        assertEquals(2, model.getAllIniFields().size());
        assertEquals(2, model.getFieldsInUiOrder().size());
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
    public void testEasyFields() {
        String string = "page = 1\n" +
                "[Constants]\n" +
                "primingSquirtDurationMs\t\t\t= scalar, F32,\t96,\t\"*C\", 1, 0, -40, 200, 1\n" +
                "\tiat_adcChannel\t\t\t\t = bits, U08, 312, [0:7] \"PA0\", \"PA1\", \"PA2\", \"PA3\", \"PA4\", \"PA5\", \"PA6\", \"PA7\", \"PB0\", \"PB1\", \"PC0\", \"PC1\", \"PC2\", \"PC3\", \"PC4\", \"PC5\", \"Disabled\", \"PB12\", \"PB13\", \"PC14\", \"PC15\", \"PC16\", \"PC17\", \"PD3\", \"PD4\", \"PE2\", \"PE6\", \"INVALID\", \"INVALID\", \"INVALID\", \"INVALID\", \"INVALID\"\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(2, model.getAllIniFields().size());
    }

    @Test
    public void testEnumListFields() {
        String string = "#define gpio_list=\"NONE\", \"INVALID\", \"PA0\", \"PA1\", \"PA2\", \"PA3\", \"PA4\"\n" +
                "page = 1\n" +
                "[Constants]\n" +
                "primingSquirtDurationMs\t\t\t= scalar, F32,\t96,\t\"*C\", 1, 0, -40, 200, 1\n" +
                "\tiat_adcChannel\t\t\t\t = bits, U08, 312, [0:7] $gpio_list\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);
        assertEquals(1, model.getDefines().size());

        EnumIniField field = (EnumIniField) model.getAllIniFields().get("iat_adcChannel");
        assertEquals(7, field.getEnums().size());
        assertEquals(2, model.getAllIniFields().size());
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

    private static @NotNull IniFileModel readLines(RawIniFile lines) {
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
}
