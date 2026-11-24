package com.rusefi.ui.logview;

import com.rusefi.ui.widgets.SegmentedControl;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JFormattedTextField;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JSeparator;
import javax.swing.JSpinner;
import javax.swing.JToggleButton;
import javax.swing.SpinnerNumberModel;
import javax.swing.SwingUtilities;
import javax.swing.text.DefaultFormatter;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.Window;
import java.util.Locale;

/**
 * One signal's line options, opened from "Line…" in its right-click menus as a dialog centred on the window, like
 * per-signal display smoothing: the chart's default width or its own in pixels, solid / dashed / dotted, and the
 * dash and gap (or the space between dots) in pixels, with a sample of the line. Changes show on the plot while
 * editing; Cancel puts the signal's line back. Dash and gap follow the width until typed in; Reset forgets
 * everything set for the signal.
 */
final class TraceLineEditor extends JPanel {
    /** Shows the editor and answers whether to keep the changes; a seam so tests need no modal dialog. */
    interface Confirm {
        boolean ask(Component parent, TraceLineEditor editor, String title);
    }

    static final Confirm DIALOG = (parent, editor, title) -> {
        Window window = SwingUtilities.getWindowAncestor(parent);
        return JOptionPane.showConfirmDialog(window == null ? parent : window, editor, title,
                JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE) == JOptionPane.OK_OPTION;
    };

    private static final String SOLID_CARD = "solid";
    private static final String DASHED_CARD = "dashed";
    private static final String DOTTED_CARD = "dotted";

    final JRadioButton defaultWidth;
    final JRadioButton ownWidth = new JRadioButton("Own");
    final JSpinner widthInput;
    final JToggleButton[] lineSegments = new JToggleButton[TraceStyle.Line.values().length];
    final JSpinner dashInput = lengthSpinner("Length of each dash in pixels");
    final JSpinner gapInput = lengthSpinner("Space between the dashes in pixels");
    final JSpinner spacingInput = lengthSpinner("Space between the dots in pixels");
    /** Dash and gap, spacing, or a note for a solid line; one card at a time, so the dialog keeps its size. */
    final JPanel lengths = new JPanel(new CardLayout());
    final TraceStyle.Sample sample = new TraceStyle.Sample();
    final JButton resetButton = new JButton("Reset");

    private final ChartSettings settings;
    private final String signal;
    private final Color color;
    private final float chartWidth;
    private final Runnable onChange;
    private String shownCard = SOLID_CARD;
    private boolean syncing;

    /**
     * Edits a signal's line in a dialog. The plot follows every change; Cancel restores the line it had.
     *
     * @return true when the changes were kept
     */
    static boolean edit(Component parent, ChartSettings settings, String signal, Color color, float chartWidth,
                        Runnable onChange, Confirm confirm) {
        TraceStyle.Spec before = settings.getTraceLine(signal);
        TraceLineEditor editor = new TraceLineEditor(settings, signal, color, chartWidth, onChange);
        if (confirm.ask(parent, editor, "Line — " + signal)) {
            return true;
        }
        settings.setTraceLine(signal, before);
        onChange.run();
        return false;
    }

    /**
     * @param chartWidth the chart's default width on the current plot, in pixels
     * @param onChange   called after every change, e.g. to repaint the chart
     */
    TraceLineEditor(ChartSettings settings, String signal, Color color, float chartWidth, Runnable onChange) {
        super(new GridBagLayout());
        this.settings = settings;
        this.signal = signal;
        this.color = color;
        this.chartWidth = chartWidth;
        this.onChange = onChange;
        setBorder(BorderFactory.createEmptyBorder(6, 8, 6, 8));

        defaultWidth = new JRadioButton("Default (" + pixels(chartWidth) + " px)");
        defaultWidth.setToolTipText("The width set under Grid in the toolbar, shared by every signal without its own");
        ButtonGroup widthGroup = new ButtonGroup();
        widthGroup.add(defaultWidth);
        widthGroup.add(ownWidth);
        widthInput = new JSpinner(new SpinnerNumberModel(settings.getTraceLine(signal).width(chartWidth),
                TraceStyle.MIN_WIDTH, TraceStyle.MAX_WIDTH, TraceStyle.WIDTH_STEP));
        widthInput.setToolTipText("Width of this signal's line in pixels (" + pixels(TraceStyle.MIN_WIDTH) + "–"
                + pixels(TraceStyle.MAX_WIDTH) + ")");
        commitWhileTyping(widthInput);

        ButtonGroup lineGroup = new ButtonGroup();
        TraceStyle.Line[] lines = TraceStyle.Line.values();
        for (int i = 0; i < lines.length; i++) {
            lineSegments[i] = SegmentedControl.segment(lines[i].label);
            lineGroup.add(lineSegments[i]);
        }

        lengths.add(note("<html>A solid line has no dashes to set.<br>Choose Dashed or Dotted for them.</html>"),
                SOLID_CARD);
        lengths.add(lengthCard(new String[]{"Dash", "Gap"}, new JSpinner[]{dashInput, gapInput}), DASHED_CARD);
        lengths.add(lengthCard(new String[]{"Spacing"}, new JSpinner[]{spacingInput}), DOTTED_CARD);

        int row = 0;
        addRow(this, ChartToolbar.muted(new JLabel("Width")), defaultWidth, row++);
        addRow(this, null, withUnit(ownWidth, widthInput), row++);
        separator(row++);
        addRow(this, ChartToolbar.muted(new JLabel("Line")), new SegmentedControl(lineSegments), row++);
        GridBagConstraints lengthsAt = constraints(0, row++, 2, 16);
        lengthsAt.fill = GridBagConstraints.HORIZONTAL;
        add(lengths, lengthsAt);
        separator(row++);
        GridBagConstraints sampleAt = constraints(0, row++, 2, 0);
        sampleAt.fill = GridBagConstraints.HORIZONTAL;
        add(sample, sampleAt);
        GridBagConstraints resetAt = constraints(0, row, 2, 0);
        resetAt.anchor = GridBagConstraints.EAST;
        resetAt.insets = new Insets(8, 0, 0, 0);
        resetButton.setToolTipText("Default width and a solid line for this signal");
        add(resetButton, resetAt);

        refresh();
        defaultWidth.addActionListener(e -> apply());
        ownWidth.addActionListener(e -> apply());
        widthInput.addChangeListener(e -> {
            if (!syncing) {
                // typing a width means this signal's own
                ownWidth.setSelected(true);
                apply();
            }
        });
        for (JToggleButton segment : lineSegments) {
            segment.addActionListener(e -> apply());
        }
        dashInput.addChangeListener(e -> applyLength(dashInput, true));
        gapInput.addChangeListener(e -> applyLength(gapInput, false));
        spacingInput.addChangeListener(e -> applyLength(spacingInput, false));
        resetButton.addActionListener(e -> {
            settings.setTraceLine(signal, TraceStyle.Spec.DEFAULT);
            changed();
        });
    }

    private TraceStyle.Line selectedLine() {
        for (int i = 0; i < lineSegments.length; i++) {
            if (lineSegments[i].isSelected()) {
                return TraceStyle.Line.values()[i];
            }
        }
        return TraceStyle.Line.SOLID;
    }

    /** Width and line from the controls; the dash and gap stay as they are, typed in or automatic. */
    private void apply() {
        if (syncing) {
            return;
        }
        TraceStyle.Spec spec = settings.getTraceLine(signal)
                .withWidth(ownWidth.isSelected() ? ((Number) widthInput.getValue()).doubleValue() : Double.NaN)
                .withLine(selectedLine());
        settings.setTraceLine(signal, spec);
        changed();
    }

    /** A typed dash or gap: from now on it stays in pixels rather than following the width. */
    private void applyLength(JSpinner input, boolean dash) {
        if (syncing) {
            return;
        }
        TraceStyle.Spec spec = settings.getTraceLine(signal);
        double value = ((Number) input.getValue()).doubleValue();
        settings.setTraceLine(signal, dash ? spec.withDash(value) : spec.withGap(value));
        changed();
    }

    private void changed() {
        refresh();
        onChange.run();
    }

    /** Puts the signal's settings in the controls, shows its line's lengths and redraws the sample. */
    void refresh() {
        syncing = true;
        try {
            TraceStyle.Spec spec = settings.getTraceLine(signal);
            (Double.isNaN(spec.width) ? defaultWidth : ownWidth).setSelected(true);
            float width = spec.width(chartWidth);
            widthInput.setValue((double) width);
            lineSegments[spec.line.ordinal()].setSelected(true);
            int gap = Math.round(spec.gap(width));
            dashInput.setValue(Math.round(spec.dash(width)));
            gapInput.setValue(gap);
            spacingInput.setValue(gap);
            shownCard = spec.line == TraceStyle.Line.DASHED ? DASHED_CARD
                    : spec.line == TraceStyle.Line.DOTTED ? DOTTED_CARD : SOLID_CARD;
            ((CardLayout) lengths.getLayout()).show(lengths, shownCard);
            resetButton.setEnabled(!spec.isDefault());
            sample.set(color, spec.stroke(width));
        } finally {
            syncing = false;
        }
    }

    /** "solid", "dashed" or "dotted": which lengths are shown. */
    String shownLengths() {
        return shownCard;
    }

    static String pixels(double value) {
        return value == Math.rint(value) ? String.valueOf((long) value) : String.format(Locale.ROOT, "%.1f", value);
    }

    /** Rows of a label and a pixel spinner, with the note on noisy signals below. */
    private static JPanel lengthCard(String[] labels, JSpinner[] inputs) {
        JPanel card = new JPanel(new GridBagLayout());
        card.setOpaque(false);
        int row = 0;
        for (int i = 0; i < labels.length; i++) {
            addRow(card, ChartToolbar.muted(new JLabel(labels[i])), withUnit(null, inputs[i]), row++);
        }
        GridBagConstraints noteAt = constraints(0, row, 2, 0);
        noteAt.insets = new Insets(4, 0, 2, 0);
        card.add(note("<html>Dashes and dots read best on smooth lines;<br>"
                + "on a noisy signal they break up into specks.</html>"), noteAt);
        return card;
    }

    private static JPanel note(String text) {
        JPanel panel = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 3));
        panel.setOpaque(false);
        panel.add(ChartToolbar.muted(new JLabel(text)));
        return panel;
    }

    private static JSpinner lengthSpinner(String tip) {
        JSpinner spinner = new JSpinner(new SpinnerNumberModel(TraceStyle.MIN_LENGTH, TraceStyle.MIN_LENGTH,
                TraceStyle.MAX_LENGTH, 1));
        spinner.setToolTipText(tip);
        commitWhileTyping(spinner);
        return spinner;
    }

    /** Every valid typed value reaches the model at once, not only on Enter or focus loss. */
    private static void commitWhileTyping(JSpinner spinner) {
        JFormattedTextField field = ((JSpinner.DefaultEditor) spinner.getEditor()).getTextField();
        field.setColumns(3);
        if (field.getFormatter() instanceof DefaultFormatter) {
            ((DefaultFormatter) field.getFormatter()).setCommitsOnValidEdit(true);
        }
    }

    private static JPanel withUnit(Component before, JSpinner spinner) {
        JPanel panel = new JPanel(new FlowLayout(FlowLayout.LEFT, 6, 0));
        panel.setOpaque(false);
        if (before != null) {
            panel.add(before);
        }
        panel.add(spinner);
        panel.add(ChartToolbar.muted(new JLabel("px")));
        return panel;
    }

    private static void addRow(JPanel panel, Component label, Component control, int row) {
        if (label != null) {
            panel.add(label, constraints(0, row, control == null ? 2 : 1, 0));
        }
        if (control != null) {
            panel.add(control, constraints(1, row, 1, 12));
        }
    }

    private void separator(int row) {
        GridBagConstraints c = constraints(0, row, 2, 0);
        c.fill = GridBagConstraints.HORIZONTAL;
        c.insets = new Insets(8, 0, 8, 0);
        add(new JSeparator(), c);
    }

    private static GridBagConstraints constraints(int column, int row, int width, int leftInset) {
        GridBagConstraints c = new GridBagConstraints();
        c.gridx = column;
        c.gridy = row;
        c.gridwidth = width;
        c.anchor = GridBagConstraints.WEST;
        c.insets = new Insets(3, leftInset, 3, 0);
        return c;
    }
}
