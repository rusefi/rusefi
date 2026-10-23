package com.rusefi.ui.logview;

import com.rusefi.ui.widgets.SegmentedControl;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.JToggleButton;
import javax.swing.SpinnerNumberModel;
import java.awt.Component;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

/**
 * The colour settings of the histogram's Z signal, in the Colours popup: palette, Reverse, range (Auto, No
 * outliers, Manual from / to) and, for the diverging palette, its centre. Values are entered in the unit the
 * table shows (lambda or AFR) and kept in the signal's own unit.
 */
final class HistogramColorsPanel extends JPanel {
    final JToggleButton[] paletteButtons = new JToggleButton[HistogramColors.Palette.values().length];
    final JCheckBox reverseBox = new JCheckBox("Reverse");
    final JToggleButton[] rangeButtons = new JToggleButton[HistogramColors.Range.values().length];
    final JSpinner lowSpinner;
    final JSpinner highSpinner;
    final JCheckBox autoCentreBox;
    final JSpinner centreSpinner;
    final JButton resetButton = new JButton("Reset");

    private final HistogramColors colors;
    private final double factor;
    private int row;

    /**
     * @param onLayoutChange after a choice that changes which fields apply (palette, range, reset): recolour and
     *                       rebuild the panel
     * @param onValueChange  after a value changes: recolour only
     */
    HistogramColorsPanel(HistogramColors colors, String zSignal, ColourScale table, Runnable onLayoutChange,
                         Runnable onValueChange) {
        super(new GridBagLayout());
        this.colors = colors;
        this.factor = table.mixture().factor;
        setBorder(BorderFactory.createEmptyBorder(10, 12, 10, 12));
        // Time and Count ignore the manual range and centre
        boolean values = table.showsValues();

        JLabel title = new JLabel("Colours for " + zSignal);
        title.setFont(title.getFont().deriveFont(Font.BOLD));
        addRow(title, null);

        ButtonGroup palettes = new ButtonGroup();
        for (HistogramColors.Palette palette : HistogramColors.Palette.values()) {
            JToggleButton button = SegmentedControl.segment(palette.text);
            button.setToolTipText(palette.tooltip);
            button.setSelected(palette == colors.palette());
            button.addActionListener(e -> {
                colors.setPalette(palette);
                onLayoutChange.run();
            });
            palettes.add(button);
            paletteButtons[palette.ordinal()] = button;
        }
        addRow(ChartToolbar.muted(new JLabel("Palette")), new SegmentedControl(paletteButtons));
        reverseBox.setSelected(colors.reversed());
        reverseBox.setToolTipText("Run the palette the other way: high values get the low-end colour");
        reverseBox.addActionListener(e -> {
            colors.setReversed(reverseBox.isSelected());
            onValueChange.run();
        });
        addRow(null, reverseBox);

        ButtonGroup ranges = new ButtonGroup();
        for (HistogramColors.Range range : HistogramColors.Range.values()) {
            JToggleButton button = SegmentedControl.segment(range.text);
            button.setToolTipText(range.tooltip);
            button.setSelected(range == colors.range());
            button.addActionListener(e -> {
                if (range == HistogramColors.Range.MANUAL && !(colors.manualHigh() > colors.manualLow())) {
                    // start from the colours on screen
                    colors.setManual(table.scaleLow() / factor, table.scaleHigh() / factor);
                }
                colors.setRange(range);
                onLayoutChange.run();
            });
            ranges.add(button);
            rangeButtons[range.ordinal()] = button;
        }
        addRow(ChartToolbar.muted(new JLabel("Range")), new SegmentedControl(rangeButtons));

        double low = colors.manualHigh() > colors.manualLow() ? colors.manualLow() * factor : table.scaleLow();
        double high = colors.manualHigh() > colors.manualLow() ? colors.manualHigh() * factor : table.scaleHigh();
        double step = LogChart.niceStep(Math.max(1e-6, Math.abs(high - low)) / 50);
        lowSpinner = spinner(low, step);
        highSpinner = spinner(high, step);
        Runnable manualChanged = () -> {
            colors.setManual(number(lowSpinner) / factor, number(highSpinner) / factor);
            onValueChange.run();
        };
        lowSpinner.addChangeListener(e -> manualChanged.run());
        highSpinner.addChangeListener(e -> manualChanged.run());
        if (colors.range() == HistogramColors.Range.MANUAL) {
            JPanel fromTo = flow(lowSpinner, ChartToolbar.muted(new JLabel("to")), highSpinner,
                    ChartToolbar.muted(new JLabel(unit(table))));
            addRow(ChartToolbar.muted(new JLabel("From")), fromTo);
            lowSpinner.setEnabled(values);
            highSpinner.setEnabled(values);
        }

        boolean mixture = table.mixture() != HistogramTable.Mixture.NONE;
        autoCentreBox = new JCheckBox(mixture ? "Stoichiometric" : "Middle of the range");
        double centre = Double.isFinite(colors.centre()) ? colors.centre() * factor
                : Double.isFinite(table.scaleCentre()) ? table.scaleCentre() : (low + high) / 2;
        centreSpinner = spinner(centre, step);
        autoCentreBox.setSelected(!Double.isFinite(colors.centre()));
        autoCentreBox.setToolTipText("Centre the colours automatically, or on the value entered");
        centreSpinner.setEnabled(!autoCentreBox.isSelected());
        autoCentreBox.addActionListener(e -> {
            colors.setCentre(autoCentreBox.isSelected() ? Double.NaN : number(centreSpinner) / factor);
            centreSpinner.setEnabled(!autoCentreBox.isSelected());
            onValueChange.run();
        });
        centreSpinner.addChangeListener(e -> {
            if (!autoCentreBox.isSelected()) {
                colors.setCentre(number(centreSpinner) / factor);
                onValueChange.run();
            }
        });
        if (colors.palette() == HistogramColors.Palette.DIVERGING) {
            addRow(ChartToolbar.muted(new JLabel("Centre")), flow(autoCentreBox, centreSpinner,
                    ChartToolbar.muted(new JLabel(unit(table)))));
        }

        String note = values
                ? "Kept for " + zSignal + "; other signals keep their own."
                : "Time and Count colour from 0 up; the manual range and centre apply to Mean, Min and Max.";
        addRow(ChartToolbar.muted(new JLabel(note)), null);
        resetButton.setToolTipText("Back to the heatmap without outliers");
        resetButton.addActionListener(e -> {
            colors.reset();
            onLayoutChange.run();
        });
        addRow(null, resetButton);
    }

    private static String unit(ColourScale table) {
        return table.zUnit();
    }

    private static JSpinner spinner(double value, double step) {
        JSpinner spinner = new JSpinner(new SpinnerNumberModel(value, -1e9, 1e9, step));
        int decimals = Math.max(0, Math.min(4, (int) Math.ceil(-Math.log10(step))));
        StringBuilder pattern = new StringBuilder("0");
        if (decimals > 0) {
            pattern.append('.');
            for (int i = 0; i < decimals; i++) {
                pattern.append('0');
            }
        }
        spinner.setEditor(new JSpinner.NumberEditor(spinner, pattern.toString()));
        ((JSpinner.DefaultEditor) spinner.getEditor()).getTextField().setColumns(5);
        return spinner;
    }

    private static double number(JSpinner spinner) {
        return ((Number) spinner.getValue()).doubleValue();
    }

    private static JPanel flow(JComponent... components) {
        JPanel panel = new JPanel(new FlowLayout(FlowLayout.LEFT, 6, 0));
        panel.setOpaque(false);
        for (JComponent component : components) {
            panel.add(component);
        }
        return panel;
    }

    private void addRow(Component label, Component control) {
        GridBagConstraints left = new GridBagConstraints();
        left.gridx = 0;
        left.gridy = row;
        left.gridwidth = control == null ? 2 : 1;
        left.anchor = GridBagConstraints.WEST;
        left.insets = new Insets(4, 0, 4, 0);
        if (label != null) {
            add(label, left);
        }
        if (control != null) {
            GridBagConstraints right = new GridBagConstraints();
            right.gridx = 1;
            right.gridy = row;
            right.anchor = GridBagConstraints.WEST;
            right.insets = new Insets(4, 12, 4, 0);
            add(control, right);
        }
        row++;
    }
}
