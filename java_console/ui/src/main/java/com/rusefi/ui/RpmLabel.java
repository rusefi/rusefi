package com.rusefi.ui;

import com.rusefi.io.ConnectionStatusLogic;

import javax.swing.*;
import java.awt.*;

/**
 * Digital RPM gauge which stays green while rusEfi is connected
 * <p/>
 * 9/17/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class RpmLabel {
    private static final String NO_CONNECTION = "N/C";
    private final JPanel content = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 0));

    private final JLabel rpmValue = new JLabel();
    private final JLabel rpmCaption = new JLabel("RPM:");
    private final RpmModel.RpmListener rpmListener;
    private final ConnectionStatusLogic.Listener connectionStatusListener;

    public RpmLabel(UIContext uiContext) {
        this(uiContext, 1);
    }

    public RpmLabel(UIContext uiContext, int size) {
        this(uiContext, size, true);
    }

    public RpmLabel(UIContext uiContext, int size, boolean withCaption) {
        String initialLabel = uiContext.getLinkManager().isLogViewer() ? "LOG" : NO_CONNECTION;
        rpmValue.setText(initialLabel);
        rpmValue.setForeground(Color.red);

        content.setBorder(BorderFactory.createLineBorder(Color.white));
        if (withCaption) {
            content.add(rpmCaption);
        }
        content.add(rpmValue, "grow, wrap");

        rpmListener = rpm -> {
            if (ConnectionStatusLogic.INSTANCE.isConnected()) {
                updateRpmValue(rpm.getSmoothedValue());
                rpmValue.setForeground(Color.green);
            }
        };
        RpmModel.getInstance().addListener(rpmListener);

        connectionStatusListener = new ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                if (isConnected) {
                    updateRpmValue(RpmModel.getInstance().getSmoothedValue());
                    rpmValue.setForeground(Color.green);
                } else {
                    rpmValue.setText(NO_CONNECTION);
                    rpmValue.setForeground(Color.red);
                }
            }
        };
        ConnectionStatusLogic.INSTANCE.addAndFireListener(connectionStatusListener);
        setSize(size);
    }

    private void updateRpmValue(int value) {
        if (value == -1) {
            rpmValue.setText("Noise");
        } else {
            rpmValue.setText(value + "");
        }
    }

    public JPanel getContent() {
        return content;
    }

    public RpmLabel setSize(int size) {
        Font f = rpmCaption.getFont();
        int fontSize = size * f.getSize();
        Font font = new Font(f.getName(), f.getStyle(), fontSize);
        setFont(font);
        return this;
    }

    public void destroy() {
        RpmModel.getInstance().removeListener(rpmListener);
        ConnectionStatusLogic.INSTANCE.removeListener(connectionStatusListener);
    }

    private void setFont(Font font) {
        rpmCaption.setFont(font);
        rpmValue.setFont(font);
    }
}
