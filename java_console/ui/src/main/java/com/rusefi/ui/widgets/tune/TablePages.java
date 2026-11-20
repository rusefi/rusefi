package com.rusefi.ui.widgets.tune;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;

import java.util.Map;

/**
 * A table can be opened by either name from its {@code table = tableId, map3dId, ...} line.
 * The INI menu and dialogs use the second one (e.g. {@code subMenu = veTableMap, "VE 3D view"},
 * {@code panel = veTableMap}) for TunerStudio's 3D map of that table; the console opens the same
 * table with its 3D view showing [tag:table_map3d_pages].
 */
final class TablePages {
    private TablePages() {
    }

    /** The table named by its table id or by its 3D map id; null if neither. */
    static TableModel find(IniFileModel ini, String key) {
        if (ini == null || key == null) {
            return null;
        }
        TableModel table = ini.getTable(key);
        if (table != null) {
            return table;
        }
        Map<String, TableModel> tables = ini.getTables();
        if (tables == null) {
            return null;
        }
        for (TableModel candidate : tables.values()) {
            if (key.equals(candidate.getMap3dId())) {
                return candidate;
            }
        }
        return null;
    }

    /** True when the key names a table's 3D map rather than the table itself. */
    static boolean isMap3d(IniFileModel ini, String key) {
        return ini != null && key != null && ini.getTable(key) == null && find(ini, key) != null;
    }
}
