package com.rusefi.ui.logview;

import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.SegmentedControl;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JSeparator;
import javax.swing.JSpinner;
import javax.swing.JToggleButton;
import javax.swing.SpinnerNumberModel;
import javax.swing.UIManager;
import java.awt.Color;
import java.awt.Component;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

/**
 * Toolbar above the log chart, styled like the Gauges toolbar: Open, plot colours, a Grid button that
 * opens the grid options, and the cursor Snap / Free switch. [tag:console_theme]
 */
final class ChartToolbar extends ThemedStrip {
    final JToggleButton lightPlot = SegmentedControl.segment("Light");
    final JToggleButton darkPlot = SegmentedControl.segment("Dark");
    final JButton openButton = new JButton("Open…");
    final JButton gridButton = new JButton();
    final JToggleButton snapCursor = SegmentedControl.segment("Snap");
    final JToggleButton freeCursor = SegmentedControl.segment("Free");

    // grid options, shown in a popup under the Grid button
    final JPanel gridPanel = new JPanel(new GridBagLayout());
    final JCheckBox verticalBox = new JCheckBox("Vertical lines");
    final JToggleButton timeTicksMode = SegmentedControl.segment("Time ticks");
    final JToggleButton samplesMode = SegmentedControl.segment("Every sample");
    final JSpinner timeSpacing;
    final JCheckBox horizontalBox = new JCheckBox("Horizontal lines");
    final JSpinner horizontalDivisions;

    private final JPopupMenu gridPopup = new JPopupMenu();
    private final ChartSettings settings;
    private final Runnable onChange;

    ChartToolbar(ChartSettings settings, Runnable onChange) {
        this(settings, onChange, null);
    }

    /** @param onOpen action of an Open button at the start of the toolbar, or null for no button */
    ChartToolbar(ChartSettings settings, Runnable onChange, Runnable onOpen) {
        super(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, 0), "Console.Toolbar.background", false);
        this.settings = settings;
        this.onChange = onChange;
        setBorder(BorderFactory.createEmptyBorder(8, JoinedButtons.ROW_END_INSET - JoinedButtons.ROW_GAP, 9, 0));

        ButtonGroup plotGroup = new ButtonGroup();
        plotGroup.add(lightPlot);
        plotGroup.add(darkPlot);
        (settings.isDarkPlot() ? darkPlot : lightPlot).setSelected(true);
        lightPlot.setToolTipText("Plot area with a light background");
        darkPlot.setToolTipText("Plot area with a dark background");

        gridButton.setToolTipText("Grid line options");
        gridButton.addActionListener(e -> gridPopup.show(gridButton, 0, gridButton.getHeight() + 4));

        ButtonGroup cursorGroup = new ButtonGroup();
        cursorGroup.add(snapCursor);
        cursorGroup.add(freeCursor);
        (settings.isSnapCursor() ? snapCursor : freeCursor).setSelected(true);
        snapCursor.setToolTipText("The cursor line jumps to each logged sample");
        freeCursor.setToolTipText("<html>The cursor line follows the mouse; values come from the<br>"
                + "nearest logged sample, marked with a dot on each trace</html>");

        timeSpacing = spinner(new SpinnerNumberModel(settings.getTimeSpacing(),
                ChartSettings.MIN_TIME_SPACING, ChartSettings.MAX_TIME_SPACING, 10));
        horizontalDivisions = spinner(new SpinnerNumberModel(settings.getHorizontalDivisions(),
                ChartSettings.MIN_HORIZONTAL_DIVISIONS, ChartSettings.MAX_HORIZONTAL_DIVISIONS, 1));
        buildGridPanel();
        gridPopup.add(gridPanel);

        lightPlot.addActionListener(e -> apply());
        darkPlot.addActionListener(e -> apply());
        snapCursor.addActionListener(e -> apply());
        freeCursor.addActionListener(e -> apply());
        verticalBox.addActionListener(e -> apply());
        timeTicksMode.addActionListener(e -> apply());
        samplesMode.addActionListener(e -> apply());
        timeSpacing.addChangeListener(e -> apply());
        horizontalBox.addActionListener(e -> apply());
        horizontalDivisions.addChangeListener(e -> apply());

        if (onOpen != null) {
            openButton.setIcon(UIManager.getIcon("Tree.openIcon"));
            openButton.setToolTipText("Open a data log (" + LogViewerHost.SUPPORTED + ") - Ctrl+O, or drop a file on the viewer");
            openButton.addActionListener(e -> onOpen.run());
            add(new JoinedButtons(openButton));
            add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        }
        add(muted(new JLabel("Plot")));
        add(new SegmentedControl(lightPlot, darkPlot));
        add(new JoinedButtons(gridButton));
        add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        add(muted(new JLabel("Cursor")));
        add(new SegmentedControl(snapCursor, freeCursor));
        updateControls();
    }

    private void buildGridPanel() {
        gridPanel.setBorder(BorderFactory.createEmptyBorder(10, 12, 10, 12));
        ButtonGroup modeGroup = new ButtonGroup();
        modeGroup.add(timeTicksMode);
        modeGroup.add(samplesMode);
        (settings.getVerticalMode() == ChartSettings.VerticalMode.SAMPLES ? samplesMode : timeTicksMode).setSelected(true);

        verticalBox.setSelected(settings.isVertical());
        timeTicksMode.setToolTipText("A line at each labelled time");
        samplesMode.setToolTipText("A line at each logged sample, shown once zoomed in far enough to tell them apart");
        timeSpacing.setToolTipText("Approximate distance between time ticks, in pixels ("
                + ChartSettings.MIN_TIME_SPACING + "–" + ChartSettings.MAX_TIME_SPACING
                + "); also spaces the time labels");
        horizontalBox.setSelected(settings.isHorizontal());
        horizontalDivisions.setToolTipText("Number of horizontal bands ("
                + ChartSettings.MIN_HORIZONTAL_DIVISIONS + "–" + ChartSettings.MAX_HORIZONTAL_DIVISIONS + ")");

        int row = 0;
        addRow(verticalBox, null, row++, 0);
        addRow(muted(new JLabel("Lines at")), new SegmentedControl(timeTicksMode, samplesMode), row++, 24);
        addRow(muted(new JLabel("Spacing")), withUnit(timeSpacing, "px"), row++, 24);
        GridBagConstraints separator = constraints(0, row++, 2, 0);
        separator.fill = GridBagConstraints.HORIZONTAL;
        separator.insets = new Insets(8, 0, 8, 0);
        gridPanel.add(new JSeparator(), separator);
        addRow(horizontalBox, null, row++, 0);
        addRow(muted(new JLabel("Bands")), horizontalDivisions, row++, 24);
        JLabel note = muted(new JLabel("<html>Each trace has its own scale, so horizontal<br>"
                + "lines mark fractions of each trace's range.</html>"));
        GridBagConstraints noteConstraints = constraints(0, row, 2, 0);
        noteConstraints.insets = new Insets(8, 0, 0, 0);
        gridPanel.add(note, noteConstraints);
    }

    private void addRow(Component label, Component control, int row, int indent) {
        GridBagConstraints left = constraints(0, row, control == null ? 2 : 1, indent);
        gridPanel.add(label, left);
        if (control != null) {
            GridBagConstraints right = constraints(1, row, 1, 12);
            gridPanel.add(control, right);
        }
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

    private static JPanel withUnit(JSpinner spinner, String unit) {
        JPanel panel = new JPanel(new FlowLayout(FlowLayout.LEFT, 6, 0));
        panel.setOpaque(false);
        panel.add(spinner);
        panel.add(muted(new JLabel(unit)));
        return panel;
    }

    static JLabel muted(JLabel label) {
        label.setForeground(themed("Console.LogViewer.mutedForeground", Color.GRAY));
        return label;
    }

    private static JSpinner spinner(SpinnerNumberModel model) {
        JSpinner spinner = new JSpinner(model);
        ((JSpinner.DefaultEditor) spinner.getEditor()).getTextField().setColumns(3);
        return spinner;
    }

    /** Grid button text, so the current state is visible without opening the popup. */
    static String gridSummary(ChartSettings settings) {
        String state;
        if (settings.isVertical() && settings.isHorizontal()) {
            state = "Both";
        } else if (settings.isVertical()) {
            state = "Vertical";
        } else if (settings.isHorizontal()) {
            state = "Horizontal";
        } else {
            state = "Off";
        }
        return "Grid: " + state + "  ▾";
    }

    private void apply() {
        settings.setDarkPlot(darkPlot.isSelected());
        settings.setSnapCursor(snapCursor.isSelected());
        settings.setVertical(verticalBox.isSelected());
        settings.setVerticalMode(samplesMode.isSelected()
                ? ChartSettings.VerticalMode.SAMPLES : ChartSettings.VerticalMode.TIME_TICKS);
        settings.setTimeSpacing((Integer) timeSpacing.getValue());
        settings.setHorizontal(horizontalBox.isSelected());
        settings.setHorizontalDivisions((Integer) horizontalDivisions.getValue());
        updateControls();
        onChange.run();
    }

    private void updateControls() {
        gridButton.setText(gridSummary(settings));
        timeTicksMode.setEnabled(settings.isVertical());
        samplesMode.setEnabled(settings.isVertical());
        // Spacing also sets the time labels, so it stays usable while the vertical lines are off.
        timeSpacing.setEnabled(settings.getVerticalMode() == ChartSettings.VerticalMode.TIME_TICKS || !settings.isVertical());
        horizontalDivisions.setEnabled(settings.isHorizontal());
    }
}
