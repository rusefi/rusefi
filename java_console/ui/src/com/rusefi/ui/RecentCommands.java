package com.rusefi.ui;

import com.rusefi.io.CommandQueue;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;
import java.util.concurrent.atomic.AtomicBoolean;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

/**
 * This UI element displays an array of buttons, each for one of the recently used commands
 */

public class RecentCommands {
    private final static int NUMBER_OF_COMMANDS = 40;
    private static final String KEY = "recent_commands";
    private static final String DELIMETER = "|";

    private final JPanel content = new JPanel(new GridLayout(NUMBER_OF_COMMANDS, 1));

    private final LinkedHashMap<Entry, Object> entries = new LinkedHashMap<Entry, Object>() {
        @Override
        protected boolean removeEldestEntry(Map.Entry<Entry, Object> eldest) {
            return size() > NUMBER_OF_COMMANDS;
        }
    };

    private final AtomicBoolean reentrant = new AtomicBoolean();

    private final JScrollPane messagesScroll = new JScrollPane(content, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

    public RecentCommands() {
        CommandQueue.getInstance().addListener(new CommandQueue.CommandQueueListener() {
            @Override
            public void onCommand(String command) {
                if (!reentrant.get())
                    add(command);
            }
        });

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
        add("help");
        add("showconfig");
        add("analoginfo");
        add("triggerinfo");
        add("writeconfig");
        add("rewriteconfig");
        add("enable injection");
        add("disable injection");
        add("enable ignition");
        add("disable ignition");

        add("enable self_stimulation");
        add("disable self_stimulation");

        add("sparkbench 5 400 2");
        add("fuelbench 5 400 2");
        add("fuelpumpbench");
        add("fanbench");
        add("milbench");
    }

    public void add(String command) {
        synchronized (entries) {
            entries.put(new Entry(command), null);
        }

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                content.removeAll();
                synchronized (entries) {
                    Set<Entry> sorted = new TreeSet<Entry>();
                    sorted.addAll(entries.keySet());

                    for (Entry entry : sorted) {
                        content.add(createButton(reentrant, entry.command));
                    }
                }
                UiUtils.trueRepaint(content.getParent());
                UiUtils.trueLayout(content.getParent());
            }
        });
        getConfig().getRoot().setProperty(KEY, pack());
    }

    public static JComponent createButton(final AtomicBoolean reentrant, final String command) {
        JButton button = new JButton(command);
        button.setBorder(BorderFactory.createEmptyBorder(3, 5, 0, 5));

        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                reentrant.set(true);
                int timeout = CommandQueue.getTimeout(command);
                CommandQueue.getInstance().write(command, timeout);
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
}