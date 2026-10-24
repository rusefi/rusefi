package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.PanelModel;
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
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.function.Consumer;
import java.util.function.Supplier;

/** Owns the main calibration view and the persisted, modeless copies of selected pages. */
public final class CalibrationPanelWorkspace {
    public static final String CONFIG_KEY = "detached_panels";
    private final UIContext uiContext;
    private final Node config;
    private final Node tableOrientationConfig;
    private final CalibrationDialogWidget primary;
    private final JScrollPane primaryScroll;
    private final DetachPanelLayer layer;
    private final Map<CalibrationPanelKey, DetachedPanel> panels = new LinkedHashMap<>();
    private final WindowFactory windowFactory;
    private final Supplier<List<Rectangle>> screens;
    private Consumer<ConfigurationImage> onConfigChange = image -> { };
    private Consumer<String> onShowInPinout = value -> { };
    private IniFileModel ini;
    private ConfigurationImage image;
    private String primaryKey;
    private byte[] detachedImageContent;
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
        this(uiContext, config, null);
    }

    public CalibrationPanelWorkspace(UIContext uiContext, Node config, Node tableOrientationConfig) {
        this(uiContext, config, tableOrientationConfig, SwingPanelWindow::new,
            CalibrationPanelWorkspace::usableScreens);
    }

    // Window factory keeps persistence and edit synchronization testable without a display.
    CalibrationPanelWorkspace(UIContext uiContext, Node config, WindowFactory windowFactory,
                              Supplier<List<Rectangle>> screens) {
        this(uiContext, config, null, windowFactory, screens);
    }

    CalibrationPanelWorkspace(UIContext uiContext, Node config, Node tableOrientationConfig,
                              WindowFactory windowFactory, Supplier<List<Rectangle>> screens) {
        this.uiContext = uiContext;
        this.config = config == null ? new Node() : config;
        this.tableOrientationConfig = tableOrientationConfig == null ? new Node() : tableOrientationConfig;
        this.windowFactory = windowFactory;
        this.screens = screens;
        primary = new CalibrationDialogWidget(uiContext, this.tableOrientationConfig);
        primaryScroll = new JScrollPane(primary.getViewportPane());
        layer = new DetachPanelLayer(primaryScroll, primary.getPanelFrame(), this::detachCurrentPanel);
        primary.setOnViewChanged(this::primaryViewChanged);
        primary.setOnTableOrientationChange(() -> orientationChanged(primary));
        primary.setOnConfigChange(changed -> edited(primary, changed));
        primary.setOnShowInPinout(value -> onShowInPinout.accept(value));
        for (int i = 0; i < this.config.getIntProperty("count", 0); i++) {
            Node saved = this.config.getChild("panel" + i);
            String key = saved.getProperty("key");
            if (!key.isEmpty()) {
                CalibrationPanelKey panelKey = new CalibrationPanelKey(key, saved.getBoolProperty("ownContent"));
                panels.put(panelKey, new DetachedPanel(panelKey, new Rectangle(
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
        IniFileModel currentIni = primary.getCurrentIniFileModel();
        ConfigurationImage currentImage = primary.getWorkingImage();
        String currentKey = primary.getCurrentKey();
        boolean onlyNavigated = !Objects.equals(primaryKey, currentKey)
            && ini == currentIni && hasSameContent(detachedImageContent, currentImage);
        ini = currentIni;
        image = currentImage;
        primaryKey = currentKey;
        detachedImageContent = copyContent(currentImage);
        layer.setAvailable(image != null && new CalibrationPanelKey(primary.getCurrentKey()).isSupported(ini));
        if (!onlyNavigated) {
            refreshDetached(null);
        }
    }

    /** [tag:detached_calibration_sync] Every view edits a clone. Publish to all peers before
     * notifying the session/toolbar, so a subsequent edit or burn cannot use an older image.
     * The source stays intact to preserve its text caret and table/curve drag state.
     * Primary replacements (undo, discard, load/reconnect) use the same fan-out; navigation
     * only fans out when it also changes image contents or the INI model.
     */
    private void edited(CalibrationDialogWidget source, ConfigurationImage changed) {
        if (synchronizing || destroyed) {
            return;
        }
        image = changed;
        detachedImageContent = copyContent(changed);
        synchronizing = true;
        try {
            if (source != primary) {
                refresh(primary, primaryScroll, new CalibrationPanelKey(primary.getCurrentKey()));
            }
            refreshDetached(source);
        } finally {
            synchronizing = false;
        }
        onConfigChange.accept(changed);
    }

    private void orientationChanged(CalibrationDialogWidget source) {
        SwingUtilities.invokeLater(() -> {
            if (destroyed || image == null) {
                return;
            }
            synchronizing = true;
            try {
                if (source != primary && primary.getCurrentKey() != null) {
                    refresh(primary, primaryScroll, new CalibrationPanelKey(primary.getCurrentKey()));
                }
                refreshDetached(source);
            } finally {
                synchronizing = false;
            }
        });
    }

    private static byte[] copyContent(ConfigurationImage image) {
        return image == null ? null : image.getContent().clone();
    }

    private static boolean hasSameContent(byte[] content, ConfigurationImage image) {
        return content == null ? image == null : image != null && Arrays.equals(content, image.getContent());
    }

    public void detachCurrentPanel() {
        CalibrationPanelKey key = new CalibrationPanelKey(primary.getCurrentKey());
        if (destroyed || image == null || !key.isSupported(ini)) {
            return;
        }
        DetachedPanel panel = panels.get(key);
        if (panel == null) {
            // A fresh window packs its own form. Only restored windows have saved outer bounds.
            panel = new DetachedPanel(key, null);
            panels.put(key, panel);
        }
        restoring = true;
        refreshDetached(null);
        panel.window.toFront();
        saveLayout();
    }

    private void refreshDetached(CalibrationDialogWidget source) {
        for (DetachedPanel panel : panels.values()) {
            if (image == null || !panel.key.isSupported(ini)) {
                panel.dispose();
                continue;
            }
            if (panel.window == null && restoring) {
                List<Rectangle> availableScreens = screens.get();
                Rectangle screen = owner == null ? availableScreens.get(0)
                    : screenForBounds(owner.getBounds(), availableScreens);
                openPanel(panel, new Point(screen.x + 40, screen.y + 40), screen);
            } else if (panel.widget != null && panel.widget != source) {
                refresh(panel.widget, panel.scroll, panel.key);
            }
            if (panel.window != null) {
                panel.window.setCanSplit(!panel.key.splitParts(ini).isEmpty());
            }
        }
    }

    private void openPanel(DetachedPanel panel, Point position, Rectangle screen) {
        panel.widget = new CalibrationDialogWidget(uiContext, tableOrientationConfig);
        panel.scroll = new JScrollPane(panel.widget.getViewportPane());
        panel.widget.setOnConfigChange(changed -> edited(panel.widget, changed));
        CalibrationDialogWidget widget = panel.widget;
        panel.widget.setOnTableOrientationChange(() -> orientationChanged(widget));
        panel.widget.setOnShowInPinout(value -> onShowInPinout.accept(value));
        refresh(panel.widget, panel.scroll, panel.key);
        panel.window = windowFactory.create(owner, title(panel.key), panel.scroll,
            () -> closePanel(panel.key), () -> splitPanel(panel.key), !panel.key.splitParts(ini).isEmpty(), bounds -> {
                panel.bounds = new Rectangle(bounds);
                saveLayout();
            });
        boolean fresh = panel.bounds == null;
        if (fresh) {
            panel.bounds = panel.window.getBounds();
            panel.bounds.setLocation(position);
        }
        panel.bounds = fitBounds(panel.bounds, fresh ? Collections.singletonList(screen) : screens.get());
        panel.window.setBounds(panel.bounds);
        panel.widget.setActive(true);
        panel.window.show();
    }

    private String title(CalibrationPanelKey key) {
        return panelTitle(key.name) + (key.ownContent ? " — General" : "");
    }

    private String panelTitle(String key) {
        if (ini.getDialogs().containsKey(key)) {
            String title = ini.getDialogs().get(key).getUiName();
            if (title != null && !title.trim().isEmpty()) {
                return title;
            }
            // Anonymous layout groups still need a useful window title after splitting.
            for (DialogModel parent : ini.getDialogs().values()) {
                if (parent.getUiName() == null || parent.getUiName().trim().isEmpty()) {
                    continue;
                }
                for (PanelModel panel : parent.getPanels()) {
                    if (key.equals(panel.getPanelName())) {
                        return parent.getUiName() + " — " + placementLabel(panel.getPlacement());
                    }
                }
            }
            return "Settings";
        }
        return ini.getTable(key) != null ? ini.getTable(key).getTitle() : ini.getCurves().get(key).getTitle();
    }

    private static String placementLabel(String placement) {
        if ("west".equalsIgnoreCase(placement)) {
            return "Left";
        }
        if ("east".equalsIgnoreCase(placement)) {
            return "Right";
        }
        if ("north".equalsIgnoreCase(placement)) {
            return "Top";
        }
        if ("south".equalsIgnoreCase(placement)) {
            return "Bottom";
        }
        return "Settings";
    }

    private void refresh(CalibrationDialogWidget widget, JScrollPane scroll, CalibrationPanelKey key) {
        Point position = scroll.getViewport().getViewPosition();
        if (key.ownContent) {
            widget.update(key.ownDialog(ini), ini, image);
        } else {
            widget.update(key.name, ini, image);
        }
        scroll.getViewport().setViewPosition(position);
    }

    private void splitPanel(CalibrationPanelKey key) {
        DetachedPanel parent = panels.get(key);
        List<CalibrationPanelKey> parts = key.splitParts(ini);
        if (destroyed || parent == null || parent.window == null || parts.isEmpty()
                || !commitEditors(parent.scroll)) {
            return;
        }
        Rectangle origin = parent.window.getBounds();
        int offset = 0;
        for (CalibrationPanelKey part : parts) {
            DetachedPanel child = panels.get(part);
            if (child == null) {
                child = new DetachedPanel(part, null);
                panels.put(part, child);
            }
            if (child.window == null) {
                Rectangle screen = screenForBounds(origin, screens.get());
                openPanel(child, new Point(origin.x + offset, origin.y + offset), screen);
            }
            child.window.toFront();
            offset += 32;
        }
        // Commit editors and create all replacement views before disposing the original.
        closePanel(key);
    }

    private static boolean commitEditors(Component component) {
        if (component instanceof JTable) {
            JTable table = (JTable) component;
            if (table.isEditing() && !table.getCellEditor().stopCellEditing()) {
                return false;
            }
        }
        if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                if (!commitEditors(child)) {
                    return false;
                }
            }
        }
        return true;
    }

    private void closePanel(CalibrationPanelKey key) {
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
            saved.setProperty("key", panel.key.name);
            saved.setProperty("ownContent", panel.key.ownContent);
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
        Rectangle screen = screenForBounds(bounds, screens);
        int width = Math.min(screen.width, Math.max(1, bounds.width));
        int height = Math.min(screen.height, Math.max(1, bounds.height));
        return new Rectangle(Math.max(screen.x, Math.min(bounds.x, screen.x + screen.width - width)),
            Math.max(screen.y, Math.min(bounds.y, screen.y + screen.height - height)), width, height);
    }

    private static Rectangle screenForBounds(Rectangle bounds, List<Rectangle> screens) {
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
        return screen;
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
        final CalibrationPanelKey key;
        Rectangle bounds;
        CalibrationDialogWidget widget;
        JScrollPane scroll;
        PanelWindow window;

        DetachedPanel(CalibrationPanelKey key, Rectangle bounds) {
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
        void setBounds(Rectangle bounds);
        void setCanSplit(boolean canSplit);
        void dispose();
    }

    interface WindowFactory {
        PanelWindow create(Window owner, String title, JScrollPane content,
                           Runnable close, Runnable split, boolean canSplit, Consumer<Rectangle> moved);
    }

    private static final class SwingPanelWindow implements PanelWindow {
        private final JDialog dialog;
        private final Consumer<Boolean> setCanSplit;
        private boolean disposed;

        SwingPanelWindow(Window owner, String title, JScrollPane content,
                         Runnable close, Runnable split, boolean canSplit, Consumer<Rectangle> moved) {
            dialog = new JDialog(owner, title, Dialog.ModalityType.MODELESS);
            dialog.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
            setCanSplit = DetachedPanelTitleBar.install(dialog, split, canSplit);
            dialog.add(content);
            dialog.pack();
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
        public void setBounds(Rectangle bounds) {
            dialog.setBounds(bounds);
        }

        @Override
        public void setCanSplit(boolean canSplit) {
            setCanSplit.accept(canSplit);
        }

        @Override
        public void dispose() {
            disposed = true;
            dialog.dispose();
        }
    }
}
