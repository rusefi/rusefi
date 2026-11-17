package com.rusefi.ui.widgets;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TsStringFunction;
import com.rusefi.core.ISensorHolder;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.UIContext;

/** Resolves a selection label when the menu opens, so search uses the displayed name. */
public final class GaugeMenuLabel {
    private GaugeMenuLabel() {
    }

    public static String getTitle(UIContext context, GaugeModel gauge) {
        if (!gauge.getTitleValue().isExpression()) {
            return gauge.getTitle();
        }

        String title = null;
        IniFileModel ini = context.iniFileState.getIniFileModel();
        if (ini != null && TsStringFunction.containsStringFunction(gauge.getTitle())) {
            title = TsStringFunction.resolve(gauge.getTitle(), ini, context.getCurrentConfigurationImage(), null);
        }
        if (title == null && !context.isOfflineMode()) {
            ISensorHolder.ResolvedGaugeLabels labels = SensorCentral.getInstance().getResolvedLabels(gauge.getName());
            if (labels != null && !gauge.getTitle().equals(labels.getTitle())) {
                title = labels.getTitle();
            }
        }
        // Keep unnamed or unavailable expressions selectable without exposing INI syntax.
        return title == null || title.trim().isEmpty() ? gauge.getName() : title;
    }

    /**
     * The gauge's units, with an expression such as the coolant gauge's metric/imperial choice resolved like
     * {@link #getTitle}; empty while an expression cannot be resolved, never INI syntax.
     */
    public static String getUnits(UIContext context, GaugeModel gauge) {
        if (!gauge.getUnitsValue().isExpression()) {
            return gauge.getUnits();
        }
        String units = null;
        IniFileModel ini = context.iniFileState.getIniFileModel();
        if (ini != null && TsStringFunction.containsStringFunction(gauge.getUnits())) {
            units = TsStringFunction.resolve(gauge.getUnits(), ini, context.getCurrentConfigurationImage(), null);
        }
        if (units == null && !context.isOfflineMode()) {
            ISensorHolder.ResolvedGaugeLabels labels = SensorCentral.getInstance().getResolvedLabels(gauge.getName());
            if (labels != null && !gauge.getUnits().equals(labels.getUnits())) {
                units = labels.getUnits();
            }
        }
        return units == null ? "" : units;
    }
}
