package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PanelModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.ui.util.WrapLayout;

import javax.swing.*;
import java.awt.*;
import java.util.*;
import java.util.List;

/**
 * [tag:settings_summary_line] Some pages repeat small settings panels that are edited on another page.
 * The console shows those panels as one read-only line, "Fuel strategy: Speed Density · ...", so the page
 * keeps the context while the table gets the room. Each setting name opens the page where it is edited.
 * The .ini is unchanged: TunerStudio still shows the panels.
 */
final class SettingsSummaryLine {
    /** Page dialog key -> the panels it shows as one line. */
    private static final Map<String, List<String>> SUMMARIZED = Collections.singletonMap(
        "veTableDialog", Arrays.asList("fuelStrategyPanel", "veOverrideModeSelector"));

    private SettingsSummaryLine() {
    }

    /** Opens the page where a setting in a summary line is edited. */
    interface SettingOpener {
        SettingOpener NONE = new SettingOpener() {
            @Override
            public String pageName(String fieldKey, String fromPageKey) {
                return null;
            }

            @Override
            public void open(String fieldKey, String fromPageKey) {
            }
        };

        /** Name of another page showing the setting, or null when there is none. */
        String pageName(String fieldKey, String fromPageKey);

        void open(String fieldKey, String fromPageKey);
    }

    static final class Item {
        final String fieldKey;
        final String label;
        final String value;

        Item(String fieldKey, String label, String value) {
            this.fieldKey = fieldKey;
            this.label = label;
            this.value = value;
        }

        @Override
        public String toString() {
            return label + ": " + value;
        }
    }

    static boolean isSummarized(String pageKey, PanelModel panel) {
        List<String> panels = SUMMARIZED.get(pageKey);
        return panels != null && panels.contains(panel.getPanelName());
    }

    /** The page's summarized settings that apply now: hidden and disabled ones are left out. */
    static List<Item> items(String pageKey, IniFileModel ini, ConfigurationImage image) {
        List<Item> items = new ArrayList<>();
        for (String panelName : SUMMARIZED.getOrDefault(pageKey, Collections.emptyList())) {
            DialogModel panel = ini.getDialogs().get(panelName);
            if (panel == null) {
                continue;
            }
            for (DialogModel.Field field : panel.getFields()) {
                Optional<IniField> iniField = field.getKey() == null ? Optional.empty() : ini.findIniField(field.getKey());
                if (!iniField.isPresent() || !applies(field.getVisibleExpression(), ini, image)
                    || !applies(field.getEnableExpression(), ini, image)) {
                    continue;
                }
                String value;
                try {
                    value = displayValue(ConfigurationImageGetterSetter.getStringValue(iniField.get(), image));
                } catch (RuntimeException e) {
                    continue;
                }
                items.add(new Item(field.getKey(), field.getUiName(), value));
            }
        }
        return items;
    }

    private static boolean applies(String expression, IniFileModel ini, ConfigurationImage image) {
        if (expression == null) {
            return true;
        }
        Boolean result = ExpressionEvaluator.evaluateBooleanExpression(expression, ini, image);
        return result == null || result;
    }

    /** Enum labels without their quotes; on/off bits as On and Off. */
    static String displayValue(String raw) {
        String value = raw.length() >= 2 && raw.startsWith("\"") && raw.endsWith("\"")
            ? raw.substring(1, raw.length() - 1) : raw;
        if ("true".equalsIgnoreCase(value)) {
            return "On";
        }
        if ("false".equalsIgnoreCase(value)) {
            return "Off";
        }
        return value;
    }

    static JComponent create(String pageKey, List<Item> items, SettingOpener opener) {
        JPanel line = new JPanel(new WrapLayout(FlowLayout.LEFT, 0, 2));
        line.setName("settingsSummaryLine");
        line.setAlignmentX(Component.LEFT_ALIGNMENT);
        line.setBorder(BorderFactory.createEmptyBorder(2, 4, 2, 4));
        for (int i = 0; i < items.size(); i++) {
            Item item = items.get(i);
            // A setting and its value wrap together.
            JPanel part = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
            if (i > 0) {
                part.add(styled(new JLabel("  ·  ")));
            }
            String page = opener.pageName(item.fieldKey, pageKey);
            if (page == null) {
                part.add(styled(new JLabel(item.label + ": ")));
            } else {
                JButton link = SettingHelpCard.linkButton(item.label, "settingsSummaryLink:" + item.fieldKey,
                    () -> opener.open(item.fieldKey, pageKey));
                link.setFocusable(true);
                link.setToolTipText("Change in " + page);
                part.add(styled(link));
                part.add(styled(new JLabel(": ")));
            }
            JLabel value = styled(new JLabel(item.value));
            value.setFont(value.getFont().deriveFont(Font.BOLD));
            part.add(value);
            line.add(part);
        }
        return line;
    }

    private static <T extends JComponent> T styled(T component) {
        CalibrationFieldFactory.applyStyle(component);
        return component;
    }
}
