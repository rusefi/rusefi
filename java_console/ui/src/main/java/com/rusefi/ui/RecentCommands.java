package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.CommandQueue;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.*;
import java.util.List;
import java.util.concurrent.atomic.AtomicBoolean;

import static com.rusefi.IoUtil.*;
import static com.rusefi.config.generated.Fields.CMD_DATE;
import static com.rusefi.config.generated.Fields.CMD_TRIGGERINFO;
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
    private static final String HELP = "help";
    private static final String SENSORINFO = "sensorinfo";
    private static final String MAPINFO = "mapinfo";
    private static final String CANINFO = "caninfo";
    private static final String TSINFO = "tsinfo";
    private static final String FUELINFO = "fuelinfo";
    private static final String SDINFO = "sdinfo";
    private static final String PINS = "pins";

    private final static Map<String, Icon> COMMAND_ICONS = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    static {
        COMMAND_ICONS.put(STOPENGINE, UiUtils.loadIcon("stop.jpg"));
        ImageIcon infoIcon = UiUtils.loadIcon("info.png");
        COMMAND_ICONS.put(HELP, UiUtils.loadIcon("help.jpg"));
        COMMAND_ICONS.put(SENSORINFO, infoIcon);
        COMMAND_ICONS.put(CMD_TRIGGERINFO, UiUtils.loadIcon("trigger.jpg"));
        COMMAND_ICONS.put(TSINFO, infoIcon);
        COMMAND_ICONS.put(MAPINFO, infoIcon);
        COMMAND_ICONS.put(CANINFO, infoIcon);
        COMMAND_ICONS.put(FUELINFO, infoIcon);
        COMMAND_ICONS.put(SDINFO, UiUtils.loadIcon("sdinfo.jpg"));
        COMMAND_ICONS.put(PINS, infoIcon);
        COMMAND_ICONS.put(Fields.CMD_WRITECONFIG, UiUtils.loadIcon("writeconfig.jpg"));
    }

    private final JPanel content = new JPanel(new GridLayout(NUMBER_OF_COMMANDS + 1, 1));

    private static final LinkedHashMap<Entry, Object> entries = new LinkedHashMap<Entry, Object>() {
        @Override
        protected boolean removeEldestEntry(Map.Entry<Entry, Object> eldest) {
            return size() > NUMBER_OF_COMMANDS;
        }
    };

    private final AtomicBoolean reentrant = new AtomicBoolean();

    private final JScrollPane messagesScroll = new JScrollPane(content, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
    private final UIContext uiContext;

    public RecentCommands(UIContext uiContext) {
        this.uiContext = uiContext;
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
        add(STOPENGINE);
        add(SENSORINFO);
        add(CMD_TRIGGERINFO);
        add(TSINFO);
        add(CANINFO);
        add(Fields.CMD_WRITECONFIG);
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

                JButton reset = new JButton(UiUtils.loadIcon("undo.jpg"));
                reset.setContentAreaFilled(false);
                reset.setFocusPainted(false);
                reset.setBorder(BorderFactory.createEmptyBorder());
                content.add(UiUtils.wrap(reset));
                reset.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        entries.clear();
                        addDefaults();
                    }
                });

                synchronized (entries) {
                    Set<Entry> sorted = new TreeSet<>(entries.keySet());

                    for (Entry entry : sorted) {
                        content.add(createButton(uiContext, reentrant, entry.command));
                    }
                }
                UiUtils.trueLayout(content.getParent());
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

        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                reentrant.set(true);
                int timeout = CommandQueue.getTimeout(command);
                uiContext.getCommandQueue().write(command, timeout);
                reentrant.set(false);
            }
        });

        return UiUtils.wrap(button);
    }

    public Component getContent() {
        return messagesScroll;
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

            return Objects.equals(command, entry.command);
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