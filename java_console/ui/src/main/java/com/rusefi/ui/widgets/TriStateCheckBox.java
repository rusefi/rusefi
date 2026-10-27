package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;

/**
 * Tri-state check box supporting CHECKED, UNCHECKED, and INDETERMINATE (grayed square) states.
 */
public class TriStateCheckBox extends JCheckBox {
    public enum State {
        UNCHECKED,
        CHECKED,
        INDETERMINATE
    }

    private State state = State.UNCHECKED;

    public TriStateCheckBox() {
        this(null, State.UNCHECKED);
    }

    public TriStateCheckBox(String text) {
        this(text, State.UNCHECKED);
    }

    public TriStateCheckBox(String text, State initialState) {
        super(text);
        setModel(new TriStateButtonModel());
        setIcon(new TriStateIcon());
        setState(initialState != null ? initialState : State.UNCHECKED);
    }

    public State getState() {
        return state;
    }

    public void setState(State state) {
        if (state == null) {
            state = State.UNCHECKED;
        }
        this.state = state;
        if (getModel() instanceof TriStateButtonModel) {
            ((TriStateButtonModel) getModel()).internalSetSelected(state == State.CHECKED);
        } else {
            getModel().setSelected(state == State.CHECKED);
        }
        repaint();
    }

    public boolean isIndeterminate() {
        return state == State.INDETERMINATE;
    }

    @Override
    public void setSelected(boolean b) {
        setState(b ? State.CHECKED : State.UNCHECKED);
    }

    @Override
    public boolean isSelected() {
        return state == State.CHECKED;
    }

    private class TriStateButtonModel extends JToggleButton.ToggleButtonModel {
        void internalSetSelected(boolean b) {
            super.setSelected(b);
        }

        @Override
        public void setSelected(boolean b) {
            state = b ? State.CHECKED : State.UNCHECKED;
            super.setSelected(b);
        }

        @Override
        public boolean isSelected() {
            return state == State.CHECKED;
        }
    }

    private class TriStateIcon implements Icon {
        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Icon defaultIcon = UIManager.getIcon("CheckBox.icon");
            if (defaultIcon != null) {
                defaultIcon.paintIcon(c, g, x, y);
            } else {
                paintFallbackBox(c, g, x, y);
            }

            if (state == State.INDETERMINATE) {
                paintIndeterminateSquare(c, g, x, y);
            }
        }

        private void paintFallbackBox(Component c, Graphics g, int x, int y) {
            int w = getIconWidth();
            int h = getIconHeight();
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setColor(c.isEnabled() ? Color.WHITE : new Color(240, 240, 240));
                g2.fillRect(x + 1, y + 1, w - 2, h - 2);
                g2.setColor(c.isEnabled() ? Color.DARK_GRAY : Color.GRAY);
                g2.drawRect(x + 1, y + 1, w - 3, h - 3);

                if (state == State.CHECKED) {
                    g2.setColor(c.isEnabled() ? Color.BLACK : Color.GRAY);
                    g2.setStroke(new BasicStroke(2));
                    g2.drawLine(x + 3, y + h / 2, x + w / 2 - 1, y + h - 4);
                    g2.drawLine(x + w / 2 - 1, y + h - 4, x + w - 3, y + 3);
                }
            } finally {
                g2.dispose();
            }
        }

        private void paintIndeterminateSquare(Component c, Graphics g, int x, int y) {
            int w = getIconWidth();
            int h = getIconHeight();
            int squareSize = Math.max(4, Math.min(w, h) - 6);
            int sx = x + (w - squareSize) / 2;
            int sy = y + (h - squareSize) / 2;

            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setColor(c.isEnabled() ? new Color(0x70, 0x70, 0x70) : Color.LIGHT_GRAY);
                g2.fillRect(sx, sy, squareSize, squareSize);
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            Icon defaultIcon = UIManager.getIcon("CheckBox.icon");
            return defaultIcon != null ? defaultIcon.getIconWidth() : 14;
        }

        @Override
        public int getIconHeight() {
            Icon defaultIcon = UIManager.getIcon("CheckBox.icon");
            return defaultIcon != null ? defaultIcon.getIconHeight() : 14;
        }
    }
}
