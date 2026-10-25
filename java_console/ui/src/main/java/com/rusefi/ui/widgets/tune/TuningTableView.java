package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.StringFormatter;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.maintenance.CalibrationsInfo;

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
import java.util.Optional;
import java.util.function.Consumer;
import java.util.function.BiConsumer;

public class TuningTableView {
    private static final int TOOLBAR_GAP = 6;
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
                && event.getKeyCode() != KeyEvent.VK_BACK_SPACE) {
                selectionInput = "";
            }
            return super.processKeyBinding(stroke, event, condition, pressed);
        }
    };
    private final Surface3DView surface3DView = new Surface3DView();
    private final Surface3DView mini3DView = new Surface3DView();
    private final JPanel gridPanel = new JPanel(new BorderLayout());
    private final JScrollPane tableScrollPane = new JScrollPane(table);
    private boolean full3D;
    private boolean previewEligible;
    private final CardLayout cardLayout = new CardLayout();
    private final JPanel tableContainer = new JPanel(cardLayout);
    private final JPanel content = new JPanel();
    private final String title;
    private double minValue = Double.MAX_VALUE;
    private double maxValue = -Double.MAX_VALUE;
    private ConfigurationImage imageTarget;
    private ArrayIniField zBinsField;
    private ConfigurationImage axisImageTarget;
    private ArrayIniField xBinsField;
    private ArrayIniField yBinsField;
    private final JTextField xAxisEditor = new JTextField();
    private int xAxisEditingColumn = -1;
    private Runnable onEdit;
    private Runnable onOrientationChange = () -> { };
    private String selectionInput = "";
    private boolean applyingSelectionInput;
    private IniFileModel currentIni;
    private String currentTableName;
    private Node orientationNode;
    private boolean flipAxes;
    private boolean invertX;
    private boolean invertY;

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
        table.setSelectionBackground(Color.ORANGE);
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

        table.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
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
        });

        mini3DView.setPreferredSize(new Dimension(320, 160));
        mini3DView.setToolTipText("3D preview - drag to rotate; select 3D view for a larger view");
        mini3DView.setVisible(false);
        gridPanel.add(tableScrollPane, BorderLayout.CENTER);
        gridPanel.add(mini3DView, BorderLayout.SOUTH);
        tableContainer.add(gridPanel, "table");
        tableContainer.add(surface3DView, "3d");

        JCheckBox view3d = new JCheckBox("3D view");
        view3d.addActionListener(e -> {
            full3D = view3d.isSelected();
            cardLayout.show(tableContainer, view3d.isSelected() ? "3d" : "table");
            updatePreviewVisibility();
        });

        JTextField deltaField = new JTextField("0.5", 5);
        deltaField.setToolTipText("Amount added or subtracted by the Up and Down buttons");
        deltaField.setMaximumSize(new Dimension(100, 30));
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

        JPanel editControls = new JPanel(new FlowLayout(FlowLayout.LEFT, TOOLBAR_GAP, 0));
        editControls.add(new JLabel("delta:"));
        editControls.add(deltaField);
        editControls.add(upButton);
        editControls.add(downButton);
        editControls.add(equalsButton);
        editControls.add(horizontalButton);
        editControls.add(verticalButton);
        editControls.add(interpolateButton);
        editControls.add(smoothButton);

        JPanel topPanel = new JPanel();
        topPanel.setLayout(new BoxLayout(topPanel, BoxLayout.X_AXIS));
        topPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
        topPanel.setBorder(BorderFactory.createEmptyBorder(4, 8, 4, 8));
        topPanel.add(new JLabel(title));
        topPanel.add(Box.createHorizontalStrut(10));
        topPanel.add(view3d);

        if (!viewMode) {
            topPanel.add(Box.createHorizontalStrut(10));
            topPanel.add(editControls);

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
            displayTable(currentIni, currentTableName, imageTarget, axisImageTarget);
            onOrientationChange.run();
        });
        menu.add(option);
    }

    private void updatePreviewVisibility() {
        // Keep the grid usable in small dialogs. Large tables can scroll above the preview.
        Insets border = tableScrollPane.getInsets();
        int gridHeight = Math.min(320, table.getRowCount() * table.getRowHeight())
            + table.getTableHeader().getPreferredSize().height + border.top + border.bottom;
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
        boolean visible = previewEligible && !full3D && tableContainer.getWidth() >= 320 && fits;
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
        previewEligible = data.length >= 2 && data[0].length >= 2
            && Surface3DView.hasUsableAxis(xBins, data[0].length)
            && Surface3DView.hasUsableAxis(yBins, data.length);
        updatePreviewVisibility();
    }

    private void installAxisHighlighting() {
        TableCellRenderer headerRenderer = table.getTableHeader().getDefaultRenderer();
        table.getTableHeader().setDefaultRenderer((source, value, selected, focused, row, column) -> {
            Component renderer = headerRenderer.getTableCellRendererComponent(source, value, selected, focused, row, column);
            Color background = table.getTableHeader().getBackground();
            renderer.setBackground(isActiveValueCell() && column == activeColumn()
                ? axisHighlight(background) : background);
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
        Color selection = table.getSelectionBackground();
        return new Color((3 * background.getRed() + selection.getRed()) / 4,
            (3 * background.getGreen() + selection.getGreen()) / 4,
            (3 * background.getBlue() + selection.getBlue()) / 4);
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
                } else if (character == '.' && selectionInput.isEmpty()) {
                    adjustSelectedValuesByPrecision(1);
                } else if (table.getSelectedRowCount() * table.getSelectedColumnCount() > 1) {
                    if (character == '.' && (selectionInput.contains(".")
                        || ((TuningTableModel) table.getModel()).precision == 0)) {
                        return;
                    }
                    applySelectionInput(selectionInput + character);
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
            value = text.isEmpty() ? 0 : Double.parseDouble(text);
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
            if (Double.isFinite(Double.parseDouble(field.getText().trim()))) {
                return true;
            }
        } catch (NumberFormatException ignored) {
        }
        field.setBorder(BorderFactory.createLineBorder(Color.RED));
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

    private Action tableShortcut(Action action) {
        return new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (!table.isEditing() && xAxisEditingColumn < 0) {
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

    private void handleTableWheel(MouseWheelEvent event) {
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
                double value = Double.parseDouble(valueField.getText());
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
        if (field.isPresent() && field.get() instanceof ArrayIniField) {
            ArrayIniField array = (ArrayIniField) field.get();
            if (array.getRows() == 1 || array.getCols() == 1) {
                return array;
            }
        }
        return null;
    }

    private void setAxisValue(boolean xAxis, int index, Object value) {
        ArrayIniField field = xAxis ? xBinsField : yBinsField;
        if (viewMode || field == null || axisImageTarget == null || value == null
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
            number = Double.parseDouble(value.toString().trim());
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
        ConfigurationImage candidate = axisImageTarget.clone();
        ConfigurationImage.setScalarValue(candidate.getByteBuffer(offset, field.getType().getStorageSize()),
            field.getType(), Double.toString(number), com.rusefi.config.Field.NO_BIT_OFFSET, field.getMultiplier(), 0);
        double encoded = ConfigurationImageGetterSetter.getArrayValues(field, candidate)[row][column];
        if (!Double.isFinite(encoded) || Double.compare(bins[index], encoded) == 0) {
            return;
        }
        System.arraycopy(candidate.getContent(), offset, axisImageTarget.getContent(), offset,
            field.getType().getStorageSize());
        bins[index] = encoded;
        if (xAxis) {
            table.getColumnModel().getColumn(index + 1).setHeaderValue(model.getColumnName(index + 1));
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
            double delta = Double.parseDouble(deltaField.getText()) * sign;
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

        // X and Y bins (RPM and load) are on page 1
        Double[] storedX = extractAxisBins(iniFile, iniTable.getXBinsConstant(), axisImage);
        Double[] storedY = extractAxisBins(iniFile, iniTable.getYBinsConstant(), axisImage);

        currentIni = iniFile;
        currentTableName = tableName;
        orientationNode = orientationConfig.getChild(tableName);
        flipAxes = orientationNode.getBoolProperty("flip_xy");
        invertX = orientationNode.getBoolProperty("invert_x");
        invertY = orientationNode.getBoolProperty("invert_y");
        Double[][] dataValues = orientData(storedValues);
        Double[] xBins = orientAxis(flipAxes ? storedY : storedX, invertX);
        Double[] yBins = orientAxis(flipAxes ? storedX : storedY, invertY);

        calculateMinMax(dataValues);

        this.imageTarget = zImage;
        this.zBinsField = ltft;
        this.axisImageTarget = axisImage;
        this.xBinsField = editableAxisField(iniFile,
            flipAxes ? iniTable.getYBinsConstant() : iniTable.getXBinsConstant());
        this.yBinsField = editableAxisField(iniFile,
            flipAxes ? iniTable.getXBinsConstant() : iniTable.getYBinsConstant());

        TuningTableModel model = new TuningTableModel(dataValues, xBins, yBins, precision, !viewMode,
            edited -> commitEdit(edited, table.getSelectedRows(), table.getSelectedColumns()),
            flipAxes ? "RPM \\ Load" : "Load \\ RPM");
        if (!viewMode && axisImage != null && yBinsField != null) {
            model.onYAxisEdit = (index, value) -> setAxisValue(false, index, value);
        }
        model.xPrecision = xBinsField == null ? precision : IniField.parseDigits(xBinsField.getDigits());
        model.yPrecision = yBinsField == null ? precision : IniField.parseDigits(yBinsField.getDigits());
        String xLabel = plainLabel(flipAxes ? iniTable.getYLabel() : iniTable.getXLabel());
        String yLabel = plainLabel(flipAxes ? iniTable.getXLabel() : iniTable.getYLabel());
        String zLabel = plainLabel(ltft.getUnits());
        surface3DView.setAxisTitles(xLabel, yLabel, zLabel);
        mini3DView.setAxisTitles(xLabel, yLabel, zLabel);
        table.setModel(model);
        table.clearSelection();
        updateSurfaceSelection();
        updateSurfaces(dataValues, xBins, yBins);
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
        displayTable(info.getIniFile(), tableName, new ConfigurationImage(zBinsBuffer), image);
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
        Optional<IniField> binsField = iniFile.findIniField(binConstant);
        if (!binsField.isPresent() || !(binsField.get() instanceof ArrayIniField)) {
            return null;
        }

        ArrayIniField field = (ArrayIniField) binsField.get();
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
                number = Double.parseDouble(value.toString().trim());
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

        private String formatNumber(Object value) {
            return formatNumber(value, precision);
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
            Component c = super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
            if (column == 0) {
                c.setBackground(isActiveValueCell() && row == activeRow()
                    ? axisHighlight(table.getBackground()) : table.getBackground());
                c.setForeground(table.getForeground());
                return c;
            }
            if (isSelected) {
                c.setBackground(Color.ORANGE);
                c.setForeground(Color.BLACK);
                return c;
            }

            if (value instanceof String) {
                String strValue = (String) value;
                if (!strValue.isEmpty()) {
                    try {
                        double val = Double.parseDouble(strValue);
                        applyGradient(c, val);
                    } catch (NumberFormatException ignored) {
                    }
                }
            }
            return c;
        }

        private void applyGradient(Component c, double value) {
            Color background = CalibrationColorPalette.background(value, minValue, maxValue);
            c.setBackground(background);
            c.setForeground(CalibrationColorPalette.foreground(background));
        }
    }
}
