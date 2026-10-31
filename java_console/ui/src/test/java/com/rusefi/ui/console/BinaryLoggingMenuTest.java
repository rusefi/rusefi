package com.rusefi.ui.console;

import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.sensor_logs.SensorLogger;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.io.File;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class BinaryLoggingMenuTest {
    @Test
    public void binaryLogExtensionIsAddedWhenMissing() {
        assertEquals(new File("capture.mlg"), BinaryLoggingMenu.ensureMlgExtension(new File("capture")));
        assertEquals(new File("capture.MLG"), BinaryLoggingMenu.ensureMlgExtension(new File("capture.MLG")));
    }

    @Test
    void loggingShortcutFollowsTheAvailableAction() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            ConnectionStatusValue previous = ConnectionStatusLogic.INSTANCE.getValue();
            try {
                ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
                UIContext context = mock(UIContext.class);
                context.sensorLogger = mock(SensorLogger.class);
                BinaryLoggingMenu logging = new BinaryLoggingMenu(context, null, null, null);
                JMenuItem start = logging.getMenu().getItem(0);
                JMenuItem stop = logging.getMenu().getItem(1);
                KeyStroke toggle = KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent.CTRL_DOWN_MASK);

                assertFalse(start.isEnabled());
                assertFalse(stop.isEnabled());
                assertEquals(toggle, start.getAccelerator());
                assertNull(stop.getAccelerator());

                ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
                logging.refresh();
                assertTrue(start.isEnabled());
                assertFalse(stop.isEnabled());

                when(context.sensorLogger.isLogging()).thenReturn(true);
                logging.refresh();
                assertFalse(start.isEnabled());
                assertTrue(stop.isEnabled());
                assertNull(start.getAccelerator());
                assertEquals(toggle, stop.getAccelerator());

                doAnswer(invocation -> {
                    when(context.sensorLogger.isLogging()).thenReturn(false);
                    return null;
                }).when(context.sensorLogger).stop();
                stop.doClick();
                verify(context.sensorLogger).stop();
                assertTrue(start.isEnabled());
                assertFalse(stop.isEnabled());
                assertEquals(toggle, start.getAccelerator());
                assertNull(stop.getAccelerator());
            } finally {
                ConnectionStatusLogic.INSTANCE.setValue(previous);
            }
        });
    }
}
