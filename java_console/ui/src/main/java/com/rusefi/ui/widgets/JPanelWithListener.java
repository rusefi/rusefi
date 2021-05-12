package com.rusefi.ui.widgets;

import javax.swing.*;
import java.util.List;
import java.awt.*;
import java.awt.event.MouseListener;
import java.util.LinkedList;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 8/21/2015.
 */
public class JPanelWithListener extends JPanel {
    private final List<MouseListener> listeners = new LinkedList<>();

    public JPanelWithListener(LayoutManager layoutManager) {
        super(layoutManager);
    }

    @Override
    public synchronized void addMouseListener(MouseListener l) {
        super.addMouseListener(l);
        listeners.add(l);
    }

    @Override
    public synchronized void removeMouseListener(MouseListener l) {
        super.removeMouseListener(l);
        listeners.remove(l);
    }

    public synchronized void removeAllMouseListeners() {
        for (MouseListener l : listeners)
            super.removeMouseListener(l);
        listeners.clear();
    }

    public void removeAllChildrenAndListeners() {
        removeAll();
        removeAllMouseListeners();
    }
}
