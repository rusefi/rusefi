package com.rusefi.autodetect;

import com.rusefi.IoUtil;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.light.LightweightGUI;

import java.util.concurrent.atomic.AtomicBoolean;

public class ReconnectSandbox {
    public static void main(String[] args) {

        LinkManager linkManager = new LinkManager();

        LightweightGUI.waitForDeviceAndStart(linkManager);

        AtomicBoolean status = new AtomicBoolean();

        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> status.set(isConnected));

        while (true) {
            System.out.println("Hello " + status);
            IoUtil.sleepSeconds(1);
        }
    }

}
