package com.rusefi.ui;

import com.rusefi.FileLog;
import com.rusefi.PaneSettings;
import com.rusefi.core.Sensor;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.AnyCommand;
import com.rusefi.ui.widgets.DetachedSensor;
import com.rusefi.ui.widgets.PopupMenuButton;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.ArrayList;
import java.util.List;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

/**
 * Date: 2/5/13
 * (c) Andrey Belomutskiy
 *
 * @see GaugesGridElement
 */
public class GaugesPanel {
    private static final Sensor[] DEFAULT_LAYOUT = {
            Sensor.RPM,
            Sensor.MAF,
            Sensor.CLT,
            Sensor.IAT,
            Sensor.TPS,

            Sensor.MAP,
            Sensor.tCharge,
            Sensor.baseFuel,
            Sensor.cltCorrection,
            Sensor.iatCorrection,

            Sensor.injectorLagMs,
            Sensor.lastErrorCode,
            Sensor.AFR,
            Sensor.VBATT,
            Sensor.VSS,

    };
    private static final String GAUGES_ROWS = "gauges_rows";
    private static final String GAUGES_COLUMNS = "gauges_cols";
    private static final String SHOW_MESSAGES = "show_messages";
    private static final String SHOW_RPM = "show_rpm";
    private static final String SPLIT_LOCATION = "SPLIT_LOCATION";
    public static final String DISABLE_LOGS = "DISABLE_LOGS";
    public static boolean IS_PAUSED; // dirty but works for not

    static {
        int expected = SizeSelectorPanel.WIDTH * SizeSelectorPanel.HEIGHT;
        if (DEFAULT_LAYOUT.length != expected)
            throw new IllegalStateException("Invalid gauges panel size " + DEFAULT_LAYOUT.length + " while " + expected  + " expected");
    }

    private final JPanel content = new JPanel(new BorderLayout());
    private final GaugesGrid gauges;
    private final Node config;

    private boolean showRpmPanel;
    private boolean showMessagesPanel;
    private final JPanel lowerRpmPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 0, 0));
    /**
     * this panel is displayed on the right
     */
    private final JPanel messagesPanel = new JPanel(new BorderLayout());
    private final JSplitPane middleSplitPanel;

    public GaugesPanel(final Node config, PaneSettings paneSettings) {
        gauges = new GaugesGrid(3, 5);
        this.config = config;
        showRpmPanel = config.getBoolProperty(SHOW_RPM, true);
        showMessagesPanel = config.getBoolProperty(SHOW_MESSAGES, true);

        prepareMessagesPanel();

        lowerRpmPanel.add(new RpmLabel(15).getContent());

        int rows = config.getIntProperty(GAUGES_ROWS, SizeSelectorPanel.HEIGHT);
        int columns = config.getIntProperty(GAUGES_COLUMNS, SizeSelectorPanel.WIDTH);

        setSensorGridDimensions(rows, columns);

        middleSplitPanel = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, createMiddleLeftPanel(), messagesPanel);

        content.add(createTopPanel(config), BorderLayout.NORTH);

        content.add(middleSplitPanel, BorderLayout.CENTER);

        content.add(new WarningPanel().getPanel(), BorderLayout.SOUTH);

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
        selectorMenu.add(new SizeSelectorPanel(new SizeSelectorPanel.SizeSelectorListener() {
            @Override
            public void onSelected(int row, int column) {
                System.out.println("new size " + row + "/" + column);
                setSensorGridDimensions(row, column);
            }
        }));

        JButton selector = new JButton("O");
        selector.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Component c = (Component) e.getSource();
                selectorMenu.show(c, -1, c.getHeight());
            }
        });
        rightUpperPanel.add(selector);

        JButton menuButton = new PopupMenuButton("#", createMenu(config));
        rightUpperPanel.add(menuButton);
        return rightUpperPanel;
    }

    @NotNull
    private JPopupMenu createMenu(final Node config) {
        JPopupMenu menu = new JPopupMenu();
        final JCheckBoxMenuItem saveDetailedLogs = new JCheckBoxMenuItem("Save detailed logs");
        saveDetailedLogs.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                FileLog.suspendLogging = !saveDetailedLogs.isSelected();
                getConfig().getRoot().setBoolProperty(DISABLE_LOGS, FileLog.suspendLogging);
            }
        });
        saveDetailedLogs.setSelected(!FileLog.suspendLogging);

        final JCheckBoxMenuItem showRpmItem = new JCheckBoxMenuItem("Show RPM");
        final JCheckBoxMenuItem showCommandsItem = new JCheckBoxMenuItem("Show Commands");
        showRpmItem.setSelected(showRpmPanel);
        ActionListener showCheckboxListener = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                showRpmPanel = showRpmItem.isSelected();
                showMessagesPanel = showCommandsItem.isSelected();
                config.setProperty(SHOW_RPM, showRpmPanel);
                config.setProperty(SHOW_MESSAGES, showMessagesPanel);
                applyShowFlags();
                // todo: this is not needed if we show/hide RPM panel. TODO: split into two different listeners
                middleSplitPanel.setDividerLocation(0.5);
            }
        };
        showRpmItem.addActionListener(showCheckboxListener);
        showCommandsItem.addActionListener(showCheckboxListener);

        menu.add(showRpmItem);
        showCommandsItem.setSelected(showMessagesPanel);
        menu.add(showCommandsItem);
        menu.add(new JPopupMenu.Separator());
        menu.add(saveDetailedLogs);
        menu.add(new JPopupMenu("Reset Config")); // todo looks like not working
        return menu;
    }

    private void prepareMessagesPanel() {
        MessagesPanel mp = new MessagesPanel(null);
        messagesPanel.add(BorderLayout.NORTH, mp.getButtonPanel());
        messagesPanel.add(BorderLayout.CENTER, mp.getMessagesScroll());
    }

    @NotNull
    private JPanel createLeftTopPanel() {
        JPanel leftUpperPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 0));
        leftUpperPanel.add(createPauseButton());
        leftUpperPanel.add(createSaveImageButton());
        leftUpperPanel.add(new RpmLabel(2).getContent());
        AnyCommand command = AnyCommand.createField(config, false, false);
        leftUpperPanel.add(command.getContent());
        return leftUpperPanel;
    }

    private Component createPauseButton() {
        final JButton pauseButton = UiUtils.createPauseButton();
        pauseButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                IS_PAUSED = !IS_PAUSED;
                UiUtils.setPauseButtonText(pauseButton, IS_PAUSED);
            }
        });
        return pauseButton;
    }

    @NotNull
    private JButton createSaveImageButton() {
        JButton saveImageButton = UiUtils.createSaveImageButton();
        saveImageButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String fileName = FileLog.getDate() + "_gauges.png";

                UiUtils.saveImageWithPrompt(fileName, content, gauges.panel);
            }
        });
        return saveImageButton;
    }

    private void setSensorGridDimensions(int rows, int columns) {
        gauges.setLayout(rows, columns);

        for (int i = 0; i < rows * columns; i++) {
            Component element = GaugesGridElement.read(config.getChild("element_" + i), DEFAULT_LAYOUT[i]);

            gauges.panel.add(element);
        }

        saveConfig(rows, columns);
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

    public enum DetachedRepository {
        INSTANCE;

        public static final String COUNT = "count";
        public static final String DETACHED = "detached";
        private List<DetachedSensor> list = new ArrayList<>();
        private Node config;

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
                DetachedSensor.create(config.getChild(DETACHED + i));
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
