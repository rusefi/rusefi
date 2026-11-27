package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.TsStringFunction;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.PcVariableArrayIniField;
import com.rusefi.config.StringFormatter;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.ui.util.WrapLayout;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.SegmentedControl;

import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.JTableHeader;
import javax.swing.text.DefaultEditorKit;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.HierarchyBoundsAdapter;
import java.awt.event.HierarchyEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Point2D;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.function.Consumer;
import java.util.function.BiConsumer;
import java.util.function.Function;
import java.util.function.Supplier;

public class TuningTableView {
    private static final double SMOOTHING_FACTOR = 0.25;
    private static final int MIN_PREVIEW_HEIGHT = 240;
    private final boolean viewMode;
    private final Node orientationConfig;
    private final JTable table = new JTable() {
        @Override
        public boolean isCellEditable(int row, int column) {
            return getSelectedRowCount() <= 1 && getSelectedColumnCount() <= 1
                && super.isCellEditable(row, column);
        }

        @Override
        protected boolean processKeyBinding(KeyStroke stroke, KeyEvent event, int condition, boolean pressed) {
            if (pressed && !Character.isDigit(event.getKeyChar())
                && event.getKeyCode() != KeyEvent.VK_PERIOD && event.getKeyCode() != KeyEvent.VK_DECIMAL
                && event.getKeyCode() != KeyEvent.VK_COMMA
                && event.getKeyCode() != KeyEvent.VK_BACK_SPACE) {
                selectionInput = "";
            }
            return super.processKeyBinding(stroke, event, condition, pressed);
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            paintLiveMarker(g);
        }

        @Override
        public void updateUI() {
            super.updateUI();
            // Cells as tiles: no grid lines, the table background shows in the gaps.
            setShowGrid(false);
            setIntercellSpacing(new Dimension(CELL_GAP, CELL_GAP));
        }

        @Override
        public void changeSelection(int row, int column, boolean toggle, boolean extend) {
            super.changeSelection(row, column, toggle, extend);
            // JTable scrolls to the cell without its gap, which leaves the first row 1 px under the top.
            if (getAutoscrolls() && row >= 0 && column >= 0) {
                scrollRectToVisible(getCellRect(row, column, true));
            }
        }

        @Override
        public boolean getScrollableTracksViewportWidth() {
            // Columns stretch to fill a wide window, but never below their minimum widths
            // (fitColumnWidths): a narrower window scrolls the grid sideways instead of
            // squeezing every value to "...".
            return getAutoResizeMode() != AUTO_RESIZE_OFF
                && (!(getParent() instanceof JViewport) || getParent().getWidth() >= minimumGridWidth());
        }

        @Override
        public Dimension getPreferredScrollableViewportSize() {
            // [tag:table_window_fit] As tall as the rows: JTable asks for 400 px whatever its row
            // count, which left an empty band inside the grid's frame once windows fit their content.
            return new Dimension(super.getPreferredScrollableViewportSize().width, getRowCount() * getRowHeight());
        }
    };
    /** Gap between cells, where the table background shows. */
    private static final int CELL_GAP = 2;
    /** Space around a cell's text, so values do not touch the grid lines. */
    private static final int CELL_TEXT_PADDING = 8;
    /** A column never needs more room than this to stay readable. */
    private static final int MAX_COLUMN_MIN_WIDTH = 90;
    /** The live dot is a ring about as tall as the digits, so it circles a centred value rather than hiding it. */
    static final int LIVE_DOT_RADIUS = 7;
    private static final int TRAIL_DOT_RADIUS = 3;
    /** How long earlier operating points stay on the trail, fading out. */
    static final long TRAIL_MILLIS = 2000;
    static final int TRAIL_MAX_POINTS = 40;
    /** Raw {x value, y value, time} of recent live points, oldest first. */
    private final java.util.ArrayDeque<double[]> trail = new java.util.ArrayDeque<>();
    /** Live values of the displayed X and Y axis channels, after the flip option. */
    private final LiveChannelFeed live = new LiveChannelFeed(table, this::onLiveChange);
    private final Surface3DView surface3DView = new Surface3DView();
    private final Surface3DView mini3DView = new Surface3DView();
    private final JPanel gridPanel = new JPanel(new BorderLayout());
    private final JScrollPane tableScrollPane = new JScrollPane(table);
    private boolean full3D;
    private boolean previewEligible;
    private final CardLayout cardLayout = new CardLayout();
    private final JPanel tableContainer = new JPanel(cardLayout) {
        @Override
        public Dimension getPreferredSize() {
            // [tag:table_window_fit] The size of the view shown: CardLayout asks for the largest
            // card, so the grid would get the full 3D view's height.
            if (isPreferredSizeSet()) {
                return super.getPreferredSize();
            }
            for (Component card : getComponents()) {
                if (card.isVisible()) {
                    Dimension size = card.getPreferredSize();
                    Insets insets = getInsets();
                    return new Dimension(size.width + insets.left + insets.right, size.height + insets.top + insets.bottom);
                }
            }
            return super.getPreferredSize();
        }
    };
    private final JPanel content = new JPanel();
    private final String title;
    private double minValue = Double.MAX_VALUE;
    private double maxValue = -Double.MAX_VALUE;
    private ConfigurationImage imageTarget;
    private ArrayIniField zBinsField;
    private ConfigurationImage axisImageTarget;
    private ConfigurationImage yAxisImageTarget;
    private ArrayIniField xBinsField;
    private ArrayIniField yBinsField;
    private final JTextField xAxisEditor = new JTextField();
    private int xAxisEditingColumn = -1;
    private Runnable onEdit;
    private Runnable onOrientationChange = () -> { };
    private String selectionInput = "";
    /** Whether the wheel edits the grid rather than scrolling the page; see {@link TableMouse}. */
    private boolean wheelEditing;
    private boolean applyingSelectionInput;
    private IniFileModel currentIni;
    private String currentTableName;
    private Node orientationNode;
    private boolean flipAxes;
    private boolean invertX;
    private boolean invertY;
    /** [tag:unburned_marks] The last burned tune; null marks nothing. */
    private Supplier<ConfigurationImage> burnedImage = () -> null;
    private Supplier<UnburnedWording> unburnedWording = () -> UnburnedWording.BURNED;
    private final JLabel unburnedCountLabel = UnburnedCellMark.createCountLabel();
    private final JToggleButton tableSegment = SegmentedControl.segment("Table");
    private final JToggleButton surfaceSegment = SegmentedControl.segment("3D");
    /**
     * The grid with a small 3D preview below it. Tables open on "Table": the preview made table
     * windows tall enough to hide the gauges. Choosing "Table" or "Both" is one choice for every
     * table, kept with the orientation settings; "3D" is a look and is not remembered.
     * [tag:table_window_fit] Detached windows fit their height to the content without the preview.
     */
    private final JToggleButton bothSegment = SegmentedControl.segment("Both");
    static final String PREVIEW_PROPERTY = "mini_3d_preview";
    private boolean previewOn;
    /** Grid editing buttons; disabled while the 3D view replaces the grid. */
    private final java.util.List<Component> editControls = new java.util.ArrayList<>();

    public TuningTableView(String title) {
        this(title, false);
    }

    public TuningTableView(String title, boolean viewMode) {
        this(title, viewMode, new Node());
    }

    public TuningTableView(String title, boolean viewMode, Node orientationConfig) {
        this.title = title;
        this.viewMode = viewMode;
        this.orientationConfig = orientationConfig;
        table.setTableHeader(new JTableHeader(table.getColumnModel()) {
            @Override
            public void doLayout() {
                super.doLayout();
                positionXAxisEditor();
            }
        });
        table.getTableHeader().setReorderingAllowed(false);
        table.setDefaultRenderer(Object.class, new GradientRenderer());
        table.setCellSelectionEnabled(true);
        table.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
        table.setToolTipText(viewMode
            ? "Arrows or Ctrl+mouse wheel to move; Shift+arrows to select; Ctrl+A to select all"
            : "Type digits to replace; F2 to edit text; Enter to accept; Esc to cancel; Tab to accept and move; "
                + "+/- to adjust by 1; wheel to adjust; Shift+wheel for 3 steps; Ctrl+wheel to move");
        installAxisHighlighting();
        installKeyboardEditing();
        installXAxisEditing();
        table.getTableHeader().setToolTipText(viewMode ? null : "Double-click an axis value to edit");
        table.getTableHeader().addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (e.getButton() != MouseEvent.BUTTON1 || e.getClickCount() != 2) {
                    return;
                }
                int column = table.getTableHeader().columnAtPoint(e.getPoint());
                startXAxisEdit(column);
            }
        });
        table.getTableHeader().addMouseListener(new MouseAdapter() {
            private void showOrientationMenu(MouseEvent event) {
                if (event.isPopupTrigger()) {
                    JPopupMenu menu = orientationMenuFor(table.getTableHeader().columnAtPoint(event.getPoint()));
                    if (menu != null) {
                        menu.show(table.getTableHeader(), event.getX(), event.getY());
                    }
                }
            }

            @Override
            public void mousePressed(MouseEvent event) {
                showOrientationMenu(event);
            }

            @Override
            public void mouseReleased(MouseEvent event) {
                showOrientationMenu(event);
            }
        });
        table.addMouseWheelListener(this::handleTableWheel);
        table.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent e) {
                // The cell editor is part of the table; focus anywhere else ends wheel editing.
                Component opposite = e.getOppositeComponent();
                if (!e.isTemporary() && (opposite == null || !SwingUtilities.isDescendingFrom(opposite, table))) {
                    wheelEditing = false;
                }
            }
        });

        table.addMouseListener(new TableMouse());

        mini3DView.setPreferredSize(new Dimension(320, 160));
        mini3DView.setToolTipText("3D preview - drag to rotate; click 3D above for a larger view");
        mini3DView.setVisible(false);
        gridPanel.add(tableScrollPane, BorderLayout.CENTER);
        gridPanel.add(mini3DView, BorderLayout.SOUTH);
        tableContainer.add(gridPanel, "table");
        tableContainer.add(surface3DView, "3d");

        // One switch for the three views rather than a checkbox per view
        tableSegment.setToolTipText("Edit the values in a grid");
        bothSegment.setToolTipText("Edit the values in a grid, with a small 3D preview below it");
        surfaceSegment.setToolTipText("Show the table as a large 3D surface - drag to rotate");
        ButtonGroup viewGroup = new ButtonGroup();
        viewGroup.add(tableSegment);
        viewGroup.add(bothSegment);
        viewGroup.add(surfaceSegment);
        previewOn = orientationConfig.getBoolProperty(PREVIEW_PROPERTY);
        (previewOn ? bothSegment : tableSegment).setSelected(true);
        tableSegment.addActionListener(e -> showGrid(false));
        bothSegment.addActionListener(e -> showGrid(true));
        surfaceSegment.addActionListener(e -> show3D(true));
        SegmentedControl viewSwitch = new SegmentedControl(tableSegment, bothSegment, surfaceSegment);
        viewSwitch.setMaximumSize(viewSwitch.getPreferredSize());

        JTextField deltaField = new JTextField("0.5", 5);
        deltaField.setToolTipText("Amount added or subtracted by the Up and Down buttons");
        deltaField.setHorizontalAlignment(JTextField.RIGHT);
        // as tall as the joined buttons beside it, so the row lines up
        deltaField.setPreferredSize(new Dimension(deltaField.getPreferredSize().width, JoinedButtons.HEIGHT));
        JButton upButton = new JButton("Up");
        JButton downButton = new JButton("Down");
        JButton equalsButton = new JButton("=");
        upButton.setToolTipText("Increase selected values by delta");
        downButton.setToolTipText("Decrease selected values by delta");
        equalsButton.setToolTipText("Set selected cells to a value");
        Action horizontalAction = new AbstractAction("H") {
            @Override
            public void actionPerformed(ActionEvent e) {
                interpolateHorizontal();
            }
        };
        Action verticalAction = new AbstractAction("V") {
            @Override
            public void actionPerformed(ActionEvent e) {
                interpolateVertical();
            }
        };
        Action interpolateAction = new AbstractAction("Interpolate") {
            @Override
            public void actionPerformed(ActionEvent e) {
                interpolateSelection();
            }
        };
        Action smoothAction = new AbstractAction("Smooth") {
            @Override
            public void actionPerformed(ActionEvent e) {
                smoothSelection();
            }
        };
        JButton horizontalButton = new JButton(horizontalAction);
        JButton verticalButton = new JButton(verticalAction);
        JButton interpolateButton = new JButton(interpolateAction);
        JButton smoothButton = new JButton(smoothAction);

        upButton.addActionListener(e -> applyDelta(deltaField, 1));
        downButton.addActionListener(e -> applyDelta(deltaField, -1));
        equalsButton.addActionListener(e -> showSetDialog());
        horizontalButton.setToolTipText("Interpolate Horizontal - Key: H");
        verticalButton.setToolTipText("Interpolate Vertical - Key: V");
        interpolateButton.setToolTipText("Interpolate selected cells - Key: /");
        smoothButton.setToolTipText("Smooth selected cells - Key: S");

        JLabel deltaLabel = new JLabel("Delta");
        editControls.add(deltaLabel);
        editControls.add(deltaField);
        editControls.add(upButton);
        editControls.add(downButton);
        editControls.add(equalsButton);
        editControls.add(horizontalButton);
        editControls.add(verticalButton);
        editControls.add(interpolateButton);
        editControls.add(smoothButton);

        // Wraps onto further lines in a narrow window rather than squeezing or cutting off controls.
        JPanel topPanel = new JPanel(new WrapLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, 4));
        topPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
        topPanel.setBorder(BorderFactory.createEmptyBorder(0, 2, 0, 2));
        topPanel.add(new JLabel(title));
        topPanel.add(viewSwitch);
        topPanel.add(unburnedCountLabel);

        if (!viewMode) {
            // The label and its field wrap together.
            JPanel delta = new JPanel(new FlowLayout(FlowLayout.LEFT, 4, 0));
            delta.add(deltaLabel);
            delta.add(deltaField);
            topPanel.add(delta);
            // Related actions share one outlined control, like the other console toolbars.
            topPanel.add(new JoinedButtons(upButton, downButton, equalsButton));
            topPanel.add(new JoinedButtons(horizontalButton, verticalButton));
            topPanel.add(new JoinedButtons(interpolateButton, smoothButton));

            InputMap inputMap = content.getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT);
            inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_H, 0), "interpolateHorizontal");
            inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_V, 0), "interpolateVertical");
            inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_SLASH, 0), "interpolateSelection");
            inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_S, 0), "smoothSelection");
            content.getActionMap().put("interpolateHorizontal", tableShortcut(horizontalAction));
            content.getActionMap().put("interpolateVertical", tableShortcut(verticalAction));
            content.getActionMap().put("interpolateSelection", tableShortcut(interpolateAction));
            content.getActionMap().put("smoothSelection", tableShortcut(smoothAction));
        }

        content.setLayout(new BoxLayout(content, BoxLayout.Y_AXIS));
        content.add(topPanel);
        content.add(tableContainer);
        tableContainer.addComponentListener(new ComponentAdapter() {
            @Override
            public void componentResized(ComponentEvent e) {
                updatePreviewVisibility();
            }
        });
        tableContainer.addHierarchyBoundsListener(new HierarchyBoundsAdapter() {
            @Override
            public void ancestorResized(HierarchyEvent e) {
                // Ancestor layout may not yet have updated the viewport's extent.
                SwingUtilities.invokeLater(() -> updatePreviewVisibility());
            }
        });
        tableContainer.addHierarchyListener(e -> {
            if ((e.getChangeFlags() & (HierarchyEvent.PARENT_CHANGED | HierarchyEvent.SHOWING_CHANGED)) != 0) {
                SwingUtilities.invokeLater(() -> updatePreviewVisibility());
            }
        });
    }

    LiveChannelFeed liveFeedForUnitTest() {
        return live;
    }

    /**
     * Records the live values for the trail: a point that moves adds one, a point that stays only
     * refreshes its time, and points older than {@link #TRAIL_MILLIS} drop off. Values are kept
     * raw, so the trail follows axis edits; no data, or a new orientation, clears it.
     */
    private void onLiveChange() {
        double x = live.value(0);
        double y = live.value(1);
        if (Double.isNaN(x) || Double.isNaN(y)) {
            trail.clear();
        } else {
            long now = live.nowMillis();
            double[] last = trail.peekLast();
            if (last != null && last[0] == x && last[1] == y) {
                last[2] = now;
            } else {
                trail.addLast(new double[]{x, y, now});
            }
            while (!trail.isEmpty() && (trail.size() > TRAIL_MAX_POINTS || now - trail.peekFirst()[2] > TRAIL_MILLIS)) {
                trail.pollFirst();
            }
        }
        table.repaint();
        updateSurfaceLive();
    }

    /**
     * The trail, oldest first and ending at the current point, as {x index, y index, opacity}:
     * fractional data-cell indexes into the displayed axes and an opacity fading with age.
     */
    java.util.List<double[]> liveTrail() {
        java.util.List<double[]> points = new java.util.ArrayList<>();
        if (!(table.getModel() instanceof TuningTableModel)) {
            return points;
        }
        TuningTableModel model = (TuningTableModel) table.getModel();
        if (model.data.length == 0 || model.data[0].length == 0) {
            return points;
        }
        long now = live.nowMillis();
        for (double[] entry : trail) {
            Double x = LiveOperatingPoint.axisPosition(model.xBins, model.data[0].length, entry[0]);
            Double y = LiveOperatingPoint.axisPosition(model.yBins, model.data.length, entry[1]);
            if (x != null && y != null) {
                points.add(new double[]{x, y, Math.max(0, 1 - (now - entry[2]) / (double) TRAIL_MILLIS)});
            }
        }
        return points;
    }

    private void updateSurfaceLive() {
        java.util.List<double[]> points = liveTrail();
        double[][] surfaceTrail = points.isEmpty() ? null : points.toArray(new double[0][]);
        surface3DView.setLiveTrail(surfaceTrail);
        mini3DView.setLiveTrail(surfaceTrail);
    }

    /** Pixel centre of fractional data-cell indexes; rows show the Y bins bottom-up. */
    private Point2D.Double cellPixel(double xIndex, double yIndex) {
        int rows = ((TuningTableModel) table.getModel()).data.length;
        return new Point2D.Double(
            interpolateCentre(xIndex, index -> table.getCellRect(0, index + 1, true).getCenterX()),
            interpolateCentre(yIndex, index -> table.getCellRect(rows - 1 - index, 1, true).getCenterY()));
    }

    /** Earlier live points as fading dots joined by lines, under the current marker. */
    private void paintTrail(Graphics2D g2) {
        java.util.List<double[]> points = liveTrail();
        Point2D.Double previous = null;
        for (int i = 0; i < points.size(); i++) {
            double[] point = points.get(i);
            Point2D.Double pixel = cellPixel(point[0], point[1]);
            int alpha = (int) Math.round(255 * point[2]);
            if (previous != null && alpha > 0) {
                g2.setStroke(new BasicStroke(3f));
                g2.setColor(new Color(0, 0, 0, alpha)); // [tag:fixed_color] trail edge over the value gradient
                g2.drawLine((int) previous.x, (int) previous.y, (int) pixel.x, (int) pixel.y);
                g2.setStroke(new BasicStroke(1f));
                g2.setColor(new Color(255, 255, 255, alpha)); // [tag:fixed_color] trail core over the value gradient
                g2.drawLine((int) previous.x, (int) previous.y, (int) pixel.x, (int) pixel.y);
            }
            if (i < points.size() - 1 && alpha > 0) {
                Ellipse2D dot = new Ellipse2D.Double(pixel.x - TRAIL_DOT_RADIUS, pixel.y - TRAIL_DOT_RADIUS,
                    2 * TRAIL_DOT_RADIUS, 2 * TRAIL_DOT_RADIUS);
                g2.setColor(new Color(255, 255, 255, alpha)); // [tag:fixed_color] trail dot over the value gradient
                g2.fill(dot);
                g2.setColor(new Color(0, 0, 0, alpha)); // [tag:fixed_color] trail dot edge over the value gradient
                g2.draw(dot);
            }
            previous = pixel;
        }
        g2.setStroke(new BasicStroke(1f));
    }

    /**
     * The live operating point as fractional data-cell indexes into the displayed axes
     * (x along xBins, y along yBins), or null when there is nothing to show.
     */
    private Point2D.Double livePosition() {
        if (!(table.getModel() instanceof TuningTableModel)) {
            return null;
        }
        TuningTableModel model = (TuningTableModel) table.getModel();
        if (model.data.length == 0 || model.data[0].length == 0) {
            return null;
        }
        Double x = LiveOperatingPoint.axisPosition(model.xBins, model.data[0].length, live.value(0));
        Double y = LiveOperatingPoint.axisPosition(model.yBins, model.data.length, live.value(1));
        return x == null || y == null ? null : new Point2D.Double(x, y);
    }

    /** The table cell (row, column) nearest the live operating point, or null when none is shown. */
    Point liveCell() {
        Point2D.Double position = livePosition();
        if (position == null) {
            return null;
        }
        int rows = ((TuningTableModel) table.getModel()).data.length;
        return new Point(rows - 1 - (int) Math.round(position.y), (int) Math.round(position.x) + 1);
    }

    /**
     * Outlines the cell nearest the live operating point and puts a dot where the firmware
     * interpolates between the surrounding cells. Rows show the Y bins bottom-up.
     */
    private void paintLiveMarker(Graphics g) {
        Point cell = liveCell();
        if (cell == null) {
            return;
        }
        Point2D.Double position = livePosition();
        Point2D.Double centre = cellPixel(position.x, position.y);
        double x = centre.x;
        double y = centre.y;
        Rectangle bounds = table.getCellRect(cell.x, cell.y, true);
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            Graphics2D trailGraphics = (Graphics2D) g2.create();
            trailGraphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            paintTrail(trailGraphics);
            trailGraphics.dispose();
            // The marker looks the same in every theme: black-white-black rings stay visible on the
            // light cells of the light theme and the dark cells of the dark theme alike.
            g2.setColor(Color.BLACK); // [tag:fixed_color] live cell outline over the value gradient
            g2.drawRect(bounds.x, bounds.y, bounds.width - 1, bounds.height - 1);
            g2.drawRect(bounds.x + 2, bounds.y + 2, bounds.width - 5, bounds.height - 5);
            Color middleRing = Color.WHITE; // [tag:fixed_color] middle ring and dot core of the live marker
            g2.setColor(middleRing);
            g2.drawRect(bounds.x + 1, bounds.y + 1, bounds.width - 3, bounds.height - 3);
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            // A hollow dot, with the value redrawn over it, so the number under the live point stays readable.
            Ellipse2D dot = new Ellipse2D.Double(x - LIVE_DOT_RADIUS, y - LIVE_DOT_RADIUS,
                2 * LIVE_DOT_RADIUS, 2 * LIVE_DOT_RADIUS);
            g2.setStroke(new BasicStroke(4f));
            g2.setColor(Color.BLACK); // [tag:fixed_color] live point dot outline over the value gradient
            g2.draw(dot);
            g2.setStroke(new BasicStroke(2f));
            g2.setColor(middleRing);
            g2.draw(dot);
            paintCellTextWithHalo(g2, cell.x, cell.y);
        } finally {
            g2.dispose();
        }
    }

    /**
     * Draws the cell's value again, where its renderer put it, with a one-pixel halo in the opposite
     * text colour, so the live dot and trail under it do not hide the number.
     */
    private void paintCellTextWithHalo(Graphics2D g2, int row, int column) {
        Component rendered = table.prepareRenderer(table.getCellRenderer(row, column), row, column);
        if (!(rendered instanceof JLabel)) {
            return;
        }
        JLabel label = (JLabel) rendered;
        String text = label.getText();
        if (text == null || text.isEmpty()) {
            return;
        }
        Rectangle cell = table.getCellRect(row, column, false);
        Insets insets = label.getInsets();
        Rectangle view = new Rectangle(cell.x + insets.left, cell.y + insets.top,
            cell.width - insets.left - insets.right, cell.height - insets.top - insets.bottom);
        FontMetrics metrics = label.getFontMetrics(label.getFont());
        Rectangle textBounds = new Rectangle();
        String shown = SwingUtilities.layoutCompoundLabel(label, metrics, text, null,
            label.getVerticalAlignment(), label.getHorizontalAlignment(),
            label.getVerticalTextPosition(), label.getHorizontalTextPosition(),
            view, new Rectangle(), textBounds, 0);
        Graphics2D textGraphics = (Graphics2D) g2.create();
        try {
            Object desktopHints = Toolkit.getDefaultToolkit().getDesktopProperty("awt.font.desktophints");
            if (desktopHints instanceof java.util.Map) {
                textGraphics.addRenderingHints((java.util.Map<?, ?>) desktopHints);
            }
            textGraphics.setFont(label.getFont());
            int baseline = textBounds.y + metrics.getAscent();
            textGraphics.setColor(CalibrationColorPalette.foreground(label.getForeground()));
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx != 0 || dy != 0) {
                        textGraphics.drawString(shown, textBounds.x + dx, baseline + dy);
                    }
                }
            }
            textGraphics.setColor(label.getForeground());
            textGraphics.drawString(shown, textBounds.x, baseline);
        } finally {
            textGraphics.dispose();
        }
    }

    private static double interpolateCentre(double position, java.util.function.IntToDoubleFunction centre) {
        int from = (int) Math.floor(position);
        double fraction = position - from;
        double start = centre.applyAsDouble(from);
        return fraction == 0 ? start : start + fraction * (centre.applyAsDouble(from + 1) - start);
    }

    JPopupMenu orientationMenuFor(int column) {
        if (column != 0 || orientationNode == null) {
            return null;
        }
        JPopupMenu menu = new JPopupMenu();
        addOrientationOption(menu, "Flip X/Y axes", "flip_xy", flipAxes);
        addOrientationOption(menu, "Invert X axis", "invert_x", invertX);
        addOrientationOption(menu, "Invert Y axis", "invert_y", invertY);
        return menu;
    }

    private void addOrientationOption(JPopupMenu menu, String label, String key, boolean selected) {
        JCheckBoxMenuItem option = new JCheckBoxMenuItem(label, selected);
        option.addActionListener(event -> {
            if (!finishXAxisEdit(true) || (table.isEditing() && !table.getCellEditor().stopCellEditing())) {
                option.setSelected(!option.isSelected());
                return;
            }
            orientationNode.setBoolProperty(key, option.isSelected());
            // Convert the current displayed targets back to source X/Y before applying the new orientation.
            ConfigurationImage sourceX = flipAxes ? yAxisImageTarget : axisImageTarget;
            ConfigurationImage sourceY = flipAxes ? axisImageTarget : yAxisImageTarget;
            displayTable(currentIni, currentTableName, imageTarget, sourceX, sourceY);
            onOrientationChange.run();
        });
        menu.add(option);
    }

    private void updatePreviewVisibility() {
        // Keep the grid usable in small dialogs. Large tables can scroll above the preview.
        Insets border = tableScrollPane.getInsets();
        int gridHeight = Math.min(320, table.getRowCount() * table.getRowHeight())
            + table.getTableHeader().getPreferredSize().height + border.top + border.bottom
            + (table.getScrollableTracksViewportWidth() ? 0
                : tableScrollPane.getHorizontalScrollBar().getPreferredSize().height);
        int availableHeight = tableContainer.getHeight();
        JViewport viewport = (JViewport) SwingUtilities.getAncestorOfClass(JViewport.class, tableContainer);
        if (viewport != null) {
            // Scrollable dialogs retain preferred height instead of filling a tall viewport.
            // Measure from the table's top in the dialog, independent of scrolling position.
            Point top = SwingUtilities.convertPoint(tableContainer, 0, 0, viewport.getView());
            availableHeight = viewport.getExtentSize().height - Math.max(0, top.y);
        }
        // A scrollable dialog keeps a short preview and scrolls to it; elsewhere it hides when it
        // does not fit below the table.
        boolean fits = viewport != null || availableHeight >= gridHeight + MIN_PREVIEW_HEIGHT;
        boolean visible = previewOn && previewEligible && !full3D
            && tableContainer.getWidth() >= 320 && fits;
        boolean sizeChanged = false;
        if (visible) {
            int previewHeight = Math.max(MIN_PREVIEW_HEIGHT, availableHeight - gridHeight);
            if (mini3DView.getPreferredSize().height != previewHeight
                || tableScrollPane.getPreferredSize().height != gridHeight) {
                mini3DView.setPreferredSize(new Dimension(320, previewHeight));
                tableScrollPane.setPreferredSize(new Dimension(
                    tableScrollPane.getPreferredSize().width, gridHeight));
                sizeChanged = true;
            }
        } else if (tableScrollPane.isPreferredSizeSet()) {
            tableScrollPane.setPreferredSize(null);
            sizeChanged = true;
        }
        if (mini3DView.isVisible() != visible) {
            mini3DView.setVisible(visible);
            sizeChanged = true;
        }
        if (sizeChanged) {
            gridPanel.revalidate();
            gridPanel.repaint();
        }
    }

    private void updateSurfaces(Double[][] data, Double[] xBins, Double[] yBins) {
        surface3DView.setData(data, xBins, yBins, minValue, maxValue);
        mini3DView.setData(data, xBins, yBins, minValue, maxValue);
        updateSurfaceLive();
        previewEligible = data.length >= 2 && data[0].length >= 2
            && Surface3DView.hasUsableAxis(xBins, data[0].length)
            && Surface3DView.hasUsableAxis(yBins, data.length);
        updatePreviewVisibility();
    }

    private void installAxisHighlighting() {
        TableCellRenderer headerRenderer = table.getTableHeader().getDefaultRenderer();
        table.getTableHeader().setDefaultRenderer((source, value, selected, focused, row, column) -> {
            Component renderer = headerRenderer.getTableCellRendererComponent(source, value, selected, focused, row, column);
            Color background = CalibrationColorPalette.axisBackground();
            renderer.setBackground(isActiveValueCell() && column == activeColumn()
                ? axisHighlight(background) : background);
            renderer.setForeground(CalibrationColorPalette.axisForeground());
            renderer.setFont(axisFont(table.getTableHeader().getFont()));
            UnburnedCellMark.apply(renderer, String.valueOf(value), burnedXAxisText(column), unburnedWording.get());
            return renderer;
        });
        table.getSelectionModel().addListSelectionListener(e -> repaintAxes());
        table.getColumnModel().getSelectionModel().addListSelectionListener(e -> repaintAxes());
    }

    private void repaintAxes() {
        table.repaint();
        table.getTableHeader().repaint();
        updateSurfaceSelection();
    }

    /** Shows the grid's selected cells on both 3D views; table rows are shown top-down. */
    private void updateSurfaceSelection() {
        if (!(table.getModel() instanceof TuningTableModel)) {
            return;
        }
        Double[][] data = ((TuningTableModel) table.getModel()).data;
        boolean[][] selection = null;
        for (int row : table.getSelectedRows()) {
            for (int column : table.getSelectedColumns()) {
                if (column > 0 && row < data.length && column - 1 < data[0].length
                    && table.isCellSelected(row, column)) {
                    if (selection == null) {
                        selection = new boolean[data.length][data[0].length];
                    }
                    selection[data.length - 1 - row][column - 1] = true;
                }
            }
        }
        surface3DView.setSelection(selection);
        mini3DView.setSelection(selection);
    }

    /**
     * Turns an INI {@code xyLabels} entry into display text, or null. Labels can be expressions that
     * name the selected load source, e.g. {@code {bitStringValue(veAxisLabels, veLoadSrc)}}; their
     * settings are read from whichever of the table's images holds that settings page.
     */
    private static Function<String, String> axisLabelResolver(IniFileModel ini, TableModel iniTable,
                                                              ConfigurationImage zImage,
                                                              ConfigurationImage xAxisImage,
                                                              ConfigurationImage yAxisImage) {
        Map<Integer, ConfigurationImage> imageByPage = new HashMap<>();
        putPageImage(imageByPage, ini, iniTable.getXBinsConstant(), xAxisImage);
        putPageImage(imageByPage, ini, iniTable.getYBinsConstant(), yAxisImage);
        putPageImage(imageByPage, ini, iniTable.getZBinsConstant(), zImage);
        Function<IniField, Double> settingValue = field -> {
            ConfigurationImage image = imageByPage.get(field.getPageIndex());
            boolean inside = image != null && field.getOffset() >= 0
                && field.getOffset() + field.getSize() <= image.getSize();
            return inside ? image.readNumericValue(field) : null;
        };
        ConfigurationImage mainPage = imageByPage.getOrDefault(0, zImage);
        return label -> {
            if (label == null || !TsStringFunction.containsStringFunction(label)) {
                return plainLabel(label);
            }
            String resolved = TsStringFunction.resolveDisplayLabelWithVariables(label, ini, mainPage,
                expression -> ExpressionEvaluator.resolveVariables(expression, ini, settingValue));
            return plainLabel(resolved);
        };
    }

    private static void putPageImage(Map<Integer, ConfigurationImage> imageByPage, IniFileModel ini,
                                     String fieldName, ConfigurationImage image) {
        if (fieldName == null || image == null) {
            return;
        }
        ini.findIniField(fieldName).ifPresent(field -> imageByPage.putIfAbsent(field.getPageIndex(), image));
    }

    /**
     * Corner header names, "row axis \ column axis", so tuners can tell which axis is which: the
     * resolved INI label; without one, the axis output channel, then the bins constant.
     */
    private static String axisName(String text, String channel, String binsConstant) {
        if (text != null) {
            return text;
        }
        if (channel != null && !channel.trim().isEmpty()) {
            return channel.trim();
        }
        return binsConstant == null ? "" : binsConstant;
    }

    /** INI axis labels and units can be expressions; only plain text is shown. */
    private static String plainLabel(String label) {
        if (label == null) {
            return null;
        }
        String text = label.trim();
        if (text.length() >= 2 && text.startsWith("\"") && text.endsWith("\"")) {
            text = text.substring(1, text.length() - 1).trim();
        }
        return text.isEmpty() || text.contains("{") || text.contains("@") ? null : text;
    }

    private int activeRow() {
        return table.getSelectionModel().getLeadSelectionIndex();
    }

    private int activeColumn() {
        return table.getColumnModel().getSelectionModel().getLeadSelectionIndex();
    }

    private boolean isActiveValueCell() {
        return activeRow() >= 0 && activeRow() < table.getRowCount()
            && activeColumn() > 0 && activeColumn() < table.getColumnCount()
            && table.isCellSelected(activeRow(), activeColumn());
    }

    private Color axisHighlight(Color background) {
        return CalibrationColorPalette.mix(background, CalibrationColorPalette.selection(), 0.25);
    }

    private Font axisFontBase;
    private Font axisFont;

    /** Axis values are bold, so they read as headers rather than as table values. */
    private Font axisFont(Font base) {
        if (base != axisFontBase) {
            axisFontBase = base;
            axisFont = base == null ? null : base.deriveFont(Font.BOLD);
        }
        return axisFont;
    }

    private void installKeyboardEditing() {
        // Only numeric entry starts replacement editing; letter shortcuts keep their actions.
        table.putClientProperty("JTable.autoStartsEdit", false);
        table.setFocusTraversalKeysEnabled(false);
        JTextField field = new JTextField();
        field.setHorizontalAlignment(SwingConstants.CENTER);
        DefaultCellEditor editor = new DefaultCellEditor(field) {
            @Override
            public boolean stopCellEditing() {
                if (!validateNumericEditor(field)) {
                    return false;
                }
                return super.stopCellEditing();
            }

            @Override
            public Component getTableCellEditorComponent(JTable source, Object value, boolean selected, int row, int column) {
                Component component = super.getTableCellEditorComponent(source, value, selected, row, column);
                field.setBorder(UIManager.getBorder("TextField.border"));
                field.setToolTipText("Enter to accept; Esc to cancel; Tab/Shift+Tab to accept and move; "
                    + "F2 for text-cursor navigation");
                field.selectAll();
                return component;
            }
        };
        table.setDefaultEditor(Object.class, editor);
        Action accept = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                selectionInput = "";
                if (finishCellEdit(false)) {
                    table.requestFocusInWindow();
                }
            }
        };
        for (String key : new String[]{"ENTER", "shift ENTER"}) {
            bindKey(table, JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT, key, "acceptValue", accept);
            bindKey(field, JComponent.WHEN_FOCUSED, key, "acceptValue", accept);
        }
        bindKey(table, JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT, "F2", "editValue", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                selectionInput = "";
                startCellEdit(true);
            }
        });
        Action cancel = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                selectionInput = "";
                if (table.isEditing()) {
                    table.getCellEditor().cancelCellEditing();
                }
                table.requestFocusInWindow();
            }
        };
        bindKey(table, JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT, "ESCAPE", "cancelValue", cancel);
        bindKey(field, JComponent.WHEN_FOCUSED, "ESCAPE", "cancelValue", cancel);
        for (char digit = '0'; digit <= '9'; digit++) {
            bindNumericEntry(digit);
        }
        bindNumericEntry('.');
        bindNumericEntry(',');
        bindValueAdjustment('+', 1);
        bindValueAdjustment('-', -1);
        for (boolean backwards : new boolean[]{false, true}) {
            String key = backwards ? "BACK_SPACE" : "DELETE";
            bindKey(table, JComponent.WHEN_FOCUSED, key, key, new AbstractAction() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    deleteFromCell(backwards);
                }
            });
        }
        FocusAdapter finishOnFocusLost = new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent e) {
                // Replacement entry keeps focus on the grid; F2 transfers it into the editor.
                if (e.getOppositeComponent() != table.getEditorComponent()) {
                    finishCellEdit(true);
                    selectionInput = "";
                }
            }
        };
        table.addFocusListener(finishOnFocusLost);
        field.addFocusListener(finishOnFocusLost);
        table.getSelectionModel().addListSelectionListener(e -> resetSelectionInput());
        table.getColumnModel().getSelectionModel().addListSelectionListener(e -> resetSelectionInput());
        field.setFocusTraversalKeysEnabled(false);
        for (boolean backwards : new boolean[]{false, true}) {
            String key = backwards ? "shift TAB" : "TAB";
            Action move = new AbstractAction() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    moveValueCell(backwards);
                }
            };
            bindKey(table, JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT, key, key, move);
            bindKey(field, JComponent.WHEN_FOCUSED, key, key, move);
            String focusKey = backwards ? "ctrl shift TAB" : "ctrl TAB";
            Action leave = new AbstractAction() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    finishCellEdit(true);
                    selectionInput = "";
                    if (backwards) {
                        table.transferFocusBackward();
                    } else {
                        table.transferFocus();
                    }
                }
            };
            bindKey(table, JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT, focusKey, focusKey, leave);
            bindKey(field, JComponent.WHEN_FOCUSED, focusKey, focusKey, leave);
        }
        installCellNavigation(field);
    }

    private void resetSelectionInput() {
        if (!applyingSelectionInput) {
            selectionInput = "";
        }
    }

    private boolean startCellEdit(boolean focusEditor) {
        if (viewMode || !finishXAxisEdit(true)) {
            return false;
        }
        if (table.getSelectedRow() < 0 && table.getRowCount() > 0 && table.getColumnCount() > 1) {
            table.changeSelection(0, 1, false, false);
        }
        if (!table.isEditing() && (activeRow() < 0 || activeColumn() < 0
            || !table.isCellSelected(activeRow(), activeColumn())
            || !table.editCellAt(activeRow(), activeColumn()))) {
            return false;
        }
        if (focusEditor) {
            table.getEditorComponent().requestFocusInWindow();
        }
        return true;
    }

    private boolean finishCellEdit(boolean cancelInvalid) {
        if (table.isEditing() && !table.getCellEditor().stopCellEditing()) {
            if (!cancelInvalid) {
                return false;
            }
            table.getCellEditor().cancelCellEditing();
        }
        return true;
    }

    private void bindNumericEntry(char character) {
        String name = "typeValue" + character;
        table.getInputMap(JComponent.WHEN_FOCUSED).put(KeyStroke.getKeyStroke(character), name);
        table.getActionMap().put(name, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (viewMode || !(table.getModel() instanceof TuningTableModel)) {
                    return;
                }
                if (table.isEditing()) {
                    ((JTextField) table.getEditorComponent()).replaceSelection(Character.toString(character));
                } else if (DecimalInput.isSeparator(character) && selectionInput.isEmpty()) {
                    adjustSelectedValuesByPrecision(1);
                } else if (table.getSelectedRowCount() * table.getSelectedColumnCount() > 1) {
                    if (DecimalInput.isSeparator(character) && (selectionInput.contains(".")
                        || ((TuningTableModel) table.getModel()).precision == 0)) {
                        return;
                    }
                    // the typed digits are kept with "." whichever separator was typed
                    applySelectionInput(selectionInput + (DecimalInput.isSeparator(character) ? '.' : character));
                } else if (startCellEdit(false)) {
                    ((JTextField) table.getEditorComponent()).setText(Character.toString(character));
                }
            }
        });
    }

    private void bindValueAdjustment(char character, double delta) {
        Action action = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (viewMode || xAxisEditingColumn >= 0) {
                    return;
                }
                if (table.isEditing()) {
                    ((JTextField) table.getEditorComponent()).replaceSelection(Character.toString(character));
                } else {
                    selectionInput = "";
                    adjustSelectedValues(delta);
                }
            }
        };
        // Character bindings cover the main keyboard and numpad without handling a key twice.
        for (String modifier : new String[]{"", "shift "}) {
            bindKey(table, JComponent.WHEN_FOCUSED, modifier + "typed " + character,
                "adjustValue" + character, action);
        }
    }

    private void applySelectionInput(String text) {
        if (viewMode || getSelectedDataColumns().length == 0) {
            return;
        }
        double value;
        try {
            value = text.isEmpty() ? 0 : DecimalInput.parse(text);
        } catch (NumberFormatException e) {
            return;
        }
        if (!Double.isFinite(value)) {
            return;
        }
        selectionInput = text;
        applyingSelectionInput = true;
        try {
            setValue(value, table.getSelectedRows(), table.getSelectedColumns());
        } finally {
            applyingSelectionInput = false;
        }
    }

    private void deleteFromCell(boolean backwards) {
        if (viewMode) {
            return;
        }
        if (table.isEditing()) {
            JTextField field = (JTextField) table.getEditorComponent();
            String action = backwards ? DefaultEditorKit.deletePrevCharAction : DefaultEditorKit.deleteNextCharAction;
            field.getActionMap().get(action).actionPerformed(new ActionEvent(field, ActionEvent.ACTION_PERFORMED, action));
        } else if (backwards && !selectionInput.isEmpty()) {
            applySelectionInput(selectionInput.substring(0, selectionInput.length() - 1));
        } else if (startCellEdit(false)) {
            ((JTextField) table.getEditorComponent()).setText("");
        }
    }

    private void installCellNavigation(JTextField field) {
        for (String key : new String[]{"LEFT", "RIGHT", "UP", "DOWN", "HOME", "END", "ctrl HOME", "ctrl END"}) {
            for (boolean extend : new boolean[]{false, true}) {
                String stroke = (extend ? "shift " : "") + key;
                Action move = new AbstractAction() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        navigateCell(key, extend);
                    }
                };
                bindKey(table, JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT, stroke, stroke, move);
                if (key.equals("UP") || key.equals("DOWN")) {
                    bindKey(field, JComponent.WHEN_FOCUSED, stroke, stroke, move);
                }
            }
        }
    }

    private void navigateCell(String key, boolean extend) {
        selectionInput = "";
        if (!finishCellEdit(false) || table.getRowCount() == 0 || table.getColumnCount() <= 1) {
            return;
        }
        int row = Math.max(0, activeRow());
        int column = activeColumn() < 0 ? 1 : activeColumn();
        int firstColumn = column == 0 ? 0 : 1;
        switch (key) {
            case "LEFT": column = Math.max(firstColumn, column - 1); break;
            case "RIGHT": column = Math.min(table.getColumnCount() - 1, column + 1); break;
            case "UP": row = Math.max(0, row - 1); break;
            case "DOWN": row = Math.min(table.getRowCount() - 1, row + 1); break;
            case "HOME": column = 1; break;
            case "END": column = table.getColumnCount() - 1; break;
            case "ctrl HOME": row = 0; break;
            case "ctrl END": row = table.getRowCount() - 1; break;
        }
        table.changeSelection(row, column, false, extend);
        table.requestFocusInWindow();
    }

    private boolean validateNumericEditor(JTextField field) {
        try {
            if (Double.isFinite(DecimalInput.parse(field.getText()))) {
                return true;
            }
        } catch (NumberFormatException ignored) {
        }
        field.setBorder(BorderFactory.createLineBorder(TuningStyle.color("Console.ErrorText.foreground", Color.RED)));
        field.setToolTipText("Enter a finite number, or press Esc to cancel");
        return false;
    }

    private void installXAxisEditing() {
        xAxisEditor.setHorizontalAlignment(SwingConstants.CENTER);
        xAxisEditor.setVisible(false);
        xAxisEditor.setFocusTraversalKeysEnabled(false);
        table.getTableHeader().add(xAxisEditor);
        xAxisEditor.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent e) {
                finishXAxisEdit(true);
            }
        });
        bindKey(xAxisEditor, JComponent.WHEN_FOCUSED, "ENTER", "acceptAxis", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (finishXAxisEdit(true)) {
                    table.requestFocusInWindow();
                }
            }
        });
        bindKey(xAxisEditor, JComponent.WHEN_FOCUSED, "ESCAPE", "cancelAxis", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                finishXAxisEdit(false);
                table.requestFocusInWindow();
            }
        });
        for (boolean backwards : new boolean[]{false, true}) {
            String key = backwards ? "shift TAB" : "TAB";
            bindKey(xAxisEditor, JComponent.WHEN_FOCUSED, key, key, new AbstractAction() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    int column = xAxisEditingColumn;
                    if (column > 0 && finishXAxisEdit(true, true)) {
                        int count = Math.min(table.getColumnCount() - 1, xBinsField.getRows() * xBinsField.getCols());
                        startXAxisEdit(1 + Math.floorMod(column - 1 + (backwards ? -1 : 1), count));
                    }
                }
            });
        }
    }

    private void positionXAxisEditor() {
        if (xAxisEditingColumn > 0 && xAxisEditingColumn < table.getColumnCount()) {
            xAxisEditor.setBounds(table.getTableHeader().getHeaderRect(xAxisEditingColumn));
        }
    }

    private void startXAxisEdit(int column) {
        if (viewMode || xBinsField == null || axisImageTarget == null || column <= 0
            || column >= table.getColumnCount() || !(table.getModel() instanceof TuningTableModel)) {
            return;
        }
        TuningTableModel model = (TuningTableModel) table.getModel();
        if (model.xBins == null || column - 1 >= model.xBins.length || !finishXAxisEdit(true, true)
            || (table.isEditing() && !table.getCellEditor().stopCellEditing())) {
            return;
        }
        xAxisEditingColumn = column;
        xAxisEditor.setText(table.getColumnName(column));
        xAxisEditor.setFont(table.getFont());
        xAxisEditor.setBorder(UIManager.getBorder("TextField.border"));
        xAxisEditor.setToolTipText("Enter to accept; Esc to cancel; Tab/Shift+Tab to edit the next axis value");
        positionXAxisEditor();
        xAxisEditor.setVisible(true);
        xAxisEditor.selectAll();
        xAxisEditor.requestFocusInWindow();
        table.getTableHeader().repaint();
    }

    private boolean finishXAxisEdit(boolean commit) {
        return finishXAxisEdit(commit, false);
    }

    private boolean finishXAxisEdit(boolean commit, boolean keepEditorVisible) {
        if (xAxisEditingColumn < 0) {
            return true;
        }
        if (commit && !validateNumericEditor(xAxisEditor)) {
            return false;
        }
        int column = xAxisEditingColumn;
        xAxisEditingColumn = -1;
        // Moving between headers must not generate focus-loss events for the reused editor.
        if (!keepEditorVisible) {
            xAxisEditor.setVisible(false);
        }
        if (commit) {
            setAxisValue(true, column - 1, xAxisEditor.getText());
        }
        table.getTableHeader().repaint();
        return true;
    }

    private void bindKey(JComponent component, int condition, String key, String name, Action action) {
        component.getInputMap(condition).put(KeyStroke.getKeyStroke(key), name);
        component.getActionMap().put(name, action);
    }

    /**
     * Shows the large 3D surface in place of the grid, or the grid again. The grid's editing
     * buttons and shortcuts are off while the grid is hidden.
     */
    public void show3D(boolean show) {
        full3D = show;
        (show ? surfaceSegment : previewOn ? bothSegment : tableSegment).setSelected(true);
        cardLayout.show(tableContainer, show ? "3d" : "table");
        for (Component control : editControls) {
            control.setEnabled(!show);
        }
        updatePreviewVisibility();
    }

    /** "Table" or "Both": the grid, without or with the preview; the choice is remembered. */
    private void showGrid(boolean preview) {
        boolean changed = preview != previewOn;
        previewOn = preview;
        show3D(false);
        if (changed) {
            orientationConfig.setBoolProperty(PREVIEW_PROPERTY, preview);
            // Other open tables follow, and the window fits the new height.
            onOrientationChange.run();
        }
    }

    boolean is3DShown() {
        return full3D;
    }

    private Action tableShortcut(Action action) {
        return new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (!full3D && !table.isEditing() && xAxisEditingColumn < 0) {
                    selectionInput = "";
                    action.actionPerformed(e);
                }
            }
        };
    }

    private void moveValueCell(boolean backwards) {
        selectionInput = "";
        int row = activeRow();
        int column = activeColumn();
        if (table.isEditing() && !table.getCellEditor().stopCellEditing()) {
            return;
        }
        int columns = table.getColumnCount() - 1;
        int cells = table.getRowCount() * columns;
        if (cells <= 0) {
            return;
        }
        int index = row >= 0 && column > 0 ? row * columns + column - 1 : (backwards ? 0 : -1);
        index = Math.floorMod(index + (backwards ? -1 : 1), cells);
        table.changeSelection(index / columns, index % columns + 1, false, false);
        table.requestFocusInWindow();
    }

    /**
     * Mouse handling on the grid. A click also turns on wheel editing, which lasts until the
     * pointer leaves the table or focus moves elsewhere, so scrolling the page across the table
     * doesn't change values.
     */
    class TableMouse extends MouseAdapter {
        @Override
        public void mouseExited(MouseEvent e) {
            // Moving onto the cell editor also exits the table, but stays inside its bounds.
            if (!table.contains(e.getPoint())) {
                wheelEditing = false;
            }
        }

        @Override
        public void mousePressed(MouseEvent e) {
            wheelEditing = true;
            selectionInput = "";
            int row = table.rowAtPoint(e.getPoint());
            int col = table.columnAtPoint(e.getPoint());
            if (row != -1 && col != -1) {
                if (table.isCellSelected(row, col)) {
                    // If cell is already selected, we want to keep selection as is
                    // Standard JTable would clear other selections on click.
                    // However, we must be careful not to break standard selection (Shift/Ctrl)
                    if (!e.isControlDown() && !e.isShiftDown()) {
                        // If we click an already selected cell without modifiers,
                        // we usually expect it to become the ONLY selected cell in standard JTable.
                        // But the user said "keep selection selected until click on a cell outside of selection"
                        e.consume();
                    }
                }
            }
        }
    }

    private void handleTableWheel(MouseWheelEvent event) {
        if (!wheelEditing) {
            scrollEnclosingPane(event);
            return;
        }
        // Handle the wheel here so the scroll pane doesn't also scroll independently.
        event.consume();
        if (table.isEditing() || xAxisEditingColumn >= 0 || event.getWheelRotation() == 0) {
            return;
        }
        selectionInput = "";
        if (!event.isControlDown()) {
            adjustValuesWithWheel(event);
            return;
        }
        if (!isActiveValueCell()) {
            return;
        }
        int row = Math.max(0, Math.min(table.getRowCount() - 1, activeRow() + event.getWheelRotation()));
        if (row != activeRow()) {
            // changeSelection also scrolls the selected cell into view.
            table.changeSelection(row, activeColumn(), false, false);
        }
    }

    /**
     * Hands the wheel to the nearest enclosing scroll pane that scrolls in the wheel's direction
     * (vertical, or horizontal with Shift): the table's own when the grid is clipped, otherwise the
     * page around it. AWT does not pass a wheel event on from a scroll pane that ignores it, and a
     * grid that only scrolls sideways would take a plain wheel turn meant for the page, so this
     * picks the pane itself.
     */
    private void scrollEnclosingPane(MouseWheelEvent event) {
        Component source = event.getComponent();
        for (Container parent = source.getParent(); parent != null; parent = parent.getParent()) {
            if (!(parent instanceof JScrollPane)) {
                continue;
            }
            JScrollPane pane = (JScrollPane) parent;
            JScrollBar bar = event.isShiftDown() ? pane.getHorizontalScrollBar() : pane.getVerticalScrollBar();
            if (pane.isWheelScrollingEnabled() && bar != null && bar.isVisible()) {
                MouseWheelEvent converted = (MouseWheelEvent) SwingUtilities.convertMouseEvent(source, event, parent);
                parent.dispatchEvent(converted);
                if (converted.isConsumed()) {
                    event.consume();
                    return;
                }
            }
        }
    }

    private void adjustValuesWithWheel(MouseWheelEvent event) {
        // One adjustment per event, regardless of rotation magnitude or pointer position.
        int direction = event.getWheelRotation() > 0 ? -1 : 1;
        adjustSelectedValuesByPrecision(direction * (event.isShiftDown() ? 3 : 1));
    }

    private void adjustSelectedValuesByPrecision(int steps) {
        if (table.getModel() instanceof TuningTableModel) {
            TuningTableModel model = (TuningTableModel) table.getModel();
            adjustSelectedValues(steps * Math.pow(10, -model.precision));
        }
    }

    private void adjustSelectedValues(double delta) {
        if (viewMode || !(table.getModel() instanceof TuningTableModel)) {
            return;
        }
        TuningTableModel model = (TuningTableModel) table.getModel();
        int[] selectedRows = table.getSelectedRows();
        int[] selectedColumns = getSelectedDataColumns();
        boolean changed = false;
        for (int selectedRow : selectedRows) {
            int row = model.data.length - 1 - selectedRow;
            for (int selectedColumn : selectedColumns) {
                int column = selectedColumn - 1;
                Double old = model.data[row][column];
                if (old == null || !Double.isFinite(old)) {
                    continue;
                }
                double value = old + delta;
                if (Double.isFinite(value) && Double.compare(old, value) != 0) {
                    model.data[row][column] = value;
                    changed = true;
                }
            }
        }
        if (changed) {
            commitEdit(model, selectedRows, table.getSelectedColumns());
        }
    }

    protected int showConfirmDialog(JPanel panel) {
        return JOptionPane.showConfirmDialog(table, panel, "Set Value", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
    }

    private void showSetDialog() {
        int[] selectedRows = table.getSelectedRows();
        int[] selectedCols = table.getSelectedColumns();

        if (selectedRows.length == 0 || selectedCols.length == 0) {
            return;
        }

        JTextField valueField = new JTextField("70", 10);
        JPanel panel = new JPanel(new GridLayout(0, 1));
        panel.add(new JLabel("Value:"));
        panel.add(valueField);

        int result = showConfirmDialog(panel);
        if (result == JOptionPane.OK_OPTION) {
            try {
                double value = DecimalInput.parse(valueField.getText());
                setValue(value, selectedRows, selectedCols);
            } catch (NumberFormatException ignored) {
            }
        }
    }

    protected void setValue(double value, int[] selectedRows, int[] selectedCols) {
        TuningTableModel model = (TuningTableModel) table.getModel();

        for (int row : selectedRows) {
            for (int col : selectedCols) {
                if (col == 0) {
                    continue; // Skip axis column
                }
                int reversedRowIndex = model.data.length - 1 - row;
                model.data[reversedRowIndex][col - 1] = value;
            }
        }
        commitEdit(model, selectedRows, selectedCols);
    }

    public void setOnEdit(Runnable onEdit) {
        this.onEdit = onEdit;
    }

    public void setOnOrientationChange(Runnable onOrientationChange) {
        this.onOrientationChange = onOrientationChange;
    }

    private ArrayIniField editableAxisField(IniFileModel ini, String name) {
        Optional<IniField> field = ini.findIniField(name);
        // PC variable bins are fixed by the INI and have no tune storage
        if (field.isPresent() && field.get() instanceof ArrayIniField
            && !(field.get() instanceof PcVariableArrayIniField)) {
            ArrayIniField array = (ArrayIniField) field.get();
            if (array.getRows() == 1 || array.getCols() == 1) {
                return array;
            }
        }
        return null;
    }

    private void setAxisValue(boolean xAxis, int index, Object value) {
        ArrayIniField field = xAxis ? xBinsField : yBinsField;
        ConfigurationImage target = xAxis ? axisImageTarget : yAxisImageTarget;
        if (viewMode || field == null || target == null || value == null
            || !(table.getModel() instanceof TuningTableModel)) {
            return;
        }
        TuningTableModel model = (TuningTableModel) table.getModel();
        Double[] bins = xAxis ? model.xBins : model.yBins;
        if (bins == null || index < 0 || index >= bins.length) {
            return;
        }
        double number;
        try {
            number = DecimalInput.parse(value.toString());
        } catch (NumberFormatException e) {
            return;
        }
        if (!Double.isFinite(number)) {
            return;
        }
        int storedIndex = (xAxis ? invertX : invertY) ? bins.length - 1 - index : index;
        int row = field.getRows() == 1 ? 0 : storedIndex;
        int column = field.getRows() == 1 ? storedIndex : 0;
        // Encode only the edited element, then display the actual stored precision.
        int offset = field.getOffset(row, column);
        ConfigurationImage candidate = target.clone();
        ConfigurationImage.setScalarValue(candidate.getByteBuffer(offset, field.getType().getStorageSize()),
            field.getType(), Double.toString(number), com.rusefi.config.Field.NO_BIT_OFFSET, field.getMultiplier(), 0);
        double encoded = ConfigurationImageGetterSetter.getArrayValues(field, candidate)[row][column];
        if (!Double.isFinite(encoded) || Double.compare(bins[index], encoded) == 0) {
            return;
        }
        System.arraycopy(candidate.getContent(), offset, target.getContent(), offset,
            field.getType().getStorageSize());
        bins[index] = encoded;
        if (xAxis) {
            table.getColumnModel().getColumn(index + 1).setHeaderValue(model.getColumnName(index + 1));
            fitColumnWidths();
            table.getTableHeader().repaint();
        } else {
            model.fireTableCellUpdated(model.data.length - 1 - index, 0);
        }
        updateSurfaces(model.data, model.xBins, model.yBins);
        if (onEdit != null) {
            onEdit.run();
        }
    }

    private boolean writeBackZBins(TuningTableModel model) {
        if (imageTarget == null || zBinsField == null) {
            return false;
        }
        ConfigurationImageGetterSetter.setArrayValues(zBinsField, imageTarget, toStoredData(model.data));
        Double[][] encodedValues = orientData(ConfigurationImageGetterSetter.getArrayValues(zBinsField, imageTarget));
        for (int row = 0; row < model.data.length; row++) {
            System.arraycopy(encodedValues[row], 0, model.data[row], 0, model.data[row].length);
        }
        return true;
    }

    private void commitEdit(TuningTableModel model, int[] selectedRows, int[] selectedCols) {
        int leadRow = activeRow();
        int leadColumn = activeColumn();
        boolean wroteImage = writeBackZBins(model);
        calculateMinMax(model.data);
        model.fireTableDataChanged();

        table.clearSelection();
        for (int row : selectedRows) {
            table.addRowSelectionInterval(row, row);
        }
        for (int col : selectedCols) {
            table.addColumnSelectionInterval(col, col);
        }
        ((DefaultListSelectionModel) table.getSelectionModel()).moveLeadSelectionIndex(leadRow);
        ((DefaultListSelectionModel) table.getColumnModel().getSelectionModel()).moveLeadSelectionIndex(leadColumn);

        updateSurfaces(model.data, model.xBins, model.yBins);
        if (wroteImage && onEdit != null) {
            onEdit.run();
        }
    }

    private void applyDelta(JTextField deltaField, int sign) {
        try {
            double delta = DecimalInput.parse(deltaField.getText()) * sign;
            TuningTableModel model = (TuningTableModel) table.getModel();
            int[] selectedRows = table.getSelectedRows();
            int[] selectedCols = table.getSelectedColumns();

            if (selectedRows.length == 0 || selectedCols.length == 0) {
                return;
            }

            for (int row : selectedRows) {
                for (int col : selectedCols) {
                    if (col == 0) {
                        continue; // Skip axis column
                    }
                    int reversedRowIndex = model.data.length - 1 - row;
                    model.data[reversedRowIndex][col - 1] += delta;
                }
            }
            commitEdit(model, selectedRows, selectedCols);
        } catch (NumberFormatException ignored) {
        }
    }

    private void interpolateHorizontal() {
        if (!(table.getModel() instanceof TuningTableModel)) {
            return;
        }
        TuningTableModel model = (TuningTableModel) table.getModel();
        int[] selectedRows = table.getSelectedRows();
        int[] selectedCols = getSelectedDataColumns();
        if (selectedRows.length == 0 || selectedCols.length < 2) {
            return;
        }

        int firstCol = selectedCols[0] - 1;
        int lastCol = selectedCols[selectedCols.length - 1] - 1;
        Double[][] source = copyData(model.data);
        boolean changed = false;

        for (int selectedRow : selectedRows) {
            int row = model.data.length - 1 - selectedRow;
            double firstValue = source[row][firstCol];
            double lastValue = source[row][lastCol];
            for (int selectedCol : selectedCols) {
                int col = selectedCol - 1;
                double position = (double) (col - firstCol) / (lastCol - firstCol);
                double value = firstValue + position * (lastValue - firstValue);
                if (Double.compare(model.data[row][col], value) != 0) {
                    model.data[row][col] = value;
                    changed = true;
                }
            }
        }

        if (changed) {
            commitEdit(model, selectedRows, table.getSelectedColumns());
        }
    }

    private void interpolateSelection() {
        int selectedRowCount = table.getSelectedRowCount();
        int selectedColumnCount = getSelectedDataColumns().length;
        if (selectedRowCount == 1 && selectedColumnCount > 1) {
            interpolateHorizontal();
        } else if (selectedRowCount > 1 && selectedColumnCount == 1) {
            interpolateVertical();
        } else if (selectedRowCount > 1 && selectedColumnCount > 1) {
            interpolateBilinear();
        }
    }

    private void interpolateVertical() {
        if (!(table.getModel() instanceof TuningTableModel)) {
            return;
        }
        TuningTableModel model = (TuningTableModel) table.getModel();
        int[] selectedRows = table.getSelectedRows();
        int[] selectedCols = getSelectedDataColumns();
        if (selectedRows.length < 2 || selectedCols.length == 0) {
            return;
        }

        int firstRow = selectedRows[0];
        int lastRow = selectedRows[selectedRows.length - 1];
        Double[][] source = copyData(model.data);
        boolean changed = false;

        for (int selectedCol : selectedCols) {
            int col = selectedCol - 1;
            double firstValue = source[model.data.length - 1 - firstRow][col];
            double lastValue = source[model.data.length - 1 - lastRow][col];
            for (int selectedRow : selectedRows) {
                int row = model.data.length - 1 - selectedRow;
                double position = (double) (selectedRow - firstRow) / (lastRow - firstRow);
                double value = firstValue + position * (lastValue - firstValue);
                if (Double.compare(model.data[row][col], value) != 0) {
                    model.data[row][col] = value;
                    changed = true;
                }
            }
        }

        if (changed) {
            commitEdit(model, selectedRows, table.getSelectedColumns());
        }
    }

    private void interpolateBilinear() {
        if (!(table.getModel() instanceof TuningTableModel)) {
            return;
        }
        TuningTableModel model = (TuningTableModel) table.getModel();
        int[] selectedRows = table.getSelectedRows();
        int[] selectedCols = getSelectedDataColumns();
        if (selectedRows.length < 2 || selectedCols.length < 2) {
            return;
        }

        int firstRow = selectedRows[0];
        int lastRow = selectedRows[selectedRows.length - 1];
        int firstCol = selectedCols[0] - 1;
        int lastCol = selectedCols[selectedCols.length - 1] - 1;
        Double[][] source = copyData(model.data);
        double topLeft = source[model.data.length - 1 - firstRow][firstCol];
        double topRight = source[model.data.length - 1 - firstRow][lastCol];
        double bottomLeft = source[model.data.length - 1 - lastRow][firstCol];
        double bottomRight = source[model.data.length - 1 - lastRow][lastCol];
        boolean changed = false;

        for (int selectedRow : selectedRows) {
            int row = model.data.length - 1 - selectedRow;
            double verticalPosition = (double) (selectedRow - firstRow) / (lastRow - firstRow);
            for (int selectedCol : selectedCols) {
                int col = selectedCol - 1;
                double horizontalPosition = (double) (col - firstCol) / (lastCol - firstCol);
                double value = (1 - horizontalPosition) * (1 - verticalPosition) * topLeft
                    + horizontalPosition * (1 - verticalPosition) * topRight
                    + (1 - horizontalPosition) * verticalPosition * bottomLeft
                    + horizontalPosition * verticalPosition * bottomRight;
                if (Double.compare(model.data[row][col], value) != 0) {
                    model.data[row][col] = value;
                    changed = true;
                }
            }
        }

        if (changed) {
            commitEdit(model, selectedRows, table.getSelectedColumns());
        }
    }

    private void smoothSelection() {
        if (!(table.getModel() instanceof TuningTableModel)) {
            return;
        }
        TuningTableModel model = (TuningTableModel) table.getModel();
        int[] selectedRows = table.getSelectedRows();
        int[] selectedCols = getSelectedDataColumns();
        if (selectedRows.length == 0 || selectedCols.length == 0) {
            return;
        }

        Double[][] source = copyData(model.data);
        boolean changed = false;
        for (int selectedRow : selectedRows) {
            int row = model.data.length - 1 - selectedRow;
            for (int selectedCol : selectedCols) {
                int col = selectedCol - 1;
                double sum = 0;
                int count = 0;
                for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
                    for (int colOffset = -1; colOffset <= 1; colOffset++) {
                        if (rowOffset == 0 && colOffset == 0) {
                            continue;
                        }
                        int neighborRow = row + rowOffset;
                        int neighborCol = col + colOffset;
                        if (neighborRow >= 0 && neighborRow < source.length
                            && neighborCol >= 0 && neighborCol < source[neighborRow].length) {
                            sum += source[neighborRow][neighborCol];
                            count++;
                        }
                    }
                }
                if (count == 0) {
                    continue;
                }
                double value = (1 - SMOOTHING_FACTOR) * source[row][col]
                    + SMOOTHING_FACTOR * sum / count;
                if (Double.compare(model.data[row][col], value) != 0) {
                    model.data[row][col] = value;
                    changed = true;
                }
            }
        }

        if (changed) {
            commitEdit(model, selectedRows, table.getSelectedColumns());
        }
    }

    private int[] getSelectedDataColumns() {
        int[] selectedCols = table.getSelectedColumns();
        int dataColumnCount = 0;
        for (int selectedCol : selectedCols) {
            if (selectedCol > 0) {
                dataColumnCount++;
            }
        }

        int[] result = new int[dataColumnCount];
        int index = 0;
        for (int selectedCol : selectedCols) {
            if (selectedCol > 0) {
                result[index++] = selectedCol;
            }
        }
        return result;
    }

    private Double[][] copyData(Double[][] data) {
        Double[][] copy = new Double[data.length][];
        for (int row = 0; row < data.length; row++) {
            copy[row] = data[row].clone();
        }
        return copy;
    }

    public void displayTable(IniFileModel iniFile, String tableName, ConfigurationImage zImage, ConfigurationImage axisImage) {
        displayTable(iniFile, tableName, zImage, axisImage, axisImage);
    }

    void displayTable(IniFileModel iniFile, String tableName, ConfigurationImage zImage,
                      ConfigurationImage xAxisImage, ConfigurationImage yAxisImage) {
        finishXAxisEdit(false);
        if (table.isEditing()) {
            table.getCellEditor().cancelCellEditing();
        }
        selectionInput = "";
        TableModel iniTable = iniFile.getTable(tableName);
        if (iniTable == null) {
            return;
        }

        Optional<IniField> zBinsField = iniFile.findIniField(iniTable.getZBinsConstant());
        if (!zBinsField.isPresent() || !(zBinsField.get() instanceof ArrayIniField)) {
            return;
        }

        ArrayIniField ltft = (ArrayIniField) zBinsField.get();
        int precision = IniField.parseDigits(ltft.getDigits());

        // Extract data from outputs buffer using the field's offset
        Double[][] storedValues = ConfigurationImageGetterSetter.getArrayValues(ltft, zImage);

        Double[] storedX = extractAxisBins(iniFile, iniTable.getXBinsConstant(), xAxisImage);
        Double[] storedY = extractAxisBins(iniFile, iniTable.getYBinsConstant(), yAxisImage);

        currentIni = iniFile;
        currentTableName = tableName;
        orientationNode = orientationConfig.getChild(tableName);
        flipAxes = orientationNode.getBoolProperty("flip_xy");
        invertX = orientationNode.getBoolProperty("invert_x");
        invertY = orientationNode.getBoolProperty("invert_y");
        live.setChannels(flipAxes ? iniTable.getYBinsChannel() : iniTable.getXBinsChannel(),
            flipAxes ? iniTable.getXBinsChannel() : iniTable.getYBinsChannel());
        Double[][] dataValues = orientData(storedValues);
        Double[] xBins = orientAxis(flipAxes ? storedY : storedX, invertX);
        Double[] yBins = orientAxis(flipAxes ? storedX : storedY, invertY);

        calculateMinMax(dataValues);

        this.imageTarget = zImage;
        this.zBinsField = ltft;
        // Axis targets follow displayed axes after flip_xy, not the source X/Y names.
        this.axisImageTarget = flipAxes ? yAxisImage : xAxisImage;
        this.yAxisImageTarget = flipAxes ? xAxisImage : yAxisImage;
        this.xBinsField = editableAxisField(iniFile,
            flipAxes ? iniTable.getYBinsConstant() : iniTable.getXBinsConstant());
        this.yBinsField = editableAxisField(iniFile,
            flipAxes ? iniTable.getXBinsConstant() : iniTable.getYBinsConstant());

        Function<String, String> labels = axisLabelResolver(iniFile, iniTable, zImage, xAxisImage, yAxisImage);
        String storedXLabel = labels.apply(iniTable.getXLabel());
        String storedYLabel = labels.apply(iniTable.getYLabel());
        String storedXName = axisName(storedXLabel, iniTable.getXBinsChannel(), iniTable.getXBinsConstant());
        String storedYName = axisName(storedYLabel, iniTable.getYBinsChannel(), iniTable.getYBinsConstant());
        TuningTableModel model = new TuningTableModel(dataValues, xBins, yBins, precision, !viewMode,
            edited -> commitEdit(edited, table.getSelectedRows(), table.getSelectedColumns()),
            flipAxes ? storedXName + " \\ " + storedYName : storedYName + " \\ " + storedXName);
        if (!viewMode && yAxisImageTarget != null && yBinsField != null) {
            model.onYAxisEdit = (index, value) -> setAxisValue(false, index, value);
        }
        model.xPrecision = xBinsField == null ? precision : IniField.parseDigits(xBinsField.getDigits());
        model.yPrecision = yBinsField == null ? precision : IniField.parseDigits(yBinsField.getDigits());
        String xLabel = flipAxes ? storedYLabel : storedXLabel;
        String yLabel = flipAxes ? storedXLabel : storedYLabel;
        String zLabel = plainLabel(ltft.getUnits());
        surface3DView.setAxisTitles(xLabel, yLabel, zLabel);
        mini3DView.setAxisTitles(xLabel, yLabel, zLabel);
        table.setModel(model);
        model.addTableModelListener(e -> fitColumnWidths());
        fitColumnWidths();
        table.clearSelection();
        updateSurfaceSelection();
        updateSurfaces(dataValues, xBins, yBins);
        refreshUnburnedMarks();
    }

    /** [tag:unburned_marks] Supplies the last burned tune, so changed cells and axis values are marked. */
    public void setBurnedImage(Supplier<ConfigurationImage> burnedImage) {
        this.burnedImage = burnedImage == null ? () -> null : burnedImage;
        refreshUnburnedMarks();
    }

    /** Names the reference tune in the cell tooltips: burned, or loaded while offline. */
    public void setUnburnedWording(Supplier<UnburnedWording> wording) {
        unburnedWording = wording == null ? () -> UnburnedWording.BURNED : wording;
    }

    /** Re-marks the cells and recounts them, after an edit or when the burned tune changes. */
    public void refreshUnburnedMarks() {
        UnburnedCellMark.updateCountLabel(unburnedCountLabel, unburnedCellCount());
        table.repaint();
        table.getTableHeader().repaint();
    }

    /** Changed values cells plus changed axis values. */
    int unburnedCellCount() {
        ConfigurationImage burned = burnedImage.get();
        UnburnedChanges values = UnburnedChanges.of(imageTarget, burned);
        UnburnedChanges xAxis = UnburnedChanges.of(axisImageTarget, burned);
        UnburnedChanges yAxis = UnburnedChanges.of(yAxisImageTarget, burned);
        return values.countChangedCells(zBinsField) + xAxis.countChangedCells(xBinsField)
            + yAxis.countChangedCells(yBinsField);
    }

    /**
     * Gives every column the width of its widest value or axis label as a minimum. The grid
     * stretches beyond that in wide windows and scrolls sideways in narrow ones.
     */
    private void fitColumnWidths() {
        FontMetrics values = table.getFontMetrics(table.getFont());
        JTableHeader header = table.getTableHeader();
        FontMetrics labels = header.getFontMetrics(header.getFont());
        javax.swing.table.TableColumnModel columns = table.getColumnModel();
        for (int column = 0; column < columns.getColumnCount(); column++) {
            javax.swing.table.TableColumn tableColumn = columns.getColumn(column);
            int textWidth = labels.stringWidth(String.valueOf(tableColumn.getHeaderValue()));
            for (int row = 0; row < table.getRowCount(); row++) {
                textWidth = Math.max(textWidth, values.stringWidth(String.valueOf(table.getValueAt(row, column))));
            }
            int width = Math.min(MAX_COLUMN_MIN_WIDTH, textWidth + CELL_TEXT_PADDING);
            if (tableColumn.getMinWidth() != width || tableColumn.getPreferredWidth() != width) {
                tableColumn.setMinWidth(width);
                // Without stretching, the grid is exactly as wide as its minimum.
                tableColumn.setPreferredWidth(width);
            }
        }
        table.revalidate();
    }

    private int minimumGridWidth() {
        int width = 0;
        javax.swing.table.TableColumnModel columns = table.getColumnModel();
        for (int column = 0; column < columns.getColumnCount(); column++) {
            width += columns.getColumn(column).getMinWidth();
        }
        return width;
    }

    JTable getTableForTests() {
        return table;
    }

    JLabel getUnburnedCountLabelForTests() {
        return unburnedCountLabel;
    }

    /**
     * The stored [row, column] of {@link #zBinsField} shown in table cell ({@code row}, {@code column} >= 1):
     * rows are shown bottom-up, then {@link #orientData} applies the invert and flip options.
     */
    int[] storedValuesCell(int row, int column) {
        TuningTableModel model = (TuningTableModel) table.getModel();
        int rows = model.data.length;
        int columns = model.data[0].length;
        int dataRow = rows - 1 - row;
        int dataColumn = column - 1;
        int y = invertY ? rows - 1 - dataRow : dataRow;
        int x = invertX ? columns - 1 - dataColumn : dataColumn;
        return flipAxes ? new int[]{x, y} : new int[]{y, x};
    }

    /** The stored [row, column] of a one-dimensional axis field for shown bin {@code index}. */
    private static int[] storedAxisCell(ArrayIniField field, int binCount, boolean inverted, int index) {
        int storedIndex = inverted ? binCount - 1 - index : index;
        return field.getRows() == 1 ? new int[]{0, storedIndex} : new int[]{storedIndex, 0};
    }

    /** The burned value of table cell ({@code row}, {@code column}) when it differs, else null. */
    String burnedCellText(int row, int column) {
        ConfigurationImage burned = burnedImage.get();
        if (burned == null || !(table.getModel() instanceof TuningTableModel)) {
            return null;
        }
        TuningTableModel model = (TuningTableModel) table.getModel();
        if (column == 0) {
            int index = model.data.length - 1 - row;
            if (yBinsField == null || model.yBins == null || index < 0 || index >= model.yBins.length) {
                return null;
            }
            int[] cell = storedAxisCell(yBinsField, model.yBins.length, invertY, index);
            if (!UnburnedChanges.of(yAxisImageTarget, burned).isCellChanged(yBinsField, cell[0], cell[1])) {
                return null;
            }
            return model.formatNumber(UnburnedCellMark.burnedValue(yBinsField, cell[0], cell[1], burned), model.yPrecision);
        }
        if (zBinsField == null || row < 0 || row >= model.data.length || column > model.data[0].length) {
            return null;
        }
        int[] cell = storedValuesCell(row, column);
        if (!UnburnedChanges.of(imageTarget, burned).isCellChanged(zBinsField, cell[0], cell[1])) {
            return null;
        }
        return model.formatNumber(UnburnedCellMark.burnedValue(zBinsField, cell[0], cell[1], burned));
    }

    /** The burned value of X axis header {@code column} when it differs, else null. */
    String burnedXAxisText(int column) {
        ConfigurationImage burned = burnedImage.get();
        if (burned == null || column < 1 || xBinsField == null || !(table.getModel() instanceof TuningTableModel)) {
            return null;
        }
        TuningTableModel model = (TuningTableModel) table.getModel();
        int index = column - 1;
        if (model.xBins == null || index >= model.xBins.length) {
            return null;
        }
        int[] cell = storedAxisCell(xBinsField, model.xBins.length, invertX, index);
        if (!UnburnedChanges.of(axisImageTarget, burned).isCellChanged(xBinsField, cell[0], cell[1])) {
            return null;
        }
        return model.formatNumber(UnburnedCellMark.burnedValue(xBinsField, cell[0], cell[1], burned), model.xPrecision);
    }

    public void displayTable(IniFileModel iniFile, String tableName, ConfigurationImage image) {
        displayTable(iniFile, tableName, image, image);
    }

    private Double[] orientAxis(Double[] bins, boolean inverse) {
        if (bins == null) {
            return null;
        }
        Double[] oriented = bins.clone();
        if (inverse) {
            for (int i = 0; i < bins.length; i++) {
                oriented[i] = bins[bins.length - 1 - i];
            }
        }
        return oriented;
    }

    private Double[][] orientData(Double[][] stored) {
        int rows = flipAxes ? stored[0].length : stored.length;
        int columns = flipAxes ? stored.length : stored[0].length;
        Double[][] oriented = new Double[rows][columns];
        for (int row = 0; row < rows; row++) {
            for (int column = 0; column < columns; column++) {
                int y = invertY ? rows - 1 - row : row;
                int x = invertX ? columns - 1 - column : column;
                oriented[row][column] = flipAxes ? stored[x][y] : stored[y][x];
            }
        }
        return oriented;
    }

    private Double[][] toStoredData(Double[][] oriented) {
        int rows = flipAxes ? oriented[0].length : oriented.length;
        int columns = flipAxes ? oriented.length : oriented[0].length;
        Double[][] stored = new Double[rows][columns];
        for (int row = 0; row < oriented.length; row++) {
            for (int column = 0; column < oriented[row].length; column++) {
                int y = invertY ? oriented.length - 1 - row : row;
                int x = invertX ? oriented[row].length - 1 - column : column;
                stored[flipAxes ? x : y][flipAxes ? y : x] = oriented[row][column];
            }
        }
        return stored;
    }

    @Deprecated
    public void displayTable(CalibrationsInfo info, byte[] zBinsBuffer, ConfigurationImage image, String tableName) {
        displayTable(info.getIniFile(), tableName,
            new ConfigurationImage(zBinsBuffer, info.getIniFile().getByteOrder()), image);
    }

    private void calculateMinMax(Double[][] dataValues) {
        double min = Double.MAX_VALUE;
        double max = -Double.MAX_VALUE;
        for (Double[] row : dataValues) {
            for (Double v : row) {
                if (v != null) {
                    min = Math.min(min, v);
                    max = Math.max(max, v);
                }
            }
        }
        this.minValue = min;
        this.maxValue = max;
    }

    private Double[] extractAxisBins(IniFileModel iniFile,
                                     String binConstant,
                                     ConfigurationImage image) {
        if (image == null) {
            return null;
        }
        Optional<IniField> binsField = iniFile.findIniField(binConstant);
        if (!binsField.isPresent() || !(binsField.get() instanceof ArrayIniField)) {
            return null;
        }

        ArrayIniField field = (ArrayIniField) binsField.get();
        if (!(field instanceof com.opensr5.ini.field.PcVariableArrayIniField)
                && (image == null || field.getOffset() < 0 || field.getOffset() + field.getSize() > image.getSize())) {
            return null;
        }
        Double[][] values = ConfigurationImageGetterSetter.getArrayValues(field, image);

        if (values.length == 0 || values[0].length == 0) {
            return null;
        }

        return (values.length == 1) ? values[0] :
                (values[0].length == 1) ? extractColumn(values) :
                        values[0];
    }

    private Double[] extractColumn(Double[][] values) {
        Double[] column = new Double[values.length];
        for (int i = 0; i < values.length; i++) {
            column[i] = values[i][0];
        }
        return column;
    }

    public JPanel getContent() {
        return content;
    }

    static class TuningTableModel extends AbstractTableModel {
        final Double[][] data;
        final Double[] xBins;
        final Double[] yBins;
        private final int precision;
        private final boolean editable;
        private final Consumer<TuningTableModel> onCellEdit;
        private final String axisTitle;
        private BiConsumer<Integer, Object> onYAxisEdit;
        private int xPrecision;
        private int yPrecision;

        public TuningTableModel(Double[][] data, Double[] xBins, Double[] yBins, int precision) {
            this(data, xBins, yBins, precision, true, null, "Load \\ RPM");
        }

        private TuningTableModel(Double[][] data, Double[] xBins, Double[] yBins, int precision,
                                 boolean editable, Consumer<TuningTableModel> onCellEdit, String axisTitle) {
            this.data = data;
            this.xBins = xBins;
            this.yBins = yBins;
            this.precision = precision;
            this.xPrecision = precision;
            this.yPrecision = precision;
            this.editable = editable;
            this.onCellEdit = onCellEdit;
            this.axisTitle = axisTitle;
        }

        @Override
        public boolean isCellEditable(int row, int column) {
            return editable && (column > 0 || (onYAxisEdit != null && yBins != null
                && data.length - 1 - row < yBins.length));
        }

        @Override
        public void setValueAt(Object value, int row, int column) {
            if (!isCellEditable(row, column)) {
                return;
            }
            if (column == 0) {
                onYAxisEdit.accept(data.length - 1 - row, value);
                return;
            }
            double number;
            try {
                number = DecimalInput.parse(value.toString());
            } catch (NumberFormatException e) {
                return;
            }
            int dataRow = data.length - 1 - row;
            if (!Double.isFinite(number) || Double.compare(data[dataRow][column - 1], number) == 0) {
                return;
            }
            data[dataRow][column - 1] = number;
            if (onCellEdit != null) {
                onCellEdit.accept(this);
            } else {
                fireTableCellUpdated(row, column);
            }
        }

        @Override
        public int getRowCount() {
            return data.length;
        }

        @Override
        public int getColumnCount() {
            return data[0].length + 1;
        }

        @Override
        public String getColumnName(int column) {
            if (column == 0) return axisTitle;
            if (xBins != null && column - 1 < xBins.length) {
                return formatNumber(xBins[column - 1], xPrecision);
            }
            return "Col " + column;
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) {
            // traditionally low values are displayed on the bottom and high line on top
            int reversedRowIndex = data.length - 1 - rowIndex;
            if (columnIndex == 0) {
                if (yBins != null && reversedRowIndex < yBins.length) {
                    return formatNumber(yBins[reversedRowIndex], yPrecision);
                }
                return "Row " + rowIndex;
            }
            return formatNumber(data[reversedRowIndex][columnIndex - 1]);
        }

        /** A value cell: always the INI's digits, so the decimals line up down a column. */
        private String formatNumber(Object value) {
            if (value instanceof Number) {
                return String.format(java.util.Locale.ROOT, "%." + precision + "f", ((Number) value).doubleValue());
            }
            return String.valueOf(value);
        }

        private String formatNumber(Object value, int digits) {
            if (value instanceof Number) {
                return String.format("%." + digits + "f", ((Number) value).doubleValue())
                    .replace(',', '.').replaceFirst("\\.0+$", "");
            }
            return String.valueOf(value);
        }
    }

    private class GradientRenderer extends DefaultTableCellRenderer {
        GradientRenderer() {
            setHorizontalAlignment(SwingConstants.CENTER);
        }

        @Override
        public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
            Component c = renderCell(table, value, isSelected, hasFocus, row, column);
            // [tag:unburned_marks] after the renderer set its own border for this cell
            UnburnedCellMark.apply(c, String.valueOf(value), burnedCellText(row, column), unburnedWording.get());
            return c;
        }

        private Component renderCell(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
            Component c = super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
            if (column == 0) {
                Color background = CalibrationColorPalette.axisBackground();
                c.setBackground(isActiveValueCell() && row == activeRow() ? axisHighlight(background) : background);
                c.setForeground(CalibrationColorPalette.axisForeground());
                c.setFont(axisFont(table.getFont()));
                return c;
            }

            Color valueColor = valueColor(value);
            Color background = valueColor != null ? valueColor : table.getBackground();
            if (isSelected) {
                // The value colour shows through the tint; the outline marks the block's edges.
                background = CalibrationColorPalette.selected(background);
                setBorder(BorderFactory.createCompoundBorder(CalibrationColorPalette.selectionEdges(table, row, column, 1), getBorder()));
            }
            c.setBackground(background);
            c.setForeground(valueColor != null || isSelected
                ? CalibrationColorPalette.foreground(background) : table.getForeground());
            return c;
        }

        /** The value's place on the colour scale, or null for a cell that is not a number. */
        private Color valueColor(Object value) {
            if (value instanceof String && !((String) value).isEmpty()) {
                try {
                    return CalibrationColorPalette.background(Double.parseDouble((String) value), minValue, maxValue);
                } catch (NumberFormatException ignored) {
                }
            }
            return null;
        }

    }
}
