package com.rusefi.ui.logview;

import javax.swing.BorderFactory;
import javax.swing.JCheckBox;
import javax.swing.JFormattedTextField;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;
import javax.swing.SwingUtilities;
import javax.swing.text.DefaultFormatter;
import java.awt.Component;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.Window;
import java.util.function.IntConsumer;

/** A roomy millisecond editor shared by global and per-signal timeline smoothing. */
final class TimelineSmoothingDialog {
    private TimelineSmoothingDialog() {
    }

    /** @return zero for Raw, a window in milliseconds, or null when cancelled */
    static Integer show(Component parent, String title, TimelineSmoothingWindow limits, int initialMs,
                        boolean allowRaw, IntConsumer preview) {
        boolean initiallyRaw = allowRaw && initialMs == 0;
        int initialWindow = initiallyRaw ? limits.initialMilliseconds(0)
                : Math.max(limits.minimumMs, Math.min(limits.maximumMs, initialMs));
        JSpinner input = new JSpinner(new SpinnerNumberModel(initialWindow,
                limits.minimumMs, limits.maximumMs, limits.minimumMs));
        JFormattedTextField inputField = ((JSpinner.DefaultEditor) input.getEditor()).getTextField();
        inputField.setColumns(8);
        if (inputField.getFormatter() instanceof DefaultFormatter) {
            // Push every valid typed value into the spinner model, not only Enter/focus loss.
            ((DefaultFormatter) inputField.getFormatter()).setCommitsOnValidEdit(true);
        }

        JCheckBox raw = new JCheckBox("Show raw samples", initiallyRaw);
        input.setEnabled(!initiallyRaw);
        input.addChangeListener(e -> {
            if (!raw.isSelected()) {
                preview.accept((Integer) input.getValue());
            }
        });
        raw.addActionListener(e -> {
            input.setEnabled(!raw.isSelected());
            preview.accept(raw.isSelected() ? 0 : (Integer) input.getValue());
        });

        JPanel content = new JPanel(new GridLayout(0, 1, 0, 10));
        content.setBorder(BorderFactory.createEmptyBorder(6, 8, 6, 8));
        if (allowRaw) {
            content.add(raw);
        }
        JPanel windowRow = new JPanel(new FlowLayout(FlowLayout.LEFT, 8, 0));
        windowRow.add(new JLabel("Window"));
        windowRow.add(input);
        windowRow.add(new JLabel("ms"));
        content.add(windowRow);
        content.add(new JLabel("Range " + limits.minimumMs + "\u2013" + limits.maximumMs
                + " ms; step " + limits.minimumMs + " ms"));
        content.add(new JLabel("<html><b>Display only</b> \u2014 cursor values follow the smoothed line.<br>"
                + "Min/max, statistics and exports stay raw.</html>"));

        Window window = SwingUtilities.getWindowAncestor(parent);
        Component dialogParent = window == null ? parent : window;
        int result = JOptionPane.showConfirmDialog(dialogParent, content, title,
                JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
        if (result != JOptionPane.OK_OPTION) {
            return null;
        }
        return raw.isSelected() ? 0 : (Integer) input.getValue();
    }
}
