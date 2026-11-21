package com.rusefi.maintenance;

import com.rusefi.AvailableHardware;
import com.devexperts.logging.Logging;
import com.opensr5.ini.IniLocator;
import com.rusefi.binaryprotocol.RealIniFileProvider;
import com.rusefi.core.SignatureHelper;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.io.IOException;
import java.util.Objects;
import java.util.function.Consumer;
import java.util.function.Predicate;
import java.util.function.Supplier;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Swing .ini picker registered into the (Swing-free) io module via
 * {@link RealIniFileProvider#manualPicker}. Fires when a connect cannot download or locate the
  * matching .ini (internet down / custom board, see issue #9774); a selected INI or 7z archive is cached so a
 * later port-scanner probe can connect without blocking on user input.
 */
public class ManualIniFilePicker {
    private static final Logging log = getLogging(ManualIniFilePicker.class);
    private static final String STARTUP_HOST_PROPERTY = ManualIniFilePicker.class.getName() + ".startupHost";
    private static ActiveRecovery activeRecovery;

    interface IniImporter {
        Object importIni(String signature, java.io.File file) throws IOException;
    }

    static final class StartupHost {
        final JTabbedPane tabs;
        final Component connectContent;
        final Runnable revealControls;
        final Consumer<String> progressText;
        final Consumer<IniLocator.LookupProgress> lookupProgress;
        final Predicate<String> ownsPort;
        final Supplier<AvailableHardware> latestHardware;
        JPanel activePicker;
        int connectIndex = -1;
        int previousIndex = -1;

        StartupHost(JTabbedPane tabs, Component connectContent, Runnable revealControls,
                    Consumer<String> progressText, Predicate<String> ownsPort,
                    Supplier<AvailableHardware> latestHardware) {
            this(tabs, connectContent, revealControls, progressText, null, ownsPort, latestHardware);
        }

        StartupHost(JTabbedPane tabs, Component connectContent, Runnable revealControls,
                    Consumer<String> progressText, Consumer<IniLocator.LookupProgress> lookupProgress,
                    Predicate<String> ownsPort, Supplier<AvailableHardware> latestHardware) {
            this.tabs = tabs;
            this.connectContent = connectContent;
            this.revealControls = revealControls;
            this.progressText = progressText;
            this.lookupProgress = lookupProgress;
            this.ownsPort = ownsPort;
            this.latestHardware = latestHardware;
        }
    }

    private static final class ActiveRecovery {
        final String signature;
        final String port;
        final AvailableHardware hardwareSnapshotAtStart;
        long generation;
        long progressSequence = Long.MIN_VALUE;
        IniLocator.LookupStage progressStage;
        boolean progressComplete;
        PickerSession picker;

        ActiveRecovery(String signature, String port, long generation, AvailableHardware hardwareSnapshotAtStart) {
            this.signature = signature;
            this.port = port;
            this.generation = generation;
            this.hardwareSnapshotAtStart = hardwareSnapshotAtStart;
        }
    }

    private static final class PickerSession {
        final Runnable restore;
        boolean active = true;
        SwingWorker<Object, Void> importWorker;

        PickerSession(Runnable restore) {
            this.restore = restore;
        }

        void cancelImport() {
            active = false;
            if (importWorker != null) {
                importWorker.cancel(true);
            }
        }
    }

    private static long latestGeneration = Long.MIN_VALUE;
    private static long closedThroughGeneration = Long.MIN_VALUE;

    public static void registerStartupHost(JRootPane rootPane, JTabbedPane tabs,
                                           Component connectContent, Runnable revealControls) {
        registerStartupHost(rootPane, tabs, connectContent, revealControls, ignored -> {});
    }

    public static void registerStartupHost(JRootPane rootPane, JTabbedPane tabs,
                                           Component connectContent, Runnable revealControls,
                                           Consumer<String> progressText) {
        registerStartupHost(rootPane, tabs, connectContent, revealControls, progressText, ignored -> true);
    }

    public static void registerStartupHost(JRootPane rootPane, JTabbedPane tabs,
                                           Component connectContent, Runnable revealControls,
                                           Consumer<String> progressText, Predicate<String> ownsPort) {
        registerStartupHost(rootPane, tabs, connectContent, revealControls, progressText, ownsPort, null);
    }

    public static void registerStartupHost(JRootPane rootPane, JTabbedPane tabs,
                                           Component connectContent, Runnable revealControls,
                                           Consumer<String> progressText, Predicate<String> ownsPort,
                                           Supplier<AvailableHardware> latestHardware) {
        rootPane.putClientProperty(STARTUP_HOST_PROPERTY,
            new StartupHost(tabs, connectContent, revealControls, progressText, ownsPort, latestHardware));
    }

    public static void registerStartupHostWithLookupProgress(
            JRootPane rootPane, JTabbedPane tabs, Component connectContent, Runnable revealControls,
            Consumer<IniLocator.LookupProgress> lookupProgress, Predicate<String> ownsPort,
            Supplier<AvailableHardware> latestHardware) {
        rootPane.putClientProperty(STARTUP_HOST_PROPERTY,
            new StartupHost(tabs, connectContent, revealControls, ignored -> {}, lookupProgress,
                ownsPort, latestHardware));
    }

    public static void register() {
        RealIniFileProvider.manualPicker = new RealIniFileProvider.ManualIniPicker() {
            @Override
            public void pick(String signature) {
                pick(signature, null, 0);
            }

            @Override
            public void progress(String signature, String port, long generation,
                                 IniLocator.LookupProgress progress) {
                showSearchProgress(signature, port, generation, progress);
            }

            @Override
            public void resolved(String signature, String port, long generation) {
                resolveSearch(signature, port, generation);
            }

            @Override
            public void pick(String signature, String port, long generation) {
                ManualIniFilePicker.pick(signature, port, generation);
            }
        };
    }

    private static void showSearchProgress(String signature, String port, long generation,
                                          IniLocator.LookupProgress progress) {
        SwingUtilities.invokeLater(() -> applySearchProgress(
            findStartupHost(), signature, port, generation, progress));
    }

    static void applySearchFinishedForTests(StartupHost host, String signature, String port, long generation) {
        applySearchFinished(host, signature, port, generation);
    }

    private static void applySearchFinished(StartupHost host, String signature, String port, long generation) {
        ActiveRecovery active = activeRecovery;
        if (host != null && ownsPort(host, port) && matches(active, signature, port)
                && generation >= active.generation && acceptGeneration(generation)) {
            host.progressText.accept("");
        }
    }

    static void applySearchProgressForTests(StartupHost host, String signature, String port, long generation) {
        applySearchProgress(host, signature, port, generation);
    }

    private static void applySearchProgress(StartupHost host, String signature, String port, long generation) {
        if (ownsPort(host, port) && acceptRecovery(signature, port, generation, host)) {
            host.progressText.accept("Searching Windows drives for the matching INI…");
        }
    }

    static void applyLookupProgressForTests(StartupHost host, String signature, String port, long generation,
                                            IniLocator.LookupProgress progress) {
        applySearchProgress(host, signature, port, generation, progress);
    }

    private static void applySearchProgress(StartupHost host, String signature, String port, long generation,
                                            IniLocator.LookupProgress progress) {
        if (!ownsPort(host, port) || !acceptRecovery(signature, port, generation, host)) {
            return;
        }
        ActiveRecovery active = activeRecovery;
        if (active == null || active.progressComplete || active.progressSequence >= progress.sequence
                || active.progressStage != null && progress.stage.ordinal() < active.progressStage.ordinal()) {
            return;
        }
        active.progressSequence = progress.sequence;
        active.progressStage = progress.stage;
        active.progressComplete = progress.stage == IniLocator.LookupStage.COMPLETE;
        if (host.lookupProgress != null) {
            host.lookupProgress.accept(progress);
        } else {
            host.progressText.accept(progress.stage == IniLocator.LookupStage.COMPLETE
                ? "" : progressText(progress));
        }
    }

    private static String progressText(IniLocator.LookupProgress progress) {
        switch (progress.stage) {
            case BUNDLED_INI:
                return "Checking bundled INI…";
            case CURRENT_DIRECTORY:
                return "Checking current directory…";
            case DRIVE_ROOTS:
                if (progress.totalRoots <= 0) {
                    return "Searching drive roots…";
                }
                return "Searching drive roots… — " + progress.completedRoots + " of "
                    + progress.totalRoots + " completed (" + progress.activeWorkers + " active)";
            case CONSOLE_CACHE_DOWNLOAD:
                return "Checking console cache / downloading INI…";
            case TUNERSTUDIO_CACHE:
                return "Checking TunerStudio cache…";
            case COMPLETE:
                return "";
            default:
                throw new IllegalStateException("Unhandled INI lookup stage: " + progress.stage);
        }
    }

    private static void resolveSearch(String signature, String port, long generation) {
        SwingUtilities.invokeLater(() -> {
            applySearchResolved(findStartupHost(), signature, port, generation);
        });
    }

    static void applySearchResolvedForTests(StartupHost host, String signature, String port, long generation) {
        applySearchResolved(host, signature, port, generation);
    }

    private static void applySearchResolved(StartupHost host, String signature, String port, long generation) {
        ActiveRecovery active = activeRecovery;
        if (!ownsPort(host, port) || !matches(active, signature, port)
                || generation < active.generation || !acceptGeneration(generation)) {
            return;
        }
        closeGeneration(generation);
        closeRecovery(active, host, false);
    }

    private static void pick(String signature, String port, long generation) {
        SwingUtilities.invokeLater(() -> {
            StartupHost host = findStartupHost();
            if (!ownsPort(host, port)) {
                RealIniFileProvider.allowManualPrompt(signature, port);
                return;
            }
            if (!acceptRecovery(signature, port, generation, host)) {
                return;
            }
            JFrame frame = findVisibleFrame();
            if (frame == null) {
                RealIniFileProvider.allowManualPrompt(signature, port);
                closeGeneration(generation);
                activeRecovery = null;
                return;
            }
            if (activeRecovery != null && activeRecovery.picker != null) {
                return;
            }
            showPicker(frame.getRootPane(), frame, signature, SignatureHelper::importIntoCache,
                port, generation);
        });
    }

    private static boolean acceptRecovery(String signature, String port, long generation, StartupHost host) {
        if (!acceptGeneration(generation)) {
            return false;
        }
        ActiveRecovery active = activeRecovery;
        if (active != null && generation < active.generation) {
            return false;
        }
        if (!matches(active, signature, port)) {
            if (active != null) {
                closeGeneration(active.generation);
            }
            dismissActivePicker();
            AvailableHardware hardware = host == null || host.latestHardware == null
                ? null : host.latestHardware.get();
            activeRecovery = new ActiveRecovery(signature, port, generation, hardware);
        } else if (generation > active.generation) {
            active.generation = generation;
            active.progressSequence = Long.MIN_VALUE;
            active.progressStage = null;
            active.progressComplete = false;
        }
        return true;
    }

    private static boolean acceptGeneration(long generation) {
        if (generation < latestGeneration || generation <= closedThroughGeneration) {
            return false;
        }
        latestGeneration = Math.max(latestGeneration, generation);
        return true;
    }

    private static boolean ownsPort(StartupHost host, String port) {
        return host != null && host.ownsPort.test(port);
    }

    static String activeRecoveryPort() {
        ActiveRecovery active = activeRecovery;
        return active == null ? null : active.port;
    }

    public static boolean ownsRecoveryPort(String port, String autoConnectedPort, String selectedPort,
                                    boolean initialScanIntent) {
        if (port == null) {
            return false;
        }
        String activePort = activeRecoveryPort();
        if (autoConnectedPort != null) {
            return autoConnectedPort.equals(port);
        }
        if (selectedPort != null && selectedPort.equals(port)) {
            return true;
        }
        if (activePort != null) {
            return activePort.equals(port);
        }
        return initialScanIntent;
    }

    private static void closeGeneration(long generation) {
        closedThroughGeneration = Math.max(closedThroughGeneration, generation);
    }

    private static void closeRecovery(ActiveRecovery active, StartupHost host, boolean allowRetry) {
        if (active == null) {
            return;
        }
        if (allowRetry) {
            RealIniFileProvider.allowManualPrompt(active.signature, active.port);
        }
        closeGeneration(active.generation);
        if (active.picker != null) {
            active.picker.cancelImport();
            active.picker.restore.run();
        }
        if (host != null) {
            host.progressText.accept("");
            if (host.lookupProgress != null) {
                long sequence = active.progressSequence == Long.MIN_VALUE
                    ? 0 : active.progressSequence + 1;
                host.lookupProgress.accept(new IniLocator.LookupProgress(
                    IniLocator.LookupStage.COMPLETE, 0, 0, 0, sequence));
            }
        }
        if (activeRecovery == active) {
            activeRecovery = null;
        }
    }

    public static void targetLost(String port) {
        targetLost(port, false);
    }

    public static void targetLost(String port, boolean expectedMissingIniFailure) {
        if (expectedMissingIniFailure) {
            return;
        }
        ActiveRecovery lostRecovery = activeRecovery;
        if (lostRecovery == null || !Objects.equals(lostRecovery.port, port)) {
            return;
        }
        long lostGeneration = lostRecovery.generation;
        SwingUtilities.invokeLater(() -> applyTargetLost(
            findStartupHost(), port, lostGeneration));
    }

    static void targetLostForTests(StartupHost host, String port, boolean expectedMissingIniFailure) {
        ActiveRecovery recovery = activeRecovery;
        if (!expectedMissingIniFailure && recovery != null && Objects.equals(recovery.port, port)) {
            applyTargetLost(host, port, recovery.generation);
        }
    }

    static void targetLostGenerationForTests(StartupHost host, String port, long generation) {
        applyTargetLost(host, port, generation);
    }

    private static void applyTargetLost(StartupHost host, String port, long generation) {
        ActiveRecovery active = activeRecovery;
        if (active == null || active.generation != generation || !Objects.equals(active.port, port)) {
            return;
        }
        closeRecovery(active, host, true);
    }

    public static void targetReplaced(String newPort) {
        ActiveRecovery replaced = activeRecovery;
        if (replaced == null || Objects.equals(replaced.port, newPort)) {
            return;
        }
        Runnable close = () -> {
            if (activeRecovery == replaced) {
                closeRecovery(replaced, findStartupHost(), true);
            }
        };
        if (SwingUtilities.isEventDispatchThread()) {
            close.run();
        } else {
            SwingUtilities.invokeLater(close);
        }
    }

    public static void hardwareUpdated(AvailableHardware hardware) {
        if (SwingUtilities.isEventDispatchThread()) {
            applyHardwareUpdated(findStartupHost(), hardware);
        } else {
            SwingUtilities.invokeLater(() -> applyHardwareUpdated(findStartupHost(), hardware));
        }
    }

    static void hardwareUpdatedForTests(StartupHost host, AvailableHardware hardware) {
        applyHardwareUpdated(host, hardware);
    }

    private static void applyHardwareUpdated(StartupHost host, AvailableHardware hardware) {
        ActiveRecovery active = activeRecovery;
        if (active == null) {
            return;
        }
        if (host != null && host.latestHardware != null
                && (hardware != host.latestHardware.get() || hardware == active.hardwareSnapshotAtStart)) {
            return;
        }
        if (hardware.getKnownPorts().stream()
                .noneMatch(port -> Objects.equals(port.port, active.port))) {
            closeRecovery(active, host, true);
        }
    }

    static void resetRecoveryForTests() {
        dismissActivePicker();
        activeRecovery = null;
        latestGeneration = Long.MIN_VALUE;
        closedThroughGeneration = Long.MIN_VALUE;
    }

    private static boolean matches(ActiveRecovery active, String signature, String port) {
        return active != null && Objects.equals(active.signature, signature) && Objects.equals(active.port, port);
    }

    private static void dismissActivePicker() {
        ActiveRecovery active = activeRecovery;
        if (active != null && active.picker != null) {
            active.picker.cancelImport();
            active.picker.restore.run();
            active.picker = null;
        }
    }

    private static StartupHost findStartupHost() {
        JFrame frame = findVisibleFrame();
        return frame == null ? null
            : (StartupHost) frame.getRootPane().getClientProperty(STARTUP_HOST_PROPERTY);
    }

    private static void showPicker(String signature) {
        JFrame frame = findVisibleFrame();
        if (frame == null) {
            RealIniFileProvider.allowManualPrompt(signature);
            return;
        }

        showPicker(frame.getRootPane(), frame, signature);
    }

    static void showPicker(JRootPane rootPane, Component parent, String signature) {
        showPicker(rootPane, parent, signature, SignatureHelper::importIntoCache);
    }

    static void showPicker(JRootPane rootPane, Component parent, String signature, IniImporter importer) {
        ActiveRecovery recovery = activeRecovery;
        if (recovery != null && Objects.equals(recovery.signature, signature)) {
            showPicker(rootPane, parent, signature, importer, recovery.port, recovery.generation);
        } else {
            showPicker(rootPane, parent, signature, importer, null, 0);
        }
    }

    private static void showPicker(JRootPane rootPane, Component parent, String signature, IniImporter importer,
                                  String port, long generation) {
        JFileChooser chooser = new JFileChooser();
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        chooser.setAcceptAllFileFilterUsed(false);
        chooser.setFileFilter(new FileNameExtensionFilter("INI and 7z files", "ini", "7z"));

        JLabel title = new JLabel("INI File Not Found");
        title.setFont(title.getFont().deriveFont(Font.BOLD, 24f));
        title.setAlignmentX(Component.CENTER_ALIGNMENT);
        JLabel message = new JLabel("Could not automatically find an INI file for:");
        message.setAlignmentX(Component.CENTER_ALIGNMENT);
        JLabel signatureLabel = new JLabel(signature);
        signatureLabel.setFont(signatureLabel.getFont().deriveFont(Font.BOLD));
        signatureLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        JLabel instruction = new JLabel("Select the matching INI file or a 7z archive containing it.");
        instruction.setAlignmentX(Component.CENTER_ALIGNMENT);

        JPanel content = new JPanel();
        content.setLayout(new BoxLayout(content, BoxLayout.Y_AXIS));
        content.add(title);
        content.add(Box.createVerticalStrut(12));
        content.add(message);
        content.add(Box.createVerticalStrut(4));
        content.add(signatureLabel);
        content.add(Box.createVerticalStrut(12));
        content.add(instruction);
        content.add(Box.createVerticalStrut(20));
        content.add(chooser);

        StartupHost host = (StartupHost) rootPane.getClientProperty(STARTUP_HOST_PROPERTY);
        Component previousGlassPane = rootPane.getGlassPane();
        boolean previousVisible = previousGlassPane.isVisible();
        Runnable[] startupRestore = {null};
        if (host != null) {
            startupRestore[0] = showInStartup(host, content);
        }
        JPanel overlay = null;
        if (startupRestore[0] == null) {
            overlay = new JPanel(new GridBagLayout());
            overlay.add(content);
        }
        JPanel pickerOverlay = overlay;
        PickerSession session = new PickerSession(() -> {
            if (startupRestore[0] != null) {
                startupRestore[0].run();
            } else {
                // Hide the old pane even when a newer glass pane has replaced it.
                pickerOverlay.setVisible(false);
                if (rootPane.getGlassPane() == pickerOverlay) {
                    rootPane.setGlassPane(previousGlassPane);
                    previousGlassPane.setVisible(previousVisible);
                }
            }
        });
        ActiveRecovery recovery = activeRecovery;
        if (recovery != null && Objects.equals(recovery.signature, signature)
                && Objects.equals(recovery.port, port) && recovery.generation <= generation) {
            if (recovery.picker != null) {
                recovery.picker.cancelImport();
            }
            recovery.picker = session;
            if (host != null) {
                host.progressText.accept("");
            }
        }

        chooser.addActionListener(e -> {
            if (JFileChooser.APPROVE_SELECTION.equals(e.getActionCommand())) {
                if (!session.active || session.importWorker != null) {
                    return;
                }
                java.io.File selectedFile = chooser.getSelectedFile();
                SwingWorker<Object, Void> worker = new SwingWorker<Object, Void>() {
                    @Override
                    protected Object doInBackground() throws Exception {
                        return importer.importIni(signature, selectedFile);
                    }

                    @Override
                    protected void done() {
                        if (!session.active || recovery != null
                                && (activeRecovery != recovery || recovery.picker != session)) {
                            return;
                        }
                        session.importWorker = null;
                        try {
                            if (get() == null) {
                                throw new IllegalArgumentException("Invalid ECU signature: " + signature);
                            }
                        } catch (java.util.concurrent.CancellationException ignored) {
                            return;
                        } catch (InterruptedException ex) {
                            Thread.currentThread().interrupt();
                            return;
                        } catch (java.util.concurrent.ExecutionException ex) {
                            Throwable cause = ex.getCause();
                            showImportError(parent, cause);
                            return;
                        } catch (IllegalArgumentException ex) {
                            showImportError(parent, ex);
                            return;
                        }
                        session.active = false;
                        session.restore.run();
                        closePickerRecovery(recovery, session);
                    }
                };
                session.importWorker = worker;
                worker.execute();
            } else if (!JFileChooser.CANCEL_SELECTION.equals(e.getActionCommand())) {
                return;
            } else {
                session.cancelImport();
                session.restore.run();
                closePickerRecovery(recovery, session);
                return;
            }
        });

        if (startupRestore[0] != null) {
            chooser.requestFocusInWindow();
            return;
        }
        rootPane.setGlassPane(pickerOverlay);
        pickerOverlay.setVisible(true);
        chooser.requestFocusInWindow();
    }

    private static void showImportError(Component parent, Throwable error) {
        String message = error == null || error.getMessage() == null
            ? "Unable to import the selected INI file." : error.getMessage();
        log.error("Failed to import picked INI/7z archive into cache", error);
        JOptionPane.showMessageDialog(parent, message, "Failed to Import INI", JOptionPane.ERROR_MESSAGE);
    }

    private static void closePickerRecovery(ActiveRecovery recovery, PickerSession session) {
        if (recovery != null && recovery.picker == session) {
            closeGeneration(recovery.generation);
            recovery.picker = null;
            if (activeRecovery == recovery) {
                activeRecovery = null;
            }
        }
    }

    private static Runnable showInStartup(StartupHost host, JPanel content) {
        int connectIndex = host.activePicker == null
            ? host.tabs.indexOfComponent(host.connectContent)
            : host.tabs.indexOfComponent(host.activePicker);
        if (connectIndex < 0) {
            return null;
        }
        if (host.activePicker == null) {
            host.connectIndex = connectIndex;
            host.previousIndex = host.tabs.getSelectedIndex();
            host.revealControls.run();
        }
        JButton updateFirmware = new JButton("Go to Update ECU firmware");
        updateFirmware.addActionListener(e -> host.tabs.setSelectedIndex(0));
        content.add(Box.createVerticalStrut(12));
        content.add(updateFirmware);

        JPanel picker = new JPanel(new GridBagLayout());
        picker.add(content);
        host.tabs.setSelectedIndex(connectIndex);
        host.tabs.setComponentAt(connectIndex, picker);
        host.activePicker = picker;

        return () -> {
            if (host.activePicker != picker) {
                return;
            }
            host.tabs.setComponentAt(host.connectIndex, host.connectContent);
            host.activePicker = null;
            if (host.tabs.getSelectedIndex() == host.connectIndex && host.previousIndex >= 0) {
                host.tabs.setSelectedIndex(host.previousIndex);
            }
        };
    }

    private static JFrame findVisibleFrame() {
        Window active = KeyboardFocusManager.getCurrentKeyboardFocusManager().getActiveWindow();
        if (active instanceof JFrame && active.isVisible()) {
            return (JFrame) active;
        }
        for (Frame frame : JFrame.getFrames()) {
            if (frame instanceof JFrame && frame.isVisible()) {
                return (JFrame) frame;
            }
        }
        return null;
    }
}
