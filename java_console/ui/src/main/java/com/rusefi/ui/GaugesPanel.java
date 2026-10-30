package com.rusefi.ui;

import com.rusefi.core.ISensorCentral;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.WellKnownGauges;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.ui.widgets.AnyCommand;
import com.formdev.flatlaf.FlatClientProperties;
import com.rusefi.ui.widgets.EditIcon;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.DetachedSensor;
import com.rusefi.ui.widgets.GaugeMenuSearch;
import com.rusefi.ui.widgets.SensorGauge;
import com.rusefi.ui.widgets.tune.IndicatorStrip;
import com.rusefi.ui.widgets.tune.IndicatorStripSettings;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import javax.swing.event.MenuEvent;
import javax.swing.event.MenuListener;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Locale;


/**
 * Date: 2/5/13
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see GaugesGridElement
 */
public class GaugesPanel {
    private static final String[] DEFAULT_LAYOUT = {
        "RPMGauge",
        "internalMcuTemperatureGauge",
        "CLTGauge",
        "IATGauge",
        "TPSGauge",
        "MAPGauge",
        "lastErrorCodeGauge",
        "VBattGauge",
        "VSSGauge",
    };
    public static final String DISABLE_LOGS = "DISABLE_LOGS";
    private static final int DEFAULT_ROWS = 3;
    private static final int DEFAULT_COLUMNS = 3;
    static final String EDIT_DASHBOARD = "Edit dashboard";
    static final String EDIT_LAYOUT = "Arrange gauges";
    static final String DONE_EDITING = "Done";
    static final String UNDO = "Undo";
    static final String REDO = "Redo";
    static final String RESET_ARRANGE = "Reset";
    static final String RESET_LAYOUT = "Reset Layout";
    static final String SAVE_LAYOUT = "Save layout…";
    static final String LOAD_LAYOUT = "Load layout…";
    static final String RESET = "Reset layout…";
    static final String INDICATORS = "Indicators";

    private final JPanel content = new JPanel(new BorderLayout());
    private final GaugesGrid gauges;
    private final FreeGaugeArea freeArea;
    private final JPanel layoutArea = new JPanel(new BorderLayout());
    /*
     * Dashboard editing lives in one "Edit dashboard" menu; the toolbar only shows the current
     * layout and, while gauges are being arranged, Undo / Redo / Reset and a Done button to leave that mode.
     */
    private final JPopupMenu editMenu = new JPopupMenu();
    private final JRadioButtonMenuItem gridModeItem = new JRadioButtonMenuItem("Grid layout");
    private final JRadioButtonMenuItem freeModeItem = new JRadioButtonMenuItem("Free layout");
    private final JMenu gridSizeMenu = new JMenu("Grid size");
    private final JMenuItem addGaugeItem = new JMenuItem("Add gauge…");
    private final JMenuItem editLayoutItem = new JMenuItem(EDIT_LAYOUT);
    private final JMenu indicatorsMenu = new JMenu(INDICATORS);
    private final JButton editDashboardButton = new JButton(EDIT_DASHBOARD, new EditIcon());
    private final JButton doneEditingButton = new JButton(DONE_EDITING);
    private final JButton undoButton = new JButton(UNDO);
    private final JButton redoButton = new JButton(REDO);
    private final JButton resetArrangeButton = new JButton(RESET_ARRANGE);
    private final JLabel layoutModeLabel = new JLabel();
    private final Node config;
    private final UIContext uiContext;
    /** Tracked so we can call destroy() before replacing on grid resize. */
    private final List<GaugesGridElement> gaugeElements = new ArrayList<>();
    private final ISensorCentral.ListenerToken rpmDemandToken;
    private final IndicatorStrip indicatorStrip;
    private RpmLabel rpmLabel;
    private boolean active;

    public GaugesPanel(UIContext uiContext, final Node config) {
        this.uiContext = uiContext;
        gauges = new GaugesGrid(DEFAULT_ROWS, DEFAULT_COLUMNS);
        this.config = config;
        freeArea = new FreeGaugeArea(uiContext, config, this::updateFreeControls);
        rpmDemandToken = SensorCentral.getInstance().addListener(
            WellKnownGauges.RPMGauge.getOutputChannelName(), value -> { });
        rpmDemandToken.setActive(false);
        indicatorStrip = new IndicatorStrip(() -> uiContext.iniFileState.getIniFileModel(),
            uiContext.indicatorStripSettings);

        int rows = config.getIntProperty(GaugeLayout.ROWS, DEFAULT_ROWS);
        int columns = config.getIntProperty(GaugeLayout.COLUMNS, DEFAULT_COLUMNS);
        saveConfig(rows, columns);

        content.add(createTopPanel(), BorderLayout.NORTH);
        showLayout();
        content.add(layoutArea, BorderLayout.CENTER);
    }

    @NotNull
    private JPanel createTopPanel() {
        // white toolbar with a divider below, matching the status strip at the bottom (WarningPanel)
        JPanel toolbar = new JPanel() {
            @Override
            protected void paintComponent(Graphics g) {
                Color background = UIManager.getColor("Console.Toolbar.background");
                g.setColor(background != null ? background : getBackground());
                g.fillRect(0, 0, getWidth(), getHeight());
                Color line = UIManager.getColor("Separator.foreground");
                g.setColor(line != null ? line : Color.LIGHT_GRAY);
                g.drawLine(0, getHeight() - 1, getWidth(), getHeight() - 1);
            }
        };
        toolbar.setLayout(new BoxLayout(toolbar, BoxLayout.Y_AXIS));
        // same right inset as the tab row above, so Edit dashboard stacks under Launch Wizard
        int rightInset = JoinedButtons.ROW_END_INSET - JoinedButtons.ROW_GAP;
        toolbar.setBorder(BorderFactory.createEmptyBorder(8, 0, 5, rightInset));

        JPanel commandRow = new JPanel(new BorderLayout());
        commandRow.setOpaque(false);
        commandRow.setBorder(BorderFactory.createEmptyBorder(0, 0, 4, 0));
        JPanel left = createLeftTopPanel();
        JPanel right = createRightTopPanel();
        left.setOpaque(false);
        right.setOpaque(false);
        commandRow.add(left, BorderLayout.CENTER);
        commandRow.add(right, BorderLayout.EAST);
        toolbar.add(commandRow);

        // the indicator chips, under the command bar; the strip's 6px gap plus 2 lines them up with the RPM label
        JComponent indicators = indicatorStrip.getContent();
        indicators.setBorder(BorderFactory.createEmptyBorder(0, 2, 0, 0));
        toolbar.add(indicators);
        return toolbar;
    }

    @NotNull
    private JPanel createRightTopPanel() {
        JPanel rightUpperPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT, JoinedButtons.ROW_GAP, 0));

        layoutModeLabel.setForeground(UIManager.getColor("Label.disabledForeground"));
        rightUpperPanel.add(layoutModeLabel);

        // shown only while gauges are being arranged; return focus to the editor so its keys keep working
        undoButton.setToolTipText("Undo the last layout change (Ctrl+Z)");
        undoButton.addActionListener(e -> {
            freeArea.undo();
            freeArea.getEditor().requestFocusInWindow();
        });
        redoButton.setToolTipText("Redo the last undone layout change (Ctrl+Y)");
        redoButton.addActionListener(e -> {
            freeArea.redo();
            freeArea.getEditor().requestFocusInWindow();
        });
        resetArrangeButton.setToolTipText("Return to the grid you started from; Undo brings your arrangement back");
        resetArrangeButton.addActionListener(e -> {
            freeArea.resetToGrid();
            freeArea.getEditor().requestFocusInWindow();
        });
        rightUpperPanel.add(new JoinedButtons(undoButton, redoButton, resetArrangeButton));

        // shown only while gauges are being arranged: the way out of that mode (Esc works too)
        // a plain button (not a JoinedButtons group) so it shows and hides in step with the editor
        doneEditingButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        doneEditingButton.setToolTipText("Finish arranging gauges (Esc)");
        doneEditingButton.setPreferredSize(new Dimension(
            doneEditingButton.getPreferredSize().width + 24, JoinedButtons.HEIGHT));
        doneEditingButton.addActionListener(e -> freeArea.setEditing(false));
        rightUpperPanel.add(doneEditingButton);

        gridModeItem.setToolTipText("Gauges fill a grid of rows and columns");
        freeModeItem.setToolTipText("Gauges can be placed anywhere and sized individually; the grid is kept");
        ButtonGroup modeGroup = new ButtonGroup();
        modeGroup.add(gridModeItem);
        modeGroup.add(freeModeItem);
        gridModeItem.addActionListener(e -> setFreeMode(false));
        freeModeItem.addActionListener(e -> setFreeMode(true));
        editMenu.add(gridModeItem);
        editMenu.add(freeModeItem);
        editMenu.addSeparator();

        gridSizeMenu.setToolTipText("Choose the number of gauge rows and columns");
        gridSizeMenu.add(new SizeSelectorPanel((row, column) -> {
            System.out.println("new size " + row + "/" + column);
            // close the whole menu once a size is picked
            MenuSelectionManager.defaultManager().clearSelectedPath();
            setSensorGridDimensions(row, column);
        }));
        editMenu.add(gridSizeMenu);

        addGaugeItem.setToolTipText("Add a gauge to the free layout");
        addGaugeItem.addActionListener(e ->
            createAddGaugeMenu().show(editDashboardButton, 0, editDashboardButton.getHeight()));
        editMenu.add(addGaugeItem);
        editLayoutItem.setToolTipText("Move, resize, stack and remove gauges");
        editLayoutItem.addActionListener(e -> freeArea.setEditing(true));
        editMenu.add(editLayoutItem);
        editMenu.addSeparator();

        // the same menu as right-clicking the indicator strip, filled when opened since it lists the hidden chips
        indicatorsMenu.setToolTipText("Show hidden indicators or reset their order; drag a chip to move it");
        indicatorsMenu.addMenuListener(new MenuListener() {
            @Override
            public void menuSelected(MenuEvent e) {
                indicatorsMenu.removeAll();
                indicatorStrip.fillMenu(indicatorsMenu, null);
            }

            @Override
            public void menuDeselected(MenuEvent e) {
            }

            @Override
            public void menuCanceled(MenuEvent e) {
            }
        });
        editMenu.add(indicatorsMenu);
        editMenu.addSeparator();

        JMenuItem saveLayout = new JMenuItem(SAVE_LAYOUT);
        saveLayout.setToolTipText("Save the gauge layout and graph settings to a file");
        saveLayout.addActionListener(e -> chooseLayoutFile(true));
        editMenu.add(saveLayout);

        JMenuItem loadLayout = new JMenuItem(LOAD_LAYOUT);
        loadLayout.setToolTipText("Replace the gauge layout with a saved one");
        loadLayout.addActionListener(e -> chooseLayoutFile(false));
        editMenu.add(loadLayout);

        JMenuItem resetLayout = new JMenuItem(RESET);
        resetLayout.setToolTipText("Grid: return to the default gauges. Free: return to the grid you started from");
        resetLayout.addActionListener(e -> confirmResetLayout());
        editMenu.add(resetLayout);

        editDashboardButton.setIconTextGap(8);
        editDashboardButton.setToolTipText("Layout, gauges and saved dashboards");
        editDashboardButton.addActionListener(e ->
            editMenu.show(editDashboardButton, 0, editDashboardButton.getHeight()));
        rightUpperPanel.add(new JoinedButtons(editDashboardButton));

        return rightUpperPanel;
    }

    private void chooseLayoutFile(boolean save) {
        JFileChooser chooser = new JFileChooser();
        chooser.setDialogTitle(save ? "Save Gauge Layout" : "Load Gauge Layout");
        chooser.setFileFilter(new FileNameExtensionFilter("Gauge layouts (*.gauges)", "gauges"));
        if (save) {
            chooser.setSelectedFile(new File("dashboard.gauges"));
        }
        int result = save ? chooser.showSaveDialog(content) : chooser.showOpenDialog(content);
        if (result != JFileChooser.APPROVE_OPTION) {
            return;
        }
        File file = chooser.getSelectedFile();
        if (save && !file.getName().toLowerCase(Locale.ROOT).endsWith(".gauges")) {
            file = new File(file.getPath() + ".gauges");
        }
        if (save && file.exists() && JOptionPane.showConfirmDialog(content,
            "Replace " + file.getName() + "?", "Save Gauge Layout",
            JOptionPane.YES_NO_OPTION) != JOptionPane.YES_OPTION) {
            return;
        }
        try {
            if (save) {
                saveLayout(file.toPath());
            } else {
                loadLayout(file.toPath());
                PersistentConfiguration.getConfig().save();
            }
        } catch (IOException e) {
            JOptionPane.showMessageDialog(content, "Could not " + (save ? "save" : "load")
                + " gauge layout: " + e.getMessage(), "Gauge Layout", JOptionPane.ERROR_MESSAGE);
        }
    }

    // Package-private so tests exercise the same file operations as the chooser.
    void saveLayout(Path file) throws IOException {
        GaugeLayout.save(file, config, uiContext.indicatorStripSettings);
    }

    void loadLayout(Path file) throws IOException {
        Node layout = GaugeLayout.load(file);
        // the indicator strip travels with the layout but is kept apart from the gauges
        Object indicators = layout.getConfig().remove(IndicatorStripSettings.NODE);
        if (indicators instanceof Node) {
            uiContext.indicatorStripSettings.replaceWith((Node) indicators);
        }
        // Leave warning and command preferences intact; discard old, including hidden, slots.
        // A version 1 file has no free layout, so it returns to Grid.
        config.getConfig().keySet().removeIf(key -> key.matches("element_\\d+") || key.equals(GaugeLayout.MODE));
        GaugeLayout.clearFreeLayout(config);
        config.getConfig().putAll(layout.getConfig());
        if (!GaugeLayout.isFreeMode(config)) {
            GaugeLayout.clearFreeLayout(config);
        }
        showLayout();
    }

    @NotNull
    private JPanel createLeftTopPanel() {
        JPanel leftUpperPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 8, 0));
        rpmLabel = new RpmLabel(uiContext, 2);
        leftUpperPanel.add(rpmLabel.getContent());
        AnyCommand command = AnyCommand.createField(uiContext, config, false, false);
        leftUpperPanel.add(command.getContent());
        return leftUpperPanel;
    }

    private void confirmResetLayout() {
        String question = GaugeLayout.isFreeMode(config)
            ? "Undo all free layout changes and return to the grid you started from?"
            : "Replace the gauges with the default " + DEFAULT_ROWS + "x" + DEFAULT_COLUMNS + " layout?";
        if (JOptionPane.showConfirmDialog(content, question, RESET_LAYOUT,
            JOptionPane.OK_CANCEL_OPTION) == JOptionPane.OK_OPTION) {
            resetLayout();
        }
    }

    /**
     * Grid: the default gauges in the default grid, each shown as a round gauge.
     * Free: a fresh copy of the current grid, which Free mode never changes, so this
     * undoes every free-layout edit and stays in Free mode; while arranging, Undo reverts it.
     */
    // Package-private so tests reset without the confirmation dialog.
    void resetLayout() {
        if (GaugeLayout.isFreeMode(config)) {
            freeArea.resetToGrid();
            return;
        }
        config.getConfig().keySet().removeIf(key -> key.matches("element_\\d+"));
        saveConfig(DEFAULT_ROWS, DEFAULT_COLUMNS);
        showLayout();
    }

    private void setSensorGridDimensions(int rows, int columns) {
        saveConfig(rows, columns);
        showLayout();
    }

    /** Switches between the grid and the free layout; Free always starts from the current grid. */
    void setFreeMode(boolean free) {
        if (free == GaugeLayout.isFreeMode(config)) {
            return;
        }
        if (free) {
            GaugeLayout.seedFreeFromGrid(config);
        } else {
            GaugeLayout.clearFreeLayout(config);
        }
        config.setProperty(GaugeLayout.MODE, free ? GaugeLayout.MODE_FREE : GaugeLayout.MODE_GRID);
        showLayout();
    }

    boolean isFreeMode() {
        return GaugeLayout.isFreeMode(config);
    }

    /** Rebuilds the gauges of the current mode from the configuration. */
    private void showLayout() {
        // Destroy old elements before discarding them so their ConnectionStatusLogic
        // listeners are removed and their widgets can be garbage-collected.
        for (GaugesGridElement element : gaugeElements) {
            element.destroy();
        }
        gaugeElements.clear();
        layoutArea.removeAll();
        freeArea.destroy();

        boolean free = GaugeLayout.isFreeMode(config);
        if (free) {
            freeArea.setActive(active);
            freeArea.rebuild(getDefaultLayout());
        } else {
            freeArea.setEditing(false);
            showGrid(config.getIntProperty(GaugeLayout.ROWS, DEFAULT_ROWS),
                config.getIntProperty(GaugeLayout.COLUMNS, DEFAULT_COLUMNS));
        }
        layoutArea.add(free ? freeArea.getPanel() : gauges.panel, BorderLayout.CENTER);
        updateFreeControls();
        layoutArea.revalidate();
        layoutArea.repaint();
    }

    private void showGrid(int rows, int columns) {
        gauges.setLayout(rows, columns);

        List<String> defaultLayout = getDefaultLayout();
        for (int i = 0; i < rows * columns; i++) {
            // sometimes grid is quite large so we shall be careful with default sensor index
            String defaultGaugeName = defaultLayout.get(Math.min(i, defaultLayout.size() - 1));
            GaugesGridElement element = GaugesGridElement.create(uiContext,
                config.getChild(GaugeLayout.GRID_ELEMENT + i), defaultGaugeName);
            element.setActive(active);
            gaugeElements.add(element);
            gauges.panel.add(element.getContent());
        }
    }

    /** Keeps the layout label, the Edit dashboard menu and the arranging buttons in step with the mode and the editor. */
    private void updateFreeControls() {
        boolean free = GaugeLayout.isFreeMode(config);
        boolean editing = freeArea.isEditing();
        layoutModeLabel.setText(free ? "Free layout" : "Grid layout · "
            + config.getIntProperty(GaugeLayout.ROWS, DEFAULT_ROWS) + " × "
            + config.getIntProperty(GaugeLayout.COLUMNS, DEFAULT_COLUMNS));
        gridModeItem.setSelected(!free);
        freeModeItem.setSelected(free);
        gridSizeMenu.setEnabled(!free);
        addGaugeItem.setEnabled(free);
        editLayoutItem.setEnabled(free && !editing);
        doneEditingButton.setVisible(editing);
        undoButton.setVisible(editing);
        redoButton.setVisible(editing);
        resetArrangeButton.setVisible(editing);
        undoButton.setEnabled(freeArea.canUndo());
        redoButton.setEnabled(freeArea.canRedo());
    }

    /** Gauge catalog with search; the chosen gauge is added to the free layout. */
    JPopupMenu createAddGaugeMenu() {
        JPopupMenu menu = new JPopupMenu();
        if (!freeArea.canAddGauge()) {
            JMenuItem full = new JMenuItem("The free layout holds at most " + GaugeLayout.MAX_FREE_GAUGES + " gauges");
            full.setEnabled(false);
            menu.add(full);
            return menu;
        }
        SensorGauge.addGaugeCatalog(uiContext, menu, freeArea::addGauge);
        GaugeMenuSearch.addTo(menu);
        return menu;
    }

    // Package-private so tests see the indicator strip under the command bar.
    IndicatorStrip getIndicatorStrip() {
        return indicatorStrip;
    }

    // Package-private so tests drive the free layout and its editor.
    FreeGaugeArea getFreeArea() {
        return freeArea;
    }

    JPanel getFreePanel() {
        return freeArea.getPanel();
    }

    JPopupMenu getEditMenu() {
        return editMenu;
    }

    JMenuItem getEditLayoutItem() {
        return editLayoutItem;
    }

    JMenuItem getAddGaugeItem() {
        return addGaugeItem;
    }

    JMenu getGridSizeMenu() {
        return gridSizeMenu;
    }

    JButton getDoneEditingButton() {
        return doneEditingButton;
    }

    JButton getUndoButton() {
        return undoButton;
    }

    JButton getRedoButton() {
        return redoButton;
    }

    JButton getResetArrangeButton() {
        return resetArrangeButton;
    }

    JLabel getLayoutModeLabel() {
        return layoutModeLabel;
    }

    private List<String> getDefaultLayout() {
        return Arrays.asList(DEFAULT_LAYOUT);
    }

    private void saveConfig(int rows, int columns) {
        config.setProperty(GaugeLayout.ROWS, rows);
        config.setProperty(GaugeLayout.COLUMNS, columns);
    }

    public JComponent getContent() {
        return content;
    }

    public void setActive(boolean active) {
        this.active = active;
        rpmDemandToken.setActive(active);
        indicatorStrip.setActive(active);
        for (GaugesGridElement element : gaugeElements) {
            element.setActive(active);
        }
        freeArea.setActive(active);
    }

    public void destroy() {
        for (GaugesGridElement element : gaugeElements) {
            element.destroy();
        }
        gaugeElements.clear();
        freeArea.destroy();
        rpmDemandToken.remove();
        indicatorStrip.destroy();
        rpmLabel.destroy();
    }

    public static class DetachedRepository {

        public static final String COUNT = "count";
        public static final String DETACHED = "detached";
        private final UIContext uiContext;
        private List<DetachedSensor> list = new ArrayList<>();
        private Node config;

        public DetachedRepository(UIContext uiContext) {

            this.uiContext = uiContext;
        }

        public void add(DetachedSensor detachedSensor) {
            list.add(detachedSensor);
            saveConfig();
        }

        public void remove(DetachedSensor detachedSensor) {
            list.remove(detachedSensor);
            saveConfig();
        }

        public void load() {
            int count = config.getIntProperty(COUNT, 0);
            for (int i = 0; i < count; i++) {
                DetachedSensor.create(uiContext, config.getChild(DETACHED + i));
            }
        }

        public void saveConfig() {
            config.setProperty(COUNT, list.size());
            int index = 0;
            for (DetachedSensor sensor : list) {
                sensor.saveConfig(config.getChild(DETACHED + index++));
            }
        }

        public void init(Node config) {
            this.config = config;
        }
    }
}
