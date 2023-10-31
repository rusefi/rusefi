package com.rusefi.ui.console;

import com.devexperts.logging.Logging;
import com.rusefi.*;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.EngineState;
import com.rusefi.io.*;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.util.IoUtils;
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
            FileLog.MAIN.close();
        }
    };

    public ConnectionFailedListener listener;

    public MainFrame(ConsoleUI consoleUI, TabbedPanel tabbedPane) {
        this.consoleUI = Objects.requireNonNull(consoleUI);
        this.tabbedPane = tabbedPane;
        listener = (String s) -> {
        };
    }

    private void windowOpenedHandler() {
        setTitle();
        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> SwingUtilities.invokeLater(() -> {
            setTitle();
            UiUtils.trueRepaint(tabbedPane.tabbedPane); // this would repaint status label
            if (ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.CONNECTED) {
                LocalDateTime dateTime = LocalDateTime.now(ZoneOffset.systemDefault());
                String isoDateTime = dateTime.format(DateTimeFormatter.ISO_LOCAL_DATE_TIME);
                consoleUI.uiContext.getLinkManager().execute(() -> consoleUI.uiContext.getCommandQueue().write(IoUtil.getSetCommand(Fields.CMD_DATE) +
                                " " + isoDateTime, CommandQueue.DEFAULT_TIMEOUT,
                        InvocationConfirmationListener.VOID, false));
            }
        }));

        final LinkManager linkManager = consoleUI.uiContext.getLinkManager();
        linkManager.getConnector().connectAndReadConfiguration(new ConnectionStateListener() {
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
                    /**
                     * todo: we are definitely not handling reconnect properly, no code to shut down old instance of server
                     * before launching new instance
                     */
                    new BinaryProtocolServer().start(linkManager);
                });

            }
        });

        consoleUI.uiContext.getLinkManager().getEngineState().registerStringValueAction(Fields.PROTOCOL_VERSION_TAG, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String firmwareVersion) {
                Launcher.firmwareVersion.set(firmwareVersion);
                setTitle();
            }
        });
    }

    public FrameHelper getFrame() {
        return frame;
    }

    private void setTitle() {
        String disconnected = ConnectionStatusLogic.INSTANCE.isConnected() ? "" : "DISCONNECTED ";
        BinaryProtocol bp = consoleUI.uiContext.getLinkManager().getCurrentStreamState();
        String signature = bp == null ? "not loaded" : "Signature: " + bp.signature;
        frame.getFrame().setTitle(disconnected + "FOME EFI Console " + Launcher.CONSOLE_VERSION + "; firmware=" + Launcher.firmwareVersion.get() + "@" + consoleUI.getPort() + " " + signature);
    }

    private void windowClosedHandler() {
        /**
         * looks like reconnectTimer in {@link com.rusefi.ui.RpmPanel} keeps AWT alive. Simplest solution would be to 'exit'
         */
        SimulatorHelper.onWindowClosed();
        Node root = getConfig().getRoot();
        root.setProperty("version", Launcher.CONSOLE_VERSION);
        root.setProperty(ConsoleUI.TAB_INDEX, tabbedPane.tabbedPane.getSelectedIndex());
        getConfig().save();
        BinaryProtocol bp = consoleUI.uiContext.getLinkManager().getCurrentStreamState();
        if (bp != null && !bp.isClosed)
            bp.close(); // it could be that serial driver wants to be closed explicitly
        IoUtils.exit("windowClosedHandler", 0);
    }
}
