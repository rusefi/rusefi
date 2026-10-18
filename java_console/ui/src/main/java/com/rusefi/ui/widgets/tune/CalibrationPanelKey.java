package com.rusefi.ui.widgets.tune;

import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PanelModel;

import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Objects;
import java.util.Set;
import java.util.stream.Collectors;

/** A whole INI panel, or the controls owned by a dialog after its child panels are split out. */
final class CalibrationPanelKey {
    final String name;
    final boolean ownContent;

    CalibrationPanelKey(String name) {
        this(name, false);
    }

    CalibrationPanelKey(String name, boolean ownContent) {
        this.name = name;
        this.ownContent = ownContent;
    }

    boolean isSupported(IniFileModel ini) {
        if (ini == null || name == null) {
            return false;
        }
        if (ownContent) {
            return hasOwnContent(ini.getDialogs().get(name));
        }
        return ini.getDialogs().containsKey(name) || ini.getTable(name) != null || ini.getCurves().containsKey(name);
    }

    /** Split exactly one layout level. Anonymous layout groups remain groups and can split again. */
    List<CalibrationPanelKey> splitParts(IniFileModel ini) {
        DialogModel dialog = ini == null ? null : ini.getDialogs().get(name);
        if (ownContent || dialog == null || dialog.getPanels().isEmpty()) {
            return Collections.emptyList();
        }
        Set<CalibrationPanelKey> parts = new LinkedHashSet<>();
        if (hasOwnContent(dialog)) {
            parts.add(new CalibrationPanelKey(name, true));
        }
        for (PanelModel panel : dialog.getPanels()) {
            CalibrationPanelKey child = new CalibrationPanelKey(panel.getPanelName());
            // Keep the original intact if any layout component cannot be represented separately.
            if (equals(child) || !child.isSupported(ini)) {
                return Collections.emptyList();
            }
            parts.add(child);
        }
        return parts.size() > 1 ? new ArrayList<>(parts) : Collections.emptyList();
    }

    DialogModel ownDialog(IniFileModel ini) {
        DialogModel dialog = ini.getDialogs().get(name);
        List<DialogModel.DialogEntry> entries = dialog.getOrderedEntries().stream()
            .filter(entry -> entry.kind != DialogModel.DialogEntry.Kind.PANEL).collect(Collectors.toList());
        return new DialogModel(name, dialog.getUiName(), dialog.getFields(), dialog.getCommandsOfCurrentDialog(),
            Collections.emptyList(), dialog.getIndicators(), dialog.getReadouts(), dialog.getReadoutColumns(),
            dialog.getGaugeNames(), dialog.getTopicHelp(), null, entries, dialog.getSettingSelectors());
    }

    private static boolean hasOwnContent(DialogModel dialog) {
        return dialog != null && (!dialog.getFields().isEmpty() || !dialog.getCommandsOfCurrentDialog().isEmpty()
            || !dialog.getIndicators().isEmpty() || !dialog.getReadouts().isEmpty()
            || !dialog.getGaugeNames().isEmpty() || !dialog.getSettingSelectors().isEmpty()
            || dialog.getOrderedEntries().stream().anyMatch(entry -> entry.kind != DialogModel.DialogEntry.Kind.PANEL));
    }

    @Override
    public boolean equals(Object other) {
        if (!(other instanceof CalibrationPanelKey)) {
            return false;
        }
        CalibrationPanelKey key = (CalibrationPanelKey) other;
        return Objects.equals(name, key.name) && ownContent == key.ownContent;
    }

    @Override
    public int hashCode() {
        return Objects.hash(name, ownContent);
    }
}
