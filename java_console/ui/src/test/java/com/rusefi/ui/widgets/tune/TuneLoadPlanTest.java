package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.CurveModel;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PanelModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.config.FieldType;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import org.junit.jupiter.api.Test;

import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Insets;
import java.awt.Rectangle;
import java.nio.ByteOrder;
import java.util.*;
import java.util.concurrent.CompletableFuture;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class TuneLoadPlanTest {
    @Test
    void reviewsSecondaryPageAndKeepsIndependentEnumBitsSelectable() {
        EnumIniField first = enumField("first", 0, 0);
        EnumIniField second = enumField("second", 0, 1);
        StringIniField secondary = new StringIniField("secondary", 0, 3);
        secondary.setPageIndex(0x0100);
        IniFileModel ini = ini(fields(first, second), fields(secondary), Collections.emptyMap());
        ConfigurationImage now = image(new byte[]{0});
        ConfigurationImage next = image(new byte[]{3});
        ConfigurationImage secondaryNow = image(new byte[]{'a', 0, 0});
        ConfigurationImage secondaryNext = image(new byte[]{'b', 0, 0});
        TuneLoadPlan plan = TuneLoadPlan.create(ini, pages(0, now, 0x0100, secondaryNow),
            pages(0, next, 0x0100, secondaryNext), new ArrayList<>());

        assertEquals(3, plan.changes().size());
        TuneLoadPlan.Change firstChange = plan.changes().stream().filter(c -> c.label().equals("first")).findFirst().get();
        Map<Integer, ConfigurationImage> selected = plan.select(Collections.singleton(firstChange));
        assertEquals(1, selected.get(0).getContent()[0] & 0xff);
        assertEquals('a', selected.get(0x0100).getContent()[0]);

        Map<Integer, ConfigurationImage> destination = pages(0, image(new byte[]{4}),
            0x0100, image(new byte[]{'z', 0, 0}));
        Map<Integer, ConfigurationImage> overlaid = plan.select(Collections.singleton(firstChange), destination);
        assertEquals(5, overlaid.get(0).getContent()[0] & 0xff);
        assertEquals('z', overlaid.get(0x0100).getContent()[0]);
        assertEquals(4, destination.get(0).getContent()[0] & 0xff);
    }

    @Test
    void groupsChangedTableAxesAcrossPagesAndShowsValueCount() {
        ArrayIniField values = new ArrayIniField("tableValues", 0, FieldType.INT16, 2, 1,
            "", 1, null, null, "0");
        ArrayIniField xBins = new ArrayIniField("xBins", 0, FieldType.INT16, 2, 1,
            "", 1, null, null, "0");
        xBins.setPageIndex(0x0100);
        TableModel table = new TableModel("table", "tableValues", "Fuel table", null,
            "Load", "RPM", "xBins", null, false, "yBins", null, false,
            "tableValues", null, null, null, null);
        IniFileModel ini = ini(fields(values), fields(xBins), Collections.singletonMap("table", table));
        TuneLoadPlan plan = TuneLoadPlan.create(ini, pages(0, image(new byte[4]), 0x0100, image(new byte[4])),
            pages(0, image(new byte[]{1, 0, 2, 0}), 0x0100, image(new byte[]{3, 0, 4, 0})),
            new ArrayList<>());

        assertEquals(1, plan.changes().size());
        TuneLoadPlan.Change change = plan.changes().get(0);
        assertEquals("Fuel table", change.label());
        assertEquals("4 values changed", change.current());
        assertEquals(4, change.details().size());
    }

    @Test
    void luaCurrentAndIncomingAreFullNullTerminatedAsciiText() {
        StringIniField lua = new StringIniField("luaScript", 0, 12);
        IniFileModel ini = ini(fields(lua), Collections.emptyMap(), Collections.emptyMap());
        TuneLoadPlan plan = TuneLoadPlan.create(ini, pages(0, image(ascii("old\0xxxxxxxx"))),
            pages(0, image(ascii("new\0yyyyyyyy"))), new ArrayList<>());

        TuneLoadPlan.Change change = plan.changes().get(0);
        assertTrue(change.isLua());
        assertEquals("old", change.current());
        assertEquals("new", change.incoming());
    }

    @Test
    void iatTemperatureShowsUnitsAndEnoughPrecisionToRevealTheChange() throws IniParsingException {
        String text = "[Constants]\nsignature = \"review-temperature\"\nnPages = 1\npageSize = 8\n"
            + "pageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
            + "useMetricOnInterface = bits, U08, 0, [0:0], \"Imperial\", \"Metric\"\n"
            + "iat_tempC_2 = scalar, F32, 4, {bitStringValue(unitsLabels, useMetricOnInterface)},"
            + " {useMetricOnInterface ? 1 : 1.8}, {useMetricOnInterface ? 0 : 17.77777}, -55, 200, 1\n"
            + "[PcVariables]\nunitsLabels = bits, U08, [0:1], \"F\", \"C\"\n"
            + "[UserDefined]\ndialog = iat, \"IAT sensor\"\nfield = \"Middle temperature\", iat_tempC_2\n";
        RawIniFile raw = IniFileReaderUtil.read(new java.io.ByteArrayInputStream(ascii(text)));
        IniFileModel ini = IniFileReaderUtil.readIniFile(raw, "review-temperature.ini", new IniFileMetaInfoImpl(raw));
        for (byte units : new byte[]{1, 0}) {
            ConfigurationImage before = image(new byte[]{units, 0, 0, 0, 0x33, 0x33, (byte) 0xBF, 0x41});
            ConfigurationImage after = image(new byte[]{units, 0, 0, 0, 0x78, 0x1C, (byte) 0xBF, 0x41});
            TuneLoadPlan plan = TuneLoadPlan.create(ini, Map.of(0, before), Map.of(0, after), List.of());
            assertEquals(1, plan.changes().size());
            TuneLoadPlan.Change change = plan.changes().get(0);
            TuneLoadPlan.Detail temperature = change.details().get(0);
            assertEquals("Middle temperature", temperature.label());
            assertEquals(units == 1 ? "23.90 °C" : "75.02 °F", temperature.current());
            assertEquals(units == 1 ? "23.89 °C" : "75.00 °F", temperature.incoming());
            assertArrayEquals(after.getContent(), plan.select(Set.of(change)).get(0).getContent());
        }
    }

    @Test
    void showsCompleteReadOnlyTablesWithSeparateAxesFromEachTune() throws Exception {
        ArrayIniField values = array("veSecondary", 0, 2, 2);
        values.setPageIndex(0x0300);
        ArrayIniField rpm = array("rpm", 0, 2, 1);
        ArrayIniField load = array("load", 4, 2, 1);
        TableModel table = new TableModel("ve", "veMap", "VE Table 2", null, "RPM", "Load",
            "rpm", null, false, "load", null, false, "veSecondary", null, null, null, null);
        IniFileModel ini = ini(fields(rpm, load), fields(values), Map.of("ve", table));
        TuneLoadPlan plan = TuneLoadPlan.create(ini,
            Map.of(0, shorts(1000, 2000, 30, 60), 0x0300, shorts(15, 20, 25, 30)),
            Map.of(0, shorts(1000, 3000, 30, 65), 0x0300, shorts(15, 22, 25, 30)), List.of());
        TuneLoadPlan.TableComparison comparison = plan.changes().get(0).tables.get(0);
        assertEquals("VE Table 2", comparison.title);
        assertArrayEquals(new String[]{"Load / RPM", "1000", "2000"}, comparison.previous[0]);
        assertArrayEquals(new String[]{"Load / RPM", "1000", "3000"}, comparison.incoming[0]);
        assertTrue(comparison.isChanged(0, 2));
        assertTrue(comparison.isChanged(2, 0));
        assertTrue(comparison.isChanged(1, 2));
        assertFalse(comparison.isChanged(2, 2));
        SwingUtilities.invokeAndWait(() -> {
            TuneReviewDialog.Review review = new TuneReviewDialog.Review(plan.changes(), plan.warnings(), true);
            assertTrue(contains(review.content, c -> c instanceof JTable && ((JTable) c).getColumnCount() == 3
                && ((JTable) c).getColumnName(2).equals("2000")
                && ((JTable) c).getValueAt(1, 2).equals("30") && !((JTable) c).isCellEditable(1, 2)));
            assertTrue(contains(review.content, c -> c instanceof JTable && ((JTable) c).getColumnCount() == 3
                && ((JTable) c).getColumnName(2).equals("3000") && ((JTable) c).getValueAt(0, 2).equals("22")));
            assertTrue(contains(review.content, c -> c instanceof JButton && ((JButton) c).getText().equals("Apply and Burn")));
        });
    }

    @Test
    void curveShowsBothPointTablesAndAnOverlayPlot() throws Exception {
        ArrayIniField x = array("temperature", 0, 2, 1);
        ArrayIniField y = array("taper", 4, 2, 1);
        IniFileModel ini = ini(fields(x, y), Map.of(), Map.of());
        CurveModel curve = new CurveModel("idleTaper", "Idle valve Crank-to-Run Taper",
            "Temperature", "Duration", null, null, "temperature", "taper");
        when(ini.getCurves()).thenReturn(Map.of("idleTaper", curve));
        TuneLoadPlan plan = TuneLoadPlan.create(ini, Map.of(0, shorts(20, 40, 200, 200)),
            Map.of(0, shorts(20, 40, 200, 100)), List.of());
        TuneLoadPlan.TableComparison comparison = plan.changes().get(0).tables.get(0);
        assertArrayEquals(new double[]{40, 200}, comparison.previousPoints[1]);
        assertArrayEquals(new double[]{40, 100}, comparison.incomingPoints[1]);
        SwingUtilities.invokeAndWait(() -> {
            TuneReviewDialog.Review review = new TuneReviewDialog.Review(plan.changes(), List.of(), false);
            assertTrue(contains(review.content, c -> c instanceof JTable && ((JTable) c).getColumnName(1).equals("Duration")
                && ((JTable) c).getValueAt(1, 1).equals("100")));
            assertTrue(contains(review.content, c -> c instanceof JComponent
                && ((JComponent) c).getToolTipText() != null && ((JComponent) c).getToolTipText().startsWith("Previous is dashed")));
            assertFalse(contains(review.content, c -> c instanceof JButton && ((JButton) c).getText().equals("Apply and Burn")));
        });
    }

    @Test
    void mafReviewShowsOnlyNeededDigitsAndKeepsPaddingOffThePlot() {
        ArrayIniField volts = new ArrayIniField("mafDecodingBins", 0, FieldType.FLOAT, 1, 6, "V", 1, "-5", "150", "2");
        ArrayIniField flow = new ArrayIniField("mafDecoding", 24, FieldType.FLOAT, 1, 6, "kg/hour", 1, "-500", "4000", "2");
        IniFileModel ini = ini(fields(volts, flow), Map.of(), Map.of());
        CurveModel curve = new CurveModel("mafDecodingCurve", "MAF sensor", "Voltage", "kg/hour", null, null,
            volts.getName(), flow.getName());
        when(ini.getCurves()).thenReturn(Map.of(curve.getCurveId(), curve));
        ConfigurationImage before = floats(0f, 0.78f, 2.91f, 4.98f, 2014f, 2015f,
            -34.5f, -6f, 105.3f, 738f, 3000f, 3000f);
        ConfigurationImage after = floats(0f, 0.78125f, 2.91015625f, 4.98046875f, 4.99f, 2015f,
            -34.5f, -6f, 105.3f, 738f, 739f, 3000f);
        TuneLoadPlan plan = TuneLoadPlan.create(ini, Map.of(0, before), Map.of(0, after), List.of());
        TuneLoadPlan.Change change = plan.changes().get(0);
        TuneLoadPlan.TableComparison comparison = change.tables.get(0);
        assertEquals("0.780", comparison.previous[2][0]);
        assertEquals("0.781", comparison.incoming[2][0]);
        assertEquals("2.9100", comparison.previous[3][0]);
        assertEquals("2.9102", comparison.incoming[3][0]);
        assertEquals("4.9800", comparison.previous[4][0]);
        assertEquals("4.9805", comparison.incoming[4][0]);
        assertEquals("105.3", comparison.incoming[3][1]);
        assertArrayEquals(new String[]{"2015.0", "3000.0"}, comparison.incoming[6]);
        assertEquals(4, comparison.previousPoints.length);
        assertEquals(5, comparison.incomingPoints.length);
        assertArrayEquals(after.getContent(), plan.select(Set.of(change)).get(0).getContent());
    }

    @Test
    void reviewsOnlyStoredFieldsAndOnlyVisibleStringChanges() throws IniParsingException {
        IniFileModel ini = parseIni(8,
            "engineType = bits, U08, 0, [0:3], \"Default\", \"New engine\"\n"
                + "vehicleName = string, ASCII, 4, 4\n"
                + "[PcVariables]\nunitsLabels = bits, U08, [0:1], \"F\", \"C\"\n");
        ConfigurationImage a = image(new byte[]{0, 0, 0, 0, 'A', 0, 0, 0});
        ConfigurationImage b = image(new byte[]{1, 0, 0, 0, 'A', 0, 42, 0});
        TuneLoadPlan plan = TuneLoadPlan.create(ini, Map.of(0, a), Map.of(0, b), List.of());
        assertTrue(ini.findIniField("unitsLabels").isPresent(), "INI expressions can still resolve the labels");
        assertEquals(1, plan.changes().size());
        assertEquals(List.of("engineType"), plan.changes().get(0).details().stream()
            .map(TuneLoadPlan.Detail::label).collect(java.util.stream.Collectors.toList()));
        assertArrayEquals(new byte[]{1, 0, 0, 0, 'A', 0, 0, 0}, plan.select(Set.copyOf(plan.changes())).get(0).getContent());

        b.getContent()[4] = 'B';
        TuneLoadPlan renamed = TuneLoadPlan.create(ini, Map.of(0, a), Map.of(0, b), List.of());
        TuneLoadPlan.Change change = renamed.changes().stream().filter(c -> c.label().equals("vehicleName")).findFirst().orElseThrow();
        assertEquals("A", change.current());
        assertEquals("B", change.incoming());
    }

    @Test
    void targetTableUsesEachTunesScalingDigitsAndUnits() throws Exception, IniParsingException {
        IniFileModel ini = parseIni(10,
            "useLambdaOnInterface = bits, U08, 0, [0:0], \"AFR\", \"Lambda\"\n"
                + "rpm = array, U16, 2, [1], \"RPM\", 1, 0, 0, 10000, 0\n"
                + "load = array, U16, 4, [1], \"kPa\", 1, 0, 0, 1000, 0\n"
                + "lambdaTable = array, U08, 6, [1x1], {useLambdaOnInterface ? \"lambda\" : \"afr\"},"
                + " {useLambdaOnInterface ? 1/147 : 1/10}, 0, 0, 25, {useLambdaOnInterface ? 2 : 1}\n"
                + "[TableEditor]\ntable = afrTableTbl, afrTableMap, \"Target AFR Table\", 1, { !useLambdaOnInterface }\n"
                + "xBins = rpm\nyBins = load\nzBins = lambdaTable\n"
                + "table = lambdaTableTbl, lambdaTableMap, \"Target Lambda Table\", 1, { useLambdaOnInterface }\n"
                + "xBins = rpm\nyBins = load\nzBins = lambdaTable\n");
        ConfigurationImage a = image(new byte[]{0, 0, 10, 0, 20, 0, (byte) 147, 0, 0, 0});
        ConfigurationImage b = image(new byte[]{0, 0, 10, 0, 20, 0, (byte) 149, 0, 0, 0});
        TuneLoadPlan plan = TuneLoadPlan.create(ini, Map.of(0, a), Map.of(0, b), List.of());
        assertEquals(1, plan.changes().get(0).tables.size());
        TuneLoadPlan.TableComparison afr = plan.changes().get(0).tables.get(0);
        assertEquals("Target AFR Table", afr.title);
        assertEquals("14.7", afr.previous[1][1]);
        assertEquals("14.9", afr.incoming[1][1]);
        assertEquals("afr", afr.incomingUnits);
        b.getContent()[0] = 1;
        TuneLoadPlan mixed = TuneLoadPlan.create(ini, Map.of(0, a), Map.of(0, b), List.of());
        TuneLoadPlan.TableComparison target = mixed.changes().stream().flatMap(c -> c.tables.stream()).findFirst().orElseThrow();
        assertEquals("Target mixture", target.title);
        assertEquals("14.7", target.previous[1][1]);
        assertEquals("1.01", target.incoming[1][1]);
        assertEquals("afr", target.previousUnits);
        assertEquals("lambda", target.incomingUnits);
        TuneLoadPlan.Change tableChange = mixed.changes().stream().filter(c -> c.tables.contains(target)).findFirst().orElseThrow();
        assertEquals(0, mixed.select(Set.of(tableChange)).get(0).getContent()[0], "the unchecked unit selector stays unchanged");
        SwingUtilities.invokeAndWait(() -> {
            TuneReviewDialog.Review review = new TuneReviewDialog.Review(mixed.changes(), mixed.warnings(), true);
            assertTrue(contains(review.content, c -> {
                if (c instanceof JTable && ((JTable) c).getColumnName(0).equals("Apply")) {
                    int unitRow = mixed.changes().indexOf(tableChange) == 0 ? 1 : 0;
                    ((JTable) c).setValueAt(false, unitRow, 0);
                    return true;
                }
                return false;
            }));
            assertTrue(contains(review.content, c -> c instanceof javax.swing.JLabel && c.isVisible()
                && ((javax.swing.JLabel) c).getText().contains("full incoming tune")));
        });
    }

    @Test
    void analyzerPreviewsUseConfiguredDefaultsAndStoredSeriesInsteadOfLiveZeros() throws IniParsingException {
        IniFileModel ini = parseIni(12,
            "metric = bits, U08, 0, [0:0], \"Imperial\", \"Metric\"\n"
                + "temps = array, S16, 4, [2], {bitStringValue(units, metric)}, {metric ? 1 : 1.8},"
                + " {metric ? 0 : 17.77777}, -40, 392, 0\n"
                + "wue = array, U16, 8, [2], \"%\", 1, 0, 0, 500, 0\n"
                + "[PcVariables]\nunits = bits, U08, [0:0], \"F\", \"C\"\n"
                + "offsets = array, S16, [ 2], \"AFR\", 1, 0, -3, 3, 1\n"
                + "live = array, U16, [ 2], \"%\", 1, 0, 0, 500, 0\n"
                + "[ConstantsExtensions]\ndefaultValue = offsets, -1.5 0\n"
                + "[CurveEditor]\ncurve = offsetsCurve, \"AFR Target Temperature Adjustment\"\n"
                + "xAxis = -40, 120, 5\nyAxis = -3, 3, 5\nxBins = temps\nyBins = offsets\n"
                + "curve = wueCurve, \"Warmup Enrichment\"\n"
                + "xAxis = -40, 120, 5\nyAxis = 0, 200, 5\nxBins = temps\nyBins = wue\nyBins = live\n"
                + "curve = multi, \"Two series\"\ncolumnLabel = \"X\", \"First\", \"Second\"\n"
                + "xAxis = -40, 120, 5\nyAxis = 0, 200, 5\nxBins = temps\nyBins = wue\nyBins = offsets\n"
                + "lineLabel = \"A series\"\n");
        ConfigurationImage a = shorts(0, 0, 20, 40, 110, 120);
        ConfigurationImage b = shorts(1, 0, 21, 40, 115, 120);
        TuneLoadPlan plan = TuneLoadPlan.create(ini, Map.of(0, a), Map.of(0, b), List.of());
        Map<String, TuneLoadPlan.TableComparison> previews = plan.changes().stream().flatMap(c -> c.tables.stream())
            .collect(java.util.stream.Collectors.toMap(t -> t.title, t -> t));
        TuneLoadPlan.TableComparison afr = previews.get("AFR Target Temperature Adjustment");
        assertEquals("68", afr.previous[1][0]);
        assertEquals("21", afr.incoming[1][0]);
        assertEquals("°F", afr.previous[0][0]);
        assertEquals("°C", afr.incoming[0][0]);
        assertEquals("-1.5", afr.previous[1][1]);
        assertFalse(afr.hasSharedCurveAxes());
        TuneLoadPlan.TableComparison wue = previews.get("Warmup Enrichment");
        assertTrue(wue.omittedSeries);
        assertEquals("110", wue.previous[1][1]);
        assertEquals("115", wue.incoming[1][1]);
        assertEquals("A series", previews.get("Two series — A series").previous[0][1]);
        assertEquals("Second", previews.get("Two series — Second").incoming[0][1]);
    }

    @Test
    void gridPreviewDoesNotHideOtherSettingsInASharedStorageGroup() throws Exception {
        ArrayIniField cells = array("cells", 0, 1, 1);
        ScalarIniField alias = new ScalarIniField("scalarAlias", 0, "", FieldType.UINT16, 1, "0", 0);
        IniFileModel ini = ini(fields(cells, alias), Map.of(), Map.of());
        TuneLoadPlan plan = TuneLoadPlan.create(ini, Map.of(0, shorts(1)), Map.of(0, shorts(2)), List.of());
        assertEquals(1, plan.changes().size());
        SwingUtilities.invokeAndWait(() -> {
            TuneReviewDialog.Review review = new TuneReviewDialog.Review(plan.changes(), plan.warnings(), true);
            assertTrue(contains(review.content, c -> c instanceof javax.swing.JTabbedPane
                && ((javax.swing.JTabbedPane) c).indexOfTab("Other changed settings") >= 0));
        });
    }

    private static IniFileModel parseIni(int size, String body) throws IniParsingException {
        String text = "[Constants]\nsignature = \"review-audit\"\nnPages = 1\npageSize = " + size
            + "\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n" + body;
        RawIniFile raw = IniFileReaderUtil.read(new java.io.ByteArrayInputStream(ascii(text)));
        return IniFileReaderUtil.readIniFile(raw, "review-audit.ini", new IniFileMetaInfoImpl(raw));
    }

    @Test
    void curveLabelsReadTheSelectorFromItsOwnPage() {
        ArrayIniField x = array("temperature", 0, 2, 1);
        ArrayIniField y = array("taper", 4, 2, 1);
        ScalarIniField unit = new ScalarIniField("unitIndex", 0, "", FieldType.UINT16, 1, "0", 0);
        unit.setPageIndex(0x0300);
        EnumIniField names = new EnumIniField("unitNames", 0, FieldType.UINT8,
            new EnumIniField.EnumKeyValueMap(Map.of(0, "°C", 1, "°F")), 0, 0);
        IniFileModel ini = ini(fields(x, y, names), fields(unit), Map.of());
        CurveModel curve = new CurveModel("taperCurve", "Taper curve",
            "{bitStringValue(unitNames, unitIndex)}", "Duration", null, null, "temperature", "taper");
        when(ini.getCurves()).thenReturn(Map.of("taperCurve", curve));
        TuneLoadPlan plan = TuneLoadPlan.create(ini, Map.of(0, shorts(20, 40, 200, 200), 0x0300, shorts(0)),
            Map.of(0, shorts(20, 40, 200, 100), 0x0300, shorts(1)), List.of());
        TuneLoadPlan.TableComparison comparison = plan.changes().stream().flatMap(c -> c.tables.stream()).findFirst().orElseThrow();
        assertEquals("°C", comparison.previous[0][0]);
        assertEquals("°F", comparison.incoming[0][0]);
    }

    @Test
    void namedIniPanelGroupsPidInputsUnderOneSelection() {
        IniField p = new ScalarIniField("etb_pFactor", 0, "", FieldType.INT16, 1, "0", 0);
        IniField i = new ScalarIniField("etb_iFactor", 2, "", FieldType.INT16, 1, "0", 0);
        IniField d = new ScalarIniField("etb_dFactor", 4, "", FieldType.INT16, 1, "0", 0);
        IniField unrelated = new ScalarIniField("other", 6, "", FieldType.INT16, 1, "0", 0);
        IniFileModel ini = ini(fields(p, i, d, unrelated), Map.of(), Map.of());
        List<DialogModel.Field> labels = List.of(new DialogModel.Field(p.getName(), "P factor"),
            new DialogModel.Field(i.getName(), "I factor"), new DialogModel.Field(d.getName(), "D factor"));
        DialogModel pid = new DialogModel("pid", "PID settings", labels, List.of());
        DialogModel etb = new DialogModel("etb", "ETB", List.of(), List.of(),
            List.of(new PanelModel("pid", "center", null, null)), null);
        when(ini.getDialogs()).thenReturn(Map.of("etb", etb, "pid", pid));
        when(ini.getFieldsInUiOrder()).thenReturn(Map.of(p.getName(), labels.get(0), i.getName(), labels.get(1), d.getName(), labels.get(2)));
        TuneLoadPlan plan = TuneLoadPlan.create(ini, Map.of(0, shorts(1, 2, 3, 4)),
            Map.of(0, shorts(5, 6, 7, 8)), List.of());
        assertEquals(2, plan.changes().size());
        TuneLoadPlan.Change group = plan.changes().stream().filter(c -> c.label().equals("ETB — PID settings")).findFirst().orElseThrow();
        assertEquals("P factor", group.details().get(0).label());
        assertEquals(3, group.details().size());
        assertArrayEquals(shorts(5, 6, 7, 4).getContent(), plan.select(Set.of(group)).get(0).getContent());
    }

    @Test
    void reviewMountsAnEightyPercentCardAndEscapeCancelsIt() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuneLoadPlan plan = TuneLoadPlan.create(ini(Map.of(), Map.of(), Map.of()), Map.of(), Map.of(), List.of());
            CompletableFuture<TuneReviewDialog.Decision> result = new CompletableFuture<>();
            JPanel host = new JPanel(new BorderLayout());
            TuneReviewDialog.show("example.msq", plan, false, modal -> {
                host.add(modal);
                return () -> host.remove(modal);
            }, result);
            host.setSize(1200, 800);
            host.doLayout();
            JPanel modal = (JPanel) host.getComponent(0);
            modal.doLayout();
            assertEquals(new Rectangle(120, 24, 960, 752), modal.getComponent(0).getBounds());
            assertTrue(modal.isFocusCycleRoot());
            assertFalse(result.isDone());
            modal.getActionForKeyStroke(KeyStroke.getKeyStroke("ESCAPE")).actionPerformed(null);
            assertTrue(result.isDone());
            assertNull(result.join());
            assertEquals(0, host.getComponentCount());
        });
    }

    @Test
    void dismissingTheHostCannotApproveTuneWrites() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuneLoadPlan plan = changedScalarPlan();
            CompletableFuture<TuneReviewDialog.Decision> result = new CompletableFuture<>();
            JPanel host = new JPanel();
            TuneReviewDialog.show("example.msq", plan, true, modal -> {
                host.add(modal);
                return () -> host.remove(modal);
            }, result);
            JButton staleApply = findButton(host, "Apply and Burn");
            assertNotNull(staleApply);
            host.removeAll();
            assertTrue(result.isDone());
            staleApply.doClick(0);
            assertNull(result.join());
            assertEquals(0, host.getComponentCount());
        });
    }

    @Test
    void anotherConsoleCardCancelsThePendingReview() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            CardLayout cards = new CardLayout();
            JPanel root = new JPanel(cards);
            JPanel host = new JPanel();
            root.add(new JPanel(), "console");
            root.add(host, "review");
            root.addNotify();
            try {
                CompletableFuture<TuneReviewDialog.Decision> result = new CompletableFuture<>();
                TuneReviewDialog.show("example.msq", changedScalarPlan(), true, modal -> {
                    host.add(modal);
                    cards.show(root, "review");
                    return () -> host.remove(modal);
                }, result);
                assertTrue(host.isShowing());
                assertFalse(result.isDone());
                cards.show(root, "console");
                assertTrue(result.isDone());
                assertNull(result.join());
                assertEquals(0, host.getComponentCount());
            } finally {
                root.removeNotify();
            }
        });
    }

    @Test
    void applyActionsCloseTheHostAndReturnTheSelectedWriteMode() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            TuneLoadPlan plan = changedScalarPlan();
            for (boolean burn : new boolean[]{false, true}) {
                CompletableFuture<TuneReviewDialog.Decision> result = new CompletableFuture<>();
                JPanel host = new JPanel();
                TuneReviewDialog.show("example.msq", plan, true, modal -> {
                    host.add(modal);
                    return () -> host.remove(modal);
                }, result);
                String label = burn ? "Apply and Burn" : "Apply selected to ECU RAM";
                JButton button = findButton(host, label);
                assertNotNull(button, label);
                button.doClick(0);
                assertTrue(result.isDone());
                assertEquals(burn, result.join().burn);
                assertEquals(Set.copyOf(plan.changes()), result.join().selected);
                assertEquals(0, host.getComponentCount());
            }
        });
    }

    private static TuneLoadPlan changedScalarPlan() {
        IniField field = new ScalarIniField("test", 0, "", FieldType.UINT16, 1, "0", 0);
        return TuneLoadPlan.create(ini(fields(field), Map.of(), Map.of()), Map.of(0, shorts(1)),
            Map.of(0, shorts(2)), List.of());
    }

    private static JButton findButton(Component component, String label) {
        JButton[] found = {null};
        contains(component, child -> {
            if (child instanceof JButton && label.equals(((JButton) child).getText())) {
                found[0] = (JButton) child;
                return true;
            }
            return false;
        });
        return found[0];
    }

    @Test
    void curvePlotReservesSpaceForLargeFontsAndLongTicks() {
        java.awt.Graphics2D graphics = new java.awt.image.BufferedImage(1, 1,
            java.awt.image.BufferedImage.TYPE_INT_ARGB).createGraphics();
        try {
            graphics.setFont(new java.awt.Font("Dialog", java.awt.Font.PLAIN, 28));
            java.awt.FontMetrics font = graphics.getFontMetrics();
            Insets border = new Insets(24, 8, 8, 8);
            Rectangle plot = TuneReviewDialog.curvePlotArea(640, 360, border, font,
                new String[]{"0.0", "12345.678"}, new String[]{"-3000.0", "0.0"});
            assertTrue(plot.x - font.stringWidth("-3000.0") - 6 >= border.left);
            assertTrue(plot.x + plot.width + font.stringWidth("12345.678") / 2 <= 640 - border.right);
            assertTrue(plot.y >= border.top + 2 * font.getHeight());
            assertTrue(plot.y + plot.height + 2 * font.getHeight() <= 360 - border.bottom);
            assertTrue(plot.width > 0 && plot.height > 0);
        } finally {
            graphics.dispose();
        }
    }

    @Test
    void mapTableHighlightsDifferencesHiddenAtNormalPrecision() {
        ArrayIniField rpm = array("rpm", 0, 2, 1);
        ArrayIniField tps = new ArrayIniField("tps", 4, FieldType.UINT16, 1, 2, "% TPS", 0.01, null, null, "1");
        ArrayIniField values = new ArrayIniField("mapEstimate", 8, FieldType.UINT16, 2, 2, "kPa", 0.01, null, null, "1");
        TableModel table = new TableModel("map", "map3d", "Estimated MAP - RPM x TPS (kPa)", null, "RPM", "% TPS",
            "rpm", null, false, "tps", null, false, "mapEstimate", null, null, null, null);
        IniFileModel ini = ini(fields(rpm, tps, values), Map.of(), Map.of("map", table));
        TuneLoadPlan plan = TuneLoadPlan.create(ini, Map.of(0, shorts(650, 800, 0, 667, 6000, 6000, 6000, 6000)),
            Map.of(0, shorts(650, 800, 0, 670, 6000, 6001, 6000, 6000)), List.of());
        TuneLoadPlan.TableComparison comparison = plan.changes().get(0).tables.get(0);
        assertEquals("6.67", comparison.previous[2][0]);
        assertEquals("6.70", comparison.incoming[2][0]);
        assertTrue(comparison.isChanged(2, 0));
        assertEquals("60.00", comparison.previous[1][2]);
        assertEquals("60.01", comparison.incoming[1][2]);
        assertTrue(comparison.isChanged(1, 2));
        assertFalse(comparison.isChanged(1, 1));
    }

    @Test
    void afrAxisResolvesPcVariableLabelAndScaleFromEachTune() throws IniParsingException {
        String text = "[Constants]\nsignature = \"review-axes\"\nnPages = 2\npageSize = 24, 2\n"
            + "pageIdentifier = \"\\x00\\x00\", \"\\x00\\x04\"\npageReadCommand = \"r\", \"r\"\nochBlockSize = 0\npage = 1\n"
             + "rpm = array, U16, 0, [2], \"RPM\", 1, 0, 0, 9000, 0\n"
             + "load = array, U16, 4, [2], {bitStringValue(afrLoadUnitLabels, afrLoadUnitIdx)}, "
             + "{ afrLoadUnitIdx != 0 ? 1 : 0.145038 },"
            + " 0, 0, 255, 1\n"
            + "target = array, U16, 8, [2x2], \"\", 1, 0, 0, 255, 0\n"
            + "useMetricOnInterface = bits, U08, 16, [0:0], \"Off\", \"On\"\n"
            + "page = 2\n"
            + "afrOverrideMode = scalar, U08, 0, \"\", 1, 0, 0, 5, 0\n"
            + "fuelAlgorithm = scalar, U08, 1, \"\", 1, 0, 0, 5, 0\n"
            + "[PcVariables]\n"
            + "afrLoadUnitLabels = bits, U08, [0:1], \"psi\", \"kPa\", \"%\", \"Lua\"\n"
            + "targetAfrAxisLabels = bits, U08, [0:2], \"MAP\", \"TPS\", \"Air Charge\", \"Lua\", \"Acc Pedal\", \"Cyl Filling\"\n"
            + "afrLoadUnitIdx = scalar, U08, \"\", 1, 0, 0, 3, 0, noMsqSave\n"
            + "afrLoadSrc = scalar, U08, \"\", 1, 0, 0, 5, 0, noMsqSave\n"
            + "[ConstantsExtensions]\n"
            + "defaultValue = afrLoadUnitIdx, 1\n"
            + "maintainConstantValue = afrLoadUnitIdx, { (afrOverrideMode == 1 || (afrOverrideMode == 0 && fuelAlgorithm == 0)) "
            + "? useMetricOnInterface : ((afrOverrideMode == 0 && fuelAlgorithm == 3) ? 3 : 2) }\n"
            + "maintainConstantValue = afrLoadSrc, { afrOverrideMode == 1 ? 0 : afrOverrideMode == 2 ? 1 : afrOverrideMode == 3 ? 4"
            + " : afrOverrideMode == 4 ? 5 : (fuelAlgorithm == 0 ? 0 : fuelAlgorithm == 2 ? 1 : fuelAlgorithm == 1 ? 2 : 3) }\n"
            + "[TableEditor]\ntable = afr, afrMap, \"Target AFR Table\"\n"
            + "xBins = rpm\nyBins = load\nzBins = target\nxyLabels = \"RPM\", {bitStringValue(targetAfrAxisLabels, afrLoadSrc)}\n";
        RawIniFile raw = IniFileReaderUtil.read(new java.io.ByteArrayInputStream(ascii(text)));
        IniFileModel ini = IniFileReaderUtil.readIniFile(raw, "review-axes.ini", new IniFileMetaInfoImpl(raw));
        ConfigurationImage previous = shorts(650, 800, 30, 100, 20, 20, 20, 20, 1, 0, 0, 0);
        ConfigurationImage incoming = shorts(650, 800, 30, 100, 20, 21, 20, 20, 0, 0, 0, 0);
        ConfigurationImage secondary = shorts(0);
        TuneLoadPlan plan = TuneLoadPlan.create(ini, Map.of(0, previous, 0x0400, secondary),
            Map.of(0, incoming, 0x0400, secondary), List.of());
        TuneLoadPlan.TableComparison comparison = plan.changes().stream().flatMap(c -> c.tables.stream()).findFirst().orElseThrow();
        assertEquals("MAP (kPa) / RPM", comparison.previous[0][0]);
        assertEquals("MAP (psi) / RPM", comparison.incoming[0][0]);
        assertEquals("100.0", comparison.previous[2][0]);
        assertEquals("14.5", comparison.incoming[2][0]);
        assertTrue(comparison.isChanged(0, 0));
        assertEquals(1.0, ini.findIniField("load").map(f -> ((ArrayIniField) f).getMultiplier()).orElseThrow());
    }

    @Test
    void offlineReviewShowsBothReadOnlyLuaTextsAndAnOfflineApplyAction() throws Exception {
        StringIniField lua = new StringIniField("luaScript", 0, 8);
        lua.setPageIndex(0x0400);
        IniFileModel ini = ini(Map.of(), fields(lua), Map.of());
        TuneLoadPlan plan = TuneLoadPlan.create(ini, Map.of(0x0400, image(ascii("old\0xxxx"))),
            Map.of(0x0400, image(ascii("new\0yyyy"))), List.of());
        SwingUtilities.invokeAndWait(() -> {
            TuneReviewDialog.Review review = new TuneReviewDialog.Review(plan.changes(), plan.warnings(), false);
            assertTrue(contains(review.content, component -> component instanceof JTextArea
                && !((JTextArea) component).isEditable() && ((JTextArea) component).getText().equals("old")));
            assertTrue(contains(review.content, component -> component instanceof JTextArea
                && !((JTextArea) component).isEditable() && ((JTextArea) component).getText().equals("new")));
            assertTrue(contains(review.content, component -> component instanceof JButton
                && ((JButton) component).getText().equals("Apply selected offline")));
        });
    }

    private static boolean contains(Component component, java.util.function.Predicate<Component> check) {
        if (check.test(component)) {
            return true;
        }
        if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                if (contains(child, check)) {
                    return true;
                }
            }
        }
        return false;
    }

    private static EnumIniField enumField(String name, int offset, int bit) {
        Map<Integer, String> names = new LinkedHashMap<>();
        names.put(0, "Off");
        names.put(1, "On");
        return new EnumIniField(name, offset, FieldType.UINT8, new EnumIniField.EnumKeyValueMap(names), bit, 0);
    }

    private static IniFileModel ini(Map<String, IniField> primary, Map<String, IniField> secondary,
                                    Map<String, TableModel> tables) {
        IniFileModel ini = mock(IniFileModel.class);
        when(ini.getAllIniFields()).thenReturn(primary);
        when(ini.getSecondaryIniFields()).thenReturn(secondary);
        when(ini.getTables()).thenReturn(tables);
        when(ini.getCurves()).thenReturn(Collections.emptyMap());
        when(ini.getDialogs()).thenReturn(Collections.emptyMap());
        when(ini.getFieldsInUiOrder()).thenReturn(Collections.emptyMap());
        Map<String, IniField> all = new LinkedHashMap<>(primary);
        all.putAll(secondary);
        when(ini.findIniField(anyString())).thenAnswer(call -> Optional.ofNullable(all.get(call.getArgument(0))));
        return ini;
    }

    private static ArrayIniField array(String name, int offset, int rows, int cols) {
        return new ArrayIniField(name, offset, FieldType.INT16, rows, cols, "", 1, null, null, "0");
    }

    private static ConfigurationImage floats(float... values) {
        java.nio.ByteBuffer buffer = java.nio.ByteBuffer.allocate(values.length * Float.BYTES).order(ByteOrder.LITTLE_ENDIAN);
        for (float value : values) {
            buffer.putFloat(value);
        }
        return image(buffer.array());
    }

    private static ConfigurationImage shorts(int... values) {
        java.nio.ByteBuffer bytes = java.nio.ByteBuffer.allocate(values.length * 2).order(ByteOrder.LITTLE_ENDIAN);
        for (int value : values) {
            bytes.putShort((short) value);
        }
        return image(bytes.array());
    }

    private static Map<String, IniField> fields(IniField... values) {
        Map<String, IniField> result = new LinkedHashMap<>();
        for (IniField value : values) { result.put(value.getName(), value); }
        return result;
    }

    private static Map<Integer, ConfigurationImage> pages(Object... pairs) {
        Map<Integer, ConfigurationImage> result = new LinkedHashMap<>();
        for (int i = 0; i < pairs.length; i += 2) {
            result.put((Integer) pairs[i], (ConfigurationImage) pairs[i + 1]);
        }
        return result;
    }

    private static ConfigurationImage image(byte[] content) {
        return new ConfigurationImage(content, ByteOrder.LITTLE_ENDIAN);
    }

    private static byte[] ascii(String text) {
        return text.getBytes(java.nio.charset.StandardCharsets.US_ASCII);
    }
}
