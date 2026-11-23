package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.tune.xml.Msq;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
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
    /** The open tune with the point's values: what Restore as changes puts in place. */
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
        ConfigurationImage restored = point.applyOnto(current, ini);
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

    private static String value(IniField field, ConfigurationImage image) {
        try {
            String value = ConfigurationImageGetterSetter.getStringValue(field, image);
            return value == null ? "" : value.replace("\"", "");
        } catch (RuntimeException e) {
            return "?";
        }
    }
}
