package com.rusefi.ui.widgets;

import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.io.LinkManager;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

/**
 * Tests for the purple bootloader indicator on the connection status icon [tag:better_ux_for_flashing].
 */
public class ConnectionStatusIconTest {

    private JTabbedPane tabbedPane;
    private LinkManager linkManager;

    @BeforeEach
    void setUp() {
        tabbedPane = new JTabbedPane();
        linkManager = mock(LinkManager.class);
        // Reset ConnectionStatusLogic to disconnected state
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
    }

    @AfterEach
    void tearDown() {
        tabbedPane = null;
    }

    @Test
    void paintsRedWhenDisconnected() {
        ConnectionStatusIcon icon = new ConnectionStatusIcon(linkManager, tabbedPane);
        Icon delegate = icon.getIcon();
        // Force a paint to a test image to capture the color
        java.awt.image.BufferedImage img = new java.awt.image.BufferedImage(16, 16, java.awt.image.BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = img.createGraphics();
        delegate.paintIcon(icon, g, 0, 0);
        g.dispose();
        // The center pixel should be red (disconnected)
        int rgb = img.getRGB(8, 8);
        Color c = new Color(rgb, true);
        assertTrue(c.getRed() > c.getGreen() && c.getRed() > c.getBlue(), "Expected red when disconnected, got " + c);
    }

    @Test
    void paintsGreenWhenConnected() {
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
        ConnectionStatusIcon icon = new ConnectionStatusIcon(linkManager, tabbedPane);
        Icon delegate = icon.getIcon();
        java.awt.image.BufferedImage img = new java.awt.image.BufferedImage(16, 16, java.awt.image.BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = img.createGraphics();
        delegate.paintIcon(icon, g, 0, 0);
        g.dispose();
        int rgb = img.getRGB(8, 8);
        Color c = new Color(rgb, true);
        assertTrue(c.getGreen() > c.getRed() && c.getGreen() > c.getBlue(), "Expected green when connected, got " + c);
    }

    @Test
    void paintsPurpleWhenBootloaderMode() {
        // Bootloader mode overrides connection state
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
        tabbedPane.putClientProperty("bootloaderMode", "OpenBLT");
        ConnectionStatusIcon icon = new ConnectionStatusIcon(linkManager, tabbedPane);
        Icon delegate = icon.getIcon();
        java.awt.image.BufferedImage img = new java.awt.image.BufferedImage(16, 16, java.awt.image.BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = img.createGraphics();
        delegate.paintIcon(icon, g, 0, 0);
        g.dispose();
        int rgb = img.getRGB(8, 8);
        Color c = new Color(rgb, true);
        // Purple: high blue, medium red, low green
        assertTrue(c.getBlue() > c.getRed() && c.getBlue() > c.getGreen(), "Expected purple in bootloader mode, got " + c);
        assertTrue(c.getRed() > 100, "Purple should have significant red component, got " + c.getRed());
    }

    @Test
    void tooltipShowsBootloaderMode() {
        tabbedPane.putClientProperty("bootloaderMode", "DFU");
        ConnectionStatusIcon icon = new ConnectionStatusIcon(linkManager, tabbedPane);
        String tip = icon.getToolTipText();
        assertNotNull(tip);
        assertTrue(tip.contains("DFU"), "Tooltip should mention bootloader mode: " + tip);
        assertTrue(tip.contains("update firmware"), "Tooltip should direct user to update firmware: " + tip);
    }

    @Test
    void tooltipShowsConnectedWhenNotBootloader() {
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
        ConnectionStatusIcon icon = new ConnectionStatusIcon(linkManager, tabbedPane);
        String tip = icon.getToolTipText();
        assertNotNull(tip);
        assertTrue(tip.contains("Connected"), "Expected connected tooltip: " + tip);
    }

    @Test
    void tooltipShowsDisconnectedWhenNotBootloaderAndNotConnected() {
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
        ConnectionStatusIcon icon = new ConnectionStatusIcon(linkManager, tabbedPane);
        String tip = icon.getToolTipText();
        assertNotNull(tip);
        assertTrue(tip.contains("Disconnected"), "Expected disconnected tooltip: " + tip);
    }

    @Test
    void updatesWhenBootloaderModeChanges() {
        // Start with no bootloader mode
        ConnectionStatusIcon icon = new ConnectionStatusIcon(linkManager, tabbedPane);
        String tipBefore = icon.getToolTipText();
        assertFalse(tipBefore.contains("DFU"), "Should not mention DFU initially: " + tipBefore);

        // Change the property
        tabbedPane.putClientProperty("bootloaderMode", "DFU");
        // Wait for the SwingUtilities.invokeLater to process
        try {
            SwingUtilities.invokeAndWait(() -> {});
        } catch (Exception e) {
            fail(e);
        }
        String tipAfter = icon.getToolTipText();
        assertTrue(tipAfter.contains("DFU"), "Should mention DFU after property change: " + tipAfter);
    }

    @Test
    void tooltipShowsOfflineWhenOfflineModeAndDisconnected() {
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
        tabbedPane.putClientProperty("offlineMode", Boolean.TRUE);
        ConnectionStatusIcon icon = new ConnectionStatusIcon(linkManager, tabbedPane);
        String tip = icon.getToolTipText();
        assertNotNull(tip);
        assertTrue(tip.contains("Offline tune"), "Expected offline tooltip: " + tip);
    }

    @Test
    void connectedTakesPrecedenceOverOfflineTooltip() {
        // Offline flag can be stale; a live connection must win.
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
        tabbedPane.putClientProperty("offlineMode", Boolean.TRUE);
        ConnectionStatusIcon icon = new ConnectionStatusIcon(linkManager, tabbedPane);
        String tip = icon.getToolTipText();
        assertNotNull(tip);
        assertTrue(tip.contains("Connected"), "Connected should win over offline: " + tip);
    }

    @Test
    void bootloaderTakesPrecedenceOverOfflineTooltip() {
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
        tabbedPane.putClientProperty("offlineMode", Boolean.TRUE);
        tabbedPane.putClientProperty("bootloaderMode", "DFU");
        ConnectionStatusIcon icon = new ConnectionStatusIcon(linkManager, tabbedPane);
        String tip = icon.getToolTipText();
        assertNotNull(tip);
        assertTrue(tip.contains("DFU"), "Bootloader should win over offline: " + tip);
    }

    @Test
    void updatesWhenOfflineModeChanges() {
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
        ConnectionStatusIcon icon = new ConnectionStatusIcon(linkManager, tabbedPane);
        assertFalse(icon.getToolTipText().contains("Offline tune"), "Should not be offline initially");

        tabbedPane.putClientProperty("offlineMode", Boolean.TRUE);
        try {
            SwingUtilities.invokeAndWait(() -> {});
        } catch (Exception e) {
            fail(e);
        }
        assertTrue(icon.getToolTipText().contains("Offline tune"), "Should show offline after property change");
    }

    @Test
    void worksWithNullTabbedPane() {
        // Should not throw, falls back to connected/disconnected only
        assertDoesNotThrow(() -> new ConnectionStatusIcon(linkManager, null));
    }
}
