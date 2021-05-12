package com.rusefi;

import com.rusefi.ui.UIContext;

public class FixedCommandControl extends CommandControl {
    private final String command;

    public FixedCommandControl(UIContext uiContext, String labelText, String iconFileName, String buttonText, String command) {
        super(uiContext, labelText, iconFileName, buttonText);
        this.command = command;
    }

    @Override
    protected String getCommand() {
        return command;
    }
}
