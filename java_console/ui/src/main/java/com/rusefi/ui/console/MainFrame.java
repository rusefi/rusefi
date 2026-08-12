package com.rusefi.ui.console;

import com.devexperts.logging.FileLogger;
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
import com.rusefi.ui.util.URLLabel;
import com.rusefi.util.ExitUtil;
import javax.swing.Action;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.io.File;
import java.util.Locale;
import java.util.Objects;
import java.util.function.Consumer;
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

    enum FirmwareUpdateCheckResult {
        AVAILABLE,
        UP_TO_DATE,
        UNABLE_TO_CHECK
    }

    static final class FirmwareUpdateCheckOverlay extends JPanel {
        private static final Color GREEN = new Color(0, 128, 0);
        private final JLabel message = new JLabel("", SwingConstants.CENTER);
        private final JButton updateButton = createLargeButton("Update ECU Firmware");
        private final JButton closeButton = createLargeButton("Close");

        FirmwareUpdateCheckOverlay(Runnable onUpdate, Runnable onClose) {
            super(new GridBagLayout());
            setFocusCycleRoot(true);
            message.setFont(message.getFont().deriveFont(Font.BOLD, 32f));

            updateButton.setMnemonic(KeyEvent.VK_U);
            updateButton.addActionListener(e -> onUpdate.run());
            closeButton.setMnemonic(KeyEvent.VK_C);
            closeButton.addActionListener(e -> onClose.run());

            Dimension buttonSize = new Dimension(
                Math.max(updateButton.getPreferredSize().width, closeButton.getPreferredSize().width),
                Math.max(updateButton.getPreferredSize().height, closeButton.getPreferredSize().height)
            );
            updateButton.setPreferredSize(buttonSize);
            closeButton.setPreferredSize(buttonSize);

            JPanel actions = new JPanel(new FlowLayout(FlowLayout.CENTER, 12, 8));
            actions.add(updateButton);
            actions.add(closeButton);

            JPanel content = new JPanel(new BorderLayout(0, 24));
            content.setBorder(BorderFactory.createEmptyBorder(32, 32, 32, 32));
            content.add(message, BorderLayout.CENTER);
            content.add(actions, BorderLayout.SOUTH);
            add(content);

            showChecking();
        }

        void showChecking() {
            message.setText("Checking ECU firmware...");
            message.setForeground(Color.DARK_GRAY);
            updateButton.setVisible(false);
            closeButton.setVisible(true);
            message.getAccessibleContext().setAccessibleName(message.getText());
        }

        void requestInitialFocus() {
            closeButton.requestFocusInWindow();
        }

        void showResult(FirmwareUpdateCheckResult result) {
            switch (result) {
                case AVAILABLE:
                    message.setText("ECU firmware update available");
                    message.setForeground(GREEN);
                    updateButton.setVisible(true);
                    break;
                case UP_TO_DATE:
                    message.setText("ECU already matches the local firmware image");
                    message.setForeground(GREEN);
                    updateButton.setVisible(false);
                    break;
                default:
                    message.setText("Unable to check ECU firmware");
                    message.setForeground(Color.RED.darker());
                    updateButton.setVisible(false);
                    break;
            }
            closeButton.setVisible(true);
            message.getAccessibleContext().setAccessibleName(message.getText());
            revalidate();
            repaint();
            (updateButton.isVisible() ? updateButton : closeButton).requestFocusInWindow();
        }

        String getMessageForUnitTest() {
            return message.getText();
        }

        boolean isUpdateVisibleForUnitTest() {
            return updateButton.isVisible();
        }

        boolean isCloseVisibleForUnitTest() {
            return closeButton.isVisible();
        }

        void updateForUnitTest() {
            updateButton.doClick();
        }

        void closeForUnitTest() {
            closeButton.doClick();
        }
    }

    static final class ConnectionFailureOverlay extends JPanel {
        private final JTextArea message;
        private final JButton downloadButton = createLargeButton("Open Download Page");
        private final JButton closeButton = createLargeButton("Close");

        ConnectionFailureOverlay(String errorMessage, Runnable onDownload, Runnable onClose) {
            super(new GridBagLayout());
            setFocusCycleRoot(true);

            message = new JTextArea(errorMessage);
            message.setEditable(false);
            message.setOpaque(false);
            message.setFont(message.getFont().deriveFont(Font.BOLD, 32f));
            message.setLineWrap(true);
            message.setWrapStyleWord(true);
            message.setColumns(50);
            message.getAccessibleContext().setAccessibleName(errorMessage);

            downloadButton.setMnemonic(KeyEvent.VK_O);
            downloadButton.setVisible(onDownload != null);
            if (onDownload != null) {
                downloadButton.addActionListener(e -> onDownload.run());
            }
            closeButton.setMnemonic(KeyEvent.VK_C);
            closeButton.addActionListener(e -> onClose.run());

            JPanel actions = new JPanel(new FlowLayout(FlowLayout.CENTER, 12, 8));
            actions.add(downloadButton);
            actions.add(closeButton);

            JPanel content = new JPanel(new BorderLayout(0, 24));
            content.setBorder(BorderFactory.createEmptyBorder(32, 32, 32, 32));
            content.add(message, BorderLayout.CENTER);
            content.add(actions, BorderLayout.SOUTH);
            add(content);
        }

        void requestInitialFocus() {
            (downloadButton.isVisible() ? downloadButton : closeButton).requestFocusInWindow();
        }

        String getMessageForUnitTest() {
            return message.getText();
        }

        boolean isDownloadVisibleForUnitTest() {
            return downloadButton.isVisible();
        }

        void downloadForUnitTest() {
            downloadButton.doClick();
        }

        void closeForUnitTest() {
            closeButton.doClick();
        }
    }

    static final class UnsavedTuneChangesOverlay extends JPanel {
        private final JTextArea message;
        private final JButton saveButton;
        private final JButton discardButton = createLargeButton("Exit Without Saving");
        private final JButton cancelButton = createLargeButton("Cancel");

        UnsavedTuneChangesOverlay(String text, String saveText, Runnable onSave, Runnable onDiscard, Runnable onCancel) {
            super(new GridBagLayout());
            setFocusCycleRoot(true);

            message = new JTextArea(text);
            message.setEditable(false);
            message.setOpaque(false);
            message.setFont(message.getFont().deriveFont(Font.BOLD, 32f));
            message.setLineWrap(true);
            message.setWrapStyleWord(true);
            message.setColumns(50);
            message.getAccessibleContext().setAccessibleName(text);

            saveButton = createLargeButton(saveText);
            saveButton.setMnemonic(KeyEvent.VK_S);
            saveButton.addActionListener(e -> onSave.run());
            discardButton.setMnemonic(KeyEvent.VK_E);
            discardButton.addActionListener(e -> onDiscard.run());
            cancelButton.setMnemonic(KeyEvent.VK_C);
            cancelButton.addActionListener(e -> onCancel.run());

            JPanel actions = new JPanel(new FlowLayout(FlowLayout.CENTER, 12, 8));
            actions.add(saveButton);
            actions.add(discardButton);
            actions.add(cancelButton);

            JPanel content = new JPanel(new BorderLayout(0, 24));
            content.setBorder(BorderFactory.createEmptyBorder(32, 32, 32, 32));
            content.add(message, BorderLayout.CENTER);
            content.add(actions, BorderLayout.SOUTH);
            add(content);
        }

        void requestInitialFocus() {
            saveButton.requestFocusInWindow();
        }

        String getMessageForUnitTest() {
            return message.getText();
        }

        void saveForUnitTest() {
            saveButton.doClick();
        }

        void discardForUnitTest() {
            discardButton.doClick();
        }

        void cancelForUnitTest() {
            cancelButton.doClick();
        }
    }

    private static JButton createLargeButton(String text) {
        JButton button = new JButton(text);
        button.setFont(button.getFont().deriveFont(button.getFont().getSize() * 1.5f));
        button.setMargin(new Insets(10, 24, 10, 24));
        return button;
    }

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
    private JMenuItem checkEcuUpdateItem;
    private JMenuItem updateEcuItem;
    private JMenuItem startBinaryLoggingItem;
    private JMenuItem stopBinaryLoggingItem;
    private Runnable updateEcuAction;
    private Runnable exitRequestHandler;
    private boolean firmwareUpdateInProgress;
    private boolean updateSoftwareAvailable;
    private boolean updateEcuAvailable;
    private boolean firmwareUpdateCheckInProgress;
    private int firmwareUpdateCheckGeneration;
    private boolean unsupportedEcuBlocking;
    private final UnsupportedEcuCardHost unsupportedEcuHost;
    private FirmwareUpdateCheckOverlay firmwareUpdateCheckOverlay;
    private ConnectionFailureOverlay connectionFailureOverlay;
    private UnsavedTuneChangesOverlay unsavedTuneChangesOverlay;
    private Component previousGlassPane;
    private boolean previousGlassPaneVisible;
    private Component previousFocusOwner;
    private Component previousConnectionFailureGlassPane;
    private boolean previousConnectionFailureGlassPaneVisible;
    private Component previousConnectionFailureFocusOwner;
    private Component previousUnsavedTuneChangesGlassPane;
    private boolean previousUnsavedTuneChangesGlassPaneVisible;
    private Component previousUnsavedTuneChangesFocusOwner;

    public MainFrame(ConsoleUI consoleUI, TabbedPanel tabbedPane) {
        this(consoleUI, tabbedPane, null, null);
    }

    /**
     * @param reuseFrame when non-null, the console reuses this already-visible, maximized frame
     *                   (handed off from {@link StartupFrame}) instead of creating a new window (#9715).
     */
    public MainFrame(ConsoleUI consoleUI, TabbedPanel tabbedPane, JFrame reuseFrame) {
        this(consoleUI, tabbedPane, reuseFrame, null);
    }

    public MainFrame(ConsoleUI consoleUI, TabbedPanel tabbedPane, JFrame reuseFrame,
                     UnsupportedEcuCardHost unsupportedEcuHost) {
        this.consoleUI = Objects.requireNonNull(consoleUI);
        this.tabbedPane = tabbedPane;
        this.unsupportedEcuHost = unsupportedEcuHost;
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
        if (unsupportedEcuHost != null) {
            unsupportedEcuHost.addBlockingListener(blocking -> {
                unsupportedEcuBlocking = blocking;
                refreshFirmwareUpdateExclusion();
            });
        }
    }

    private void createMenuBar() {
        JMenuBar menuBar = new JMenuBar();
        JMenu fileMenu = new JMenu("File");
        fileMenu.setMnemonic(KeyEvent.VK_F);

        loadTuneItem = new JMenuItem(LoadTuneHelper.LOAD_TUNE_TEXT);
        loadTuneItem.setIcon(loadMenuIcon("folder-open"));
        loadTuneItem.setMnemonic(KeyEvent.VK_L);
        loadTuneItem.setEnabled(false);
        fileMenu.add(loadTuneItem);

        saveTuneItem = new JMenuItem(LoadTuneHelper.SAVE_TUNE_TEXT);
        saveTuneItem.setIcon(loadMenuIcon("floppy"));
        saveTuneItem.setMnemonic(KeyEvent.VK_S);
        saveTuneItem.setEnabled(false);
        fileMenu.add(saveTuneItem);

        fileMenu.addSeparator();

        JMenuItem exitItem = new JMenuItem("Exit");
        exitItem.setIcon(loadMenuIcon("logout"));
        exitItem.setMnemonic(KeyEvent.VK_X);
        exitItem.addActionListener(e -> requestExit());
        fileMenu.add(exitItem);

        menuBar.add(fileMenu);

        JMenu actionsMenu = new JMenu("Actions");
        actionsMenu.setMnemonic(KeyEvent.VK_A);

        updateSoftwareItem = new JMenuItem("Update Software");
        updateSoftwareItem.setIcon(loadMenuIcon("refresh"));
        updateSoftwareItem.setEnabled(false);
        updateSoftwareItem.addActionListener(e -> onUpdateSoftwareClicked());
        actionsMenu.add(updateSoftwareItem);

        checkEcuUpdateItem = new JMenuItem("Check for ECU Firmware Updates");
        checkEcuUpdateItem.setIcon(loadMenuIcon("refresh"));
        checkEcuUpdateItem.setEnabled(false);
        checkEcuUpdateItem.addActionListener(e -> requestFirmwareUpdateCheck(true));
        actionsMenu.add(checkEcuUpdateItem);

        updateEcuItem = new JMenuItem("No updates available");
        updateEcuItem.setIcon(loadMenuIcon("controller"));
        updateEcuItem.setEnabled(false);
        updateEcuItem.addActionListener(e -> {
            if (updateEcuAction != null) {
                updateEcuAction.run();
            }
        });
        actionsMenu.add(updateEcuItem);

        menuBar.add(actionsMenu);

        JMenu binaryLoggingMenu = new JMenu("Binary Logging");
        binaryLoggingMenu.setMnemonic(KeyEvent.VK_B);

        startBinaryLoggingItem = new JMenuItem("Start");
        startBinaryLoggingItem.setIcon(loadMenuIcon("player-play"));
        startBinaryLoggingItem.addActionListener(e -> chooseAndStartBinaryLogging());
        binaryLoggingMenu.add(startBinaryLoggingItem);

        stopBinaryLoggingItem = new JMenuItem("Stop");
        stopBinaryLoggingItem.setIcon(loadMenuIcon("player-stop"));
        stopBinaryLoggingItem.addActionListener(e -> {
            consoleUI.uiContext.sensorLogger.stop();
            refreshBinaryLoggingActions();
        });
        binaryLoggingMenu.add(stopBinaryLoggingItem);

        menuBar.add(binaryLoggingMenu);
        refreshBinaryLoggingActions();

        frame.getFrame().setJMenuBar(menuBar);
    }

    private void refreshBinaryLoggingActions() {
        boolean isLogging = consoleUI.uiContext.sensorLogger.isLogging();
        boolean isConnected = ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.CONNECTED;
        startBinaryLoggingItem.setEnabled(isConnected && !isLogging);
        stopBinaryLoggingItem.setEnabled(isLogging);
    }

    private void chooseAndStartBinaryLogging() {
        FileLogger.createFolderIfNeeded();
        JFileChooser chooser = new JFileChooser(new File(FileLogger.DIR));
        chooser.setDialogTitle("Save data Log");
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        chooser.setFileFilter(new FileNameExtensionFilter("Binary log files (.mlg)", "mlg"));
        chooser.setSelectedFile(new File(FileLogger.DIR,
                "rusEFI_outputChannels_" + FileLogger.getDate() + ".mlg").getAbsoluteFile());
        if (chooser.showSaveDialog(frame.getFrame()) != JFileChooser.APPROVE_OPTION) {
            return;
        }

        File file = ensureMlgExtension(chooser.getSelectedFile());
        if (file.exists() && JOptionPane.showConfirmDialog(frame.getFrame(),
                file.getName() + " already exists. Replace it?",
                "Replace Binary Log",
                JOptionPane.YES_NO_OPTION,
                JOptionPane.WARNING_MESSAGE) != JOptionPane.YES_OPTION) {
            return;
        }

        if (!consoleUI.uiContext.sensorLogger.start(file)) {
            JOptionPane.showMessageDialog(frame.getFrame(),
                    "No supported output channels are available for binary logging.",
                    "Binary Logging",
                    JOptionPane.WARNING_MESSAGE);
        }
        refreshBinaryLoggingActions();
    }

    static File ensureMlgExtension(File file) {
        return file.getName().toLowerCase(Locale.ROOT).endsWith(".mlg")
                ? file
                : new File(file.getPath() + ".mlg");
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
                "An ECU update operation is still in progress. Exiting now may leave the ECU unfinished. Exit anyway?",
                "ECU Update In Progress",
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


    static FirmwareUpdateCheckResult firmwareUpdateCheckResult(RusEfiSignature ecuSig, String srecName) {
        if (ecuSig == null || srecName == null) {
            return FirmwareUpdateCheckResult.UNABLE_TO_CHECK;
        }
        RusEfiSignature srecSig = SignatureHelper.parseSrec(srecName);
        if (srecSig == null) {
            return FirmwareUpdateCheckResult.UNABLE_TO_CHECK;
        }
        boolean needsUpdate;
        if (!srecSig.getIsLegacyFormat()) {
            needsUpdate = !srecSig.getHash().equals(ecuSig.getHash());
        } else {
            needsUpdate = !ecuSig.getYear().equals(srecSig.getYear())
                || !ecuSig.getMonth().equals(srecSig.getMonth())
                || !ecuSig.getDay().equals(srecSig.getDay());
        }
        return needsUpdate ? FirmwareUpdateCheckResult.AVAILABLE : FirmwareUpdateCheckResult.UP_TO_DATE;
    }

    static boolean needsFirmwareUpdate(RusEfiSignature ecuSig, String srecName) {
        return firmwareUpdateCheckResult(ecuSig, srecName) == FirmwareUpdateCheckResult.AVAILABLE;
    }

    static boolean isFirmwareUpdateConnectionReady(ConnectionStatusValue status) {
        return status == ConnectionStatusValue.CONNECTED;
    }

    private FirmwareUpdateCheckResult checkFirmwareUpdate(String firmwareVersion) {
        log.info("checkFirmwareUpdate: " + firmwareVersion);
        RusEfiSignature ecuSig = SignatureHelper.parse(firmwareVersion);
        if (ecuSig == null) {
            log.info("checkFirmwareUpdate: could not parse ECU signature");
            return FirmwareUpdateCheckResult.UNABLE_TO_CHECK;
        }
        String srecPath = FindFileHelper.findSrecFileForConnectedBoard(
            consoleUI.uiContext.getLinkManager().getConnectedEcuTarget());
        if (srecPath == null) {
            log.info("checkFirmwareUpdate: no srec file found");
            return FirmwareUpdateCheckResult.UNABLE_TO_CHECK;
        }
        String srecName = new File(srecPath).getName();
        log.info("checkFirmwareUpdate: srec=" + srecName);
        FirmwareUpdateCheckResult result = firmwareUpdateCheckResult(ecuSig, srecName);
        log.info("checkFirmwareUpdate: result=" + result);
        return result;
    }

    private void requestFirmwareUpdateCheck(boolean userInitiated) {
        if (!SwingUtilities.isEventDispatchThread()) {
            SwingUtilities.invokeLater(() -> requestFirmwareUpdateCheck(userInitiated));
            return;
        }
        if (firmwareUpdateCheckInProgress) {
            return;
        }

        BinaryProtocol bp = consoleUI.uiContext.getBinaryProtocol();
        String firmwareVersion = bp == null ? null : bp.signature;
        if (!isFirmwareUpdateConnectionReady(ConnectionStatusLogic.INSTANCE.getValue()) || firmwareVersion == null) {
            applyFirmwareUpdateCheckResult(FirmwareUpdateCheckResult.UNABLE_TO_CHECK);
            if (userInitiated) {
                showFirmwareUpdateCheckOverlay();
                firmwareUpdateCheckOverlay.showResult(FirmwareUpdateCheckResult.UNABLE_TO_CHECK);
            }
            return;
        }

        firmwareUpdateCheckInProgress = true;
        int generation = ++firmwareUpdateCheckGeneration;
        refreshFirmwareUpdateExclusion();
        if (userInitiated) {
            showFirmwareUpdateCheckOverlay();
        }

        Thread checkThread = new Thread(() -> {
            FirmwareUpdateCheckResult result;
            try {
                result = checkFirmwareUpdate(firmwareVersion);
            } catch (RuntimeException e) {
                log.error("checkFirmwareUpdate failed", e);
                result = FirmwareUpdateCheckResult.UNABLE_TO_CHECK;
            }
            FirmwareUpdateCheckResult completedResult = result;
            SwingUtilities.invokeLater(() -> completeFirmwareUpdateCheck(
                generation, firmwareVersion, completedResult, userInitiated));
        }, "firmware-update-check");
        checkThread.setDaemon(true);
        checkThread.start();
    }

    private void completeFirmwareUpdateCheck(int generation, String firmwareVersion,
                                             FirmwareUpdateCheckResult result, boolean userInitiated) {
        if (generation != firmwareUpdateCheckGeneration) {
            return;
        }
        firmwareUpdateCheckInProgress = false;
        BinaryProtocol bp = consoleUI.uiContext.getBinaryProtocol();
        if (!isFirmwareUpdateConnectionReady(ConnectionStatusLogic.INSTANCE.getValue())
            || bp == null
            || !Objects.equals(firmwareVersion, bp.signature)) {
            closeFirmwareUpdateCheckOverlay();
            refreshFirmwareUpdateExclusion();
            return;
        }

        applyFirmwareUpdateCheckResult(result);
        if (userInitiated && firmwareUpdateCheckOverlay != null) {
            firmwareUpdateCheckOverlay.showResult(result);
        }
    }

    private void applyFirmwareUpdateCheckResult(FirmwareUpdateCheckResult result) {
        switch (result) {
            case AVAILABLE:
                updateEcuItem.setText("Update ECU Firmware");
                break;
            case UP_TO_DATE:
                updateEcuItem.setText("ECU matches local firmware");
                break;
            default:
                updateEcuItem.setText("Unable to check ECU firmware");
                break;
        }
        setUpdateEcuAvailable(result == FirmwareUpdateCheckResult.AVAILABLE);
    }

    private void showFirmwareUpdateCheckOverlay() {
        closeUnsavedTuneChangesOverlay();
        closeConnectionFailureOverlay();
        closeFirmwareUpdateCheckOverlay();
        previousGlassPane = frame.getFrame().getGlassPane();
        previousGlassPaneVisible = previousGlassPane.isVisible();
        previousFocusOwner = KeyboardFocusManager.getCurrentKeyboardFocusManager().getFocusOwner();
        firmwareUpdateCheckOverlay = new FirmwareUpdateCheckOverlay(() -> {
            closeFirmwareUpdateCheckOverlay();
            if (updateEcuItem.isEnabled() && updateEcuAction != null) {
                updateEcuAction.run();
            }
        }, this::closeFirmwareUpdateCheckOverlay);
        frame.getFrame().setGlassPane(firmwareUpdateCheckOverlay);
        firmwareUpdateCheckOverlay.setVisible(true);
        firmwareUpdateCheckOverlay.requestInitialFocus();
    }

    private void closeFirmwareUpdateCheckOverlay() {
        if (firmwareUpdateCheckOverlay == null) {
            return;
        }
        if (frame.getFrame().getGlassPane() == firmwareUpdateCheckOverlay && previousGlassPane != null) {
            frame.getFrame().setGlassPane(previousGlassPane);
            previousGlassPane.setVisible(previousGlassPaneVisible);
        }
        firmwareUpdateCheckOverlay = null;
        previousGlassPane = null;
        previousGlassPaneVisible = false;
        if (previousFocusOwner != null) {
            previousFocusOwner.requestFocusInWindow();
            previousFocusOwner = null;
        }
    }

    private void showConnectionFailureOverlay(String errorMessage) {
        closeUnsavedTuneChangesOverlay();
        closeFirmwareUpdateCheckOverlay();
        closeConnectionFailureOverlay();
        previousConnectionFailureGlassPane = frame.getFrame().getGlassPane();
        previousConnectionFailureGlassPaneVisible = previousConnectionFailureGlassPane.isVisible();
        previousConnectionFailureFocusOwner = KeyboardFocusManager.getCurrentKeyboardFocusManager().getFocusOwner();
        Runnable onDownload = errorMessage.contains(RUSEFI_WIKI_DOWNLOAD_PAGE)
            ? () -> URLLabel.open(RUSEFI_WIKI_DOWNLOAD_PAGE)
            : null;
        connectionFailureOverlay = new ConnectionFailureOverlay(errorMessage, onDownload, this::closeConnectionFailureOverlay);
        frame.getFrame().setGlassPane(connectionFailureOverlay);
        connectionFailureOverlay.setVisible(true);
        connectionFailureOverlay.requestInitialFocus();
    }

    private void closeConnectionFailureOverlay() {
        if (connectionFailureOverlay == null) {
            return;
        }
        if (frame.getFrame().getGlassPane() == connectionFailureOverlay && previousConnectionFailureGlassPane != null) {
            frame.getFrame().setGlassPane(previousConnectionFailureGlassPane);
            previousConnectionFailureGlassPane.setVisible(previousConnectionFailureGlassPaneVisible);
        }
        connectionFailureOverlay = null;
        previousConnectionFailureGlassPane = null;
        previousConnectionFailureGlassPaneVisible = false;
        if (previousConnectionFailureFocusOwner != null) {
            previousConnectionFailureFocusOwner.requestFocusInWindow();
            previousConnectionFailureFocusOwner = null;
        }
    }

    public void showUnsavedTuneChangesOverlay(String message, String saveText,
                                               Consumer<Runnable> saveAndThen, Runnable discardAndExit) {
        closeFirmwareUpdateCheckOverlay();
        closeConnectionFailureOverlay();
        closeUnsavedTuneChangesOverlay();
        previousUnsavedTuneChangesGlassPane = frame.getFrame().getGlassPane();
        previousUnsavedTuneChangesGlassPaneVisible = previousUnsavedTuneChangesGlassPane.isVisible();
        previousUnsavedTuneChangesFocusOwner = KeyboardFocusManager.getCurrentKeyboardFocusManager().getFocusOwner();
        unsavedTuneChangesOverlay = new UnsavedTuneChangesOverlay(message, saveText,
            () -> saveAndThen.accept(() -> {
                closeUnsavedTuneChangesOverlay();
                discardAndExit.run();
            }),
            () -> {
                closeUnsavedTuneChangesOverlay();
                discardAndExit.run();
            }, this::closeUnsavedTuneChangesOverlay);
        frame.getFrame().setGlassPane(unsavedTuneChangesOverlay);
        unsavedTuneChangesOverlay.setVisible(true);
        unsavedTuneChangesOverlay.requestInitialFocus();
    }

    private void closeUnsavedTuneChangesOverlay() {
        if (unsavedTuneChangesOverlay == null) {
            return;
        }
        if (frame.getFrame().getGlassPane() == unsavedTuneChangesOverlay && previousUnsavedTuneChangesGlassPane != null) {
            frame.getFrame().setGlassPane(previousUnsavedTuneChangesGlassPane);
            previousUnsavedTuneChangesGlassPane.setVisible(previousUnsavedTuneChangesGlassPaneVisible);
        }
        unsavedTuneChangesOverlay = null;
        previousUnsavedTuneChangesGlassPane = null;
        previousUnsavedTuneChangesGlassPaneVisible = false;
        if (previousUnsavedTuneChangesFocusOwner != null) {
            previousUnsavedTuneChangesFocusOwner.requestFocusInWindow();
            previousUnsavedTuneChangesFocusOwner = null;
        }
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
        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> {
            ConnectionStatusValue status = ConnectionStatusLogic.INSTANCE.getValue();
            SwingUtilities.invokeLater(() -> {
                setTitle();
                // this would repaint status label
                AutoupdateUtil.trueLayoutAndRepaint(tabbedPane.tabbedPane);
                if (isFirmwareUpdateConnectionReady(status)) {
                    LocalDateTime dateTime = LocalDateTime.now(ZoneOffset.systemDefault());
                    String isoDateTime = dateTime.format(DateTimeFormatter.ISO_LOCAL_DATE_TIME);
                    consoleUI.uiContext.getLinkManager().execute(() -> consoleUI.uiContext.getCommandQueue().write(IoUtil.getSetCommand(Integration.CMD_DATE) +
                                    " " + isoDateTime, CommandQueue.DEFAULT_TIMEOUT,
                            InvocationConfirmationListener.VOID, false));
                    BinaryProtocol bp = consoleUI.uiContext.getBinaryProtocol();
                    if (bp != null && bp.signature != null) {
                        requestFirmwareUpdateCheck(false);
                    }
                } else {
                    firmwareUpdateCheckGeneration++;
                    firmwareUpdateCheckInProgress = false;
                    closeFirmwareUpdateCheckOverlay();
                    consoleUI.uiContext.sensorLogger.stop();
                updateEcuItem.setText("No updates available");
                setUpdateEcuAvailable(false);
            }
            refreshBinaryLoggingActions();
        });
        });

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
                requestFirmwareUpdateCheck(false);
            }
        } else {
            linkManager.getConnector().connectAndReadConfiguration(new BinaryProtocol.Arguments(true), new ConnectionStatusLogic.Listener() {
                @Override
                public void onConnectionStatus(boolean isConnected) {}

                @Override
                public void onConnectionFailed(String errorMessage) {
                    log.error("onConnectionFailed " + errorMessage);
                    consoleUI.invalidatePort(linkManager.getLastTriedPort());
                    if (unsupportedEcuHost == null
                        || !unsupportedEcuHost.isBlocked(linkManager.getLastTriedPort())) {
                        SwingUtilities.invokeLater(() -> showConnectionFailedDialog(errorMessage));
                    }
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
        refreshBinaryLoggingActions();
    }

    public void setTuneActions(Action loadAction, Action saveAction) {
        loadTuneItem.setAction(loadAction);
        loadTuneItem.setIcon(loadMenuIcon("folder-open"));
        loadTuneItem.setText(LoadTuneHelper.LOAD_TUNE_TEXT);
        loadTuneItem.setMnemonic(KeyEvent.VK_L);
        saveTuneItem.setAction(saveAction);
        saveTuneItem.setIcon(loadMenuIcon("floppy"));
        saveTuneItem.setText(LoadTuneHelper.SAVE_TUNE_TEXT);
        saveTuneItem.setMnemonic(KeyEvent.VK_S);
        loadAction.addPropertyChangeListener(e -> refreshActionsAfterActionStateChange(e.getPropertyName()));
        saveAction.addPropertyChangeListener(e -> refreshActionsAfterActionStateChange(e.getPropertyName()));
        refreshFirmwareUpdateExclusion();
    }

    static Icon loadMenuIcon(String name) {
        ImageIcon icon = AutoupdateUtil.loadIcon("icons/tuning/" + name + "48.png");
        if (icon == null) {
            return null;
        }
        return new ImageIcon(icon.getImage().getScaledInstance(18, 18, Image.SCALE_SMOOTH));
    }

    private void refreshActionsAfterActionStateChange(String propertyName) {
        if ("enabled".equals(propertyName)) {
            if (SwingUtilities.isEventDispatchThread()) {
                refreshFirmwareUpdateExclusion();
            } else {
                SwingUtilities.invokeLater(this::refreshFirmwareUpdateExclusion);
            }
        }
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
        Action saveAction = saveTuneItem.getAction();
        boolean applicationActionsAllowed = !firmwareUpdateInProgress && !unsupportedEcuBlocking;
        loadTuneItem.setEnabled(applicationActionsAllowed && loadAction != null && loadAction.isEnabled());
        saveTuneItem.setEnabled(applicationActionsAllowed && saveAction != null && saveAction.isEnabled());
        updateSoftwareItem.setEnabled(applicationActionsAllowed && updateSoftwareAvailable);
        BinaryProtocol bp = consoleUI.uiContext.getBinaryProtocol();
        boolean firmwareConnectionReady = isFirmwareUpdateConnectionReady(ConnectionStatusLogic.INSTANCE.getValue());
        checkEcuUpdateItem.setEnabled(applicationActionsAllowed
            && !firmwareUpdateCheckInProgress
            && firmwareConnectionReady
            && bp != null
            && bp.signature != null);
        updateEcuItem.setEnabled(applicationActionsAllowed
            && !firmwareUpdateCheckInProgress
            && firmwareConnectionReady
            && updateEcuAvailable);
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
        showConnectionFailureOverlay(errorMessage);
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
        consoleUI.uiContext.sensorLogger.stop();
        BinaryProtocol bp = consoleUI.uiContext.getBinaryProtocol();
        if (bp != null && !bp.isClosed())
            bp.close(); // it could be that serial driver wants to be closed explicitly
        ExitUtil.exit("windowClosedHandler", 0);
    }
}
