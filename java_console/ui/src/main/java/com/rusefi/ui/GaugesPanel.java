package com.rusefi.ui;

import com.rusefi.core.ISensorCentral;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.WellKnownGauges;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.ui.widgets.AnyCommand;
import com.rusefi.ui.widgets.ChevronIcon;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.SegmentedControl;
import com.rusefi.ui.widgets.DetachedSensor;
import com.rusefi.ui.widgets.GaugeMenuSearch;
import com.rusefi.ui.widgets.SensorGauge;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
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
    static final String EDIT_LAYOUT = "Edit Layout";
    static final String DONE_EDITING = "Done";
    static final String RESET_LAYOUT = "Reset Layout";
    // no "…": after the "Layout" label a shortened "Save…" read as a truncated "Save Layout"
    static final String SAVE_LAYOUT = "Save";
    static final String LOAD_LAYOUT = "Load";
    static final String RESET = "Reset";

    private final JPanel content = new JPanel(new BorderLayout());
    private final GaugesGrid gauges;
    private final FreeGaugeArea freeArea;
    private final JButton addGaugeButton = new JButton("Add Gauge");
    private final JToggleButton editLayoutButton = new JToggleButton(EDIT_LAYOUT);
    private final JPanel layoutArea = new JPanel(new BorderLayout());
    private final JToggleButton gridModeButton = SegmentedControl.segment("Grid");
    private final JToggleButton freeModeButton = SegmentedControl.segment("Free");
    private JButton gridSizeButton;
    private final Node config;
    private final UIContext uiContext;
    /** Tracked so we can call destroy() before replacing on grid resize. */
    private final List<GaugesGridElement> gaugeElements = new ArrayList<>();
    private final ISensorCentral.ListenerToken rpmDemandToken;
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

        int rows = config.getIntProperty(GaugeLayout.ROWS, DEFAULT_ROWS);
        int columns = config.getIntProperty(GaugeLayout.COLUMNS, DEFAULT_COLUMNS);
        saveConfig(rows, columns);

        content.add(createTopPanel(), BorderLayout.NORTH);
        showLayout();
        content.add(layoutArea, BorderLayout.CENTER);

        content.add(new WarningPanel(config).getPanel(config), BorderLayout.SOUTH);
    }

    @NotNull
    private JPanel createTopPanel() {
        JPanel upperPanel = new JPanel(new BorderLayout());
        upperPanel.add(createLeftTopPanel(), BorderLayout.CENTER);
        upperPanel.add(createRightTopPanel(), BorderLayout.EAST);
        return upperPanel;
    }

    @NotNull
    private JPanel createRightTopPanel() {
        JPanel rightUpperPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT, 8, 0));

        JLabel layoutLabel = new JLabel("Layout");
        layoutLabel.setForeground(UIManager.getColor("Label.disabledForeground"));
        rightUpperPanel.add(layoutLabel);
        gridModeButton.setToolTipText("Gauges fill a grid of rows and columns");
        freeModeButton.setToolTipText("Gauges can be placed anywhere and sized individually; the grid is kept");
        ButtonGroup modeGroup = new ButtonGroup();
        modeGroup.add(gridModeButton);
        modeGroup.add(freeModeButton);
        gridModeButton.addActionListener(e -> setFreeMode(false));
        freeModeButton.addActionListener(e -> setFreeMode(true));
        rightUpperPanel.add(new SegmentedControl(gridModeButton, freeModeButton));

        addGaugeButton.setToolTipText("Add a gauge to the free layout");
        addGaugeButton.addActionListener(e -> createAddGaugeMenu().show(addGaugeButton, 0, addGaugeButton.getHeight()));

        editLayoutButton.setToolTipText("Move, resize, stack and remove gauges");
        editLayoutButton.addActionListener(e -> freeArea.setEditing(editLayoutButton.isSelected()));
        // Free-mode tools; the group hides itself in Grid mode, when both buttons are hidden
        rightUpperPanel.add(new JoinedButtons(addGaugeButton, editLayoutButton));

        // short names: the "Layout" label and the tooltips say what is saved, loaded or reset
        JButton saveLayout = new JButton(SAVE_LAYOUT);
        saveLayout.setToolTipText("Save the gauge layout and graph settings to a file");
        saveLayout.addActionListener(e -> chooseLayoutFile(true));

        JButton loadLayout = new JButton(LOAD_LAYOUT);
        loadLayout.setToolTipText("Replace the gauge layout with a saved one");
        loadLayout.addActionListener(e -> chooseLayoutFile(false));

        JButton resetLayout = new JButton(RESET);
        resetLayout.setToolTipText("Reset layout. Grid: return to the default gauges. Free: return to the grid you started from");
        resetLayout.addActionListener(e -> confirmResetLayout());
        rightUpperPanel.add(new JoinedButtons(saveLayout, loadLayout, resetLayout));

        final JPopupMenu selectorMenu = new JPopupMenu();
        selectorMenu.add(new SizeSelectorPanel((row, column) -> {
            System.out.println("new size " + row + "/" + column);
            setSensorGridDimensions(row, column);
        }));

        // shows the current size, e.g. "Grid 3 × 3", and opens the size picker
        JButton selector = new JButton(gridSizeText(), new ChevronIcon());
        selector.setHorizontalTextPosition(SwingConstants.LEADING);
        selector.setIconTextGap(8);
        gridSizeButton = selector;
        selector.setToolTipText("Choose the number of gauge rows and columns");
        selector.addActionListener(e -> {
            Component c = (Component) e.getSource();
            selectorMenu.show(c, -1, c.getHeight());
        });
        rightUpperPanel.add(new JoinedButtons(selector));


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
        GaugeLayout.save(file, config);
    }

    void loadLayout(Path file) throws IOException {
        Node layout = GaugeLayout.load(file);
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
     * undoes every free-layout edit and stays in Free mode.
     */
    // Package-private so tests reset without the confirmation dialog.
    void resetLayout() {
        if (GaugeLayout.isFreeMode(config)) {
            GaugeLayout.seedFreeFromGrid(config);
        } else {
            config.getConfig().keySet().removeIf(key -> key.matches("element_\\d+"));
            saveConfig(DEFAULT_ROWS, DEFAULT_COLUMNS);
        }
        showLayout();
    }

    String gridSizeText() {
        return "Grid " + config.getIntProperty(GaugeLayout.ROWS, DEFAULT_ROWS)
            + " × " + config.getIntProperty(GaugeLayout.COLUMNS, DEFAULT_COLUMNS);
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
        gridModeButton.setSelected(!free);
        freeModeButton.setSelected(free);
        gridSizeButton.setVisible(!free);
        gridSizeButton.setText(gridSizeText());
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

    /** Keeps the free-layout buttons in step with the mode and the editor. */
    private void updateFreeControls() {
        boolean free = GaugeLayout.isFreeMode(config);
        addGaugeButton.setVisible(free);
        editLayoutButton.setVisible(free);
        editLayoutButton.setSelected(freeArea.isEditing());
        editLayoutButton.setText(freeArea.isEditing() ? DONE_EDITING : EDIT_LAYOUT);
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

    // Package-private so tests drive the free layout and its editor.
    FreeGaugeArea getFreeArea() {
        return freeArea;
    }

    JPanel getFreePanel() {
        return freeArea.getPanel();
    }

    JToggleButton getEditLayoutButton() {
        return editLayoutButton;
    }

    JButton getAddGaugeButton() {
        return addGaugeButton;
    }

    JButton getGridSizeButton() {
        return gridSizeButton;
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
