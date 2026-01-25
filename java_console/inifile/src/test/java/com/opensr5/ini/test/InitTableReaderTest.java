package com.opensr5.ini.test;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.TableModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;

import static com.opensr5.ini.test.IniFileReaderTest.readLines;
import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class InitTableReaderTest {
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
        assertEquals(4.0, table.getGridHeight(), IniFileReaderTest.EPS);

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

}

