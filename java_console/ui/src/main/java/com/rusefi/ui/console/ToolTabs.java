package com.rusefi.ui.console;

import javax.swing.*;
import java.util.*;

/**
 * Developer tabs kept out of the main tab row (Messages, Engine Sniffer). The Tools menu opens one at
 * the end of the row and its tab's close button removes it again. The content is built once and kept
 * while the tab is closed, so for example Messages keeps collecting. All calls run on the EDT.
 */
public class ToolTabs {
    private final TabbedPanel tabs;
    private final Map<String, JComponent> contents = new LinkedHashMap<>();
    private final Map<String, JComponent> titles = new HashMap<>();

    public ToolTabs(TabbedPanel tabs) {
        this.tabs = tabs;
    }

    public void register(String title, JComponent content) {
        contents.put(title, content);
    }

    /** Titles in the order they were registered, as the Tools menu lists them. */
    public List<String> getTitles() {
        return new ArrayList<>(contents.keySet());
    }

    /** Adds the tab at the end of the row unless it is already open, and selects it. */
    public void open(String title) {
        JComponent content = contents.get(title);
        if (content == null) {
            return;
        }
        if (!isOpen(title)) {
            JComponent tabTitle = titles.computeIfAbsent(title,
                t -> tabs.createClosableTitle(t, content, () -> close(t)));
            tabs.addTab(title, content, tabTitle);
        }
        tabs.tabbedPane.setSelectedComponent(content);
    }

    public void close(String title) {
        JComponent content = contents.get(title);
        if (content != null) {
            tabs.tabbedPane.remove(content);
        }
    }

    public boolean isOpen(String title) {
        JComponent content = contents.get(title);
        return content != null && tabs.tabbedPane.indexOfComponent(content) >= 0;
    }
}
