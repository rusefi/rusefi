package com.rusefi;

public class FixedCommandControl extends CommandControl {
    private final String command;

    public FixedCommandControl(String labelText, String iconFileName, String buttonText, String command) {
        super(labelText, iconFileName, buttonText);
        this.command = command;
    }

    @Override
    protected String getCommand() {
        return command;
    }
}
