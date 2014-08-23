package com.rusefi.ui;

import com.rusefi.StimulationInputs;
import com.rusefi.io.CommandQueue;
import com.rusefi.ui.widgets.UpDownImage;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.Map;

public class RecentCommands {
    private final static int NUMBER_OF_COMMANDS = 12;

    private final JPanel content = new JPanel(new GridLayout(NUMBER_OF_COMMANDS, 1));

    private final LinkedHashMap<Entry, Object> entries = new LinkedHashMap<Entry, Object>() {
        @Override
        protected boolean removeEldestEntry(Map.Entry<Entry, Object> eldest) {
            return size() > NUMBER_OF_COMMANDS;
        }
    };

    private boolean reentrant;

    public RecentCommands() {

        CommandQueue.getInstance().addListener(new CommandQueue.CommandQueueListener() {
            @Override
            public void onCommand(String command) {
                if (!reentrant)
                    add(command);
            }
        });

        add("showconfig");
        add("writeconfig");
        add("enable_injection");
        add("disable_injection");
        add("enable_ignition");
        add("disable_ignition");
    }

    public void add(String command) {
        entries.put(new Entry(command), null);

        content.removeAll();
        for (Entry entry : entries.keySet()) {
            content.add(createButton(entry));
        }
        UpDownImage.trueRepaint(content.getParent());
//        UpDownImage.trueLayout(content);
    }

    private JComponent createButton(final Entry entry) {
        JButton button = new JButton(entry.command);
        button.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));

        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                reentrant = true;
                CommandQueue.getInstance().write(entry.command);
                reentrant = false;
            }
        });

        return StimulationInputs.wrap(button);
    }

    public Component getContent() {
        return content;
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