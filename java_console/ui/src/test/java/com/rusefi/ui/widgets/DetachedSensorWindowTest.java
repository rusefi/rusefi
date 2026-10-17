package com.rusefi.ui.widgets;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModelMocks;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.InputEvent;
import java.awt.event.WindowEvent;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.BooleanSupplier;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assumptions.assumeFalse;
import static org.junit.jupiter.api.Assumptions.assumeTrue;

/** Native shape/input smoke test. Run with a desktop or xvfb-run. */
class DetachedSensorWindowTest {
    @Test
    void circularWindowsHoverDragExpireAndClose() throws Exception {
        assumeFalse(GraphicsEnvironment.isHeadless());
        assumeTrue(GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice()
            .isWindowTranslucencySupported(GraphicsDevice.WindowTranslucency.PERPIXEL_TRANSPARENT));
        GaugeModel model = CircularGaugeTest.model(0, 100, 80, 0);
        UIContext context = new UIContext();
        context.iniFileState.setIniFileModelForTest(IniFileModelMocks.withGauges(model));
        Node config = new Node();
        context.DetachedRepositoryINSTANCE.init(config);
        SensorCentral.getInstance().setValue(44, model.getChannel());
        JFrame[] windows = new JFrame[3];
        JButton[] close = new JButton[2];
        Robot robot = new Robot();
        robot.setAutoDelay(40);
        robot.mouseMove(0, 0);
        try {
            SwingUtilities.invokeAndWait(() -> {
                JFrame background = new JFrame();
                background.setUndecorated(true);
                background.getContentPane().setBackground(new Color(0xEEEEEE));
                background.setBounds(80, 80, 800, 380);
                background.setVisible(true);
                windows[2] = background;
                for (int i = 0; i < 2; i++) {
                    DetachedSensor sensor = new DetachedSensor(context, model.getName(), 320);
                    JFrame frame = sensor.getFrameForTests();
                    windows[i] = frame;
                    frame.setLocation(100 + i * 400, 100);
                    frame.setVisible(true);
                    close[i] = (JButton) frame.getContentPane().getComponent(0);
                    assertTrue(frame.isUndecorated());
                    assertNotNull(frame.getShape());
                    assertFalse(frame.getShape().contains(0, 0));
                    assertTrue(frame.getShape().contains(160, 160));
                    assertFalse(close[i].isVisible());
                }
                assertEquals(2, config.getIntProperty("count", 0));
                assertEquals(320, config.getChild("detached0").getIntProperty("width", 0));
            });

            robot.mouseMove(260, 260);
            await(() -> close[0].isVisible(), "First gauge receives the real mouse hover");
            robot.mouseMove(660, 260);
            await(() -> close[1].isVisible() && !close[0].isVisible(), "Only the most recently hovered gauge shows close");
            SwingUtilities.invokeAndWait(() -> {
                assertFalse(windows[0].getShape().intersects(close[0].getBounds()));
                assertTrue(windows[1].getShape().contains(close[1].getBounds()));
            });
            String preview = System.getProperty("detachedGaugePreview");
            if (preview != null) {
                Path file = Paths.get(preview);
                Files.createDirectories(file.toAbsolutePath().getParent());
                ImageIO.write(robot.createScreenCapture(new Rectangle(80, 80, 800, 380)), "png", file.toFile());
            }

            // The badge is outside the dial, but remains a reachable native hit region.
            robot.mouseMove(802, 118);
            robot.mousePress(InputEvent.BUTTON1_DOWN_MASK);
            robot.mouseRelease(InputEvent.BUTTON1_DOWN_MASK);
            await(() -> !windows[1].isDisplayable(), "Clicking the badge closes the window");
            SwingUtilities.invokeAndWait(() -> assertEquals(1, config.getIntProperty("count", 0)));

            robot.mouseMove(260, 260);
            robot.mousePress(InputEvent.BUTTON1_DOWN_MASK);
            robot.mouseMove(320, 300);
            robot.mouseRelease(InputEvent.BUTTON1_DOWN_MASK);
            await(() -> windows[0].getLocation().equals(new Point(160, 140)), "Dragging replaces title-bar movement");
            await(() -> config.getChild("detached0").getIntProperty("xpos", 0) == 160
                && config.getChild("detached0").getIntProperty("ypos", 0) == 140, "Dragged position is saved");

            robot.mouseMove(0, 0);
            await(() -> !close[0].isVisible(), "Real one-shot timer hides close after one second");
            SwingUtilities.invokeAndWait(() -> {
                assertFalse(windows[0].getShape().intersects(close[0].getBounds()));
                windows[0].dispatchEvent(new WindowEvent(windows[0], WindowEvent.WINDOW_CLOSING));
                assertEquals(0, config.getIntProperty("count", -1));
                assertFalse(SensorCentral.getInstance().getOutputChannelDemand().getChannels()
                    .contains(model.getChannel().toLowerCase()));
            });
        } finally {
            SwingUtilities.invokeAndWait(() -> {
                for (JFrame window : windows) {
                    if (window != null) {
                        window.dispatchEvent(new WindowEvent(window, WindowEvent.WINDOW_CLOSING));
                        window.dispose();
                    }
                }
            });
        }
    }

    private static void await(BooleanSupplier condition, String message) throws Exception {
        long deadline = System.nanoTime() + 5_000_000_000L;
        AtomicBoolean ready = new AtomicBoolean();
        do {
            SwingUtilities.invokeAndWait(() -> ready.set(condition.getAsBoolean()));
            if (ready.get()) {
                return;
            }
            Thread.sleep(20);
        } while (System.nanoTime() < deadline);
        fail(message);
    }
}
