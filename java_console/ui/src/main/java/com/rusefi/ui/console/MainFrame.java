package com.rusefi.ui.console;

import com.devexperts.logging.Logging;
import com.rusefi.*;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.EngineState;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.*;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.maintenance.VersionChecker;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.util.ExitUtil;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.util.Objects;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.time.ZoneOffset;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;

public class MainFrame {
    private static final Logging log = getLogging(Launcher.class);

    @NotNull
    private final ConsoleUI consoleUI;
    private final TabbedPanel tabbedPane;

    /**
     * @see StartupFrame
     */
    private final FrameHelper frame = new FrameHelper() {
        @Override
        protected void onWindowOpened() {
            log.info("onWindowOpened");
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
            log.info("onWindowClosed");
        }
    };

    public final ConnectionStatusLogic.ConnectionStateListener listener;

    public MainFrame(ConsoleUI consoleUI, TabbedPanel tabbedPane) {
        this.consoleUI = Objects.requireNonNull(consoleUI);
        this.tabbedPane = tabbedPane;
        listener = ConnectionStatusLogic.ConnectionStateListener.VOID;
    }

    private void windowOpenedHandler() {
        setTitle();
        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> SwingUtilities.invokeLater(() -> {
            setTitle();
            // this would repaint status label
            AutoupdateUtil.trueLayoutAndRepaint(tabbedPane.tabbedPane);
            if (ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.CONNECTED) {
                LocalDateTime dateTime = LocalDateTime.now(ZoneOffset.systemDefault());
                String isoDateTime = dateTime.format(DateTimeFormatter.ISO_LOCAL_DATE_TIME);
                consoleUI.uiContext.getLinkManager().execute(() -> consoleUI.uiContext.getCommandQueue().write(IoUtil.getSetCommand(Integration.CMD_DATE) +
                                " " + isoDateTime, CommandQueue.DEFAULT_TIMEOUT,
                        InvocationConfirmationListener.VOID, false));
            }
        }));

        final LinkManager linkManager = consoleUI.uiContext.getLinkManager();
        linkManager.getConnector().connectAndReadConfiguration(new BinaryProtocol.Arguments(true), new ConnectionStatusLogic.ConnectionStateListener() {
            @Override
            public void onConnectionFailed(String errorMessage) {
                log.error("onConnectionFailed " + errorMessage);
                String message = "This copy of rusEFI console is not compatible with this version of firmware\r\n" +
                        errorMessage;
                JOptionPane.showMessageDialog(frame.getFrame(), message);
            }

            @Override
            public void onConnectionEstablished() {
                ConnectionWatchdog.init(linkManager);

                SwingUtilities.invokeLater(() -> {
//                    tabbedPane.settingsTab.showContent(linkManager);
                    tabbedPane.logsManager.showContent();
                    /**
                     * todo: we are definitely not handling reconnect properly, no code to shut down old instance of server
                     * before launching new instance
                     */
                    new BinaryProtocolServer().start(linkManager);
                });

            }
        });

        consoleUI.uiContext.getLinkManager().getEngineState().registerStringValueAction(Integration.PROTOCOL_VERSION_TAG, new EngineState.ValueCallback<String>() {
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
        String consoleVersion = "Console " + Launcher.CONSOLE_VERSION;
        String frameTitle;
        if (ConnectionStatusLogic.INSTANCE.isConnected()) {
            BinaryProtocol bp = consoleUI.uiContext.getBinaryProtocol();
            String signature = bp == null ? "not loaded" : bp.signature;
            frameTitle = consoleVersion + "; firmware=" + Launcher.firmwareVersion.get() + "@" + consoleUI.getPort() + " " + signature;
            frame.getFrame().setTitle(frameTitle);
        } else {
            frameTitle = "DISCONNECTED " + consoleVersion;
        }
        frame.getFrame().setTitle(frameTitle);
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
        BinaryProtocol bp = consoleUI.uiContext.getBinaryProtocol();
        if (bp != null && !bp.isClosed())
            bp.close(); // it could be that serial driver wants to be closed explicitly
        ExitUtil.exit("windowClosedHandler", 0);
    }
}
