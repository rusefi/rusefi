package com.rusefi.ui.engine;

import javax.swing.*;
import java.awt.*;

/**
 * One cell of the Engine Sniffer's channel column: the channel name, then its physical pin and the
 * number of events in the current capture. Sits beside its {@link UpDownImage} row in the scroll
 * pane's row header, so the names stay visible while a zoomed chart scrolls sideways
 * [tag:console_theme].
 */
class SnifferRowLabel extends JPanel {
    static final int WIDTH = 168;
    private final String channel;
    private final JLabel name = new JLabel();
    private final JLabel details = new JLabel();
    private String pin;
    private int events;

    SnifferRowLabel(String channel, String pin) {
        super(new BorderLayout());
        this.channel = channel;
        this.pin = pin;
        name.setText(NameUtil.getUiName(channel));
        name.setFont(name.getFont().deriveFont(Font.BOLD));
        details.setForeground(UIManager.getColor("Label.disabledForeground"));

        JPanel lines = new JPanel(new GridLayout(0, 1, 0, 2));
        lines.setOpaque(false);
        lines.add(name);
        lines.add(details);
        add(lines, BorderLayout.NORTH);
        setBorder(BorderFactory.createEmptyBorder(6, 12, 6, 8));
        updateDetails();
    }

    void setPin(String pin) {
        this.pin = pin;
        updateDetails();
    }

    void setEvents(int events) {
        this.events = events;
        updateDetails();
    }

    String getDetailsForTests() {
        return details.getText();
    }

    private void updateDetails() {
        String count = events + (events == 1 ? " event" : " events");
        details.setText(pin == null ? count : pin + " · " + count);
        String tooltip = "Channel " + NameUtil.getUiName(channel) + (pin == null ? "" : ", physical pin " + pin);
        setToolTipText(tooltip);
    }

    @Override
    public Dimension getPreferredSize() {
        return new Dimension(WIDTH, super.getPreferredSize().height);
    }
}
