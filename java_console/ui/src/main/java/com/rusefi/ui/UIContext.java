package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ini.IniFileState;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.LinkManager;
import com.rusefi.sensor_logs.SensorLogger;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

/**
 * key frontend app state singleton
 */
public class UIContext {
    private final LinkManager linkManager;

    /** Tests and offline tools: the LinkManager records the board identity into its own private instance. */
    public UIContext() {
        this.linkManager = new LinkManager();
    }

    /**
     * Production consoles: share the process-wide {@code ConnectivityContext} board identity so the
     * target recorded at connect time is visible to offline flashing decisions.
     */
    public UIContext(com.rusefi.core.io.ConnectedEcuTarget connectedEcuTarget) {
        this.linkManager = new LinkManager(connectedEcuTarget);
    }

    public SensorLogger sensorLogger = new SensorLogger(this);

    public IniFileState iniFileState = new IniFileState(this);
    public GaugesPanel.DetachedRepository DetachedRepositoryINSTANCE = new GaugesPanel.DetachedRepository(this);

    /** [tag:offline_tune] True when the console was opened in offline mode (no ECU connection, tune loaded from file). */
    private boolean offlineMode;

    @NotNull
    public LinkManager getLinkManager() {
        return linkManager;
    }

    // shortcut method considering how often it's needed
    public BinaryProtocol getBinaryProtocol() {
        return linkManager.getBinaryProtocol();
    }

    public CommandQueue getCommandQueue() {
        return linkManager.getCommandQueue();
    }

    private final List<Consumer<Boolean>> offlineModeListeners = new ArrayList<>();

    public boolean isOfflineMode() {
        return offlineMode;
    }

    public void setOfflineMode(boolean offlineMode) {
        this.offlineMode = offlineMode;
        for (Consumer<Boolean> l : offlineModeListeners) l.accept(offlineMode);
    }

    /** [tag:offline_tune] Notified (on the calling thread) whenever offline mode is toggled. */
    public void addOfflineModeListener(Consumer<Boolean> listener) {
        offlineModeListeners.add(listener);
    }

    /**
     * [tag:offline_tune] Sets the INI model directly for offline editing (no ECU connection).
     */
    public void setIniFileModelForOffline(IniFileModel ini) {
        iniFileState.setIniFileModelForTest(ini);
    }

    // ---- Config-image change notifications ----

    private final List<Consumer<ConfigurationImage>> configImageListeners = new ArrayList<>();

    public void addConfigImageListener(Consumer<ConfigurationImage> listener) {
        configImageListeners.add(listener);
    }

    /** Call on the Swing EDT whenever the working configuration image changes. */
    public void fireConfigImageChanged(ConfigurationImage ci) {
        for (Consumer<ConfigurationImage> l : configImageListeners) l.accept(ci);
    }
}
