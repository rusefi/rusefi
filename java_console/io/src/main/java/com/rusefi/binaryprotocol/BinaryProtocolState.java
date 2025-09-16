package com.rusefi.binaryprotocol;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.generated.Integration;
import org.jetbrains.annotations.Nullable;

public class BinaryProtocolState {
    private final Object imageLock = new Object();
    private ConfigurationImage configurationImage;
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
