package com.rusefi.ui;

import com.rusefi.io.LinkManager;

public class UIContext {
    private final LinkManager linkManager = new LinkManager();

    public LinkManager getLinkManager() {
        return linkManager;
    }
}
