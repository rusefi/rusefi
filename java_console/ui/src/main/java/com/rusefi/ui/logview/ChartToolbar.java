package com.rusefi.ui.logview;

import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JSpinner;
import javax.swing.JToolBar;
import javax.swing.SpinnerNumberModel;
import javax.swing.Box;

/** Grid and cursor controls above the log chart. */
final class ChartToolbar extends JToolBar {
    private static final String[] VERTICAL_MODES = {"Time ticks", "Every sample"};

    final JCheckBox verticalBox = new JCheckBox("Vertical");
    final JComboBox<String> verticalMode = new JComboBox<>(VERTICAL_MODES);
    final JSpinner timeSpacing;
    final JCheckBox horizontalBox = new JCheckBox("Horizontal");
    final JSpinner horizontalDivisions;
    final JCheckBox snapCursorBox = new JCheckBox("Snap cursor to samples");
    private final ChartSettings grid;
    private final Runnable onChange;

    ChartToolbar(ChartSettings grid, Runnable onChange) {
        this.grid = grid;
        this.onChange = onChange;
        setFloatable(false);
        timeSpacing = spinner(new SpinnerNumberModel(grid.getTimeSpacing(),
                ChartSettings.MIN_TIME_SPACING, ChartSettings.MAX_TIME_SPACING, 10));
        horizontalDivisions = spinner(new SpinnerNumberModel(grid.getHorizontalDivisions(),
                ChartSettings.MIN_HORIZONTAL_DIVISIONS, ChartSettings.MAX_HORIZONTAL_DIVISIONS, 1));

        verticalBox.setSelected(grid.isVertical());
        verticalBox.setToolTipText("Show vertical grid lines");
        verticalMode.setSelectedIndex(grid.getVerticalMode().ordinal());
        verticalMode.setToolTipText("<html>Time ticks: a line at each labelled time<br>"
                + "Every sample: a line at each logged sample, shown once zoomed in far enough</html>");
        timeSpacing.setToolTipText("Approximate distance between time ticks, in pixels ("
                + ChartSettings.MIN_TIME_SPACING + "–" + ChartSettings.MAX_TIME_SPACING + ")");
        horizontalBox.setSelected(grid.isHorizontal());
        horizontalBox.setToolTipText("<html>Show horizontal grid lines<br>"
                + "Each trace has its own scale, so lines mark equal fractions of each trace's range</html>");
        horizontalDivisions.setToolTipText("Number of horizontal bands ("
                + ChartSettings.MIN_HORIZONTAL_DIVISIONS + "–" + ChartSettings.MAX_HORIZONTAL_DIVISIONS + ")");

        snapCursorBox.setSelected(grid.isSnapCursor());
        snapCursorBox.setToolTipText("<html>On: the cursor line jumps to each logged sample<br>"
                + "Off: the line follows the mouse; values come from the nearest sample, marked with a dot</html>");

        verticalBox.addActionListener(e -> apply());
        snapCursorBox.addActionListener(e -> apply());
        verticalMode.addActionListener(e -> apply());
        timeSpacing.addChangeListener(e -> apply());
        horizontalBox.addActionListener(e -> apply());
        horizontalDivisions.addChangeListener(e -> apply());

        add(new JLabel("Grid  "));
        add(verticalBox);
        add(verticalMode);
        add(Box.createHorizontalStrut(6));
        add(timeSpacing);
        add(new JLabel(" px"));
        addSeparator();
        add(horizontalBox);
        add(horizontalDivisions);
        add(new JLabel(" divisions"));
        addSeparator();
        add(snapCursorBox);
        add(Box.createHorizontalGlue());
        updateEnabled();
    }

    private static JSpinner spinner(SpinnerNumberModel model) {
        JSpinner spinner = new JSpinner(model);
        ((JSpinner.DefaultEditor) spinner.getEditor()).getTextField().setColumns(3);
        spinner.setMaximumSize(spinner.getPreferredSize());
        return spinner;
    }

    private void apply() {
        grid.setVertical(verticalBox.isSelected());
        grid.setVerticalMode(ChartSettings.VerticalMode.values()[verticalMode.getSelectedIndex()]);
        grid.setTimeSpacing((Integer) timeSpacing.getValue());
        grid.setHorizontal(horizontalBox.isSelected());
        grid.setHorizontalDivisions((Integer) horizontalDivisions.getValue());
        grid.setSnapCursor(snapCursorBox.isSelected());
        updateEnabled();
        onChange.run();
    }

    private void updateEnabled() {
        verticalMode.setEnabled(grid.isVertical());
        // Spacing also sets the time labels, so it stays usable while the vertical lines are off.
        timeSpacing.setEnabled(grid.getVerticalMode() == ChartSettings.VerticalMode.TIME_TICKS || !grid.isVertical());
        horizontalDivisions.setEnabled(grid.isHorizontal());
    }
}
