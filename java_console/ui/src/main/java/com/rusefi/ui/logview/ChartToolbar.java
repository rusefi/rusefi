package com.rusefi.ui.logview;

import com.formdev.flatlaf.FlatClientProperties;
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
import java.awt.Color;
import java.awt.Component;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.util.function.Consumer;

/**
 * Toolbar above the log chart, styled like the Gauges toolbar: Open, plot colours, a Grid button that
 * opens the grid options, the cursor Snap / Free switch, a Values button that opens the options of the values
 * at the cursor, the Measure (A/B cursors) and Stats toggles.
 * [tag:console_theme]
 */
final class ChartToolbar extends ThemedStrip {
    final JToggleButton lightPlot = SegmentedControl.segment("Light");
    final JToggleButton darkPlot = SegmentedControl.segment("Dark");
    final JButton openButton = LogViewerHost.openLogButton("Open  ▾");
    final JButton gridButton = new JButton();
    final JToggleButton snapCursor = SegmentedControl.segment("Snap");
    final JToggleButton freeCursor = SegmentedControl.segment("Free");
    final JButton valuesButton = new JButton();

    // values at the cursor options, shown in a popup under the Values button
    final JPanel valuesPanel = new JPanel(new GridBagLayout());
    final JToggleButton valuesOff = SegmentedControl.segment("Off");
    final JToggleButton valuesLabels = SegmentedControl.segment("Labels");
    final JToggleButton valuesList = SegmentedControl.segment("List");
    final JToggleButton atMouse = SegmentedControl.segment("Mouse");
    final JToggleButton atClick = SegmentedControl.segment("Clicked time");
    private final JLabel atNote = muted(new JLabel());
    private final JPopupMenu valuesPopup = new JPopupMenu();
    /** Shows the statistics table; the viewer panel listens to it. */
    final JToggleButton measureButton = new JToggleButton("Measure");
    final JToggleButton statsButton = new JToggleButton("Stats");

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

    /** @param onOpen action of the Open button at the start of the toolbar, given the button, or null for none */
    ChartToolbar(ChartSettings settings, Runnable onChange, Consumer<Component> onOpen) {
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

        valuesButton.setToolTipText("How the signal values at the cursor show on the plots");
        valuesButton.addActionListener(e -> valuesPopup.show(valuesButton, 0, valuesButton.getHeight() + 4));
        buildValuesPanel();
        valuesPopup.add(valuesPanel);

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
            openButton.addActionListener(e -> onOpen.accept(openButton));
            add(new JoinedButtons(openButton));
            add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        }
        // right after the view switch, as in the Scatter view's toolbar, so it stays put when switching views
        add(labelled("Plot", new SegmentedControl(lightPlot, darkPlot)));
        add(new JoinedButtons(gridButton));
        add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        add(muted(new JLabel("Cursor")));
        add(new SegmentedControl(snapCursor, freeCursor));
        add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        add(new JoinedButtons(valuesButton));
        add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        // a light accent tint when on, instead of the toolbar buttons' heavy grey [tag:console_theme]
        measureButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "viewToggle");
        measureButton.setToolTipText("<html>Measure between two points: click a plot to place cursor A, then B "
                + "(later clicks move the nearer one).<br>Shows the time between them and each signal's change; "
                + "statistics cover A–B. Drag a flag to move it, Esc clears.</html>");
        add(new JoinedButtons(measureButton));
        add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        statsButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "viewToggle");
        statsButton.setToolTipText("Show the value at the cursor and the min, max and average over the "
                + "visible time of every plotted signal");
        add(new JoinedButtons(statsButton));
        updateControls();
    }

    /** Puts the Timeline / Histogram switch at the start of the toolbar, after Open. */
    void addViewSwitch(Component viewSwitch) {
        // Open and its gap come first when the toolbar has them
        int index = openButton.getParent() != null ? 2 : 0;
        add(viewSwitch, index);
        add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP), index + 1);
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

    private void buildValuesPanel() {
        valuesPanel.setBorder(BorderFactory.createEmptyBorder(10, 12, 10, 12));
        ButtonGroup styleGroup = new ButtonGroup();
        for (JToggleButton segment : valuesSegments()) {
            styleGroup.add(segment);
            segment.addActionListener(e -> apply());
        }
        valuesSegments()[settings.getCursorValues().ordinal()].setSelected(true);
        valuesOff.setToolTipText("Values only in the legends above the plots");
        valuesLabels.setToolTipText("<html>A value beside each trace where it crosses the cursor line;<br>"
                + "close or crossing traces get stacked values with a line back to their trace</html>");
        valuesList.setToolTipText("A box beside the cursor line lists each plot's values");

        ButtonGroup atGroup = new ButtonGroup();
        atGroup.add(atMouse);
        atGroup.add(atClick);
        (settings.isPinValues() ? atClick : atMouse).setSelected(true);
        atMouse.setToolTipText("The values follow the mouse over the plots");
        atClick.setToolTipText("The values stay where you clicked while the mouse moves on");
        atMouse.addActionListener(e -> apply());
        atClick.addActionListener(e -> apply());

        int row = 0;
        addRow(valuesPanel, muted(new JLabel("Show")), new SegmentedControl(valuesSegments()), row++, 0);
        addRow(valuesPanel, muted(new JLabel("At")), new SegmentedControl(atMouse, atClick), row++, 0);
        GridBagConstraints noteConstraints = constraints(0, row, 2, 0);
        noteConstraints.insets = new Insets(8, 0, 0, 0);
        valuesPanel.add(atNote, noteConstraints);
    }

    /** The Values button's text: "Values: Labels  ▾", "Values: List at click  ▾". */
    static String valuesSummary(ChartSettings settings) {
        String style;
        switch (settings.getCursorValues()) {
            case OFF:
                return "Values: Off  ▾";
            case LIST:
                style = "List";
                break;
            default:
                style = "Labels";
        }
        return "Values: " + style + (settings.isPinValues() ? " at click" : "") + "  ▾";
    }

    private void addRow(Component label, Component control, int row, int indent) {
        addRow(gridPanel, label, control, row, indent);
    }

    private static void addRow(JPanel panel, Component label, Component control, int row, int indent) {
        GridBagConstraints left = constraints(0, row, control == null ? 2 : 1, indent);
        panel.add(label, left);
        if (control != null) {
            GridBagConstraints right = constraints(1, row, 1, 12);
            panel.add(control, right);
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

    /** A muted label and its control as one piece, so a wrapping toolbar never puts them on different lines. */
    static JPanel labelled(String text, Component control) {
        JPanel pair = new JPanel(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, 0));
        pair.setOpaque(false);
        pair.add(muted(new JLabel(text)));
        pair.add(control);
        return pair;
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

    /** The Values segments, in {@link ChartSettings.CursorValues} order. */
    private JToggleButton[] valuesSegments() {
        return new JToggleButton[]{valuesOff, valuesLabels, valuesList};
    }

    private void apply() {
        settings.setDarkPlot(darkPlot.isSelected());
        settings.setSnapCursor(snapCursor.isSelected());
        JToggleButton[] values = valuesSegments();
        for (int i = 0; i < values.length; i++) {
            if (values[i].isSelected()) {
                settings.setCursorValues(ChartSettings.CursorValues.values()[i]);
            }
        }
        settings.setPinValues(atClick.isSelected());
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
        valuesButton.setText(valuesSummary(settings));
        boolean valuesShown = settings.getCursorValues() != ChartSettings.CursorValues.OFF;
        atMouse.setEnabled(valuesShown);
        atClick.setEnabled(valuesShown);
        atNote.setText(!valuesShown ? "<html>The legends above the plots still show<br>the values at the cursor.</html>"
                : settings.isPinValues() ? "<html>Click a plot to show the values there.<br>"
                + "Click the same spot again or press Esc to hide them.</html>"
                : "<html>Move the mouse over the plots<br>to see the values at the cursor.</html>");
        // the note's height changes with its text
        valuesPopup.pack();
        timeTicksMode.setEnabled(settings.isVertical());
        samplesMode.setEnabled(settings.isVertical());
        // Spacing also sets the time labels, so it stays usable while the vertical lines are off.
        timeSpacing.setEnabled(settings.getVerticalMode() == ChartSettings.VerticalMode.TIME_TICKS || !settings.isVertical());
        horizontalDivisions.setEnabled(settings.isHorizontal());
    }
}
