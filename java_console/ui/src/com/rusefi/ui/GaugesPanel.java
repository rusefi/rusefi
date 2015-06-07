package com.rusefi.ui;

import com.rusefi.FileLog;
import com.rusefi.core.Sensor;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.PopupMenuButton;
import com.rusefi.ui.widgets.SensorGauge;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Date: 2/5/13
 * (c) Andrey Belomutskiy
 */
public class GaugesPanel {
    private static final Sensor[] DEFAULT_LAYOUT = {
            Sensor.RPM,
            Sensor.MAF,
            Sensor.CLT,
            Sensor.IAT,
            Sensor.TPS,
            Sensor.MAP,
            Sensor.MAP_RAW,
            Sensor.T_CHARGE,
            Sensor.DWELL1,
            Sensor.DWELL0,
            Sensor.DUTY0,
            Sensor.ADVANCE0,
            Sensor.FUEL,
            Sensor.BARO,
            Sensor.FUEL_CLT,
            Sensor.FUEL_IAT,
            Sensor.FUEL_LAG,
            Sensor.AFR,
            Sensor.DEFAULT_FUEL,
            Sensor.TIMING,
            Sensor.VREF

    };
    private static final String GAUGES_ROWS = "gauges_rows";
    private static final String GAUGES_COLUMNS = "gauges_cols";
    public static final String SHOW_MESSAGES = "show_messages";
    public static final String SHOW_RPM = "show_rpm";

    static {
        if (DEFAULT_LAYOUT.length != SizeSelectorPanel.WIDTH * SizeSelectorPanel.HEIGHT)
            throw new IllegalStateException("Not expected " + DEFAULT_LAYOUT.length);
    }

    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel gauges = new JPanel(new GridLayout(3, 5));
    private final Node config;

    private boolean showRpmPanel = true;
    private boolean showMessagesPanel = true;
    private final JPanel lowerRpmPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 0, 0));
    private final JPanel messagesPanel = new JPanel(new BorderLayout());

    public GaugesPanel(final Node config) {
        this.config = config;
        showRpmPanel = config.getBoolProperty(SHOW_RPM, true);
        showMessagesPanel = config.getBoolProperty(SHOW_MESSAGES, true);

        prepareMessagesPanel(config);

        lowerRpmPanel.add(new RpmLabel(15).getContent());

        int rows = config.getIntProperty(GAUGES_ROWS, SizeSelectorPanel.HEIGHT);
        int columns = config.getIntProperty(GAUGES_COLUMNS, SizeSelectorPanel.WIDTH);

        applySize(rows, columns);

        content.add(createTopPanel(config), BorderLayout.NORTH);

        content.add(createMiddleLeftPanel(), BorderLayout.CENTER);
        content.add(messagesPanel, BorderLayout.EAST);

        content.add(new WarningPanel().getPanel(), BorderLayout.SOUTH);

        applyShowFlags();
    }

    @NotNull
    private JPanel createMiddleLeftPanel() {
        JPanel middleLeftPanel = new JPanel(new BorderLayout());
        middleLeftPanel.add(gauges, BorderLayout.CENTER);
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
                applySize(row, column);
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
            }
        };
        showRpmItem.addActionListener(showCheckboxListener);
        showCommandsItem.addActionListener(showCheckboxListener);

        menu.add(showRpmItem);
        showCommandsItem.setSelected(showMessagesPanel);
        menu.add(showCommandsItem);
        menu.add(new JPopupMenu.Separator());
        menu.add(new JPopupMenu("Reset Config"));
        return menu;
    }

    private void prepareMessagesPanel(Node config) {
        MessagesPanel mp = new MessagesPanel(config, false);
        messagesPanel.add(BorderLayout.NORTH, mp.getButtonPanel());
        messagesPanel.add(BorderLayout.CENTER, mp.getMessagesScroll());
    }

    @NotNull
    private JPanel createLeftTopPanel() {
        JPanel leftUpperPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 0));
        leftUpperPanel.add(createSaveImageButton());
        leftUpperPanel.add(new RpmLabel(2).getContent());
        return leftUpperPanel;
    }

    @NotNull
    private JButton createSaveImageButton() {
        JButton saveImageButton = UiUtils.createSaveImageButton();
        saveImageButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String fileName = FileLog.getDate() + "_gauges.png";

                UiUtils.saveImageWithPrompt(fileName, content, gauges);
            }
        });
        return saveImageButton;
    }

    private void applySize(int rows, int columns) {
        gauges.setLayout(new GridLayout(rows, columns));
        gauges.removeAll();

        for (int i = 0; i < rows * columns; i++) {
            String gaugeName = config.getProperty(getKey(i), DEFAULT_LAYOUT[i].name());
            Sensor sensor;
            try {
                sensor = Sensor.valueOf(Sensor.class, gaugeName);
            } catch (IllegalArgumentException e) {
                sensor = DEFAULT_LAYOUT[i];
            }
            final int currentGaugeIndex = i;
            gauges.add(SensorGauge.createGauge(sensor, new SensorGauge.GaugeChangeListener() {
                @Override
                public void onSensorChange(Sensor sensor) {
                    config.setProperty(getKey(currentGaugeIndex), sensor.name());
                }
            }));
        }

        saveConfig(rows, columns);
    }

    @NotNull
    private String getKey(int i) {
        return "gauge_" + i;
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
}
