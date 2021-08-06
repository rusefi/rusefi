package com.rusefi.ui.console;

import com.rusefi.*;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.EngineState;
import com.rusefi.io.*;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.maintenance.VersionChecker;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;

import java.util.Objects;
import java.util.TimeZone;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class MainFrame {
    @NotNull
    private final ConsoleUI consoleUI;
    private final TabbedPanel tabbedPane;
    /**
     * @see StartupFrame
     */
    private final FrameHelper frame = new FrameHelper() {
        @Override
        protected void onWindowOpened() {
            FileLog.MAIN.logLine("onWindowOpened");
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
            FileLog.MAIN.logLine("onWindowClosed");
            FileLog.MAIN.close();
        }
    };

    public ConnectionFailedListener listener;

    public MainFrame(ConsoleUI consoleUI, TabbedPanel tabbedPane) {
        this.consoleUI = Objects.requireNonNull(consoleUI);

        this.tabbedPane = tabbedPane;
        listener = () -> {
        };
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
                    consoleUI.uiContext.getCommandQueue().write("set " +
                                    Fields.CMD_DATE +
                                    " " + withOffset, CommandQueue.DEFAULT_TIMEOUT,
                            InvocationConfirmationListener.VOID, false);
                }
            }
        });

        final LinkManager linkManager = consoleUI.uiContext.getLinkManager();
        linkManager.getConnector().connectAndReadConfiguration(new ConnectionStateListener() {
            @Override
            public void onConnectionFailed() {
            }

            @Override
            public void onConnectionEstablished() {
                tabbedPane.settingsTab.showContent();
                tabbedPane.logsManager.showContent();
                tabbedPane.fuelTunePane.showContent();
                new BinaryProtocolServer().start(linkManager);
            }
        });

        consoleUI.uiContext.getLinkManager().getEngineState().registerStringValueAction(Fields.PROTOCOL_VERSION_TAG, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String firmwareVersion) {
                Launcher.firmwareVersion.set(firmwareVersion);
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
        BinaryProtocol bp = consoleUI.uiContext.getLinkManager().getCurrentStreamState();
        String signature = bp == null ? "not loaded" : bp.signature;
        frame.getFrame().setTitle(disconnected + "Console " + Launcher.CONSOLE_VERSION + "; firmware=" + Launcher.firmwareVersion.get() + "@" + consoleUI.port + " " + signature);
    }

    private void windowClosedHandler() {
        /**
         * looks like reconnectTimer in {@link com.rusefi.ui.RpmPanel} keeps AWT alive. Simplest solution would be to 'exit'
         */
        SimulatorHelper.onWindowClosed();
        Node root = getConfig().getRoot();
        root.setProperty("version", Launcher.CONSOLE_VERSION);
        root.setProperty(ConsoleUI.TAB_INDEX, tabbedPane.tabbedPane.getSelectedIndex());
        consoleUI.uiContext.DetachedRepositoryINSTANCE.saveConfig();
        getConfig().save();
        BinaryProtocol bp = consoleUI.uiContext.getLinkManager().getCurrentStreamState();
        if (bp != null && !bp.isClosed)
            bp.close(); // it could be that serial driver wants to be closed explicitly
        System.exit(0);
    }
}
