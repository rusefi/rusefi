package com.rusefi.ui.logview;

import com.opensr5.ini.DatalogEntry;
import com.opensr5.ini.GaugeCategoryModel;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import static com.rusefi.ui.logview.SignalCategories.Group;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class SignalCategoriesTest {
    private static CsvLogReader log(String... labels) throws IOException {
        StringBuilder text = new StringBuilder("Time");
        for (String label : labels) {
            text.append('\t').append(label);
        }
        text.append("\ns");
        for (String ignored : labels) {
            text.append('\t');
        }
        text.append("\n0");
        for (String ignored : labels) {
            text.append("\t1");
        }
        text.append('\n');
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static GaugeModel gauge(String channel) {
        GaugeModel gauge = mock(GaugeModel.class);
        when(gauge.getChannel()).thenReturn(channel);
        return gauge;
    }

    /** An .ini with the given [Datalog] channel/label pairs and gauge categories. */
    private static IniFileModel ini(String[][] datalog, Object[][] categories) {
        IniFileModel ini = mock(IniFileModel.class);
        List<DatalogEntry> entries = new ArrayList<>();
        for (String[] entry : datalog) {
            entries.add(new DatalogEntry(entry[0], entry[1]));
        }
        Map<String, GaugeCategoryModel> gaugeCategories = new LinkedHashMap<>();
        for (Object[] category : categories) {
            List<GaugeModel> gauges = new ArrayList<>();
            for (String channel : (String[]) category[1]) {
                gauges.add(gauge(channel));
            }
            gaugeCategories.put((String) category[0], new GaugeCategoryModel((String) category[0], gauges));
        }
        when(ini.getDatalogEntries()).thenReturn(entries);
        when(ini.getGaugeCategories()).thenReturn(gaugeCategories);
        return ini;
    }

    @Test
    void rulesPlaceSignalsByName() {
        assertEquals(Group.STATUS, SignalCategories.ruleGroup("Error: TPS"));
        assertEquals(Group.STATUS, SignalCategories.ruleGroup("isO2HeaterOn"));
        assertEquals(Group.STATUS, SignalCategories.ruleGroup("dfcoActive"));
        assertEquals(Group.STATUS, SignalCategories.ruleGroup("sparkCutReasonBlinker"));
        assertEquals(Group.RAW, SignalCategories.ruleGroup("rawTps1Primary"));
        assertEquals(Group.SENSORS, SignalCategories.ruleGroup("CLT"));
        assertEquals(Group.SENSORS, SignalCategories.ruleGroup("Oil Pressure"));
        assertEquals(Group.SENSORS, SignalCategories.ruleGroup("wb1WBO: ESR"));
        assertEquals(Group.FUEL, SignalCategories.ruleGroup("Fuel: injector lag"));
        assertEquals(Group.IGNITION, SignalCategories.ruleGroup("Ign: Timing Cyl 1"));
        assertEquals(Group.AIR, SignalCategories.ruleGroup("Air: Cylinder airmass"));
        assertEquals(Group.THROTTLE, SignalCategories.ruleGroup("etb1ETB: duty"));
        assertEquals(Group.KNOCK, SignalCategories.ruleGroup("Knock: count"));
        assertEquals(Group.TRIGGER, SignalCategories.ruleGroup("trgSync: Trigger Latest Ratio"));
        assertEquals(Group.TRANSMISSION, SignalCategories.ruleGroup("Detected Gear"));
        assertEquals(Group.LUA, SignalCategories.ruleGroup("Lua: torque"));
        assertEquals(Group.CAN, SignalCategories.ruleGroup("CAN: Rx"));
        assertEquals(Group.ECU, SignalCategories.ruleGroup("ECU: CPU Temperature"));
        assertEquals(Group.ECU, SignalCategories.ruleGroup("outputRequestPeriod"));
        assertEquals(Group.OTHER, SignalCategories.ruleGroup("gppwmXAxis 3"));
    }

    /** Names from a real drive log that the first rules missed. */
    @Test
    void rulesPlaceTypicalDriveLogNames() {
        assertEquals(Group.SENSORS, SignalCategories.ruleGroup("Rear Lambda"));
        assertEquals(Group.SENSORS, SignalCategories.ruleGroup("Front Cyl Air/Fuel Ratio"));
        assertEquals(Group.SENSORS, SignalCategories.ruleGroup("Batt V"));
        assertEquals(Group.SENSORS, SignalCategories.ruleGroup("Instant MAP"));
        assertEquals(Group.SENSORS, SignalCategories.ruleGroup("rearWheelSpeedKph"));
        assertEquals(Group.SENSORS, SignalCategories.ruleGroup("imuPitch"));
        assertEquals(Group.THROTTLE, SignalCategories.ruleGroup("tps1Split"));
        assertEquals(Group.THROTTLE, SignalCategories.ruleGroup("Twist Grip"));
        assertEquals(Group.FUEL, SignalCategories.ruleGroup("injectionMass 1"));
        assertEquals(Group.FUEL, SignalCategories.ruleGroup("currentVe"));
        assertEquals(Group.FUEL, SignalCategories.ruleGroup("afrTableYAxis"));
        assertEquals(Group.IGNITION, SignalCategories.ruleGroup("coilState1"), "contains 'oil' but is no oil sensor");
        assertEquals(Group.ECU, SignalCategories.ruleGroup("engineRunTime"));
        assertEquals(Group.IDLE, SignalCategories.ruleGroup("m_lastTargetRpm"));
        assertEquals(Group.STATUS, SignalCategories.ruleGroup("allowStarter"));
    }

    @Test
    void iniCategoriesMergeIntoFriendlyGroups() {
        assertEquals(Group.SENSORS, SignalCategories.iniGroup("Sensors - Extra 2"));
        assertEquals(Group.RAW, SignalCategories.iniGroup("Sensors - Raw"));
        assertNull(SignalCategories.iniGroup("AE / Transient fueling"), "an overview of other subsystems' inputs");
        assertEquals(Group.FUEL, SignalCategories.iniGroup("Acceleration Enrichment"));
        assertEquals(Group.FUEL, SignalCategories.iniGroup("Fuel: math"));
        assertEquals(Group.THROTTLE, SignalCategories.iniGroup("Etb PID"));
        assertEquals(Group.THROTTLE, SignalCategories.iniGroup("Throttle Body (incl. ETB)"));
        assertEquals(Group.ECU, SignalCategories.iniGroup("ECU Status"));
        assertNull(SignalCategories.iniGroup("Cooling Fans"), "no friendly group: the name rules decide");
    }

    @Test
    void iniWinsOverRulesAndPrefersTheFirstGroupInOrder() throws IOException {
        CsvLogReader log = log("CLT", "Odd Label", "Fan state", "Fuel: injector lag");
        IniFileModel ini = ini(
                new String[][]{{"coolant", "CLT"}, {"oddChannel", "Odd Label"}, {"fan", "Fan state"},
                        {"injectorLag", "Fuel: injector lag"}},
                new Object[][]{
                        {"AE / Transient fueling", new String[]{"coolant"}},
                        {"Sensors - Basic", new String[]{"coolant"}},
                        {"Knock", new String[]{"oddChannel"}},
                        {"Cooling Fans", new String[]{"fan"}},
                        {"Fueling", new String[]{"injectorLag"}},
                });
        SignalCategories categories = SignalCategories.of(log, ini);
        assertEquals(Group.SENSORS, categories.group(1), "CLT: Sensors; the AE overview page is ignored");
        assertEquals(Group.KNOCK, categories.group(2), "the .ini places a name no rule knows");
        assertEquals(Group.OTHER, categories.group(3), "category without a friendly group: rules, then Other");
        assertEquals(Group.FUEL, categories.group(4));
    }

    /** As in the generated .ini: the AE overview page also shows RPM, CLT, TPS and MAP. */
    @Test
    void theAeOverviewPageDoesNotPullInputsIntoFuel() throws IOException {
        CsvLogReader log = log("CLT", "TPS", "MAP");
        IniFileModel ini = ini(
                new String[][]{{"coolant", "CLT"}, {"TPSValue", "TPS"}, {"MAPValue", "MAP"}},
                new Object[][]{
                        {"Throttle Body (incl. ETB)", new String[]{"TPSValue"}},
                        {"AE / Transient fueling", new String[]{"coolant", "TPSValue", "MAPValue"}},
                });
        SignalCategories categories = SignalCategories.of(log, ini);
        assertEquals(Group.SENSORS, categories.group(1), "only on the overview page: the rules decide");
        assertEquals(Group.THROTTLE, categories.group(2));
        assertEquals(Group.SENSORS, categories.group(3));
    }

    @Test
    void withoutIniTheRulesDecide() throws IOException {
        SignalCategories categories = SignalCategories.of(log("CLT", "Odd Label"), null);
        assertEquals(Group.SENSORS, categories.group(1));
        assertEquals(Group.OTHER, categories.group(2));
    }

    @Test
    void commonListsTheUsualSignalsInDefaultOrder() throws IOException {
        CsvLogReader log = log("VBatt", "Intake Air IAT", "Knock: count", "RPM", "TPS");
        assertEquals(Arrays.asList(4, 5, 2, 1), SignalCategories.of(log, null).common());
    }

    @Test
    void commonMatchesByIniChannelWhateverTheLabel() throws IOException {
        CsvLogReader log = log("Engine Speed");
        IniFileModel ini = ini(new String[][]{{"RPMValue", "Engine Speed"}}, new Object[][]{});
        assertEquals(Arrays.asList(1), SignalCategories.of(log, ini).common());
    }
}
