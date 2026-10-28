package com.rusefi.ui.engine;

import com.rusefi.waves.ZoomProvider;
import com.rusefi.KeyStrokeShortcut;
import com.rusefi.ui.widgets.JoinedButtons;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

/**
 * 7/7/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
class ZoomControl extends JPanel {
    private double value;
    public ZoomControlListener listener = null;
    private final JButton resetZoom = new JButton();

    private ZoomProvider zoomProvider = new ZoomProvider() {
        @Override
        public double getZoomValue() {
            return value;
        }

        public String toString() {
            return "zoom " + value;
        }
    };

    public ZoomControl() {
        super(new FlowLayout(FlowLayout.LEFT, 0, 0));
        setOpaque(false);

        JButton minus = new JButton("−");
        minus.addActionListener(e -> zoomOut());
        minus.setToolTipText("Zoom out (Numpad -)");

        // the current zoom, a click returns to 100%; fixed width so the group does not jump while zooming
        resetZoom.addActionListener(e -> setValue(1));
        resetZoom.setToolTipText("Reset zoom to 100%");

        JButton plus = new JButton("+");
        plus.setMnemonic('x');
        plus.addActionListener(e -> zoomIn());
        plus.setToolTipText("Zoom in (Numpad + or Alt+X)");

        add(new JoinedButtons(minus, resetZoom, plus));
        // measured after JoinedButtons applied its button style
        resetZoom.setText("1000%");
        resetZoom.setPreferredSize(new Dimension(resetZoom.getPreferredSize().width, JoinedButtons.HEIGHT));
        setValue(1);

        bindKeyStrokeActions();
    }

    private void bindKeyStrokeActions() {
        InputMap inputMap = getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);

        inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_ADD, 0), KeyStrokeShortcut.ZOOM_IN);
        inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_SUBTRACT, 0), KeyStrokeShortcut.ZOOM_OUT);

        getActionMap().put(KeyStrokeShortcut.ZOOM_IN, new AbstractAction() {
            public void actionPerformed(ActionEvent event) {
                zoomIn();
            }
        });

        getActionMap().put(KeyStrokeShortcut.ZOOM_OUT, new AbstractAction() {
            public void actionPerformed(ActionEvent event) {
                zoomOut();
            }
        });
    }

    private void zoomIn() {
        setValue(value * 1.1);
    }

    private void zoomOut() {
        setValue(value / 1.1);
    }

    private void setValue(double value) {
        //log.info("Zoom setValue " + value);
        this.value = value;

        resetZoom.setText(Math.round(value * 100) + "%");
        resetZoom.setEnabled(Math.abs(1 - value) > 0.01);
        if (listener != null)
            listener.onZoomChange();
    }

    public ZoomProvider getZoomProvider() {
        return zoomProvider;
    }

    interface ZoomControlListener {
        void onZoomChange();
    }
}
