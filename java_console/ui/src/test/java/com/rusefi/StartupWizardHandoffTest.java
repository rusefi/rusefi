package com.rusefi;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.wizard.WizardContainer;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.*;

class StartupWizardHandoffTest {
    private ConnectionStatusValue previousStatus;

    @BeforeEach
    void connected() {
        previousStatus = ConnectionStatusLogic.INSTANCE.getValue();
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
    }

    @AfterEach
    void restoreStatus() {
        ConnectionStatusLogic.INSTANCE.setValue(previousStatus);
    }

    @Test
    void populatedVinHandsOffImmediately() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = new Fixture("ABCDEFGHJKLMN1234");
            f.start();
            assertEquals(List.of("console"), f.events);
            verify(f.bp, never()).uploadChanges(any());
        });
    }

    @Test
    void exitingEmptyVinWizardContinuesStartup() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = new Fixture("");
            f.start();
            assertEquals(List.of("wizard"), f.events);
            button(f.wizard, "Exit Wizard").doClick(0);
            // The interrupted connection continues even though VIN remains empty.
            assertEquals(List.of("wizard", "console"), f.events);
            assertArrayEquals(new byte[32], f.image.getContent());
            verify(f.bp, never()).uploadChanges(any());
        });
    }

    @Test
    void savingVinContinuesOnlyAfterUpload() throws Exception {
        Fixture[] holder = new Fixture[1];
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = holder[0] = new Fixture("");
            f.start();
            component(f.wizard, JTextField.class).setText("1HD1KBC11KB123456");
            JButton save = button(f.wizard, "Save and Continue");
            assertTrue(save.isEnabled());
            save.doClick(0);
            assertEquals(List.of("wizard"), f.events);
            assertEquals(1, f.uploads.size());
            f.uploads.remove(0).run();
        });
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = holder[0];
            verify(f.bp).uploadChanges(any());
            assertEquals(List.of("wizard", "console"), f.events);
        });
    }

    @Test
    void startupSkipIsRespectedByConsoleAutoLaunch() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = new Fixture("");
            f.start();
            button(f.wizard, "Exit Wizard").doClick(0);
            boolean promptedAgain = ConsoleUI.showStandaloneWizard(f.context,
                new WizardContainer(f.context), () -> f.events.add("console-wizard"));
            assertFalse(promptedAgain);
            assertEquals(List.of("wizard", "console"), f.events);
            verify(f.bp, never()).uploadChanges(any());
        });
    }

    @Test
    void disconnectedWizardReturnsToStartup() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = new Fixture("");
            f.start();
            ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
            button(f.wizard, "Exit Wizard").doClick(0);
            assertEquals(List.of("wizard", "startup"), f.events);
            verify(f.bp, never()).uploadChanges(any());
        });
    }

    @Test
    void directConsolePromptsOnceAndNewConnectionCanPromptAgain() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = new UIContext();
            context.getLinkManager().setBinaryProtocolForTests(new Fixture("").bp);
            List<String> prompts = new ArrayList<>();
            WizardContainer wizard = new WizardContainer(context);
            assertTrue(ConsoleUI.showStandaloneWizard(context, wizard, () -> prompts.add("first")));
            button(wizard, "Exit Wizard").doClick(0);
            assertFalse(ConsoleUI.showStandaloneWizard(context, wizard, () -> prompts.add("duplicate")));

            // Reconnecting creates a new protocol even for the same firmware signature.
            context.getLinkManager().setBinaryProtocolForTests(new Fixture("").bp);
            assertTrue(ConsoleUI.showStandaloneWizard(context, wizard, () -> prompts.add("reconnected")));
            assertEquals(List.of("first", "reconnected"), prompts);
        });
    }

    @Test
    void repeatedExitHandsOffOnlyOnce() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = new Fixture("");
            f.start();
            JButton exit = button(f.wizard, "Exit Wizard");
            exit.doClick(0);
            exit.doClick(0);
            assertEquals(List.of("wizard", "console"), f.events);
        });
    }

    @Test
    void lostConfigurationDoesNotHandOff() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = new Fixture("");
            f.start();
            when(f.bp.getControllerConfiguration()).thenReturn(null);
            button(f.wizard, "Exit Wizard").doClick(0);
            assertEquals(List.of("wizard", "startup"), f.events);
        });
    }

    @Test
    void exitWhileSaveIsQueuedHandsOffOnlyOnce() throws Exception {
        Fixture[] holder = new Fixture[1];
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = holder[0] = new Fixture("");
            f.start();
            component(f.wizard, JTextField.class).setText("1HD1KBC11KB123456");
            button(f.wizard, "Save and Continue").doClick(0);
            button(f.wizard, "Exit Wizard").doClick(0);
            assertEquals(List.of("wizard", "console"), f.events);
            f.uploads.remove(0).run();
        });
        SwingUtilities.invokeAndWait(() -> {
            assertEquals(List.of("wizard", "console"), holder[0].events);
            verify(holder[0].bp).uploadChanges(any());
        });
    }

    @Test
    void exitIsCenteredBelowSave() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Fixture f = new Fixture("");
            f.start();
            f.wizard.setSize(1000, 700);
            layout(f.wizard);
            JButton save = button(f.wizard, "Save and Continue");
            JButton exit = button(f.wizard, "Exit Wizard");
            Point saveCenter = SwingUtilities.convertPoint(save, save.getWidth() / 2, save.getHeight(), f.wizard);
            Point exitCenter = SwingUtilities.convertPoint(exit, exit.getWidth() / 2, 0, f.wizard);
            assertTrue(exitCenter.y > saveCenter.y);
            assertEquals(saveCenter.x, exitCenter.x, 1);
        });
    }

    private static void layout(Container root) {
        root.doLayout();
        for (Component child : root.getComponents()) {
            if (child instanceof Container) {
                layout((Container) child);
            }
        }
    }

    private static class Fixture {
        final BinaryProtocol bp = mock(BinaryProtocol.class);
        final LinkManager lm = mock(LinkManager.class);
        final ConfigurationImage image = new ConfigurationImage(new byte[32]);
        final List<Runnable> uploads = new ArrayList<>();
        final List<String> events = new ArrayList<>();
        final UIContext context = new UIContext() {
            @Override public LinkManager getLinkManager() { return lm; }
            @Override public BinaryProtocol getBinaryProtocol() { return bp; }
        };
        final WizardContainer wizard;

        Fixture(String vin) {
            byte[] bytes = vin.getBytes(StandardCharsets.US_ASCII);
            System.arraycopy(bytes, 0, image.getContent(), 0, bytes.length);
            IniFileModel ini = mock(IniFileModel.class);
            when(ini.findIniField("vinNumber")).thenReturn(Optional.of(new StringIniField("vinNumber", 0, 17)));
            context.iniFileState.setIniFileModelForTest(ini);
            when(bp.getIniFileNullable()).thenReturn(ini);
            bp.signature = "rusEFI master.2026.04.18.hd81.abc123";
            when(bp.getControllerConfiguration()).thenReturn(image);
            doAnswer(invocation -> {
                uploads.add(invocation.getArgument(0));
                return null;
            }).when(lm).submit(any(Runnable.class));
            wizard = new WizardContainer(context, true);
        }

        void start() {
            StartupFrame.continueAfterSplashConnection(context, wizard,
                () -> events.add("wizard"), () -> events.add("startup"), () -> events.add("console"));
        }
    }

    private static JButton button(Container root, String text) {
        for (Component c : root.getComponents()) {
            if (c instanceof JButton && text.equals(((JButton) c).getText())) {
                return (JButton) c;
            }
            if (c instanceof Container) {
                JButton found = button((Container) c, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static <T extends Component> T component(Container root, Class<T> type) {
        for (Component c : root.getComponents()) {
            if (type.isInstance(c)) {
                return type.cast(c);
            }
            if (c instanceof Container) {
                T found = component((Container) c, type);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
