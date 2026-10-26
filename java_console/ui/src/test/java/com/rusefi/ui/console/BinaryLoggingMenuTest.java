package com.rusefi.ui.console;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.sensor_logs.SensorLogger;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.*;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

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
                BinaryLoggingMenu logging = new BinaryLoggingMenu(context, null, null, null, null, new Node());
                JMenuItem start = logging.startItem();
                JMenuItem stop = logging.stopItem();
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

    /** Restarting the console creates a new menu; the last recording should still be viewable. */
    @Test
    void lastLogAfterRestart(@TempDir Path folder) throws Exception {
        File recording = Files.createFile(folder.resolve("drive.mlg")).toFile();
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = mock(UIContext.class);
            context.sensorLogger = mock(SensorLogger.class);
            when(context.sensorLogger.start(eq(recording), isNull(), any())).thenReturn(true);
            // the console config node outlives the menu, like the saved config across restarts
            Node settings = new Node();
            BinaryLoggingMenu beforeRestart = new BinaryLoggingMenu(context, null, null, null, null, settings);
            beforeRestart.start(recording, null);

            BinaryLoggingMenu afterRestart = new BinaryLoggingMenu(context, null, null, null, null, settings);
            List<File> viewed = new ArrayList<>();
            afterRestart.setViewLogAction(viewed::add);
            assertEquals(recording.getAbsoluteFile(), afterRestart.lastLog());
            assertTrue(afterRestart.viewLastItem().isEnabled());
            afterRestart.viewLastItem().doClick();
            assertEquals(Collections.singletonList(recording.getAbsoluteFile()), viewed);
        });
    }

    @Test
    void stopReportsTheRecordingAndItsDuration(@TempDir Path folder) throws Exception {
        File recording = Files.createFile(folder.resolve("drive.mlg")).toFile();
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = mock(UIContext.class);
            context.sensorLogger = mock(SensorLogger.class);
            when(context.sensorLogger.start(eq(recording), isNull(), any())).thenReturn(true);
            when(context.sensorLogger.getLastRecordingSeconds()).thenReturn(12.5);
            BinaryLoggingMenu logging = new BinaryLoggingMenu(context, null, null, null, null, new Node());
            List<String> stopped = new ArrayList<>();
            logging.addRecordingStoppedListener((file, seconds) -> stopped.add(file.getName() + " " + seconds));

            logging.stop();
            assertTrue(stopped.isEmpty(), "nothing was recording");

            when(context.sensorLogger.isLogging()).thenReturn(true);
            logging.start(recording, null);
            logging.stop();
            assertEquals(Collections.singletonList("drive.mlg 12.5"), stopped);
        });
    }

    @Test
    void deletedLastLogIsNotOffered(@TempDir Path folder) throws Exception {
        Node settings = new Node();
        settings.setProperty(BinaryLoggingMenu.LAST_LOG, folder.resolve("deleted.mlg").toString());
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = mock(UIContext.class);
            context.sensorLogger = mock(SensorLogger.class);
            BinaryLoggingMenu logging = new BinaryLoggingMenu(context, null, null, null, null, settings);
            logging.setViewLogAction(file -> {
            });
            assertFalse(logging.viewLastItem().isEnabled());
        });
    }

    @Test
    void viewLastLogOpensTheFinishedRecording(@TempDir Path folder) throws Exception {
        File recording = Files.createFile(folder.resolve("drive.mlg")).toFile();
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = mock(UIContext.class);
            context.sensorLogger = mock(SensorLogger.class);
            when(context.sensorLogger.start(eq(recording), isNull(), any())).thenReturn(true);
            BinaryLoggingMenu logging = new BinaryLoggingMenu(context, null, null, null, null, new Node());
            JMenuItem viewLast = logging.viewLastItem();
            List<File> viewed = new ArrayList<>();

            assertFalse(viewLast.isEnabled(), "no viewer yet");
            logging.setViewLogAction(viewed::add);
            assertFalse(viewLast.isEnabled(), "nothing recorded yet");

            when(context.sensorLogger.isLogging()).thenReturn(true);
            logging.start(recording, null);
            assertEquals(recording, logging.lastLog());
            assertFalse(viewLast.isEnabled(), "still recording");

            when(context.sensorLogger.isLogging()).thenReturn(false);
            logging.refresh();
            assertTrue(viewLast.isEnabled());
            viewLast.doClick();
            assertEquals(Collections.singletonList(recording), viewed);
        });
    }
}
