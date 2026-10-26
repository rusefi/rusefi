package com.rusefi.ui.widgets;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.Sensor;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.UIContext;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

/** A movable, circular gauge window with a shared hover-to-close affordance. */
public class DetachedSensor {
    private static final String NAME = "name";
    private static final String WIDTH = "width";
    public static final int DEFAULT_WIDTH = 320;
    public static final String XPOS = "xpos";
    public static final String YPOS = "ypos";

    private final JPanelWithListener gaugeWrapper = new JPanelWithListener(new BorderLayout());
    private final JFrame frame;
    private final DetachedGaugePanel content;
    private String gaugeName;
    @NotNull
    private final UIContext uiContext;
    private boolean closed;

    public DetachedSensor(UIContext uiContext, String gaugeName, int width) {
        this.uiContext = uiContext;
        frame = createFrame();
        content = new DetachedGaugePanel(gaugeWrapper, this::close, shape -> {
            if (frame.getGraphicsConfiguration().getDevice().isWindowTranslucencySupported(
                    GraphicsDevice.WindowTranslucency.PERPIXEL_TRANSPARENT)) {
                frame.setShape(shape);
            }
        });
        onChange(gaugeName);
        SensorGauge.createGaugeBody(uiContext, gaugeName, gaugeWrapper, this::onChange, null);
        int diameter = Math.max(width, DEFAULT_WIDTH);
        content.setPreferredSize(new Dimension(diameter, diameter));
        frame.setContentPane(content);
        frame.setMinimumSize(new Dimension(DEFAULT_WIDTH, DEFAULT_WIDTH));
        frame.pack();
        uiContext.DetachedRepositoryINSTANCE.add(this);

        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent event) {
                close();
            }

            @Override
            public void windowClosed(WindowEvent event) {
                close();
            }
        });
        frame.addComponentListener(new ComponentAdapter() {
            @Override
            public void componentMoved(ComponentEvent event) {
                savePosition();
            }

            @Override
            public void componentResized(ComponentEvent event) {
                savePosition();
            }
        });
    }

    private static JFrame createFrame() {
        GraphicsDevice device = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
        GraphicsConfiguration configuration = device.getDefaultConfiguration();
        // Use an alpha-capable configuration for smooth transparent edges where
        // available. Native window shaping also works without per-pixel alpha.
        if (device.isWindowTranslucencySupported(GraphicsDevice.WindowTranslucency.PERPIXEL_TRANSLUCENT)) {
            for (GraphicsConfiguration candidate : device.getConfigurations()) {
                if (candidate.isTranslucencyCapable()) {
                    configuration = candidate;
                    break;
                }
            }
        }
        JFrame frame = new JFrame(configuration);
        frame.setUndecorated(true);
        // Undecorated NORMAL windows still receive a rectangular compositor
        // frame/shadow on WSLg. POPUP bypasses that frame (X11 override-redirect).
        frame.setType(Window.Type.POPUP);
        frame.setAlwaysOnTop(true);
        frame.getRootPane().setWindowDecorationStyle(JRootPane.NONE);
        frame.getRootPane().setOpaque(false);
        if (configuration.isTranslucencyCapable()
                && device.isWindowTranslucencySupported(GraphicsDevice.WindowTranslucency.PERPIXEL_TRANSLUCENT)) {
            frame.setBackground(new Color(0, 0, 0, 0));
            // Swing clears its translucent back buffer to the root's background.
            // FlatLaf supplies an opaque theme color even for a non-opaque root,
            // which otherwise leaves a pale halo behind the antialiased rim.
            frame.getRootPane().setBackground(frame.getBackground());
        } else {
            frame.setBackground(new Color(0xF3F4F6));
        }
        frame.addPropertyChangeListener("graphicsConfiguration", event ->
            SwingUtilities.invokeLater(() -> restoreTransparency(frame)));
        return frame;
    }

    private static void restoreTransparency(JFrame frame) {
        if (!frame.isDisplayable() || frame.isOpaque()) {
            return;
        }
        GraphicsConfiguration configuration = frame.getGraphicsConfiguration();
        if (!configuration.isTranslucencyCapable() && configuration.getDevice()
                .isWindowTranslucencySupported(GraphicsDevice.WindowTranslucency.PERPIXEL_TRANSLUCENT)) {
            // X11 can replace the alpha visual with the destination screen's
            // opaque default when moving between monitors. Reapply transparency
            // after that peer change so AWT selects an alpha-capable visual.
            Color background = frame.getBackground();
            frame.setBackground(new Color(background.getRGB()));
            frame.setBackground(background);
        }
    }

    private void savePosition() {
        if (!closed) {
            uiContext.DetachedRepositoryINSTANCE.saveConfig();
        }
    }

    private void close() {
        if (closed) {
            return;
        }
        closed = true;
        content.dispose();
        gaugeWrapper.removeAllChildrenAndListeners();
        uiContext.DetachedRepositoryINSTANCE.remove(this);
        frame.dispose();
    }

    public void show(MouseEvent event) {
        show(event.getLocationOnScreen());
    }

    public void show(Point screenLocation) {
        frame.setLocation(screenLocation);
        frame.setVisible(true);
    }

    public void onChange(String gaugeName) {
        this.gaugeName = gaugeName;
        IniFileModel iniFile = uiContext.iniFileState.getIniFileModel();
        GaugeModel gaugeModel = iniFile == null ? null : iniFile.getGauge(gaugeName);
        frame.setTitle(gaugeModel == null ? gaugeName : gaugeModel.getTitle());
        savePosition();
    }

    public void saveConfig(Node child) {
        child.setProperty(NAME, gaugeName);
        child.setProperty(WIDTH, Math.min(frame.getWidth(), frame.getHeight()));
        child.setProperty(XPOS, frame.getLocation().x);
        child.setProperty(YPOS, frame.getLocation().y);
    }

    public static void create(UIContext uiContext, Node child) {
        String gaugeName = child.getProperty(NAME, Sensor.RPMGauge.name());
        int width = child.getIntProperty(WIDTH, DEFAULT_WIDTH);
        int xpos = child.getIntProperty(XPOS, 0);
        int ypos = child.getIntProperty(YPOS, 0);
        DetachedSensor sensor = new DetachedSensor(uiContext, gaugeName, width);
        sensor.frame.setLocation(xpos, ypos);
        sensor.frame.setVisible(true);
    }

    JFrame getFrameForTests() {
        return frame;
    }
}
