package com.rusefi.autotest;

import com.rusefi.FileLog;
import com.rusefi.IoUtil;
import com.rusefi.TestingUtils;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.EngineState;
import com.rusefi.io.LinkManager;
import org.jetbrains.annotations.NotNull;

/**
 * this class helps us to connect to real ECU once per JVM invocation
 */
public class ControllerConnectorState {
    public volatile static String firmwareVersion;
    private static LinkManager linkManager;

    @NotNull
    public synchronized static LinkManager getLinkManager() {
        if (linkManager != null)
            return linkManager;

        LinkManager linkManager = new LinkManager().setCompositeLogicEnabled(false);
        linkManager.getEngineState().registerStringValueAction(Fields.PROTOCOL_VERSION_TAG, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String firmwareVersion1) {
                firmwareVersion = firmwareVersion1;
            }
        });

        /**
         * with real hardware we have noise on all analog inputs which gives us random sensor data, we cannot really
         * test exact numbers yet
         */
        TestingUtils.isRealHardware = true;
        FileLog.MAIN.start();
        String port = System.getProperty("ecu.port");
        if (port == null) {
            port = PortDetector.autoDetectSerial(null);
            if (port == null)
                throw new IllegalStateException("ECU serial not detected");
        }

        IoUtil.realHardwareConnect(linkManager, port);
        ControllerConnectorState.linkManager = linkManager;
        return linkManager;
    }
}
