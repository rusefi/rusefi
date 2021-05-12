package com.rusefi.binaryprotocol;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.generated.Fields;

public class BinaryProtocolState {
    private final Object imageLock = new Object();
    private ConfigurationImage controller;
    /**
     * Snapshot of current gauges status
     * @see Fields#TS_OUTPUT_COMMAND
     */
    private byte[] currentOutputs;

    public void setController(ConfigurationImage controller) {
        synchronized (imageLock) {
            this.controller = controller.clone();
        }
    }

    public byte[] getCurrentOutputs() {
        return currentOutputs;
    }

    public void setCurrentOutputs(byte[] currentOutputs) {
        this.currentOutputs = currentOutputs;
    }

    public ConfigurationImage getControllerConfiguration() {
        synchronized (imageLock) {
            if (controller == null)
                return null;
            return controller.clone();
        }
    }

    public void setRange(byte[] src, int scrPos, int offset, int count) {
        synchronized (imageLock) {
            System.arraycopy(src, scrPos, controller.getContent(), offset, count);
        }
    }
}
