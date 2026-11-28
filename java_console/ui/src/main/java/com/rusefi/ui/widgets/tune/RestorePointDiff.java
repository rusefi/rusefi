package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.config.StringFormatter;
import com.rusefi.tune.xml.Msq;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

/**
 * [tag:restore_points] What restoring a restore point would change in the tune open now: the point's values applied
 * onto the open tune (by setting name, so a point from older firmware skips the settings this one lacks), how many
 * values differ (counted like the unburned marks: one per setting, one per table cell) and one row per setting.
 */
final class RestorePointDiff {
    /** Candidate for the compact main-page comparison only, never an approved restore image. */
    final ConfigurationImage restored;
    final int changedValues;
    final List<Row> rows;

    /** One changed setting; a table or curve says how many cells and shows no values. */
    static final class Row {
        final String label;
        final String then;
        final String now;

        Row(String label, String then, String now) {
            this.label = label;
            this.then = then;
            this.now = now;
        }

        @Override
        public String toString() {
            return label + ": " + then + " -> " + now;
        }
    }

    private RestorePointDiff(ConfigurationImage restored, int changedValues, List<Row> rows) {
        this.restored = restored;
        this.changedValues = changedValues;
        this.rows = Collections.unmodifiableList(rows);
    }

    static RestorePointDiff of(Msq point, ConfigurationImage current, IniFileModel ini) {
        ConfigurationImage restored = asShown(point.applyOnto(current, ini), current, ini);
        UnburnedChanges changes = UnburnedChanges.of(restored, current);
        Map<String, IniField> fields = ini.getAllIniFields();
        List<Row> rows = new ArrayList<>();
        // a setting shown under two names (e.g. a table in two units) is one row
        Set<String> seen = new HashSet<>();
        for (IniField field : inShownOrder(ini, fields)) {
            if (field.getPageIndex() != 0) {
                continue;
            }
            String place = field.getOffset() + "/" + (field instanceof EnumIniField
                ? ((EnumIniField) field).getBitPosition() : -1);
            if (field instanceof ArrayIniField) {
                int cells = changes.countChangedCells((ArrayIniField) field);
                if (cells > 0 && seen.add(place)) {
                    rows.add(new Row(String.format(Locale.ROOT, "%s (%d cell%s)", label(ini, field.getName()), cells,
                        cells == 1 ? "" : "s"), "—", "—"));
                }
            } else if (changes.isChanged(field) && seen.add(place)) {
                rows.add(new Row(label(ini, field.getName()), value(field, restored), value(field, current)));
            }
        }
        return new RestorePointDiff(restored, changes.countChanged(fields.values()), rows);
    }

    /**
     * [tag:restore_point_rounding] The open tune with only the point's values that differ as shown. A restore point
     * keeps each value at the decimals its setting shows (the .msq digits), so a value the open tune holds more finely,
     * e.g. 689.4757 shown as 689.48, comes back rounded; that is no change, and restoring keeps the finer value. A table
     * cell counts the same way; an enum or bit by its bits, a text by its characters.
     */
    static ConfigurationImage asShown(ConfigurationImage full, ConfigurationImage current, IniFileModel ini) {
        return asShown(full, current, ini.getAllIniFields().values(), 0, ini.getPcVariableNames());
    }

    static Map<Integer, ConfigurationImage> asShown(Map<Integer, ConfigurationImage> full,
                                                    Map<Integer, ConfigurationImage> current, IniFileModel ini) {
        Map<Integer, ConfigurationImage> result = new LinkedHashMap<>(full);
        List<IniField> fields = new ArrayList<>(ini.getAllIniFields().values());
        fields.addAll(ini.getSecondaryIniFields().values());
        current.forEach((id, base) -> {
            if (full.containsKey(id)) {
                result.put(id, asShown(full.get(id), base, fields, id, ini.getPcVariableNames()));
            }
        });
        return result;
    }

    private static ConfigurationImage asShown(ConfigurationImage full, ConfigurationImage current,
                                                Iterable<IniField> fields, int page, Set<String> pcVariables) {
        ConfigurationImage result = current.clone();
        UnburnedChanges raw = UnburnedChanges.onPage(full, current, page);
        UnburnedChanges copy = UnburnedChanges.onPage(result, full, page);
        for (IniField field : fields) {
            if (field.getPageIndex() != page || pcVariables.contains(field.getName())) {
                continue;
            }
            try {
                if (field instanceof ArrayIniField) {
                    copyCellsShownChanged((ArrayIniField) field, raw, full, current, result);
                } else if (field instanceof EnumIniField) {
                    if (raw.isChanged(field)) {
                        copy.copyField(field);
                    }
                } else if (raw.isChanged(field) && (field instanceof StringIniField
                    ? !TuneLoadPlan.ascii(full, field).equals(TuneLoadPlan.ascii(current, field))
                    : !ConfigurationImageGetterSetter.getStringValue(field, full)
                        .equals(ConfigurationImageGetterSetter.getStringValue(field, current)))) {
                    copy(full, result, field.getOffset(), field.getSize());
                }
            } catch (RuntimeException e) {
                // cannot be read as shown: a real difference, kept
                if (raw.isChanged(field)) {
                    copy(full, result, field.getOffset(), field.getSize());
                }
            }
        }
        return result;
    }

    private static void copyCellsShownChanged(ArrayIniField field, UnburnedChanges raw, ConfigurationImage full,
                                              ConfigurationImage current, ConfigurationImage result) {
        if (raw.countChangedCells(field) == 0) {
            return;
        }
        int precision = IniField.parseDigits(field.getDigits());
        Double[][] then = ConfigurationImageGetterSetter.getArrayValues(field, full);
        Double[][] now = ConfigurationImageGetterSetter.getArrayValues(field, current);
        int cellSize = field.getType().getStorageSize();
        for (int row = 0; row < field.getRows(); row++) {
            for (int col = 0; col < field.getCols(); col++) {
                if (raw.isCellChanged(field, row, col) && !StringFormatter.niceToString(then[row][col], precision)
                    .equals(StringFormatter.niceToString(now[row][col], precision))) {
                    copy(full, result, field.getOffset(row, col), cellSize);
                }
            }
        }
    }

    private static void copy(ConfigurationImage from, ConfigurationImage to, int offset, int size) {
        System.arraycopy(from.getContent(), offset, to.getContent(), offset,
            Math.max(0, Math.min(size, Math.min(from.getSize(), to.getSize()) - offset)));
    }

    /** The settings the Tuning pages show, in their order, then the others by where they sit in the tune. */
    private static List<IniField> inShownOrder(IniFileModel ini, Map<String, IniField> fields) {
        List<IniField> ordered = new ArrayList<>();
        Set<String> placed = new HashSet<>();
        for (String key : ini.getFieldsInUiOrder().keySet()) {
            IniField field = fields.get(key);
            if (field != null && placed.add(key)) {
                ordered.add(field);
            }
        }
        List<IniField> rest = new ArrayList<>();
        for (Map.Entry<String, IniField> entry : fields.entrySet()) {
            if (!placed.contains(entry.getKey())) {
                rest.add(entry.getValue());
            }
        }
        rest.sort(java.util.Comparator.comparingInt(IniField::getOffset).thenComparing(IniField::getName));
        ordered.addAll(rest);
        return ordered;
    }

    /** The name a setting has on its Tuning page; its .ini name when no page shows it. */
    static String label(IniFileModel ini, String key) {
        DialogModel.Field meta = ini.getFieldsInUiOrder().get(key);
        String name = meta == null ? null : meta.getUiName();
        return name == null || name.trim().isEmpty() ? key : name.trim();
    }

    /** Without the quotes around a text or enum value; quotes inside, e.g. in a Lua script, are kept. */
    static String unquoted(String value) {
        if (value == null) {
            return "";
        }
        return value.length() >= 2 && value.startsWith("\"") && value.endsWith("\"")
            ? value.substring(1, value.length() - 1) : value;
    }

    private static String value(IniField field, ConfigurationImage image) {
        try {
            return unquoted(ConfigurationImageGetterSetter.getStringValue(field, image));
        } catch (RuntimeException e) {
            return "?";
        }
    }
}
