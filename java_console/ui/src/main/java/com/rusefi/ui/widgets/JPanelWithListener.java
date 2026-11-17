package com.rusefi.ui.widgets;

import javax.swing.*;
import java.util.List;
import java.awt.*;
import java.awt.event.MouseListener;
import java.util.LinkedList;
import java.util.function.Consumer;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 8/21/2015.
 */
public class JPanelWithListener extends JPanel {
    private final List<MouseListener> listeners = new LinkedList<>();
    private final List<Runnable> cleanupActions = new LinkedList<>();
    private final List<Consumer<Boolean>> activeStateActions = new LinkedList<>();
    private boolean active = true;
    private boolean mouseOnlyOnChildren;

    public JPanelWithListener(LayoutManager layoutManager) {
        super(layoutManager);
    }

    /**
     * When set, this panel takes the mouse only where a visible child does, so a transparent
     * panel overlapping others lets clicks through to them (free gauge layout).
     */
    public void setMouseOnlyOnChildren(boolean mouseOnlyOnChildren) {
        this.mouseOnlyOnChildren = mouseOnlyOnChildren;
    }

    @Override
    public boolean contains(int x, int y) {
        if (!mouseOnlyOnChildren) {
            return super.contains(x, y);
        }
        for (Component child : getComponents()) {
            if (child.isVisible() && child.contains(x - child.getX(), y - child.getY())) {
                return true;
            }
        }
        return false;
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

    public synchronized void addCleanupAction(Runnable action) {
        cleanupActions.add(action);
    }

    public void addActiveStateAction(Consumer<Boolean> action) {
        boolean currentActive;
        synchronized (this) {
            activeStateActions.add(action);
            currentActive = active;
        }
        action.accept(currentActive);
    }

    public void setActive(boolean active) {
        List<Consumer<Boolean>> toRun;
        synchronized (this) {
            if (this.active == active) {
                return;
            }
            this.active = active;
            toRun = new LinkedList<>(activeStateActions);
        }
        for (Consumer<Boolean> action : toRun) {
            action.accept(active);
        }
    }

    public void removeAllChildrenAndListeners() {
        removeAll();
        removeAllMouseListeners();
        List<Runnable> toRun;
        synchronized (this) {
            toRun = new LinkedList<>(cleanupActions);
            cleanupActions.clear();
            activeStateActions.clear();
        }
        for (Runnable action : toRun) {
            action.run();
        }
    }
}
