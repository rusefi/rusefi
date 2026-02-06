package com.rusefi.ui;

import com.devexperts.logging.FileLogger;
import com.devexperts.logging.Logging;
import com.rusefi.core.Sensor;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.AnyCommand;
import com.rusefi.ui.widgets.DetachedSensor;
import com.rusefi.ui.widgets.PopupMenuButton;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static com.devexperts.logging.Logging.getLogging;


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
    private static final String GAUGES_ROWS = "gauges_rows";
    private static final String GAUGES_COLUMNS = "gauges_cols";
    private static final String SHOW_MESSAGES = "show_messages";
    private static final String SHOW_RPM = "show_rpm";
    private static final String SPLIT_LOCATION = "SPLIT_LOCATION";
    public static final String DISABLE_LOGS = "DISABLE_LOGS";
    private static final int DEFAULT_ROWS = 3;
    private static final int DEFAULT_COLUMNS = 3;
    public static boolean IS_PAUSED; // dirty but works for not

    private final JPanel content = new JPanel(new BorderLayout());
    private final GaugesGrid gauges;
    private final Node config;
    private final UIContext uiContext;

    private boolean showRpmPanel;
    private boolean showMessagesPanel;
    private final JPanel lowerRpmPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 0, 0));
    /**
     * this panel is displayed on the right
     */
    private final JPanel messagesPanel = new JPanel(new BorderLayout());
    private final JSplitPane middleSplitPanel;

    public GaugesPanel(UIContext uiContext, final Node config) {
        this.uiContext = uiContext;
        gauges = new GaugesGrid(DEFAULT_ROWS, DEFAULT_COLUMNS);
        this.config = config;
        showRpmPanel = config.getBoolProperty(SHOW_RPM, true);
        showMessagesPanel = config.getBoolProperty(SHOW_MESSAGES, true);

        prepareMessagesPanel();

        lowerRpmPanel.add(new RpmLabel(uiContext, 15, false).getContent());

        int rows = config.getIntProperty(GAUGES_ROWS, DEFAULT_ROWS);
        int columns = config.getIntProperty(GAUGES_COLUMNS, DEFAULT_COLUMNS);

        setSensorGridDimensions(rows, columns);

        middleSplitPanel = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, createMiddleLeftPanel(), messagesPanel);

        content.add(createTopPanel(config), BorderLayout.NORTH);

        content.add(middleSplitPanel, BorderLayout.CENTER);

        content.add(new WarningPanel(config).getPanel(config), BorderLayout.SOUTH);

        applyShowFlags();
        final int splitLocation = config.getIntProperty(SPLIT_LOCATION, -1);
        if (splitLocation != -1) {
            // this does not work. maybe because panel is not displayed yet? todo: fix it so that splitter location
            // would be persisted in the config
            middleSplitPanel.setDividerLocation(splitLocation);
        }
        middleSplitPanel.addPropertyChangeListener(JSplitPane.DIVIDER_LOCATION_PROPERTY, new PropertyChangeListener() {
            @Override
            public void propertyChange(PropertyChangeEvent pce) {
                config.setProperty(SPLIT_LOCATION, middleSplitPanel.getDividerLocation());
            }
        });
    }

    @NotNull
    private JPanel createMiddleLeftPanel() {
        JPanel middleLeftPanel = new JPanel(new BorderLayout());
        middleLeftPanel.add(gauges.panel, BorderLayout.CENTER);
        middleLeftPanel.add(lowerRpmPanel, BorderLayout.SOUTH);
        return middleLeftPanel;
    }

    @NotNull
    private JPanel createTopPanel(Node config) {
        JPanel upperPanel = new JPanel(new BorderLayout());
        upperPanel.add(createLeftTopPanel(), BorderLayout.CENTER);
        upperPanel.add(createRightTopPanel(config), BorderLayout.EAST);
        return upperPanel;
    }

    @NotNull
    private JPanel createRightTopPanel(Node config) {
        JPanel rightUpperPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT, 5, 0));

        final JPopupMenu selectorMenu = new JPopupMenu();
        selectorMenu.add(new SizeSelectorPanel((row, column) -> {
            System.out.println("new size " + row + "/" + column);
            setSensorGridDimensions(row, column);
        }));

        JButton selector = new JButton("O");
        selector.addActionListener(e -> {
            Component c = (Component) e.getSource();
            selectorMenu.show(c, -1, c.getHeight());
        });
        rightUpperPanel.add(selector);

        JButton menuButton = new PopupMenuButton("#", createMenu(config));
        rightUpperPanel.add(menuButton);
        return rightUpperPanel;
    }

    @NotNull
    private JPopupMenu createMenu(final Node config) {
        JPopupMenu menu = new JPopupMenu();

        final JCheckBoxMenuItem showRpmItem = new JCheckBoxMenuItem("Show RPM");
        final JCheckBoxMenuItem showCommandsItem = new JCheckBoxMenuItem("Show Commands");
        showRpmItem.setSelected(showRpmPanel);
        ActionListener showCheckboxListener = e -> {
            showRpmPanel = showRpmItem.isSelected();
            showMessagesPanel = showCommandsItem.isSelected();
            config.setProperty(SHOW_RPM, showRpmPanel);
            config.setProperty(SHOW_MESSAGES, showMessagesPanel);
            applyShowFlags();
            // todo: this is not needed if we show/hide RPM panel. TODO: split into two different listeners
            middleSplitPanel.setDividerLocation(0.5);
        };
        showRpmItem.addActionListener(showCheckboxListener);
        showCommandsItem.addActionListener(showCheckboxListener);

        menu.add(showRpmItem);
        showCommandsItem.setSelected(showMessagesPanel);
        menu.add(showCommandsItem);
        menu.add(new JPopupMenu.Separator());
        menu.add(new JPopupMenu("Reset Config")); // todo looks like not working
        return menu;
    }

    private void prepareMessagesPanel() {
        MessagesPanel mp = new MessagesPanel(null, config);
        messagesPanel.add(BorderLayout.NORTH, mp.getButtonPanel());
        messagesPanel.add(BorderLayout.CENTER, mp.getMessagesScroll());
    }

    @NotNull
    private JPanel createLeftTopPanel() {
        JPanel leftUpperPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 0));
        leftUpperPanel.add(createPauseButton());
        leftUpperPanel.add(createSaveImageButton());
        leftUpperPanel.add(new RpmLabel(uiContext, 2).getContent());
        AnyCommand command = AnyCommand.createField(uiContext, config, false, false);
        leftUpperPanel.add(command.getContent());
        return leftUpperPanel;
    }

    private Component createPauseButton() {
        String suffix = " Gauges";
        final JButton pauseButton = UiUtils.createPauseButton(suffix);
        pauseButton.addActionListener(e -> {
            IS_PAUSED = !IS_PAUSED;
            UiUtils.setPauseButtonText(pauseButton, IS_PAUSED, suffix);
        });
        return pauseButton;
    }

    @NotNull
    private JButton createSaveImageButton() {
        JButton saveImageButton = UiUtils.createSaveImageButton();
        saveImageButton.addActionListener(e -> {
            String fileName = FileLogger.getDate() + "_gauges.png";

            UiUtils.saveImageWithPrompt(fileName, content, gauges.panel);
        });
        return saveImageButton;
    }

    private void setSensorGridDimensions(int rows, int columns) {
        gauges.setLayout(rows, columns);

        List<String> defaultLayout = getDefaultLayout();
        for (int i = 0; i < rows * columns; i++) {
            // sometimes grid is quite large so we shall be careful with default sensor index
            String defaultGaugeName = defaultLayout.get(Math.min(i, defaultLayout.size() - 1));
            Component element = GaugesGridElement.create(uiContext, config.getChild("element_" + i), defaultGaugeName);

            gauges.panel.add(element);
        }

        saveConfig(rows, columns);
    }

    private List<String> getDefaultLayout() {
        return Arrays.asList(DEFAULT_LAYOUT);
    }

    private void saveConfig(int rows, int columns) {
        config.setProperty(GAUGES_ROWS, rows);
        config.setProperty(GAUGES_COLUMNS, columns);
    }

    private void applyShowFlags() {
        lowerRpmPanel.setVisible(showRpmPanel);
        messagesPanel.setVisible(showMessagesPanel);
    }

    public JComponent getContent() {
        return content;
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
