package com.rusefi;

import com.formdev.flatlaf.FlatClientProperties;
import org.junit.jupiter.api.Test;

import javax.swing.*;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

public class LaunchWizardHighlightTest {
    @Test
    public void blueOnlyWhileTheWizardIsIncomplete() {
        JButton button = new JButton("Launch Wizard");
        ConsoleUI.setLaunchWizardHighlight(button, true);
        assertEquals("primary", button.getClientProperty(FlatClientProperties.STYLE_CLASS));

        ConsoleUI.setLaunchWizardHighlight(button, false);
        assertNull(button.getClientProperty(FlatClientProperties.STYLE_CLASS), "a normal button once setup is done");
    }
}
