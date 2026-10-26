package com.rusefi.ui.console;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.sensor_logs.LoggingProfile;
import com.rusefi.sensor_logs.SensorLogger;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;
import java.awt.Component;
import java.io.File;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.argThat;
import static org.mockito.ArgumentMatchers.isNull;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/** The logging profile choice beside the record button and in the Logging menu. */
class ProfileButtonTest {
    private interface Body {
        void run() throws Exception;
    }

    private static void connected(Body body) throws Exception {
        Exception[] failure = new Exception[1];
        SwingUtilities.invokeAndWait(() -> {
            ConnectionStatusValue previous = ConnectionStatusLogic.INSTANCE.getValue();
            try {
                ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
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

    private static UIContext context() {
        UIContext context = mock(UIContext.class);
        context.sensorLogger = mock(SensorLogger.class);
        return context;
    }

    private static BinaryLoggingMenu menu(UIContext context, Node settings) {
        BinaryLoggingMenu logging = new BinaryLoggingMenu(context, null, null, null, null, settings);
        logging.saveTuneItem().setSelected(false);
        return logging;
    }

    private static JMenuItem item(JPopupMenu menu, String text) {
        for (Component component : menu.getComponents()) {
            if (component instanceof JMenuItem && text.equals(((JMenuItem) component).getText())) {
                return (JMenuItem) component;
            }
        }
        throw new AssertionError("no " + text + " item");
    }

    @Test
    void choosingAProfileUpdatesBothPlacesAndIsRemembered() throws Exception {
        connected(() -> {
            Node settings = new Node();
            BinaryLoggingMenu logging = menu(context(), settings);
            ProfileButton button = new ProfileButton(logging);
            assertEquals("Profile: Full", button.getText(), "recording everything until chosen otherwise");
            assertEquals("Profile: Full", logging.profileMenu().getText());

            JPopupMenu popup = button.menu();
            assertEquals(5, popup.getComponentCount(), "Full, Trigger, Idle, a separator, Edit profiles…");
            assertEquals("Edit profiles…", ((JMenuItem) popup.getComponent(4)).getText());
            item(popup, "Trigger").doClick();
            assertEquals("Profile: Trigger", button.getText());
            assertEquals("Profile: Trigger", logging.profileMenu().getText());
            assertTrue(((JMenuItem) logging.profileMenu().getMenuComponent(1)).isSelected());

            BinaryLoggingMenu afterRestart = menu(context(), settings);
            assertEquals("Profile: Trigger", new ProfileButton(afterRestart).getText());
        });
    }

    @Test
    void recordingUsesTheChosenProfileAndLocksIt(@TempDir Path logs) throws Exception {
        connected(() -> {
            UIContext context = context();
            when(context.sensorLogger.start(any(File.class), isNull(), any())).thenReturn(true);
            BinaryLoggingMenu logging = menu(context, new Node());
            logging.setLogsFolderForTests(logs.toFile());
            ProfileButton button = new ProfileButton(logging);
            logging.selectProfile(LoggingProfile.IDLE_ID);

            logging.toggle();
            verify(context.sensorLogger).start(any(File.class), isNull(),
                    argThat(profile -> LoggingProfile.IDLE_ID.equals(profile.getId())));

            when(context.sensorLogger.isLogging()).thenReturn(true);
            logging.refresh();
            assertFalse(button.isEnabled());
            assertEquals("Stop logging to change the profile", button.getToolTipText());
            assertFalse(logging.profileMenu().isEnabled());
            logging.selectProfile(LoggingProfile.FULL_ID);
            assertEquals("Profile: Idle", button.getText(), "the file's fields are fixed while recording");
        });
    }

    @Test
    void theNameIsNeverCutOff() throws Exception {
        connected(() -> {
            Node settings = new Node();
            BinaryLoggingMenu logging = menu(context(), settings);
            ProfileButton button = new ProfileButton(logging);
            logging.selectProfile(LoggingProfile.TRIGGER_ID);
            int textWidth = button.getFontMetrics(button.getFont()).stringWidth(button.getText());
            assertTrue(button.getPreferredSize().width > textWidth + button.getIcon().getIconWidth());
            assertEquals(button.getPreferredSize(), button.getMinimumSize(), "a squeezed layout cannot shrink it");

            String longName = "Boost run with the new turbo and injectors";
            logging.selectProfile(logging.profiles().create(longName).getId());
            assertEquals("Profile: Boost run with the new…", button.getText());
            assertTrue(button.getToolTipText().startsWith(longName + ": "), "the whole name is in the tooltip");
        });
    }

    @Test
    void hiddenWithTheRecordButton() throws Exception {
        connected(() -> {
            BinaryLoggingMenu logging = menu(context(), new Node());
            ProfileButton button = new ProfileButton(logging);
            assertTrue(button.isVisible());
            logging.showRecordButtonItem().doClick();
            assertFalse(button.isVisible());
        });
    }
}
