package com.rusefi.maintenance.jobs;

import javax.swing.*;

class ParentComponentContext {
    private final JComponent parent;

    ParentComponentContext(final JComponent parent) {
        this.parent = parent;
    }

    JComponent getParent() {
        return parent;
    }
}
