package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.StringFormatter;
import com.rusefi.maintenance.CalibrationsInfo;

import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellRenderer;
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
import java.util.Optional;
import java.util.function.Consumer;

public class TuningTableView {
    private static final int TOOLBAR_GAP = 6;
    private static final double SMOOTHING_FACTOR = 0.25;
    private final boolean viewMode;
    private final JTable table = new JTable();
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
    private Runnable onEdit;

    public TuningTableView(String title) {
        this(title, false);
    }

    public TuningTableView(String title, boolean viewMode) {
        this.title = title;
        this.viewMode = viewMode;
        table.getTableHeader().setReorderingAllowed(false);
        table.setSelectionBackground(Color.ORANGE);
        table.setDefaultRenderer(Object.class, new GradientRenderer());
        table.setCellSelectionEnabled(true);
        table.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
        table.setToolTipText(viewMode
            ? "Arrows or mouse wheel to move; Shift+arrows to select; Ctrl+A to select all"
            : "Arrows or mouse wheel to move; Enter/F2 to edit; Enter to accept; Esc to cancel; Tab/Shift+Tab to accept and move");
        installAxisHighlighting();
        installKeyboardEditing();
        table.addMouseWheelListener(this::moveValueCellWithWheel);

        table.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
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

    private void updatePreviewVisibility() {
        // Keep the grid usable in small dialogs. Large tables can scroll above the preview.
        int gridHeight = Math.min(320, table.getRowCount() * table.getRowHeight())
            + table.getTableHeader().getPreferredSize().height;
        int availableHeight = tableContainer.getHeight();
        JViewport viewport = (JViewport) SwingUtilities.getAncestorOfClass(JViewport.class, tableContainer);
        if (viewport != null) {
            // Scrollable dialogs retain preferred height instead of filling a tall viewport.
            // Measure from the table's top in the dialog, independent of scrolling position.
            Point top = SwingUtilities.convertPoint(tableContainer, 0, 0, viewport.getView());
            availableHeight = viewport.getExtentSize().height - Math.max(0, top.y);
        }
        boolean visible = previewEligible && !full3D && tableContainer.getWidth() >= 320
            && availableHeight >= gridHeight + 160;
        boolean sizeChanged = false;
        if (visible) {
            int previewHeight = availableHeight - gridHeight;
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
        // Editing is explicit, so letter shortcuts still operate on the selection.
        table.putClientProperty("JTable.autoStartsEdit", false);
        table.putClientProperty("terminateEditOnFocusLost", true);
        JTextField field = new JTextField();
        field.setHorizontalAlignment(SwingConstants.CENTER);
        DefaultCellEditor editor = new DefaultCellEditor(field) {
            @Override
            public boolean stopCellEditing() {
                try {
                    if (!Double.isFinite(Double.parseDouble(field.getText().trim()))) {
                        throw new NumberFormatException();
                    }
                } catch (NumberFormatException e) {
                    field.setBorder(BorderFactory.createLineBorder(Color.RED));
                    field.setToolTipText("Enter a finite number, or press Esc to cancel");
                    return false;
                }
                return super.stopCellEditing();
            }

            @Override
            public Component getTableCellEditorComponent(JTable source, Object value, boolean selected, int row, int column) {
                Component component = super.getTableCellEditorComponent(source, value, selected, row, column);
                field.setBorder(UIManager.getBorder("TextField.border"));
                field.setToolTipText("Enter to accept; Esc to cancel; Tab/Shift+Tab to accept and move");
                field.selectAll();
                return component;
            }
        };
        table.setDefaultEditor(Object.class, editor);
        Action edit = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (viewMode) {
                    return;
                }
                if (table.isEditing()) {
                    if (table.getCellEditor().stopCellEditing()) {
                        table.requestFocusInWindow();
                    }
                    return;
                }
                if (table.getSelectedRow() == -1 && table.getRowCount() > 0 && table.getColumnCount() > 1) {
                    table.changeSelection(0, 1, false, false);
                }
                if (isActiveValueCell() && table.editCellAt(activeRow(), activeColumn())) {
                    table.getEditorComponent().requestFocusInWindow();
                }
            }
        };
        bindKey(table, JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT, "ENTER", "editValue", edit);
        bindKey(table, JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT, "F2", "editValue", edit);
        bindKey(field, JComponent.WHEN_FOCUSED, "ENTER", "acceptValue", edit);
        bindKey(field, JComponent.WHEN_FOCUSED, "ESCAPE", "cancelValue", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                editor.cancelCellEditing();
                table.requestFocusInWindow();
            }
        });
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
        }
    }

    private void bindKey(JComponent component, int condition, String key, String name, Action action) {
        component.getInputMap(condition).put(KeyStroke.getKeyStroke(key), name);
        component.getActionMap().put(name, action);
    }

    private Action tableShortcut(Action action) {
        return new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (!table.isEditing()) {
                    action.actionPerformed(e);
                }
            }
        };
    }

    private void moveValueCell(boolean backwards) {
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

    private void moveValueCellWithWheel(MouseWheelEvent event) {
        // Handle the wheel here so the scroll pane doesn't also scroll independently.
        event.consume();
        if (table.isEditing() || !isActiveValueCell() || event.getWheelRotation() == 0) {
            return;
        }
        int row = Math.max(0, Math.min(table.getRowCount() - 1, activeRow() + event.getWheelRotation()));
        if (row != activeRow()) {
            // changeSelection also scrolls the selected cell into view.
            table.changeSelection(row, activeColumn(), false, false);
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

    private boolean writeBackZBins(TuningTableModel model) {
        if (imageTarget == null || zBinsField == null) {
            return false;
        }
        ConfigurationImageGetterSetter.setArrayValues(zBinsField, imageTarget, model.data);
        Double[][] encodedValues = ConfigurationImageGetterSetter.getArrayValues(zBinsField, imageTarget);
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
        Double[][] dataValues = ConfigurationImageGetterSetter.getArrayValues(ltft, zImage);

        // X and Y bins (RPM and load) are on page 1
        Double[] xBins = extractAxisBins(iniFile, iniTable.getXBinsConstant(), axisImage);
        Double[] yBins = extractAxisBins(iniFile, iniTable.getYBinsConstant(), axisImage);

        calculateMinMax(dataValues);

        this.imageTarget = zImage;
        this.zBinsField = ltft;

        table.setModel(new TuningTableModel(dataValues, xBins, yBins, precision, !viewMode,
            model -> commitEdit(model, table.getSelectedRows(), table.getSelectedColumns())));
        table.clearSelection();
        updateSurfaces(dataValues, xBins, yBins);
    }

    public void displayTable(IniFileModel iniFile, String tableName, ConfigurationImage image) {
        displayTable(iniFile, tableName, image, image);
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

        public TuningTableModel(Double[][] data, Double[] xBins, Double[] yBins, int precision) {
            this(data, xBins, yBins, precision, true, null);
        }

        private TuningTableModel(Double[][] data, Double[] xBins, Double[] yBins, int precision,
                                 boolean editable, Consumer<TuningTableModel> onCellEdit) {
            this.data = data;
            this.xBins = xBins;
            this.yBins = yBins;
            this.precision = precision;
            this.editable = editable;
            this.onCellEdit = onCellEdit;
        }

        @Override
        public boolean isCellEditable(int row, int column) {
            return editable && column > 0;
        }

        @Override
        public void setValueAt(Object value, int row, int column) {
            if (!isCellEditable(row, column)) {
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
            if (column == 0) return "Load \\ RPM";
            if (xBins != null && column - 1 < xBins.length) {
                return formatNumber(xBins[column - 1]);
            }
            return "Col " + column;
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) {
            // traditionally low values are displayed on the bottom and high line on top
            int reversedRowIndex = data.length - 1 - rowIndex;
            if (columnIndex == 0) {
                if (yBins != null && reversedRowIndex < yBins.length) {
                    return formatNumber(yBins[reversedRowIndex]);
                }
                return "Row " + rowIndex;
            }
            return formatNumber(data[reversedRowIndex][columnIndex - 1]);
        }

        private String formatNumber(Object value) {
            if (value instanceof Number) {
                return String.format("%." + precision + "f", ((Number) value).doubleValue())
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
            if (maxValue <= minValue) {
                c.setBackground(Color.WHITE);
                c.setForeground(Color.BLACK);
                return;
            }
            double ratio = (value - minValue) / (maxValue - minValue);
            ratio = Math.max(0, Math.min(1, ratio));

            // Pastel Blue (173, 216, 230) to Pastel Red (255, 182, 193)
            int r1 = 173, g1 = 216, b1 = 230; // Min value color
            int r2 = 255, g2 = 182, b2 = 193; // Max value color

            int red = (int) (r1 + ratio * (r2 - r1));
            int green = (int) (g1 + ratio * (g2 - g1));
            int blue = (int) (b1 + ratio * (b2 - b1));
            Color background = new Color(red, green, blue);
            c.setBackground(background);
            // Ensure text is readable - pastel colors are generally light, so black text should be fine
            double brightness = (red * 0.299 + green * 0.587 + blue * 0.114) / 255.0;
            c.setForeground(brightness < 0.5 ? Color.WHITE : Color.BLACK);
        }
    }
}
