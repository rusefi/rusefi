package com.rusefi.ts_plugin;

import com.rusefi.io.serial.StreamStatistics;

import javax.swing.*;

public class StreamStatusControl {
    private final StreamStatistics authenticatorToProxyStream;

    private final JLabel content = new JLabel();

    public StreamStatusControl(StreamStatistics statistics) {
        this.authenticatorToProxyStream = statistics;
    }

    public void update() {
        content.setText("In " + authenticatorToProxyStream.getBytesIn() + " Out " + authenticatorToProxyStream.getBytesOut());
    }

    public JComponent getContent() {
        return content;
    }
}
