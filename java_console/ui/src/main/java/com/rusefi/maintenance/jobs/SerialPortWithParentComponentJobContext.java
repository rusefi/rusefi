package com.rusefi.maintenance.jobs;

import com.rusefi.SerialPortScanner;

import javax.swing.*;

class SerialPortWithParentComponentJobContext extends SerialPortJobContext {
    private final JComponent parent;

    SerialPortWithParentComponentJobContext(final SerialPortScanner.PortResult port, final JComponent parent) {
        super(port);
        this.parent = parent;
    }

    JComponent getParent() {
        return parent;
    }
}
