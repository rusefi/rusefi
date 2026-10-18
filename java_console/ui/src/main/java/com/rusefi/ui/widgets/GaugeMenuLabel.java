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
}
