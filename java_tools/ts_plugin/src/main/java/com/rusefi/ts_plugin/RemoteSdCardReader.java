package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.io.IoStream;
import com.rusefi.proxy.client.LocalApplicationProxy;

import javax.swing.*;
import java.awt.*;
import java.util.function.Supplier;

public class RemoteSdCardReader {
    private final JPanel content = new JPanel(new BorderLayout());

    private final SdCardReaderPanel sdCardReaderPanel;

    public RemoteSdCardReader(Supplier<ControllerAccess> controllerAccessSupplier) {

        sdCardReaderPanel = new SdCardReaderPanel(controllerAccessSupplier, new Supplier<IoStream>() {
            @Override
            public IoStream get() {
                LocalApplicationProxy localApplicationProxy = RemoteTabController.INSTANCE.getLocalApplicationProxy();
                if (localApplicationProxy == null)
                    throw new NullPointerException("Not connected");
                return localApplicationProxy.getAuthenticatorToProxyStream();
            }
        }, content.getParent());

        content.add(sdCardReaderPanel.getContent(), BorderLayout.CENTER);
    }

    public Component getContent() {
        return new JScrollPane(content, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
    }


}
