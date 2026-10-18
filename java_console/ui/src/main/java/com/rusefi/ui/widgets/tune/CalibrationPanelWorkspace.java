package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.HierarchyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Consumer;
import java.util.function.Supplier;

/** Owns the main calibration view and the persisted, modeless copies of selected pages. */
public final class CalibrationPanelWorkspace {
    public static final String CONFIG_KEY = "detached_panels";
    private final UIContext uiContext;
    private final Node config;
    private final CalibrationDialogWidget primary;
    private final JScrollPane primaryScroll;
    private final DetachPanelLayer layer;
    private final Map<String, DetachedPanel> panels = new LinkedHashMap<>();
    private final WindowFactory windowFactory;
    private final Supplier<List<Rectangle>> screens;
    private Consumer<ConfigurationImage> onConfigChange = image -> { };
    private Consumer<String> onShowInPinout = value -> { };
    private IniFileModel ini;
    private ConfigurationImage image;
    private Window owner;
    private boolean restoring;
    private boolean synchronizing;
    private boolean destroyed;
    private final WindowAdapter ownerListener = new WindowAdapter() {
        @Override
        public void windowOpened(WindowEvent event) {
            restoreWindows();
        }
    };

    public CalibrationPanelWorkspace(UIContext uiContext, Node config) {
        this(uiContext, config, SwingPanelWindow::new, CalibrationPanelWorkspace::usableScreens);
    }

    // Window factory keeps persistence and edit synchronization testable without a display.
    CalibrationPanelWorkspace(UIContext uiContext, Node config, WindowFactory windowFactory,
                              Supplier<List<Rectangle>> screens) {
        this.uiContext = uiContext;
        this.config = config == null ? new Node() : config;
        this.windowFactory = windowFactory;
        this.screens = screens;
        primary = new CalibrationDialogWidget(uiContext);
        primaryScroll = new JScrollPane(primary.getViewportPane());
        layer = new DetachPanelLayer(primaryScroll, this::detachCurrentPanel);
        primary.setOnViewChanged(this::primaryViewChanged);
        primary.setOnConfigChange(changed -> edited(primary, changed));
        primary.setOnShowInPinout(value -> onShowInPinout.accept(value));
        for (int i = 0; i < this.config.getIntProperty("count", 0); i++) {
            Node saved = this.config.getChild("panel" + i);
            String key = saved.getProperty("key");
            if (!key.isEmpty()) {
                panels.put(key, new DetachedPanel(key, new Rectangle(
                    saved.getIntProperty("x", 80), saved.getIntProperty("y", 80),
                    saved.getIntProperty("width", 800), saved.getIntProperty("height", 600))));
            }
        }
        layer.getContent().addHierarchyListener(event -> {
            if ((event.getChangeFlags() & HierarchyEvent.SHOWING_CHANGED) != 0
                    && layer.getContent().isShowing()) {
                restore(SwingUtilities.getWindowAncestor(layer.getContent()));
            }
        });
    }

    public CalibrationDialogWidget getPrimary() {
        return primary;
    }

    public JComponent getContent() {
        return layer.getContent();
    }

    public void setOnConfigChange(Consumer<ConfigurationImage> listener) {
        onConfigChange = listener;
    }

    public void setOnShowInPinout(Consumer<String> listener) {
        onShowInPinout = listener;
    }

    /** Can be called before the owner opens, including when the Tuning tab is not selected. */
    public void restore(Window owner) {
        if (destroyed) {
            return;
        }
        if (this.owner != owner) {
            if (this.owner != null) {
                this.owner.removeWindowListener(ownerListener);
            }
            this.owner = owner;
            if (owner != null) {
                owner.addWindowListener(ownerListener);
            }
        }
        if (owner != null && owner.isShowing()) {
            restoreWindows();
        }
    }

    void restoreWindows() {
        if (destroyed) {
            return;
        }
        restoring = true;
        refreshDetached(null);
    }

    private void primaryViewChanged() {
        if (synchronizing || destroyed) {
            return;
        }
        ini = primary.getCurrentIniFileModel();
        image = primary.getWorkingImage();
        layer.setAvailable(image != null && supports(primary.getCurrentKey()));
        refreshDetached(null);
    }

    /** [tag:detached_calibration_sync] Every view edits a clone. Publish to all peers before
     * notifying the session/toolbar, so a subsequent edit or burn cannot use an older image.
     * The source stays intact to preserve its text caret and table/curve drag state.
     * Primary replacements (undo, discard, navigation, load/reconnect) use the same fan-out.
     */
    private void edited(CalibrationDialogWidget source, ConfigurationImage changed) {
        if (synchronizing || destroyed) {
            return;
        }
        image = changed;
        synchronizing = true;
        try {
            if (source != primary) {
                refresh(primary, primaryScroll, primary.getCurrentKey());
            }
            refreshDetached(source);
        } finally {
            synchronizing = false;
        }
        onConfigChange.accept(changed);
    }

    public void detachCurrentPanel() {
        String key = primary.getCurrentKey();
        if (destroyed || image == null || !supports(key)) {
            return;
        }
        DetachedPanel panel = panels.get(key);
        if (panel == null) {
            Rectangle screen = screens.get().get(0);
            Rectangle bounds = new Rectangle(screen.x + 40, screen.y + 40,
                Math.max(480, primaryScroll.getWidth()), Math.max(360, primaryScroll.getHeight()));
            panel = new DetachedPanel(key, fitBounds(bounds, screens.get()));
            panels.put(key, panel);
        }
        restoring = true;
        refreshDetached(null);
        panel.window.toFront();
        saveLayout();
    }

    private boolean supports(String key) {
        return ini != null && key != null && (ini.getDialogs().containsKey(key)
            || ini.getTable(key) != null || ini.getCurves().containsKey(key));
    }

    private void refreshDetached(CalibrationDialogWidget source) {
        for (DetachedPanel panel : panels.values()) {
            if (image == null || !supports(panel.key)) {
                panel.dispose();
                continue;
            }
            if (panel.window == null && restoring) {
                panel.widget = new CalibrationDialogWidget(uiContext);
                panel.scroll = new JScrollPane(panel.widget.getViewportPane());
                panel.widget.setOnConfigChange(changed -> edited(panel.widget, changed));
                panel.widget.setOnShowInPinout(value -> onShowInPinout.accept(value));
                panel.widget.update(panel.key, ini, image);
                panel.bounds = fitBounds(panel.bounds, screens.get());
                panel.window = windowFactory.create(owner, title(panel.key), panel.scroll, panel.bounds,
                    () -> closePanel(panel.key), bounds -> {
                        panel.bounds = new Rectangle(bounds);
                        saveLayout();
                    });
                panel.widget.setActive(true);
                panel.window.show();
            } else if (panel.widget != null && panel.widget != source) {
                refresh(panel.widget, panel.scroll, panel.key);
            }
        }
    }

    private String title(String key) {
        if (ini.getDialogs().containsKey(key)) {
            String title = ini.getDialogs().get(key).getUiName();
            return title == null || title.isEmpty() ? key : title;
        }
        return ini.getTable(key) != null ? ini.getTable(key).getTitle() : ini.getCurves().get(key).getTitle();
    }

    private void refresh(CalibrationDialogWidget widget, JScrollPane scroll, String key) {
        Point position = scroll.getViewport().getViewPosition();
        widget.update(key, ini, image);
        scroll.getViewport().setViewPosition(position);
    }

    void closePanel(String key) {
        DetachedPanel panel = panels.remove(key);
        if (panel != null) {
            panel.dispose();
            saveLayout();
        }
    }

    private void saveLayout() {
        config.getConfig().clear();
        config.setProperty("count", panels.size());
        int index = 0;
        for (DetachedPanel panel : panels.values()) {
            if (panel.window != null) {
                panel.bounds = panel.window.getBounds();
            }
            Node saved = config.getChild("panel" + index++);
            saved.setProperty("key", panel.key);
            saved.setProperty("x", panel.bounds.x);
            saved.setProperty("y", panel.bounds.y);
            saved.setProperty("width", panel.bounds.width);
            saved.setProperty("height", panel.bounds.height);
        }
    }

    public void destroy() {
        if (destroyed) {
            return;
        }
        destroyed = true;
        saveLayout();
        if (owner != null) {
            owner.removeWindowListener(ownerListener);
        }
        for (DetachedPanel panel : panels.values()) {
            panel.dispose();
        }
        primary.destroy();
        layer.getContent().setUI(null);
    }

    static Rectangle fitBounds(Rectangle bounds, List<Rectangle> screens) {
        Rectangle screen = screens.get(0);
        long bestArea = 0;
        for (Rectangle candidate : screens) {
            Rectangle intersection = bounds.intersection(candidate);
            long area = intersection.isEmpty() ? 0 : (long) intersection.width * intersection.height;
            if (area > bestArea) {
                bestArea = area;
                screen = candidate;
            }
        }
        int width = Math.min(screen.width, Math.max(320, bounds.width));
        int height = Math.min(screen.height, Math.max(240, bounds.height));
        return new Rectangle(Math.max(screen.x, Math.min(bounds.x, screen.x + screen.width - width)),
            Math.max(screen.y, Math.min(bounds.y, screen.y + screen.height - height)), width, height);
    }

    private static List<Rectangle> usableScreens() {
        List<Rectangle> screens = new ArrayList<>();
        for (GraphicsDevice device : GraphicsEnvironment.getLocalGraphicsEnvironment().getScreenDevices()) {
            GraphicsConfiguration configuration = device.getDefaultConfiguration();
            Rectangle bounds = new Rectangle(configuration.getBounds());
            Insets insets = Toolkit.getDefaultToolkit().getScreenInsets(configuration);
            bounds.x += insets.left;
            bounds.y += insets.top;
            bounds.width -= insets.left + insets.right;
            bounds.height -= insets.top + insets.bottom;
            screens.add(bounds);
        }
        return screens;
    }

    private final class DetachedPanel {
        final String key;
        Rectangle bounds;
        CalibrationDialogWidget widget;
        JScrollPane scroll;
        PanelWindow window;

        DetachedPanel(String key, Rectangle bounds) {
            this.key = key;
            this.bounds = bounds;
        }

        void dispose() {
            if (window != null) {
                bounds = window.getBounds();
                widget.setOnConfigChange(null);
                window.dispose();
                window = null;
                widget.destroy();
                widget = null;
            }
        }
    }

    interface PanelWindow {
        void show();
        void toFront();
        Rectangle getBounds();
        void dispose();
    }

    interface WindowFactory {
        PanelWindow create(Window owner, String title, JScrollPane content, Rectangle bounds,
                           Runnable close, Consumer<Rectangle> moved);
    }

    private static final class SwingPanelWindow implements PanelWindow {
        private final JDialog dialog;
        private boolean disposed;

        SwingPanelWindow(Window owner, String title, JScrollPane content, Rectangle bounds,
                         Runnable close, Consumer<Rectangle> moved) {
            dialog = new JDialog(owner, title, Dialog.ModalityType.MODELESS);
            dialog.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
            dialog.add(content);
            dialog.setBounds(bounds);
            dialog.addWindowListener(new WindowAdapter() {
                @Override
                public void windowClosing(WindowEvent event) {
                    close.run();
                }
            });
            dialog.addComponentListener(new ComponentAdapter() {
                private void saveBounds() {
                    if (!disposed) {
                        moved.accept(dialog.getBounds());
                    }
                }

                @Override
                public void componentMoved(ComponentEvent event) {
                    saveBounds();
                }

                @Override
                public void componentResized(ComponentEvent event) {
                    saveBounds();
                }
            });
        }

        @Override
        public void show() {
            dialog.setVisible(true);
        }

        @Override
        public void toFront() {
            dialog.toFront();
        }

        @Override
        public Rectangle getBounds() {
            return dialog.getBounds();
        }

        @Override
        public void dispose() {
            disposed = true;
            dialog.dispose();
        }
    }
}
