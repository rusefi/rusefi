package com.rusefi.ui.console;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.sensor_logs.SensorLogger;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;
import java.awt.Component;
import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.ArgumentMatchers.isNull;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/** The status strips' Start Data Logging / Stop Logging button. */
class RecordButtonTest {
    private interface Body {
        void run() throws Exception;
    }

    /** Runs on the event thread with the given connection state, restoring the previous one. */
    private static void withConnection(ConnectionStatusValue value, Body body) throws Exception {
        Exception[] failure = new Exception[1];
        SwingUtilities.invokeAndWait(() -> {
            ConnectionStatusValue previous = ConnectionStatusLogic.INSTANCE.getValue();
            try {
                ConnectionStatusLogic.INSTANCE.setValue(value);
                body.run();
            } catch (Exception e) {
                failure[0] = e;
            } finally {
                ConnectionStatusLogic.INSTANCE.setValue(previous);
            }
        });
        if (failure[0] != null) {
            throw failure[0];
        }
    }

    private static BinaryLoggingMenu menu(UIContext context) {
        BinaryLoggingMenu logging = new BinaryLoggingMenu(context, null, null, null, null, new Node());
        logging.saveTuneItem().setSelected(false);
        return logging;
    }

    private static UIContext context() {
        UIContext context = mock(UIContext.class);
        context.sensorLogger = mock(SensorLogger.class);
        return context;
    }

    @Test
    void idleButtonNeedsAConnection() throws Exception {
        withConnection(ConnectionStatusValue.NOT_CONNECTED, () -> {
            BinaryLoggingMenu logging = menu(context());
            RecordButton button = new RecordButton(logging);
            assertEquals(RecordButton.IDLE, button.getText());
            assertFalse(button.isEnabled());
            assertTrue(button.getToolTipText().startsWith("Connect to an ECU"));

            ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
            logging.refresh();
            assertTrue(button.isEnabled());
        });
    }

    @Test
    void oneClickRecordsIntoTheLogsFolderWithoutAsking(@TempDir Path logs) throws Exception {
        withConnection(ConnectionStatusValue.CONNECTED, () -> {
            UIContext context = context();
            when(context.sensorLogger.start(any(File.class), isNull(), any())).thenReturn(true);
            BinaryLoggingMenu logging = menu(context);
            logging.setLogsFolderForTests(logs.toFile());
            RecordButton button = new RecordButton(logging);
            logging.refresh();

            button.doClick();
            verify(context.sensorLogger).start(any(File.class), isNull(), any());
            File recording = logging.lastLog();
            assertEquals(logs.toFile().getAbsoluteFile(), recording.getParentFile());
            assertTrue(recording.getName().matches("rusEFI_\\d{4}-\\d{2}-\\d{2}_\\d{2}_\\d{2}_\\d{2}_\\d{3}\\.mlg"), recording.getName());
        });
    }

    @Test
    void recordingShowsElapsedTimeAndSizeAndAClickStops(@TempDir Path logs) throws Exception {
        File recording = Files.write(logs.resolve("drive.mlg"), new byte[2048]).toFile();
        withConnection(ConnectionStatusValue.CONNECTED, () -> {
            UIContext context = context();
            when(context.sensorLogger.start(eq(recording), isNull(), any())).thenReturn(true);
            when(context.sensorLogger.getLastRecordingSeconds()).thenReturn(12.1);
            BinaryLoggingMenu logging = menu(context);
            RecordButton button = new RecordButton(logging);

            when(context.sensorLogger.isLogging()).thenReturn(true);
            logging.start(recording, null);
            assertEquals("Stop Logging  ·  00:00  ·  2 KB", button.getText());
            assertTrue(button.getToolTipText().contains("drive.mlg"));

            doAnswer(invocation -> {
                when(context.sensorLogger.isLogging()).thenReturn(false);
                return null;
            }).when(context.sensorLogger).stop();
            button.doClick();
            verify(context.sensorLogger).stop();
            assertEquals(RecordButton.IDLE, button.getText());
            assertEquals("Log saved  ·  12.1 s  ·  2 KB", button.lastNotice);
        });
    }

    @Test
    void noticeViewsTheSavedLog(@TempDir Path logs) throws Exception {
        File recording = Files.write(logs.resolve("drive.mlg"), new byte[10]).toFile();
        withConnection(ConnectionStatusValue.CONNECTED, () -> {
            BinaryLoggingMenu logging = menu(context());
            List<File> viewed = new ArrayList<>();
            logging.setViewLogAction(viewed::add);
            RecordButton button = new RecordButton(logging);
            JPanel notice = button.noticePanel(recording);
            for (Component component : notice.getComponents()) {
                if (component instanceof JButton && "View".equals(((JButton) component).getText())) {
                    ((JButton) component).doClick();
                }
            }
            assertEquals(1, viewed.size());
            assertEquals(recording, viewed.get(0));
        });
    }

    @Test
    void loggingMenuShowsAndHidesTheButtonAndRemembersIt() throws Exception {
        withConnection(ConnectionStatusValue.CONNECTED, () -> {
            Node settings = new Node();
            BinaryLoggingMenu logging = new BinaryLoggingMenu(context(), null, null, null, null, settings);
            RecordButton button = new RecordButton(logging);
            assertTrue(logging.showRecordButtonItem().isSelected(), "shown by default");
            assertTrue(button.isVisible());

            logging.showRecordButtonItem().doClick();
            assertFalse(button.isVisible());

            BinaryLoggingMenu afterRestart = new BinaryLoggingMenu(context(), null, null, null, null, settings);
            assertFalse(new RecordButton(afterRestart).isVisible(), "the choice is kept in the console config");
        });
    }

    @Test
    void theLabelIsNeverCutOff() throws Exception {
        withConnection(ConnectionStatusValue.NOT_CONNECTED, () -> {
            RecordButton button = new RecordButton(menu(context()));
            assertEquals("Start Data Logging", button.getText());
            int textWidth = button.getFontMetrics(button.getFont()).stringWidth(RecordButton.IDLE);
            assertTrue(button.getPreferredSize().width > textWidth + button.getIcon().getIconWidth());
            assertEquals(button.getPreferredSize(), button.getMinimumSize(), "a squeezed layout cannot shrink it");

            button.setText("Starting…");
            assertTrue(button.getPreferredSize().width > textWidth, "a shorter label keeps the width");
        });
    }

    @Test
    void elapsedFormats() {
        assertEquals("00:42", RecordButton.elapsed(42_000));
        assertEquals("12:05", RecordButton.elapsed(725_400));
        assertEquals("1:02:09", RecordButton.elapsed(3_729_000));
    }
}
