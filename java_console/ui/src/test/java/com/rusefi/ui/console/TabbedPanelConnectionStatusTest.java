package com.rusefi.ui.console;

import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelMocks;
import com.opensr5.ini.field.ScalarIniField;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.FieldType;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.WellKnownGauges;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.widgets.ConnectionStatusIcon;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

class TabbedPanelConnectionStatusTest {
    // Reuse one UI for the suite: these production widgets register application-lifetime listeners.
    private static TabbedPanel panel;
    private static JComponent overlay;
    private static ConnectionStatusIcon icon;
    private ConnectionStatusValue previousStatus;

    @BeforeAll
    static void createUi() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            panel = new TabbedPanel(mock(UIContext.class));
            JRootPane root = new JRootPane();
            root.getContentPane().add(panel.getContent());
            panel.installGlassPane();
            overlay = (JComponent) root.getGlassPane();
            overlay.setSize(800, 400);
            overlay.setFont(new Font(Font.DIALOG, Font.PLAIN, 12));
            icon = new ConnectionStatusIcon(mock(LinkManager.class), panel.tabbedPane);
        });
    }

    @BeforeEach
    void setUp() throws Exception {
        previousStatus = ConnectionStatusLogic.INSTANCE.getValue();
        SensorCentral.getInstance().reset();
        SwingUtilities.invokeAndWait(() -> panel.tabbedPane.putClientProperty("isUpdating", false));
        setStatus(ConnectionStatusValue.NOT_CONNECTED);
    }

    @AfterEach
    void tearDown() throws Exception {
        SensorCentral.getInstance().reset();
        SwingUtilities.invokeAndWait(() -> panel.tabbedPane.putClientProperty("isUpdating", false));
        setStatus(previousStatus);
    }

    @Test
    void loadingShowsOverlayAndConnectedHidesIt() throws Exception {
        SwingUtilities.invokeAndWait(() -> assertFalse(overlay.isVisible()));
        setStatus(ConnectionStatusValue.LOADING);
        SwingUtilities.invokeAndWait(() -> {
            assertTrue(overlay.isVisible());
            assertTrue(paintsColor(overlay, Color.WHITE), "Loading text is white");
            assertFalse(overlay.contains(400, 200), "Status overlay must not intercept mouse input");
        });

        setStatus(ConnectionStatusValue.CONNECTED);
        SwingUtilities.invokeAndWait(() -> assertFalse(overlay.isVisible()));
    }

    @Test
    void disconnectDuringLoadingHidesOverlayAndTurnsIndicatorRed() throws Exception {
        setStatus(ConnectionStatusValue.LOADING);
        setStatus(ConnectionStatusValue.NOT_CONNECTED);

        SwingUtilities.invokeAndWait(() -> {
            assertFalse(overlay.isVisible());
            assertEquals(Color.RED, indicatorColor());
            assertTrue(icon.getToolTipText().startsWith("Disconnected."));
        });
    }

    @Test
    void updatingTakesPriorityAndClearingItRestoresLoading() throws Exception {
        setStatus(ConnectionStatusValue.LOADING);
        SwingUtilities.invokeAndWait(() -> {
            panel.tabbedPane.putClientProperty("isUpdating", true);
            assertTrue(overlay.isVisible());
            assertTrue(paintsColor(overlay, Color.ORANGE));
            assertFalse(paintsColor(overlay, Color.WHITE));
            panel.tabbedPane.putClientProperty("isUpdating", false);
            assertTrue(overlay.isVisible());
            assertTrue(paintsColor(overlay, Color.WHITE));
        });
    }

    @Test
    void endingUpdateWhileDisconnectedHidesOverlay() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            panel.tabbedPane.putClientProperty("isUpdating", true);
            assertTrue(overlay.isVisible());
            panel.tabbedPane.putClientProperty("isUpdating", false);
            assertFalse(overlay.isVisible());
        });
    }

    @Test
    void issue10282LateOutputResponseCurrentlyShowsLoadingAndConnectedForClosedStream() throws Exception {
        TestStream stream = new TestStream();
        BinaryProtocol protocol = spy(new BinaryProtocol(new LinkManager(), stream));
        try {
            String secondsName = WellKnownGauges.SECONDS.getOutputChannelName();
            IniFileModel ini = IniFileModelMocks.empty();
            IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
            when(ini.getMetaInfo()).thenReturn(meta);
            when(meta.getOchBlockSize()).thenReturn(2);
            when(ini.getBlockingFactor()).thenReturn(2);
            ScalarIniField seconds = new ScalarIniField(secondsName, 0, "s", FieldType.UINT16, 1, "0", 0);
            when(ini.getAllOutputChannels()).thenReturn(Collections.singletonMap(secondsName, seconds));
            when(ini.getOutputChannel(secondsName)).thenReturn(seconds);
            protocol.setIniFileForUnitTest(ini);
            doAnswer(invocation -> {
                // The response arrived before unplug, but decoding resumes after close.
                stream.close();
                flushEdt();
                SwingUtilities.invokeAndWait(() -> {
                    assertFalse(overlay.isVisible());
                    assertEquals(Color.RED, indicatorColor());
                });
                return new byte[]{Integration.TS_RESPONSE_OK, 42, 0};
            }).when(protocol).executeCommand(eq(Integration.TS_OUTPUT_COMMAND), any(byte[].class), anyString());

            setStatus(ConnectionStatusValue.CONNECTED);
            // Current bad behavior, per coverage-first workflow for #10282. A fix should
            // reject the stale response, keep the overlay hidden and indicator disconnected.
            assertTrue(protocol.requestOutputChannels());
            flushEdt();

            assertTrue(protocol.isClosed());
            SwingUtilities.invokeAndWait(() -> {
                assertTrue(overlay.isVisible());
                assertTrue(paintsColor(overlay, Color.WHITE));
                assertEquals(Color.GREEN, indicatorColor());
                assertTrue(icon.getToolTipText().startsWith("Connected."));
            });
            verify(protocol).executeCommand(eq(Integration.TS_OUTPUT_COMMAND), any(byte[].class), anyString());
        } finally {
            protocol.close();
        }
    }

    private static void setStatus(ConnectionStatusValue status) throws Exception {
        ConnectionStatusLogic.INSTANCE.setValue(status);
        flushEdt();
    }

    private static void flushEdt() throws Exception {
        SwingUtilities.invokeAndWait(() -> {});
    }

    private static Color indicatorColor() {
        BufferedImage image = new BufferedImage(16, 16, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = image.createGraphics();
        try {
            icon.getIcon().paintIcon(icon, g, 0, 0);
        } finally {
            g.dispose();
        }
        return new Color(image.getRGB(8, 8), true);
    }

    private static boolean paintsColor(JComponent component, Color color) {
        BufferedImage image = new BufferedImage(component.getWidth(), component.getHeight(), BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = image.createGraphics();
        try {
            component.paint(g);
        } finally {
            g.dispose();
        }
        for (int y = 0; y < image.getHeight(); y++) {
            for (int x = 0; x < image.getWidth(); x++) {
                if (image.getRGB(x, y) == color.getRGB()) {
                    return true;
                }
            }
        }
        return false;
    }

    private static class TestStream extends AbstractIoStream {
        @Override
        public void setInputListener(DataListener listener) {
        }

        @Override
        public IncomingDataBuffer getDataBuffer() {
            throw new AssertionError("UI status tests must not perform I/O");
        }
    }
}
