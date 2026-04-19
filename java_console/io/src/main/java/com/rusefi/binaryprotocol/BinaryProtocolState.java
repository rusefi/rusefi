package com.rusefi.binaryprotocol;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.generated.Integration;
import org.jetbrains.annotations.Nullable;

public class BinaryProtocolState {
    private final Object imageLock = new Object();
    private ConfigurationImage configurationImage;
    /**
     * Snapshot taken when the tune was last loaded from the ECU, used to track modifications and support diff/reset.
     */
    private ConfigurationImage cachedImage;
    /**
     * Snapshot of current gauges status
     * @see Integration#TS_OUTPUT_COMMAND
     */
    private byte[] currentOutputs;

    public void setConfigurationImage(ConfigurationImage configurationImage) {
        synchronized (imageLock) {
            this.configurationImage = configurationImage.clone();
        }
    }

    /**
     * Store a snapshot of the tune as the reference baseline (e.g., right after loading from ECU).
     */
    public void setCachedImage(ConfigurationImage configurationImage) {
        synchronized (imageLock) {
            this.cachedImage = configurationImage.clone();
        }
    }

    /**
     * @return a clone of the cached baseline image, or null if no cache has been set
     */
    @Nullable
    public ConfigurationImage getCachedImage() {
        synchronized (imageLock) {
            if (cachedImage == null)
                return null;
            return cachedImage.clone();
        }
    }

    /**
     * Reset the current in-memory image back to the cached baseline.
     * Does NOT write to the ECU — call burn separately if needed.
     *
     * @return true if a cached image existed and the reset was applied
     */
    public boolean resetToCached() {
        synchronized (imageLock) {
            if (cachedImage == null)
                return false;
            this.configurationImage = cachedImage.clone();
            return true;
        }
    }

    public byte[] getCurrentOutputs() {
        return currentOutputs;
    }

    public void setCurrentOutputs(byte[] currentOutputs) {
        this.currentOutputs = currentOutputs;
    }

    @Nullable
    public ConfigurationImage getConfigurationImage() {
        synchronized (imageLock) {
            if (configurationImage == null)
                return null;
            return configurationImage.clone();
        }
    }

    public void setRange(byte[] src, int scrPos, int offset, int count) {
        synchronized (imageLock) {
            System.arraycopy(src, scrPos, configurationImage.getContent(), offset, count);
        }
    }
}
