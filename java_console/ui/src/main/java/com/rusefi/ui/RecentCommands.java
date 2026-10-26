package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.formdev.flatlaf.FlatClientProperties;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.CommandQueue;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.CommandIcon;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.ToolbarStrip;

import javax.swing.*;
import java.awt.*;
import java.util.*;
import java.util.List;
import java.util.concurrent.atomic.AtomicBoolean;

import static com.rusefi.IoUtil.*;
import static com.rusefi.config.generated.Integration.CMD_DATE;
import static com.rusefi.config.generated.Integration.CMD_TRIGGERINFO;
import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;

/**
 * This UI element displays an array of buttons, each for one of the recently used commands
 */

public class RecentCommands {
    private final static Logging log = Logging.getLogging(RecentCommands.class);

    private final static int NUMBER_OF_COMMANDS = 40;
    private static final String KEY = "recent_commands";
    private static final String DELIMETER = "|";
    private static final String STOPENGINE = "stopengine";
    private static final String SHOWCONFIG = "showconfig";
    private static final String HELP = "help";
    private static final String SENSORINFO = "sensorinfo";
    private static final String IDLEINFO = "idleinfo";
    private static final String ALTINFO = "altinfo";
    private static final String TPSINFO = "tpsinfo";
    private static final String MAPINFO = "mapinfo";
    private static final String ACCELINFO = "accelinfo";
    private static final String CANINFO = "caninfo";
    private static final String TSINFO = "tsinfo";
    private static final String FUELINFO = "fuelinfo";
    private static final String SDINFO = "sdinfo";
    private static final String PINS = "pins";

    private final static Map<String, Icon> COMMAND_ICONS = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    static {
        // painted line icons on a transparent background, see CommandIcon [tag:console_theme]
        COMMAND_ICONS.put(STOPENGINE, CommandIcon.STOP);
        COMMAND_ICONS.put(HELP, CommandIcon.HELP);
        COMMAND_ICONS.put(SENSORINFO, CommandIcon.INFO);
        COMMAND_ICONS.put(CMD_TRIGGERINFO, CommandIcon.TRIGGER);
        COMMAND_ICONS.put(TSINFO, CommandIcon.INFO);
        COMMAND_ICONS.put(MAPINFO, CommandIcon.INFO);
        COMMAND_ICONS.put(CANINFO, CommandIcon.INFO);
        COMMAND_ICONS.put(FUELINFO, CommandIcon.INFO);
        COMMAND_ICONS.put(ACCELINFO, CommandIcon.INFO);
        COMMAND_ICONS.put(SDINFO, CommandIcon.SD_CARD);
        COMMAND_ICONS.put(PINS, CommandIcon.INFO);
        COMMAND_ICONS.put(Integration.CMD_WRITECONFIG, CommandIcon.SAVE);
        COMMAND_ICONS.put("rewriteconfig", CommandIcon.SAVE);
        COMMAND_ICONS.put(SHOWCONFIG, CommandIcon.LIST);
        COMMAND_ICONS.put("enable", CommandIcon.ENABLE);
        COMMAND_ICONS.put("disable", CommandIcon.DISABLE);
        COMMAND_ICONS.put("sparkbench", CommandIcon.SPARK);
        COMMAND_ICONS.put("fuelbench", CommandIcon.FUEL);
        COMMAND_ICONS.put("blipidle", CommandIcon.IDLE);
        COMMAND_ICONS.put("set_idle_position", CommandIcon.IDLE);
        COMMAND_ICONS.put(Integration.CMD_LUA_RESET, CommandIcon.RESET);
    }

    static final String TITLE = "Quick commands";
    static final String RESET = "Reset";
    /** Width of the column, so long commands such as "disable self_stimulation" fit. */
    private static final int COLUMN_WIDTH = 250;
    private static final int PADDING = 8;

    /** One flat, left-aligned button per remembered command. */
    private final JPanel content = new JPanel(new GridLayout(0, 1, 0, 2)) {
        @Override
        public Dimension getPreferredSize() {
            Dimension size = super.getPreferredSize();
            return new Dimension(COLUMN_WIDTH, size.height);
        }
    };

    private static final LinkedHashMap<Entry, Object> entries = new LinkedHashMap<Entry, Object>() {
        @Override
        protected boolean removeEldestEntry(Map.Entry<Entry, Object> eldest) {
            return size() > NUMBER_OF_COMMANDS;
        }
    };

    private final AtomicBoolean reentrant = new AtomicBoolean();

    private final JScrollPane messagesScroll = new JScrollPane(content, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
    private final JPanel column = new JPanel(new BorderLayout()) {
        /** The list plus its scrollbar, so neither the commands nor the header get cut. */
        @Override
        public Dimension getPreferredSize() {
            Dimension size = super.getPreferredSize();
            size.width = Math.max(size.width, COLUMN_WIDTH + messagesScroll.getVerticalScrollBar().getPreferredSize().width);
            return size;
        }

        @Override
        protected void paintChildren(Graphics g) {
            super.paintChildren(g);
            // divider between the messages and this column
            Color line = UIManager.getColor("Separator.foreground");
            g.setColor(line != null ? line : Color.LIGHT_GRAY);
            g.drawLine(0, 0, 0, getHeight());
        }
    };
    private final UIContext uiContext;

    public RecentCommands(UIContext uiContext) {
        this.uiContext = uiContext;

        // [tag:console_theme] titled header with Reset, like the Lua tab's panes
        JButton reset = new JButton(RESET);
        reset.setToolTipText("Replace the list with the default commands");
        reset.addActionListener(e -> {
            entries.clear();
            addDefaults();
        });
        column.add(ToolbarStrip.header(TITLE, new JoinedButtons(reset)), BorderLayout.NORTH);
        content.setBorder(BorderFactory.createEmptyBorder(PADDING, PADDING, PADDING, PADDING));
        messagesScroll.setBorder(BorderFactory.createEmptyBorder());
        messagesScroll.getVerticalScrollBar().setUnitIncrement(16);
        column.add(messagesScroll, BorderLayout.CENTER);
        uiContext.getCommandQueue().addListener(command -> {
                if (!reentrant.get())
                    add(command);
            }
        );

        String value = getConfig().getRoot().getProperty(KEY, null);

        if (value != null && value.trim().length() > 5) {
            unpack(value);
        } else {
            addDefaults();
        }
    }

    private void unpack(String value) {
        entries.clear();
        for (String command : value.split("\\" + DELIMETER))
            add(command);
    }

    private void addDefaults() {
        add(HELP);
        add(SHOWCONFIG);
        add(STOPENGINE);
        add(SENSORINFO);
        add(CMD_TRIGGERINFO);
        add(TSINFO);
        add(CANINFO);
        add(Integration.CMD_WRITECONFIG);
        add("rewriteconfig");

        add(getEnableCommand("injection"));
        add(getDisableCommand("injection"));

        add(getEnableCommand("ignition"));
        add(getDisableCommand("ignition"));

        add(getEnableCommand("self_stimulation"));
        add(getDisableCommand("self_stimulation"));

        add("blipidle 80 2000");
        add("set_idle_position 50");
        add("sparkbench 5 400 2");
        add("fuelbench 5 400 2");
        add(MAPINFO);
        add(ACCELINFO);
        add(FUELINFO);
        add(SDINFO);
        add(PINS);
    }

    public void add(String command) {
        if (isBoringCommand(command)) {
            // not useful to remember this one
            return;
        }
        synchronized (entries) {
            entries.put(new Entry(command), null);
        }

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                content.removeAll();
                synchronized (entries) {
                    Set<Entry> sorted = new TreeSet<>(entries.keySet());

                    for (Entry entry : sorted) {
                        content.add(createListButton(entry.command));
                    }
                }
                AutoupdateUtil.trueLayoutAndRepaint(column);
            }
        });
        getConfig().getRoot().setProperty(KEY, pack());
    }

    public static boolean isBoringCommand(String command) {
        return command.startsWith(getSetCommand(CMD_DATE));
    }

    public static JComponent createButton(UIContext uiContext, final AtomicBoolean reentrant, final String command) {
        JButton button = new JButton(command);
        Icon icon = COMMAND_ICONS.get(command);
        if (icon != null)
            button.setIcon(icon);
        button.setBorder(BorderFactory.createEmptyBorder(3, 5, 0, 5));
        button.addActionListener(e -> send(uiContext, reentrant, command));

        return UiUtils.wrap(button);
    }

    /** A flat list row: small icon (blank when the command has none, so texts line up) and the command. */
    private JButton createListButton(String command) {
        JButton button = new JButton(command, iconFor(command));
        button.putClientProperty(FlatClientProperties.BUTTON_TYPE, FlatClientProperties.BUTTON_TYPE_TOOLBAR_BUTTON);
        button.setHorizontalAlignment(SwingConstants.LEADING);
        button.setIconTextGap(8);
        button.setMargin(new Insets(5, 8, 5, 8));
        button.setFocusPainted(false);
        button.setToolTipText("Send " + command);
        button.addActionListener(e -> send(uiContext, reentrant, command));
        return button;
    }

    private static void send(UIContext uiContext, AtomicBoolean reentrant, String command) {
        reentrant.set(true);
        int timeout = CommandQueue.getTimeout(command);
        uiContext.getCommandQueue().write(command, timeout);
        reentrant.set(false);
    }

    /**
     * Icon by the command's first word, so "enable ignition" or "sparkbench 5 400 2" match their
     * kind; commands without one, e.g. typed by the user, get the console prompt icon.
     */
    static Icon iconFor(String command) {
        String firstWord = command.trim().split("\\s+")[0];
        Icon icon = COMMAND_ICONS.get(firstWord);
        return icon != null ? icon : CommandIcon.COMMAND;
    }

    public Component getContent() {
        return column;
    }

    public String pack() {
        StringBuilder sb = new StringBuilder();
        for (Entry command : entries.keySet()) {
            if (sb.length() != 0)
                sb.append(DELIMETER);
            sb.append(command.command);
        }
        return sb.toString();
    }

    static class Entry implements Comparable<Entry> {
        private final String command;

        Entry(String command) {
            this.command = command;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;

            Entry entry = (Entry) o;

            return !(command != null ? !command.equals(entry.command) : entry.command != null);

        }

        @Override
        public int hashCode() {
            return command != null ? command.hashCode() : 0;
        }

        @Override
        public String toString() {
            return "{" +
                    "command='" + command + '\'' +
                    '}';
        }

        @Override
        public int compareTo(Entry o) {
            return command.compareTo(o.command);
        }
    }

    public static String getRecent(int index) {
        List<Entry> elements;
        index = Math.max(0, index);
        synchronized (entries) {
            elements = new ArrayList<>(entries.keySet());
        }
        if (index >= elements.size())
            return elements.get(0).command;
        return elements.get(elements.size() - 1 - index).command;
    }



}
