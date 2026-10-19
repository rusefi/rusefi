package com.rusefi.ui.startup;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.util.Arrays;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.*;

public class StartupCardTest {
    @Test
    public void bannerFollowsItsMessageVisibility() throws Exception {
        JLabel message = new JLabel("Failed to check for DFU devices");
        Banner[] banner = new Banner[1];
        SwingUtilities.invokeAndWait(() -> {
            message.setVisible(false);
            banner[0] = new Banner(Banner.Kind.ERROR, message, null);
            assertFalse(banner[0].isVisible(), "a hidden message starts with a hidden banner");
            // StartupFrame keeps toggling the message itself, e.g. the 15 s DFU error timer
            message.setVisible(true);
        });
        // visibility changes arrive as queued component events
        SwingUtilities.invokeAndWait(() -> assertTrue(banner[0].isVisible()));
    }

    @Test
    public void helpTipsShowOnlyWhenExpanded() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConnectionHelpSection help = new ConnectionHelpSection(
                Arrays.asList("Check your cables.", "Check your drivers."), Collections.emptyList());
            assertFalse(help.isExpanded(), "collapsed by default");
            int collapsed = help.getPreferredSize().height;
            help.setExpanded(true);
            assertTrue(help.getPreferredSize().height > collapsed);
        });
    }

    @Test
    public void illustrationHasAnAccessibleDescription() throws Exception {
        // a plain JComponent has no accessible context; the illustration must provide one
        SwingUtilities.invokeAndWait(() -> assertNotNull(
            new ConnectionIllustration().getAccessibleContext().getAccessibleDescription()));
    }
}
