package com.opensr5.ini.test;

import com.opensr5.ini.GaugeCategoryModel;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;

import static com.opensr5.ini.test.IniFileReaderTest.readLines;
import static org.junit.jupiter.api.Assertions.*;

public class GaugeReaderTest {
    private static final double EPS = 0.00001;

    @Test
    public void testEmptyGaugeConfiguration() {
        String string = "[GaugeConfigurations]\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        assertEquals(0, model.getGaugeCategories().size());
        assertEquals(0, model.getGauges().size());
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
}
