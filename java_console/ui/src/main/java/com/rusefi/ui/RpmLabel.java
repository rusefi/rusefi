package com.rusefi.ui;

import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.WellKnownGauges;
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
        if (withCaption)
            content.add(rpmCaption);
        content.add(rpmValue, "grow, wrap");

        RpmModel.getInstance().addListener(new RpmModel.RpmListener() {
            public void onRpmChange(RpmModel rpm) {
                int value = rpm.getSmoothedValue();
                if (value == -1)
                    rpmValue.setText("Noise");
                else
                    rpmValue.setText(value + "");
            }
        });

        ConnectionStatusLogic.INSTANCE.addListener(new ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                if (isConnected) {
                    rpmValue.setText("" + SensorCentral.getInstance().getValue(WellKnownGauges.RPMGauge.getOutputChannelName()));
                    rpmValue.setForeground(Color.green);
                } else {
                    rpmValue.setText(NO_CONNECTION);
                    rpmValue.setForeground(Color.red);
                }
            }
        });
        setSize(size);
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

    private void setFont(Font font) {
        rpmCaption.setFont(font);
        rpmValue.setFont(font);
    }
}
