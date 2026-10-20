package com.rusefi.ui;

import com.rusefi.ui.widgets.GaugeScale;
import com.rusefi.ui.widgets.GaugeScaleOverrides;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;

/**
 * "Scale…" dialog of a gauge slot: optional replacements for the .ini min, max, high danger
 * value and value decimals. Empty fields show, and keep, the .ini value.
 */
final class GaugeScaleDialog {
    static final String TITLE = "Gauge Scale";
    static final String RESET = "Reset to INI";

    final JTextField min = new JTextField(10);
    final JTextField max = new JTextField(10);
    final JTextField danger = new JTextField(10);
    final JComboBox<String> decimals = new JComboBox<>();
    final JButton reset = new JButton(RESET);
    private final GaugeScale ini;
    private final JPanel content = new JPanel(new GridBagLayout());

    GaugeScaleDialog(String gaugeTitle, @Nullable String units, GaugeScale ini, GaugeScaleOverrides current) {
        this.ini = ini;
        String unitSuffix = units == null || units.trim().isEmpty() ? "" : " " + units.trim();
        setUp(min, number(ini.getMin()) + unitSuffix, current.getMin());
        setUp(max, number(ini.getMax()) + unitSuffix, current.getMax());
        setUp(danger, Double.isFinite(ini.getHighDanger()) ? number(ini.getHighDanger()) + unitSuffix : "none", current.getDanger());
        danger.setToolTipText("The gauge turns red from this value up; at or above Max there is no danger zone");
        decimals.addItem("INI default (" + ini.getValueDecimals() + ")");
        for (int i = 0; i <= GaugeScale.MAX_DECIMALS; i++) {
            decimals.addItem(Integer.toString(i));
        }
        decimals.setSelectedIndex(current.getDecimals() == null ? 0 : current.getDecimals() + 1);
        decimals.setToolTipText("Decimals of the value readout; the scale labels keep the INI decimals");
        reset.setToolTipText("Clear every field so the gauge uses its INI settings");
        reset.addActionListener(e -> {
            min.setText("");
            max.setText("");
            danger.setText("");
            decimals.setSelectedIndex(0);
        });

        GridBagConstraints c = new GridBagConstraints();
        c.insets = new Insets(4, 4, 4, 4);
        c.anchor = GridBagConstraints.WEST;
        c.gridx = 0;
        c.gridy = 0;
        c.gridwidth = 2;
        content.add(new JLabel(gaugeTitle), c);
        c.gridwidth = 1;
        addRow(c, "Min", min);
        addRow(c, "Max", max);
        addRow(c, "Danger from", danger);
        addRow(c, "Decimals", decimals);
        c.gridx = 1;
        c.gridy++;
        content.add(reset, c);
    }

    private static void setUp(JTextField field, String placeholder, @Nullable Double value) {
        field.putClientProperty("JTextField.placeholderText", placeholder);
        field.setText(value == null ? "" : number(value));
    }

    private void addRow(GridBagConstraints c, String label, JComponent field) {
        c.gridy++;
        c.gridx = 0;
        content.add(new JLabel(label), c);
        c.gridx = 1;
        content.add(field, c);
    }

    JPanel getContent() {
        return content;
    }

    GaugeScaleOverrides.Parsed parse() {
        int index = decimals.getSelectedIndex();
        return GaugeScaleOverrides.parse(min.getText(), max.getText(), danger.getText(),
            index <= 0 ? null : index - 1, ini);
    }

    /** Shows the dialog until the entry is valid or cancelled; {@code null} when cancelled. */
    @Nullable
    GaugeScaleOverrides show(Component parent) {
        while (true) {
            int choice = JOptionPane.showConfirmDialog(parent, content, TITLE, JOptionPane.OK_CANCEL_OPTION,
                JOptionPane.PLAIN_MESSAGE);
            if (choice != JOptionPane.OK_OPTION) {
                return null;
            }
            GaugeScaleOverrides.Parsed parsed = parse();
            if (parsed.overrides != null) {
                return parsed.overrides;
            }
            JOptionPane.showMessageDialog(parent, parsed.error, TITLE, JOptionPane.ERROR_MESSAGE);
        }
    }

    /** {@code value} without a trailing ".0" for whole numbers. */
    static String number(double value) {
        if (value == Math.rint(value) && Math.abs(value) < 1e15) {
            return Long.toString((long) value);
        }
        return Double.toString(value);
    }
}
