package com.rusefi.ui;

import com.rusefi.StimulationInputs;
import com.rusefi.io.CommandQueue;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.LinkedHashMap;
import java.util.Map;

/**
 * This UI element displays an array of buttons, each for one of the recently used commands
 */

public class RecentCommands {
    private final static int NUMBER_OF_COMMANDS = 20;

    private final JPanel content = new JPanel(new GridLayout(NUMBER_OF_COMMANDS, 1));

    private final LinkedHashMap<Entry, Object> entries = new LinkedHashMap<Entry, Object>() {
        @Override
        protected boolean removeEldestEntry(Map.Entry<Entry, Object> eldest) {
            return size() > NUMBER_OF_COMMANDS;
        }
    };

    private boolean reentrant;

    private final JScrollPane messagesScroll = new JScrollPane(content, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

    public RecentCommands() {

        CommandQueue.getInstance().addListener(new CommandQueue.CommandQueueListener() {
            @Override
            public void onCommand(String command) {
                if (!reentrant)
                    add(command);
            }
        });

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
        entries.put(new Entry(command), null);

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                content.removeAll();
                for (Entry entry : entries.keySet()) {
                    content.add(createButton(entry));
                }
                UiUtils.trueRepaint(content.getParent());
                UiUtils.trueLayout(content.getParent());
            }
        });

    }

    private JComponent createButton(final Entry entry) {
        JButton button = new JButton(entry.command);
        button.setBorder(BorderFactory.createEmptyBorder(3, 5, 0, 5));

        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                reentrant = true;
                int timeout = CommandQueue.getTimeout(entry.command);
                CommandQueue.getInstance().write(entry.command, timeout);
                reentrant = false;
            }
        });

        return StimulationInputs.wrap(button);
    }

    public Component getContent() {
        return messagesScroll;
    }

    static class Entry {
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
    }

}