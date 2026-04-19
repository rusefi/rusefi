package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.IndicatorModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TsStringFunction;
import com.opensr5.ini.field.IniField;
import com.rusefi.core.SensorCentral;

import javax.swing.*;
import java.awt.*;
import java.util.*;
import java.util.List;

/**
 * A self-contained panel that displays a group of {@link IndicatorModel} labels.
 * <p>
 * Labels are pre-sized to the widest text they can ever show (both on/off states and all
 * enum options for bitStringValue expressions), so the layout never shifts when the state changes.
 * <p>
 * The panel self-registers with {@link SensorCentral} to refresh whenever output-channel data
 * arrives. Call {@link #refresh(ConfigurationImage)} to push an updated config image when
 * the user edits a field (dialog indicators) or to drive the initial render.
 */
public class IndicatorPanel {
    private final JPanel panel;
    private final List<IndicatorModel> indicators;
    private final IniFileModel ini;
    private ConfigurationImage currentImage;

    /**
     * @param indicators models to display
     * @param ini        INI file model used for field lookups and label resolution
     * @param columns    {@code > 0} → {@link GridLayout} with that many columns;
     *                   {@code <= 0} → {@link FlowLayout} (left-aligned, for toolbar-style rows)
     */
    public IndicatorPanel(List<IndicatorModel> indicators, IniFileModel ini, int columns) {
        this.indicators = new ArrayList<>(indicators);
        this.ini = ini;

        if (columns > 0) {
            panel = new JPanel(new GridLayout(0, columns, 4, 4));
            panel.setAlignmentX(Component.LEFT_ALIGNMENT);
        } else {
            panel = new JPanel(new FlowLayout(FlowLayout.LEFT, 4, 2));
        }

        for (IndicatorModel model : this.indicators) {
            JLabel label = makeLabel();
            fixPreferredSize(label, model, ini);
            panel.add(label);
        }

        refresh(null);

        SensorCentral.getInstance().addListener(() -> {
            final ConfigurationImage snap = currentImage;
            SwingUtilities.invokeLater(() -> applyAll(snap));
        });
    }

    public JPanel getPanel() {
        return panel;
    }

    /**
     * Applies all indicator states using {@code image} for config-field lookups plus live sensor values.
     * Pass {@code null} if there is no config image (e.g. front-page indicators).
     */
    public void refresh(ConfigurationImage image) {
        currentImage = image;
        applyAll(image);
    }

    private void applyAll(ConfigurationImage image) {
        Component[] children = panel.getComponents();
        for (int i = 0; i < indicators.size() && i < children.length; i++) {
            if (children[i] instanceof JLabel) {
                applyState((JLabel) children[i], indicators.get(i), ini, image);
            }
        }
    }


    static JLabel makeLabel() {
        JLabel label = new JLabel(" ");
        label.setOpaque(true);
        label.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(Color.GRAY, 1),
                BorderFactory.createEmptyBorder(4, 8, 4, 8)));
        return label;
    }

    /**
     * Locks the label's preferred/minimum size to the widest text it can ever show,
     * considering both on/off states and all enum options for bitStringValue labels.
     */
    static void fixPreferredSize(JLabel label, IndicatorModel model, IniFileModel ini) {
        if (ini == null) return;
        List<String> candidates = new ArrayList<>();
        candidates.addAll(TsStringFunction.allPossibleResolutions(model.getOnLabel(), ini));
        candidates.addAll(TsStringFunction.allPossibleResolutions(model.getOffLabel(), ini));

        String saved = label.getText();
        int maxW = 0, maxH = 0;
        for (String text : candidates) {
            label.setText(text == null || text.isEmpty() ? " " : text);
            Dimension d = label.getPreferredSize();
            if (d.width > maxW) maxW = d.width;
            if (d.height > maxH) maxH = d.height;
        }
        label.setText(saved);
        if (maxW > 0) {
            Dimension fixed = new Dimension(maxW, maxH);
            label.setPreferredSize(fixed);
            label.setMinimumSize(fixed);
        }
    }

    static void applyState(JLabel label, IndicatorModel indicator, IniFileModel ini, ConfigurationImage ci) {
        Set<String> vars = new HashSet<>();
        vars.addAll(ExpressionEvaluator.extractVariables(indicator.getExpression()));
        vars.addAll(ExpressionEvaluator.extractVariables(indicator.getOnLabel()));
        vars.addAll(ExpressionEvaluator.extractVariables(indicator.getOffLabel()));
        Map<String, Double> context = new HashMap<>();
        for (String var : vars) {
            Optional<IniField> field = ini.findIniField(var);
            if (field.isPresent() && ci != null) {
                Double val = ci.readNumericValue(field.get());
                if (val != null) { context.put(var, val); continue; }
            }
            double sensorVal = SensorCentral.getInstance().getValue(var);
            if (!Double.isNaN(sensorVal)) context.put(var, sensorVal);
        }
        Boolean active = ExpressionEvaluator.evaluateBooleanExpression(indicator.getExpression(), context);
        final String text;
        final Color bg, fg;
        if (Boolean.TRUE.equals(active)) {
            text = resolveLabel(indicator.getOnLabel(), ini, context);
            bg = parseColor(indicator.getOnBg());
            fg = parseColor(indicator.getOnFg());
        } else {
            String off = resolveLabel(indicator.getOffLabel(), ini, context);
            text = off.isEmpty() ? " " : off;
            bg = parseColor(indicator.getOffBg());
            fg = parseColor(indicator.getOffFg());
        }
        label.setText(text);
        label.setBackground(bg);
        label.setForeground(fg);
    }

    static String resolveLabel(String raw, IniFileModel ini, Map<String, Double> context) {
        if (raw == null) return "";
        if (TsStringFunction.containsStringFunction(raw)) {
            String resolved = TsStringFunction.resolve(raw, ini, null, context);
            return resolved != null ? resolved : "";
        }
        String t = raw.trim();
        if (t.startsWith("{")) t = t.replaceAll("^\\{\\s*", "").replaceAll("\\s*}$", "").trim();
        return t;
    }

    static Color parseColor(String name) {
        if (name == null) return Color.LIGHT_GRAY;
        switch (name.toLowerCase().trim()) {
            case "white":  return Color.WHITE;
            case "black":  return Color.BLACK;
            case "red":    return Color.RED;
            case "yellow": return Color.YELLOW;
            case "green":  return Color.GREEN;
            case "blue":   return Color.BLUE;
            default:       return Color.LIGHT_GRAY;
        }
    }
}
