package com.rusefi.ui.console;

import com.rusefi.*;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolHolder;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.EngineState;
import com.rusefi.io.*;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.maintenance.VersionChecker;
import com.rusefi.ui.GaugesPanel;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.util.UiUtils;

import java.util.TimeZone;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class MainFrame {
    private final TabbedPanel tabbedPane;
    /**
     * @see StartupFrame
     */
    private FrameHelper frame = new FrameHelper() {
        @Override
        protected void onWindowOpened() {
            super.onWindowOpened();
            windowOpenedHandler();
        }

        @Override
        protected void onWindowClosed() {
            /**
             * here we would close the port and log a message about it
             */
            windowClosedHandler();
            /**
             * here we would close the log file
             */
            super.onWindowClosed();
        }
    };

    public MainFrame(TabbedPanel tabbedPane) {

        this.tabbedPane = tabbedPane;
    }

    private void windowOpenedHandler() {
        setTitle();
        ConnectionStatusLogic.INSTANCE.addListener(new ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                setTitle();
                UiUtils.trueRepaint(tabbedPane.tabbedPane); // this would repaint status label
                if (ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.CONNECTED) {
                    long unixGmtTime = System.currentTimeMillis() / 1000L;
                    long withOffset = unixGmtTime + TimeZone.getDefault().getOffset(System.currentTimeMillis()) / 1000;
                    CommandQueue.getInstance().write("set " +
                                    Fields.CMD_DATE +
                                    " " + withOffset, CommandQueue.DEFAULT_TIMEOUT,
                            InvocationConfirmationListener.VOID, false);
                }
            }
        });

        LinkManager.open(new ConnectionStateListener() {
            @Override
            public void onConnectionFailed() {
            }

            @Override
            public void onConnectionEstablished() {
                FileLog.MAIN.logLine("onConnectionEstablished");
                tabbedPane.tableEditor.showContent();
                tabbedPane.settingsTab.showContent();
                tabbedPane.logsManager.showContent();
                tabbedPane.fuelTunePane.showContent();
                BinaryProtocolServer.start();
            }
        });

        LinkManager.engineState.registerStringValueAction(Fields.PROTOCOL_VERSION_TAG, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String firmwareVersion) {
                Launcher.firmwareVersion.set(firmwareVersion);
                SensorLogger.init();
                setTitle();
                VersionChecker.getInstance().onFirmwareVersion(firmwareVersion);
            }
        });
    }

    public FrameHelper getFrame() {
        return frame;
    }

    private void setTitle() {
        String disconnected = ConnectionStatusLogic.INSTANCE.isConnected() ? "" : "DISCONNECTED ";
        frame.getFrame().setTitle(disconnected + "Console " + Launcher.CONSOLE_VERSION + "; firmware=" + Launcher.firmwareVersion.get() + "@" + Launcher.port);
    }

    private void windowClosedHandler() {
        /**
         * looks like reconnectTimer in {@link com.rusefi.ui.RpmPanel} keeps AWT alive. Simplest solution would be to 'exit'
         */
        SimulatorHelper.onWindowClosed();
        Node root = getConfig().getRoot();
        root.setProperty("version", Launcher.CONSOLE_VERSION);
        root.setProperty(Launcher.TAB_INDEX, tabbedPane.tabbedPane.getSelectedIndex());
        GaugesPanel.DetachedRepository.INSTANCE.saveConfig();
        getConfig().save();
        BinaryProtocol bp = BinaryProtocolHolder.getInstance().get();
        if (bp != null && !bp.isClosed)
            bp.close(); // it could be that serial driver wants to be closed explicitly
        System.exit(0);
    }
}
