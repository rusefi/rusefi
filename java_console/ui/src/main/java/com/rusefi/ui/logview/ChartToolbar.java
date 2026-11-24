package com.rusefi.ui.logview;

import com.formdev.flatlaf.FlatClientProperties;
import com.rusefi.ui.util.WrapLayout;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.SegmentedControl;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFormattedTextField;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JSeparator;
import javax.swing.JSpinner;
import javax.swing.JToggleButton;
import javax.swing.SpinnerNumberModel;
import javax.swing.text.DefaultFormatter;
import java.awt.Color;
import java.awt.Component;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseWheelEvent;
import java.util.function.Consumer;

/**
 * Toolbar above the log chart, styled like the Gauges toolbar: Open, plot colours, Grid and display-smoothing
 * options, the cursor Snap / Free switch, cursor values, Measure (A/B cursors) and Stats.
 * [tag:console_theme]
 */
final class ChartToolbar extends ThemedStrip {
    final JToggleButton lightPlot = SegmentedControl.segment("Light");
    final JToggleButton darkPlot = SegmentedControl.segment("Dark");
    final JButton openButton = LogViewerHost.openLogButton("Open  ▾");
    final JButton gridButton = new JButton();
    final JButton smoothingButton = new JButton();
    final JButton smoothingRawButton = new JButton("Raw");
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
    final JToggleButton rawShow = SegmentedControl.segment("Show");
    final JToggleButton rawHide = SegmentedControl.segment("Hide");
    /** Shown while smoothing is on: min/max stay raw although the line is smoothed. */
    final JLabel smoothingNote = muted(new JLabel("<html>Smoothing is on: cursor values follow the<br>"
            + "smoothed line, min/max are raw values.</html>"));
    final JToggleButton minMaxHover = SegmentedControl.segment("On hover");
    final JToggleButton minMaxAlways = SegmentedControl.segment("Always");
    private final JPopupMenu valuesPopup = new JPopupMenu();
    /** Shows the statistics table; the viewer panel listens to it. */
    final JToggleButton measureButton = new JToggleButton("Measure");
    final JToggleButton statsButton = new JToggleButton("Stats");
    final JLabel plotLabel = muted(new JLabel("Plot"));
    final JLabel cursorLabel = muted(new JLabel("Cursor"));
    private boolean compact;
    private int expandAtWidth;

    // grid options, shown in a popup under the Grid button
    final JPanel gridPanel = new JPanel(new GridBagLayout());
    final JCheckBox verticalBox = new JCheckBox("Vertical lines");
    final JToggleButton timeTicksMode = SegmentedControl.segment("Time ticks");
    final JToggleButton samplesMode = SegmentedControl.segment("Every sample");
    final JSpinner timeSpacing;
    final JCheckBox horizontalBox = new JCheckBox("Horizontal lines");
    final JSpinner horizontalDivisions;
    /** Default trace width in pixels; shows the plot's own width until one is chosen. */
    final JSpinner traceWidthInput = new JSpinner(new SpinnerNumberModel(2.0, TraceStyle.MIN_WIDTH,
            TraceStyle.MAX_WIDTH, TraceStyle.WIDTH_STEP));
    final JButton traceWidthPlotDefault = new JButton("Default");
    final JButton resetLinesButton = new JButton("Reset all lines");
    private boolean syncingWidth;

    private final JPopupMenu gridPopup = new JPopupMenu();

    // display smoothing options, shown in a popup under the smoothing button; changes apply at once
    final JPanel smoothingPanel = new JPanel(new GridBagLayout());
    final JToggleButton smoothingOff = SegmentedControl.segment("Raw");
    final JToggleButton smoothingOn = SegmentedControl.segment("Smoothed");
    final JSpinner smoothingInput;
    private final JPopupMenu smoothingPopup = new JPopupMenu();
    /** True while the controls are set from the settings, so that does not count as a user change. */
    private boolean syncingSmoothing;

    private final ChartSettings settings;
    private final Runnable onChange;
    private final TimelineSmoothingWindow smoothingWindow;

    ChartToolbar(ChartSettings settings, Runnable onChange) {
        this(settings, onChange, null, TimelineSmoothingWindow.unrestricted());
    }

    /** @param onOpen action of the Open button at the start of the toolbar, given the button, or null for none */
    ChartToolbar(ChartSettings settings, Runnable onChange, Consumer<Component> onOpen) {
        this(settings, onChange, onOpen, TimelineSmoothingWindow.unrestricted());
    }

    ChartToolbar(ChartSettings settings, Runnable onChange, Consumer<Component> onOpen,
                 TimelineSmoothingWindow smoothingWindow) {
        super(new WrapLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, 4), "Console.Toolbar.background", false);
        this.settings = settings;
        this.onChange = onChange;
        this.smoothingWindow = smoothingWindow;
        setBorder(BorderFactory.createEmptyBorder(4, JoinedButtons.ROW_END_INSET - JoinedButtons.ROW_GAP, 5, 0));

        ButtonGroup plotGroup = new ButtonGroup();
        plotGroup.add(lightPlot);
        plotGroup.add(darkPlot);
        (settings.isDarkPlot() ? darkPlot : lightPlot).setSelected(true);
        lightPlot.setToolTipText("Plot area with a light background");
        darkPlot.setToolTipText("Plot area with a dark background");

        gridButton.setToolTipText("Grid line and trace width options");
        gridButton.addActionListener(e -> {
            // signals' own lines change from their menus, so refresh Reset all lines before showing it
            updateControls();
            gridPopup.show(gridButton, 0, gridButton.getHeight() + 4);
        });

        smoothingButton.setToolTipText("<html>Display-only smoothing for timeline lines.<br>"
                + "Cursor values follow the smoothed line; min/max, statistics and exports stay raw.<br>"
                + "While smoothing is on, scroll the mouse wheel here to change it by "
                + WHEEL_STEP_MS + " ms.</html>");
        smoothingButton.addActionListener(e -> {
            updateControls();
            smoothingPopup.show(smoothingButton, 0, smoothingButton.getHeight() + 4);
        });
        smoothingButton.addMouseWheelListener(this::wheelSmoothing);
        smoothingInput = new JSpinner(new SpinnerNumberModel(
                smoothingWindow.initialMilliseconds(settings.getSmoothingWindowSeconds()),
                smoothingWindow.minimumMs, smoothingWindow.maximumMs, smoothingWindow.minimumMs));
        buildSmoothingPanel();
        smoothingPopup.add(smoothingPanel);
        smoothingRawButton.setToolTipText("Turn off global display smoothing and show recorded samples");
        smoothingRawButton.addActionListener(e -> setGlobalSmoothing(0));

        ButtonGroup cursorGroup = new ButtonGroup();
        cursorGroup.add(snapCursor);
        cursorGroup.add(freeCursor);
        (settings.isSnapCursor() ? snapCursor : freeCursor).setSelected(true);
        snapCursor.setToolTipText("The cursor line jumps to each logged sample");
        freeCursor.setToolTipText("<html>The cursor line follows the mouse; values come from the<br>"
                + "nearest logged sample, marked with a dot on each trace</html>");

        valuesButton.setToolTipText("How the signal values at the cursor and the min/max markers show on the plots");
        valuesButton.addActionListener(e -> {
            // per-signal smoothing changes elsewhere, so refresh the smoothing note before showing it
            updateControls();
            valuesPopup.show(valuesButton, 0, valuesButton.getHeight() + 4);
        });
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
        // only a change of the width itself sets it, so untouched it keeps following the light or dark plot
        traceWidthInput.addChangeListener(e -> {
            if (!syncingWidth) {
                settings.setDefaultLineWidth(((Number) traceWidthInput.getValue()).doubleValue());
                updateControls();
                onChange.run();
            }
        });
        traceWidthPlotDefault.addActionListener(e -> {
            settings.setDefaultLineWidth(Double.NaN);
            updateControls();
            onChange.run();
        });
        resetLinesButton.addActionListener(e -> {
            settings.resetTraceLines();
            updateControls();
            onChange.run();
        });

        if (onOpen != null) {
            openButton.addActionListener(e -> onOpen.accept(openButton));
            add(new JoinedButtons(openButton));
            add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        }
        // right after the view switch, as in the Scatter view's toolbar, so it stays put when switching views
        add(labelled(plotLabel, new SegmentedControl(lightPlot, darkPlot)));
        add(new JoinedButtons(gridButton));
        add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        add(new JoinedButtons(smoothingButton, smoothingRawButton));
        add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        add(cursorLabel);
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
        addComponentListener(new ComponentAdapter() {
            @Override
            public void componentResized(ComponentEvent event) {
                updateResponsiveLayout();
            }
        });
        updateControls();
    }

    /** Puts the Timeline / Histogram switch at the start of the toolbar, after Open. */
    void addViewSwitch(Component viewSwitch) {
        // Open and its gap come first when the toolbar has them
        int index = openButton.getParent() != null ? 2 : 0;
        add(viewSwitch, index);
        add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP), index + 1);
    }

    /** Compacts labels first; {@link WrapLayout} adds rows only when the compact controls still cannot fit. */
    void updateResponsiveLayout() {
        if (getWidth() <= 0) {
            return;
        }
        if (!compact) {
            int required = naturalRowWidth();
            if (getWidth() < required) {
                expandAtWidth = required;
                setCompact(true);
            }
        } else if (getWidth() >= expandAtWidth) {
            setCompact(false);
            int required = naturalRowWidth();
            if (getWidth() < required) {
                expandAtWidth = required;
                setCompact(true);
            }
        }
    }

    private void setCompact(boolean compact) {
        if (this.compact == compact) {
            return;
        }
        this.compact = compact;
        plotLabel.setVisible(!compact);
        cursorLabel.setVisible(!compact);
        updateControls();
        revalidate();
        repaint();
    }

    int naturalRowWidth() {
        Insets insets = getInsets();
        int width = insets.left + insets.right;
        int count = 0;
        for (Component component : getComponents()) {
            if (!component.isVisible()) {
                continue;
            }
            width += component.getPreferredSize().width;
            count++;
        }
        FlowLayout layout = (FlowLayout) getLayout();
        return width + Math.max(0, count - 1) * layout.getHgap() + layout.getHgap() * 2;
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
        GridBagConstraints noteConstraints = constraints(0, row++, 2, 0);
        noteConstraints.insets = new Insets(8, 0, 0, 0);
        gridPanel.add(note, noteConstraints);

        GridBagConstraints traceSeparator = constraints(0, row++, 2, 0);
        traceSeparator.fill = GridBagConstraints.HORIZONTAL;
        traceSeparator.insets = new Insets(10, 0, 8, 0);
        gridPanel.add(new JSeparator(), traceSeparator);
        String widthTip = "<html>Width in pixels of every signal line without a width of its own.<br>"
                + "For one signal (its own width, dashed or dotted): right-click its name, then Line…</html>";
        JLabel widthLabel = muted(new JLabel("Trace width"));
        widthLabel.setToolTipText(widthTip);
        traceWidthInput.setToolTipText(widthTip);
        ((JSpinner.DefaultEditor) traceWidthInput.getEditor()).getTextField().setColumns(3);
        traceWidthPlotDefault.setToolTipText("Back to the plot's usual width (2 px light, 1.5 px dark)");
        JPanel width = withUnit(traceWidthInput, "px");
        width.add(traceWidthPlotDefault);
        addRow(widthLabel, width, row++, 0);
        resetLinesButton.setToolTipText("<html>Every signal back to the default width and a solid line,<br>"
                + "and the default width back to the plot's usual one</html>");
        GridBagConstraints resetAt = constraints(0, row, 2, 0);
        resetAt.insets = new Insets(6, 0, 0, 0);
        gridPanel.add(resetLinesButton, resetAt);
    }

    /** Milliseconds one mouse wheel notch over the smoothing button adds or removes. */
    static final int WHEEL_STEP_MS = 100;

    /** Wheel up widens the global smoothing window, wheel down narrows it; nothing happens while it is Raw. */
    void wheelSmoothing(MouseWheelEvent event) {
        int notches = event.getWheelRotation();
        double current = settings.getSmoothingWindowSeconds();
        if (notches == 0 || current == 0) {
            return;
        }
        int milliseconds = wheelStep((int) Math.round(current * 1000), -notches, smoothingWindow);
        setGlobalSmoothing(milliseconds / 1000.0);
    }

    /**
     * The window after {@code steps} wheel notches (positive widens): it moves to the next multiple of
     * {@link #WHEEL_STEP_MS} in that direction per notch, so 150 ms goes to 200 or 100, and stays within the log's
     * limits; it never reaches Raw, which has its own button.
     */
    static int wheelStep(int currentMs, int steps, TimelineSmoothingWindow limits) {
        int milliseconds = currentMs;
        for (int i = 0; i < Math.abs(steps); i++) {
            milliseconds = steps > 0
                    ? (milliseconds / WHEEL_STEP_MS + 1) * WHEEL_STEP_MS
                    : ((milliseconds + WHEEL_STEP_MS - 1) / WHEEL_STEP_MS - 1) * WHEEL_STEP_MS;
        }
        return Math.max(limits.minimumMs, Math.min(limits.maximumMs, milliseconds));
    }

    private void buildSmoothingPanel() {
        smoothingPanel.setBorder(BorderFactory.createEmptyBorder(10, 12, 10, 12));
        ButtonGroup modeGroup = new ButtonGroup();
        modeGroup.add(smoothingOff);
        modeGroup.add(smoothingOn);
        smoothingOff.setToolTipText("Draw the recorded samples");
        smoothingOn.setToolTipText("Draw a centred moving average over the window below");
        smoothingOff.addActionListener(e -> setGlobalSmoothing(0));
        smoothingOn.addActionListener(e -> setGlobalSmoothing((Integer) smoothingInput.getValue() / 1000.0));

        JFormattedTextField field = ((JSpinner.DefaultEditor) smoothingInput.getEditor()).getTextField();
        field.setColumns(6);
        if (field.getFormatter() instanceof DefaultFormatter) {
            // apply every valid typed value, not only on Enter or focus loss
            ((DefaultFormatter) field.getFormatter()).setCommitsOnValidEdit(true);
        }
        smoothingInput.setToolTipText("Width of the moving average; the mouse wheel over the smoothing button "
                + "changes it by " + WHEEL_STEP_MS + " ms");
        smoothingInput.addChangeListener(e -> {
            if (!syncingSmoothing && smoothingOn.isSelected()) {
                setGlobalSmoothing((Integer) smoothingInput.getValue() / 1000.0);
            }
        });

        int row = 0;
        addRow(smoothingPanel, muted(new JLabel("Lines")), new SegmentedControl(smoothingOff, smoothingOn), row++, 0);
        addRow(smoothingPanel, muted(new JLabel("Window")), withUnit(smoothingInput, "ms"), row++, 0);
        if (smoothingWindow.maximumMs < Integer.MAX_VALUE) {
            GridBagConstraints rangeConstraints = constraints(0, row++, 2, 0);
            smoothingPanel.add(muted(new JLabel("Range " + smoothingWindow.minimumMs + "\u2013"
                    + smoothingWindow.maximumMs + " ms for this log")), rangeConstraints);
        }
        GridBagConstraints separator = constraints(0, row++, 2, 0);
        separator.fill = GridBagConstraints.HORIZONTAL;
        separator.insets = new Insets(8, 0, 8, 0);
        smoothingPanel.add(new JSeparator(), separator);
        smoothingPanel.add(muted(new JLabel("<html>Display only: cursor values follow the smoothed<br>"
                + "line; min/max, statistics and exports stay raw.<br>"
                + "Each signal's legend menu can override it.</html>")), constraints(0, row, 2, 0));
    }

    private void setGlobalSmoothing(double seconds) {
        if (Double.compare(settings.getSmoothingWindowSeconds(), seconds) == 0) {
            updateControls();
            return;
        }
        settings.setSmoothingWindowSeconds(seconds);
        updateControls();
        onChange.run();
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

        ButtonGroup minMaxGroup = new ButtonGroup();
        minMaxGroup.add(minMaxHover);
        minMaxGroup.add(minMaxAlways);
        (settings.isAlwaysMinMax() ? minMaxAlways : minMaxHover).setSelected(true);
        minMaxHover.setToolTipText("Min and max of the visible time on the trace you hover or pin in a legend");
        minMaxAlways.setToolTipText("<html>Min and max of the visible time on every trace;<br>"
                + "the hovered or pinned trace keeps the larger markers</html>");
        minMaxHover.addActionListener(e -> apply());
        minMaxAlways.addActionListener(e -> apply());

        ButtonGroup rawGroup = new ButtonGroup();
        rawGroup.add(rawShow);
        rawGroup.add(rawHide);
        (settings.isShowRawValues() ? rawShow : rawHide).setSelected(true);
        rawShow.setToolTipText("A smoothed trace's value is followed by the recorded one: 15.5 (12.2 raw)");
        rawHide.setToolTipText("Only the smoothed value, as on the drawn line");
        rawShow.addActionListener(e -> apply());
        rawHide.addActionListener(e -> apply());

        int row = 0;
        addRow(valuesPanel, muted(new JLabel("Show")), new SegmentedControl(valuesSegments()), row++, 0);
        addRow(valuesPanel, muted(new JLabel("At")), new SegmentedControl(atMouse, atClick), row++, 0);
        GridBagConstraints noteConstraints = constraints(0, row++, 2, 0);
        noteConstraints.insets = new Insets(8, 0, 0, 0);
        valuesPanel.add(atNote, noteConstraints);
        GridBagConstraints separator = constraints(0, row++, 2, 0);
        separator.fill = GridBagConstraints.HORIZONTAL;
        separator.insets = new Insets(8, 0, 8, 0);
        valuesPanel.add(new JSeparator(), separator);
        addRow(valuesPanel, muted(new JLabel("Min/max")), new SegmentedControl(minMaxHover, minMaxAlways), row++, 0);
        addRow(valuesPanel, muted(new JLabel("Raw values")), new SegmentedControl(rawShow, rawHide), row++, 0);
        GridBagConstraints smoothingNoteConstraints = constraints(0, row, 2, 0);
        smoothingNoteConstraints.insets = new Insets(8, 0, 0, 0);
        valuesPanel.add(smoothingNote, smoothingNoteConstraints);
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
        return labelled(muted(new JLabel(text)), control);
    }

    private static JPanel labelled(Component label, Component control) {
        JPanel pair = new JPanel(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, 0));
        pair.setOpaque(false);
        pair.add(label);
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
        settings.setAlwaysMinMax(minMaxAlways.isSelected());
        settings.setShowRawValues(rawShow.isSelected());
        settings.setVertical(verticalBox.isSelected());
        settings.setVerticalMode(samplesMode.isSelected()
                ? ChartSettings.VerticalMode.SAMPLES : ChartSettings.VerticalMode.TIME_TICKS);
        settings.setTimeSpacing((Integer) timeSpacing.getValue());
        settings.setHorizontal(horizontalBox.isSelected());
        settings.setHorizontalDivisions((Integer) horizontalDivisions.getValue());
        updateControls();
        onChange.run();
    }

    /** Refreshes the button texts and the enabled and visible states from the settings. */
    void updateControls() {
        gridButton.setText(gridSummary(settings));
        smoothingButton.setText((compact ? "Smoothing: " : "Display smoothing: ")
                + TimelineSmoothingWindow.label(settings.getSmoothingWindowSeconds()) + "  \u25BE");
        smoothingRawButton.setVisible(settings.getSmoothingWindowSeconds() > 0);
        syncingSmoothing = true;
        try {
            double windowSeconds = settings.getSmoothingWindowSeconds();
            (windowSeconds > 0 ? smoothingOn : smoothingOff).setSelected(true);
            if (windowSeconds > 0) {
                smoothingInput.setValue(smoothingWindow.initialMilliseconds(windowSeconds));
            }
            smoothingInput.setEnabled(windowSeconds > 0);
        } finally {
            syncingSmoothing = false;
        }
        valuesButton.setText(compact ? "Values  \u25BE" : valuesSummary(settings));
        boolean valuesShown = settings.getCursorValues() != ChartSettings.CursorValues.OFF;
        atMouse.setEnabled(valuesShown);
        atClick.setEnabled(valuesShown);
        atNote.setText(!valuesShown ? "<html>The legends above the plots still show<br>the values at the cursor.</html>"
                : settings.isPinValues() ? "<html>Click a plot to show the values there.<br>"
                + "Click the same spot again or press Esc to hide them.</html>"
                : "<html>Move the mouse over the plots<br>to see the values at the cursor.</html>");
        boolean smoothing = settings.isAnySmoothing();
        rawShow.setEnabled(smoothing);
        rawHide.setEnabled(smoothing);
        smoothingNote.setVisible(smoothing);
        // the notes' height changes with their text
        valuesPopup.pack();
        timeTicksMode.setEnabled(settings.isVertical());
        samplesMode.setEnabled(settings.isVertical());
        // Spacing also sets the time labels, so it stays usable while the vertical lines are off.
        timeSpacing.setEnabled(settings.getVerticalMode() == ChartSettings.VerticalMode.TIME_TICKS || !settings.isVertical());
        horizontalDivisions.setEnabled(settings.isHorizontal());
        syncingWidth = true;
        try {
            traceWidthInput.setValue((double) settings.defaultLineWidth(
                    settings.isDarkPlot() ? PlotPalette.DARK : PlotPalette.LIGHT));
        } finally {
            syncingWidth = false;
        }
        traceWidthPlotDefault.setEnabled(!Double.isNaN(settings.getDefaultLineWidth()));
        resetLinesButton.setEnabled(settings.hasCustomLines());
    }
}
