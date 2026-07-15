package com.rusefi.ui.console;

import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.rusefi.*;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.EngineState;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.*;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.maintenance.VersionChecker;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.ui.basic.LoadTuneHelper;
import com.rusefi.util.ExitUtil;
import javax.swing.Action;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.io.File;
import java.net.URI;
import java.util.Objects;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.time.ZoneOffset;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;
import static com.rusefi.core.net.ConnectionAndMeta.RUSEFI_WIKI_DOWNLOAD_PAGE;

/**
 * @see ConsoleUI
 */
public class MainFrame {
    private static final Logging log = getLogging(Launcher.class);

    @NotNull
    private final ConsoleUI consoleUI;
    private final TabbedPanel tabbedPane;

    /**
     * @see StartupFrame
     */
    private final FrameHelper frame;

    public final ConnectionStatusLogic.Listener listener;

    private JMenuItem loadTuneItem;
    private JMenuItem saveTuneItem;
    /**
     * user experience overview at Autoupdate.java
     */
    private JMenuItem updateSoftwareItem;
    private JMenuItem updateEcuItem;
    private Runnable updateEcuAction;
    private Runnable exitRequestHandler;
    private boolean firmwareUpdateInProgress;
    private boolean updateSoftwareAvailable;
    private boolean updateEcuAvailable;

    public MainFrame(ConsoleUI consoleUI, TabbedPanel tabbedPane) {
        this(consoleUI, tabbedPane, null);
    }

    /**
     * @param reuseFrame when non-null, the console reuses this already-visible, maximized frame
     *                   (handed off from {@link StartupFrame}) instead of creating a new window (#9715).
     */
    public MainFrame(ConsoleUI consoleUI, TabbedPanel tabbedPane, JFrame reuseFrame) {
        this.consoleUI = Objects.requireNonNull(consoleUI);
        this.tabbedPane = tabbedPane;
        listener = ConnectionStatusLogic.Listener.VOID;
        // reuseFrame == null creates a new window; non-null reuses the splash frame in place (#9715).
        this.frame = new FrameHelper(reuseFrame, JFrame.DO_NOTHING_ON_CLOSE) {
            @Override
            protected void onWindowOpened() {
                log.info("onWindowOpened");
                windowOpenedHandler();
            }

            @Override
            protected void onWindowClosed() {
                // close the port, then the log file
                windowClosedHandler();
                log.info("onWindowClosed");
            }

            @Override
            protected void onWindowClosing() {
                requestExit();
            }
        };

        createMenuBar();
    }

    private void createMenuBar() {
        JMenuBar menuBar = new JMenuBar();
        JMenu fileMenu = new JMenu("File");
        fileMenu.setMnemonic(KeyEvent.VK_F);

        loadTuneItem = new JMenuItem(LoadTuneHelper.LOAD_TUNE_TEXT);
        loadTuneItem.setMnemonic(KeyEvent.VK_L);
        loadTuneItem.setEnabled(false);
        fileMenu.add(loadTuneItem);

        saveTuneItem = new JMenuItem(LoadTuneHelper.SAVE_TUNE_TEXT);
        saveTuneItem.setMnemonic(KeyEvent.VK_S);
        saveTuneItem.setEnabled(false);
        fileMenu.add(saveTuneItem);

        fileMenu.addSeparator();

        JMenuItem exitItem = new JMenuItem("Exit");
        exitItem.setMnemonic(KeyEvent.VK_X);
        exitItem.addActionListener(e -> requestExit());
        fileMenu.add(exitItem);

        menuBar.add(fileMenu);

        JMenu actionsMenu = new JMenu("Actions");
        actionsMenu.setMnemonic(KeyEvent.VK_A);

        updateSoftwareItem = new JMenuItem("Update Software");
        updateSoftwareItem.setEnabled(false);
        updateSoftwareItem.addActionListener(e -> onUpdateSoftwareClicked());
        actionsMenu.add(updateSoftwareItem);

        updateEcuItem = new JMenuItem("No updates available");
        updateEcuItem.setEnabled(false);
        updateEcuItem.addActionListener(e -> {
            if (updateEcuAction != null) {
                updateEcuAction.run();
            }
        });
        actionsMenu.add(updateEcuItem);

        menuBar.add(actionsMenu);

        frame.getFrame().setJMenuBar(menuBar);
    }

    public void setUpdateEcuAction(Runnable action) {
        this.updateEcuAction = action;
    }

    public void setExitRequestHandler(Runnable exitRequestHandler) {
        this.exitRequestHandler = exitRequestHandler;
    }

    private void requestExit() {
        if (firmwareUpdateInProgress) {
            int choice = JOptionPane.showConfirmDialog(
                frame.getFrame(),
                "A firmware update is still in progress. Exiting now may leave the ECU unfinished. Exit anyway?",
                "Firmware Update In Progress",
                JOptionPane.YES_NO_OPTION,
                JOptionPane.WARNING_MESSAGE
            );
            if (choice != JOptionPane.YES_OPTION) {
                return;
            }
        }
        if (exitRequestHandler == null) {
            frame.getFrame().dispose();
        } else {
            exitRequestHandler.run();
        }
    }


    static boolean needsFirmwareUpdate(RusEfiSignature ecuSig, String srecName) {
        if (ecuSig == null || srecName == null) {
            return false;
        }
        RusEfiSignature srecSig = SignatureHelper.parseSrec(srecName);
        if (srecSig == null) {
            return false;
        }
        if (!srecSig.getIsLegacyFormat()) {
            return !srecSig.getHash().equals(ecuSig.getHash());
        } else {
            return !ecuSig.getYear().equals(srecSig.getYear())
                || !ecuSig.getMonth().equals(srecSig.getMonth())
                || !ecuSig.getDay().equals(srecSig.getDay());
        }
    }

    private void checkFirmwareUpdate(String firmwareVersion) {
        log.info("checkFirmwareUpdate: " + firmwareVersion);
        RusEfiSignature ecuSig = SignatureHelper.parse(firmwareVersion);
        if (ecuSig == null) {
            log.info("checkFirmwareUpdate: could not parse ECU signature");
            return;
        }
        String srecPath = FindFileHelper.findSrecFile();
        if (srecPath == null) {
            log.info("checkFirmwareUpdate: no srec file found");
            SwingUtilities.invokeLater(() -> {
                updateEcuItem.setText("No updates available");
                setUpdateEcuAvailable(false);
            });
            return;
        }
        String srecName = new File(srecPath).getName();
        log.info("checkFirmwareUpdate: srec=" + srecName);
        boolean needsUpdate = needsFirmwareUpdate(ecuSig, srecName);
        log.info("checkFirmwareUpdate: needsUpdate=" + needsUpdate);
        SwingUtilities.invokeLater(() -> {
            updateEcuItem.setText(needsUpdate ? "Update ECU Firmware" : "No updates available");
            setUpdateEcuAvailable(needsUpdate);
        });
    }

    private void onUpdateSoftwareClicked() {
        setUpdateSoftwareAvailable(false);
        Thread updateThread = new Thread(() ->
            Autoupdate.runManualUpdate(msg -> {
                if (msg != null) {
                    Autoupdate.relaunchConsole();
                }
            }), "manual-update");
        updateThread.setDaemon(true);
        updateThread.start();
    }

    private void windowOpenedHandler() {
        setTitle();
        tabbedPane.tabbedPane.addPropertyChangeListener("isUpdating", e -> SwingUtilities.invokeLater(this::setTitle));
        tabbedPane.tabbedPane.addPropertyChangeListener("bootloaderMode", e -> SwingUtilities.invokeLater(this::setTitle));
        // [tag:offline_tune] Refresh the OFFLINE title when offline mode toggles (e.g. Load Tune while
        // disconnected) — otherwise the title would lag until the next connection event. #9730
        consoleUI.uiContext.addOfflineModeListener(o -> SwingUtilities.invokeLater(this::setTitle));

        // Offer manual update whenever the launch-time silent update did not run - either because
        // the user preference is off or because the bundle hard-disables auto-update (#9775).
        if (!Autoupdate.isAutoUpdateEnabled()) {
            Thread checkThread = new Thread(() -> {
                boolean available = Autoupdate.isUpdateAvailable();
                if (available) {
                    SwingUtilities.invokeLater(() -> setUpdateSoftwareAvailable(true));
                }
            }, "update-availability-check");
            checkThread.setDaemon(true);
            checkThread.start();
        }
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
                BinaryProtocol bp = consoleUI.uiContext.getBinaryProtocol();
                if (bp != null && bp.signature != null) {
                    String sig = bp.signature;
                    Thread fwCheckThread = new Thread(() -> checkFirmwareUpdate(sig), "firmware-update-check");
                    fwCheckThread.setDaemon(true);
                    fwCheckThread.start();
                }
            } else {
                updateEcuItem.setText("No updates available");
                setUpdateEcuAvailable(false);
            }
        }));

        final LinkManager linkManager = consoleUI.uiContext.getLinkManager();
        BinaryProtocol existingBp = linkManager.getBinaryProtocol();
        boolean alreadyConnected = existingBp != null && existingBp.getControllerConfiguration() != null;
        if (alreadyConnected) {
            // Splash already did connectAndReadConfiguration. Running it again would re-open the
            // already-open serial port and fail. Run only the post-connect setup here.
            ConnectionWatchdog.init(linkManager);
            SwingUtilities.invokeLater(() -> {
                tabbedPane.logsManager.showContent();
                new BinaryProtocolServer().start(linkManager);
            });
            if (existingBp.signature != null) {
                String sig = existingBp.signature;
                Thread fwCheckThread = new Thread(() -> checkFirmwareUpdate(sig), "firmware-update-check");
                fwCheckThread.setDaemon(true);
                fwCheckThread.start();
            }
        } else {
            linkManager.getConnector().connectAndReadConfiguration(new BinaryProtocol.Arguments(true), new ConnectionStatusLogic.Listener() {
                @Override
                public void onConnectionStatus(boolean isConnected) {}

                @Override
                public void onConnectionFailed(String errorMessage) {
                    log.error("onConnectionFailed " + errorMessage);
                    SwingUtilities.invokeLater(() -> showConnectionFailedDialog(errorMessage));
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
        }

        consoleUI.uiContext.getLinkManager().getEngineState().registerStringValueAction(Integration.PROTOCOL_VERSION_TAG, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String firmwareVersion) {
                Launcher.firmwareVersion.set(firmwareVersion);
                setTitle();
                VersionChecker.getInstance().onFirmwareVersion(firmwareVersion);
            }
        });
    }

    public void setTuneActions(Action loadAction, Action saveAction) {
        loadTuneItem.setAction(loadAction);
        loadTuneItem.setText(LoadTuneHelper.LOAD_TUNE_TEXT);
        loadTuneItem.setMnemonic(KeyEvent.VK_L);
        saveTuneItem.setAction(saveAction);
        saveTuneItem.setText(LoadTuneHelper.SAVE_TUNE_TEXT);
        saveTuneItem.setMnemonic(KeyEvent.VK_S);
        refreshFirmwareUpdateExclusion();
    }

    public void setFirmwareUpdateInProgress(boolean firmwareUpdateInProgress) {
        this.firmwareUpdateInProgress = firmwareUpdateInProgress;
        refreshFirmwareUpdateExclusion();
    }

    private void setUpdateSoftwareAvailable(boolean available) {
        updateSoftwareAvailable = available;
        refreshFirmwareUpdateExclusion();
    }

    private void setUpdateEcuAvailable(boolean available) {
        updateEcuAvailable = available;
        refreshFirmwareUpdateExclusion();
    }

    private void refreshFirmwareUpdateExclusion() {
        Action loadAction = loadTuneItem.getAction();
        loadTuneItem.setEnabled(!firmwareUpdateInProgress && loadAction != null && loadAction.isEnabled());
        updateSoftwareItem.setEnabled(!firmwareUpdateInProgress && updateSoftwareAvailable);
        updateEcuItem.setEnabled(!firmwareUpdateInProgress && updateEcuAvailable);
    }

    public FrameHelper getFrame() {
        return frame;
    }

    private void setTitle() {
        String consoleVersion = "Console " + Launcher.CONSOLE_VERSION;
        String frameTitle;
        Object bootloaderMode = tabbedPane.tabbedPane.getClientProperty("bootloaderMode");
        if (Boolean.TRUE.equals(tabbedPane.tabbedPane.getClientProperty("isUpdating"))) {
            frameTitle = "UPDATING " + consoleVersion;
        } else if (bootloaderMode != null) {
            // Board sitting in a bootloader [tag:better_ux_for_flashing] — not connected, but not a plain "disconnected" state.
            frameTitle = bootloaderMode + " BOOTLOADER " + consoleVersion;
        } else if (ConnectionStatusLogic.INSTANCE.isConnected()) {
            BinaryProtocol bp = consoleUI.uiContext.getBinaryProtocol();
            String signature = bp == null ? "not loaded" : bp.signature;
            frameTitle = consoleVersion + "; firmware=" + Launcher.firmwareVersion.get() + "@" + consoleUI.getPort() + " " + signature;
        } else if (consoleUI.uiContext.isOfflineMode()) {
            // [tag:offline_tune] no ECU — title reflects the loaded tune's signature
            IniFileModel ini = consoleUI.uiContext.iniFileState.getIniFileModel();
            String signature = ini != null ? ini.getSignature() : "no INI";
            frameTitle = "OFFLINE " + consoleVersion + " " + signature;
        } else {
            frameTitle = "DISCONNECTED " + consoleVersion;
        }
        frame.getFrame().setTitle(frameTitle);
    }

    private void showConnectionFailedDialog(String errorMessage) {
        JTextArea textArea = new JTextArea(errorMessage);
        textArea.setEditable(false);
        textArea.setOpaque(false);
        textArea.setFont(UIManager.getFont("Label.font"));
        textArea.setLineWrap(true);
        textArea.setWrapStyleWord(true);
        textArea.setColumns(50);

        JPanel panel = new JPanel(new BorderLayout(0, 10));
        panel.add(textArea, BorderLayout.CENTER);

        if (errorMessage.contains(RUSEFI_WIKI_DOWNLOAD_PAGE)) {
            JButton downloadButton = new JButton("Open Download Page");
            downloadButton.addActionListener(e -> {
                try {
                    Desktop.getDesktop().browse(new URI(RUSEFI_WIKI_DOWNLOAD_PAGE));
                } catch (Exception ex) {
                    log.error("Failed to open download URL: " + ex);
                }
            });
            JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
            buttonPanel.add(downloadButton);
            panel.add(buttonPanel, BorderLayout.SOUTH);
        }

        JOptionPane.showMessageDialog(
            frame.getFrame(),
            panel,
            "Connection Failed",
            JOptionPane.WARNING_MESSAGE
        );
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
