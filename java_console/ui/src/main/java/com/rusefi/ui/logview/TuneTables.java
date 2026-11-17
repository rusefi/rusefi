package com.rusefi.ui.logview;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DatalogEntry;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.logview.CsvLogReader;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Optional;

/**
 * The tune's 3D tables as histogram bins: their X and Y axis values from the tune, and the log signals behind
 * those axes (the table's live channels, found through the .ini's [Datalog] labels), so histogram cells line up
 * with the cells the user edits. Axis values are read with the .ini's default (metric) scale.
 */
final class TuneTables {
    /** Offered first, in this order, when the .ini has them. */
    static final String[] PREFERRED = {"veTableTbl", "ignitionTableTbl", "lambdaTableTbl", "afrTableTbl",
            "boostTableTbl"};
    /** The table whose load axis is the default Y: what the fuel strategy looks up. */
    static final String VE_TABLE = "veTableTbl";

    private TuneTables() {
    }

    /** A table's bins and the channels behind them. */
    static final class Bins {
        final String table;
        final String title;
        final HistogramAxis x;
        final HistogramAxis y;
        final String xChannel;
        final String yChannel;

        Bins(String table, String title, HistogramAxis x, HistogramAxis y, String xChannel, String yChannel) {
            this.table = table;
            this.title = title;
            this.x = x;
            this.y = y;
            this.xChannel = xChannel;
            this.yChannel = yChannel;
        }
    }

    /** Why a table cannot give bins. */
    static final class Problem extends Exception {
        Problem(String message) {
            super(message);
        }
    }

    /** The tables with two 1D axes, preferred ones first, then the rest by title. */
    static List<TableModel> tables(IniFileModel ini) {
        List<TableModel> result = new ArrayList<>();
        if (ini == null) {
            return result;
        }
        Map<String, TableModel> tables = ini.getTables();
        for (String name : PREFERRED) {
            TableModel table = tables.get(name);
            if (table != null && hasAxes(ini, table)) {
                result.add(table);
            }
        }
        List<TableModel> others = new ArrayList<>();
        for (TableModel table : tables.values()) {
            if (!result.contains(table) && hasAxes(ini, table)) {
                others.add(table);
            }
        }
        others.sort((a, b) -> title(a).compareToIgnoreCase(title(b)));
        result.addAll(others);
        return result;
    }

    /** The table's title, without the .ini's quotes. */
    static String title(TableModel table) {
        String title = table.getTitle() == null ? "" : table.getTitle().replace("\"", "").trim();
        return title.isEmpty() ? table.getTableId() : title;
    }

    private static boolean hasAxes(IniFileModel ini, TableModel table) {
        return axisField(ini, table.getXBinsConstant()) != null && axisField(ini, table.getYBinsConstant()) != null;
    }

    private static ArrayIniField axisField(IniFileModel ini, String name) {
        if (name == null || name.isEmpty()) {
            return null;
        }
        Optional<IniField> field = ini.findIniField(name);
        if (!field.isPresent() || !(field.get() instanceof ArrayIniField)) {
            return null;
        }
        ArrayIniField array = (ArrayIniField) field.get();
        return array.getCols() == 1 && array.getRows() >= HistogramAxis.MIN_BINS ? array : null;
    }

    /** The table's axes from the tune. */
    static Bins read(IniFileModel ini, ConfigurationImage image, String tableName) throws Problem {
        if (ini == null) {
            throw new Problem("No .ini: connect to the ECU or open a tune");
        }
        TableModel table = ini.getTables().get(tableName);
        if (table == null) {
            throw new Problem("This .ini has no table " + tableName);
        }
        if (image == null) {
            throw new Problem("No tune: connect to the ECU or open a tune to use its table axes");
        }
        ArrayIniField xField = axisField(ini, table.getXBinsConstant());
        ArrayIniField yField = axisField(ini, table.getYBinsConstant());
        if (xField == null || yField == null) {
            throw new Problem(title(table) + " has no X and Y axis arrays");
        }
        return new Bins(tableName, title(table), axis(image, xField, title(table) + " X"),
                axis(image, yField, title(table) + " Y"), table.getXBinsChannel(), table.getYBinsChannel());
    }

    private static HistogramAxis axis(ConfigurationImage image, ArrayIniField field, String name) throws Problem {
        int end = field.getOffset() + field.getSize();
        if (field.getOffset() < 0 || end > image.getSize()) {
            throw new Problem(name + " axis is outside the tune");
        }
        double[] values = new double[field.getRows()];
        for (int i = 0; i < values.length; i++) {
            ByteBuffer buffer = image.getByteBuffer(field.getOffset(i, 0), field.getType().getStorageSize());
            values[i] = field.getType().readRawValue(buffer) * field.getMultiplier();
        }
        try {
            return new HistogramAxis(values);
        } catch (IllegalArgumentException e) {
            throw new Problem(name + " axis values do not increase, so it cannot bin the log");
        }
    }

    /**
     * The log signal of a table axis channel: through the [Datalog] entry for the channel (or for it without a
     * "Display" suffix, the unit-converted copy the tables show), else a signal named like the channel; -1.
     */
    static int logField(CsvLogReader log, IniFileModel ini, String channel) {
        if (channel == null || channel.isEmpty()) {
            return -1;
        }
        List<String> candidates = new ArrayList<>();
        candidates.add(channel);
        if (channel.endsWith("Display")) {
            candidates.add(channel.substring(0, channel.length() - "Display".length()));
        }
        for (String candidate : candidates) {
            if (ini != null) {
                for (DatalogEntry entry : ini.getDatalogEntries()) {
                    if (candidate.equals(entry.getChannel())) {
                        int field = HistogramSignals.byName(log, entry.getLabel());
                        if (field >= 0) {
                            return field;
                        }
                    }
                }
            }
            for (int field = 1; field < log.fieldCount(); field++) {
                if (log.name(field).equalsIgnoreCase(candidate)) {
                    return field;
                }
            }
        }
        return -1;
    }

    /** "VE table (16 × 16)". */
    static String describe(Bins bins) {
        return String.format(Locale.ROOT, "%s (%d × %d)", bins.title, bins.x.size(), bins.y.size());
    }
}
