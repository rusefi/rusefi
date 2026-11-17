package com.rusefi.maintenance;

import com.rusefi.*;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.io.DoubleCallbacks;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.AsyncJobExecutor;
import com.rusefi.maintenance.jobs.DfuManualJob;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.ui.basic.StatusPanelWithProgressBar;
import com.rusefi.ui.basic.UpdateFirmwareResult;
import com.rusefi.ui.wizard.DfuBoardSelectionPanel;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.*;
import java.io.File;
import java.net.URL;
import java.net.URLClassLoader;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;
import java.awt.Component;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

/**
 * Retains the passing bad-behavior characterization's actual job/picker stimulus. A reserved recovery
 * remains exclusive, but must not report FLASHING/isUpdating or run active hooks before board consent.
 */
public class DfuRecoveryPreparationLifecycleTest {
    @Test
    void singleExecutorReservesWithoutUpdatingWhileActualDfuJobWaitsForBoardConsent(@TempDir Path directory)
        throws Exception {
        runProbe("single", directory);
    }

    @Test
    void directExecutorDefersActiveHookUntilActualDfuJobReceivesBoardConsent(@TempDir Path directory)
        throws Exception {
        runProbe("direct", directory);
    }

    @Test
    void selectorJobRevalidatesLiveInputsAfterBoardConsentWithoutRunningTheBackend(@TempDir Path directory)
        throws Exception {
        runProbe("eligibility", directory);
    }

    @Test
    void directSelectorKeepsBusyAndCreatesNoNativeStatusWindowBeforeBoardConsent(@TempDir Path directory)
        throws Exception {
        runProbe("direct-selector", directory);
    }

    /** Active transport hooks can invalidate eligibility accepted during preflight. */
    @Test
    void dfuJobStopsBeforeProgrammingWhenActiveHookInvalidatesEligibility(@TempDir Path directory)
        throws Exception {
        runProbe("late-eligibility-characterization", directory);
    }

    private static void runProbe(String mode, Path directory) throws Exception {
        // BundleUtil reads release.txt from the process working directory. Isolate the universal
        // bundle fixture instead of overwriting any existing bundle/user file in the checkout.
        Files.write(directory.resolve("release.txt"),
            "platform=universal\nrelease=characterization\n".getBytes(StandardCharsets.US_ASCII));
        Path log = directory.resolve(mode + ".log");
        String binary = System.getProperty("os.name").toLowerCase(Locale.ROOT).contains("win") ? "java.exe" : "java";
        Process child = new ProcessBuilder(Paths.get(System.getProperty("java.home"), "bin", binary).toString(),
            "-Djava.awt.headless=true", "-cp", testClasspath(), Probe.class.getName(), mode)
            .directory(directory.toFile()).redirectErrorStream(true).redirectOutput(log.toFile()).start();
        try {
            assertTrue(child.waitFor(30, TimeUnit.SECONDS), "isolated recovery characterization timed out");
            assertEquals(0, child.exitValue(), new String(Files.readAllBytes(log), StandardCharsets.UTF_8));
        } finally {
            if (child.isAlive()) {
                child.destroyForcibly();
                child.waitFor(5, TimeUnit.SECONDS);
            }
        }
    }

    private static String testClasspath() throws Exception {
        Set<String> entries = new LinkedHashSet<>();
        for (String entry : System.getProperty("java.class.path").split(File.pathSeparator)) {
            entries.add(Paths.get(entry).toAbsolutePath().toString());
        }
        // Gradle's test worker keeps application dependencies on a URL class loader rather than
        // necessarily including them in java.class.path. Only public loader APIs are used (no reflection).
        for (ClassLoader loader = DfuRecoveryPreparationLifecycleTest.class.getClassLoader();
             loader != null; loader = loader.getParent()) {
            if (loader instanceof URLClassLoader) {
                for (URL url : ((URLClassLoader) loader).getURLs()) {
                    if ("file".equals(url.getProtocol())) {
                        entries.add(Paths.get(url.toURI()).toAbsolutePath().toString());
                    }
                }
            }
        }
        return String.join(File.pathSeparator, entries);
    }

    /** Fork entry point: runs real jobs/executors/picker without a JFrame, download or device command. */
    public static final class Probe {
        public static void main(String[] arguments) {
            try {
                if ("eligibility".equals(arguments[0])) {
                    characterizeEligibility();
                } else if ("direct-selector".equals(arguments[0])) {
                    characterizeDirectSelector();
                } else if ("late-eligibility-characterization".equals(arguments[0])) {
                    characterizeLateEligibility();
                } else {
                    characterize(arguments[0]);
                }
                System.exit(0); // Dispose application-lifetime Swing/listener infrastructure in this isolated process.
            } catch (Throwable failure) {
                failure.printStackTrace();
                System.exit(1);
            }
        }

        private static void characterizeLateEligibility() throws Exception {
            java.util.concurrent.atomic.AtomicBoolean eligible = new java.util.concurrent.atomic.AtomicBoolean(true);
            AtomicInteger validations = new AtomicInteger();
            AtomicInteger programmingPathEntered = new AtomicInteger();
            AtomicReference<Throwable> unexpected = new AtomicReference<>();
            CountDownLatch stoppedBeforeBackend = new CountDownLatch(1);
            CountDownLatch finished = new CountDownLatch(1);
            CountDownLatch released = new CountDownLatch(1);
            Thread.setDefaultUncaughtExceptionHandler((thread, failure) -> {
                if (!(failure instanceof StopBeforeFirmwarePreparation)) {
                    unexpected.set(failure);
                }
                stoppedBeforeBackend.countDown();
            });
            UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
            doAnswer(invocation -> {
                String message = invocation.getArgument(0);
                if (message.startsWith("Manual DFU recovery board: ")) {
                    // This is runSelected's FIRST operation, before FirmwarePreparation.prepare.
                    // Abort at this host-only boundary: no download, backend, device probe or flash tool can run.
                    programmingPathEntered.incrementAndGet();
                    throw new StopBeforeFirmwarePreparation();
                }
                return null;
            }).when(callbacks).log(anyString(), anyBoolean(), anyBoolean());
            SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(callbacks);
            executor.addOnJobWorkerAboutToStartListener(() -> eligible.set(false));
            executor.addOnJobInProgressFinishedListener(finished::countDown);
            executor.addOnJobReleasedListener(released::countDown);
            DfuManualJob job = new DfuManualJob(ConnectedEcuTarget.forManualRecovery("uaefi"), null,
                suggested -> "uaefi", () -> {
                    validations.incrementAndGet();
                    return eligible.get();
                });

            assertTrue(executor.startJob(job, null));
            assertTrue(finished.await(5, TimeUnit.SECONDS));
            assertTrue(released.await(5, TimeUnit.SECONDS));
            if (programmingPathEntered.get() != 0) {
                assertTrue(stoppedBeforeBackend.await(5, TimeUnit.SECONDS));
            }

            assertFalse(eligible.get());
            assertNull(unexpected.get());
            assertEquals(0, programmingPathEntered.get(), "invalidated eligibility must stop before programming");
            assertEquals(3, validations.get(), "eligibility is checked before consent, after consent, and after hooks");
            assertEquals(UpdateFirmwareResult.NONE, executor.getLastResult());
            assertTrue(executor.isNotInProgress());
            verify(callbacks, never()).done();
            verify(callbacks, never()).error();
        }

        private static final class StopBeforeFirmwarePreparation extends RuntimeException {
            private static final long serialVersionUID = 1L;
        }

        private static void characterizeDirectSelector() throws Exception {
            com.rusefi.core.net.PropertiesHolder.INSTANCE.getProperties().setProperty("board_compatibility", "uaefi");
            AtomicReference<JComponent> picker = new AtomicReference<>();
            AtomicReference<ProgramSelector> selector = new AtomicReference<>();
            AtomicReference<JButton> primary = new AtomicReference<>();
            CountDownLatch shown = new CountDownLatch(1);
            CountDownLatch released = new CountDownLatch(1);
            AtomicInteger displays = new AtomicInteger();
            SwingUtilities.invokeAndWait(() -> {
                FakePortScanner scanner = new FakePortScanner();
                PortResult dfu = new PortResult("DFU", SerialPortType.Dfu);
                scanner.fireHardwareChange(new AvailableHardware(Collections.singletonList(dfu), true, false, false));
                ProgramSelector control = new ProgramSelector(new ConnectivityContext(scanner),
                    new JComboBox<>(new PortResult[]{dfu}), panel -> {
                        assertInstanceOf(DfuBoardSelectionPanel.class, panel);
                        displays.incrementAndGet();
                        picker.set(panel);
                        shown.countDown();
                    }, () -> {});
                control.setCapabilityProviderForTests(() -> new ProgramSelector.ProgrammingCapabilities(
                    true, false, "universal", "universal", true));
                control.setDfuRecoveryConfirmationForTests(message -> true);
                control.apply(scanner.getCurrentHardware());
                selector.set(control);
                primary.set(findButton(control.getControl(), "Recover ECU"));
                primary.get().addPropertyChangeListener("enabled", event -> {
                    if (Boolean.TRUE.equals(event.getNewValue())) {
                        released.countDown();
                    }
                });
                primary.get().doClick();
            });
            try {
                assertTrue(shown.await(5, TimeUnit.SECONDS), "headless direct flow must reach the picker without a JFrame");
                SwingUtilities.invokeAndWait(() -> {
                    assertFalse(primary.get().isEnabled(), "the direct executor path also retains a busy reservation");
                    selector.get().triggerUpdateFirmware();
                    assertEquals(1, displays.get());
                });
            } finally {
                if (picker.get() != null) {
                    SwingUtilities.invokeAndWait(() -> picker.get()
                        .getActionForKeyStroke(KeyStroke.getKeyStroke("ESCAPE")).actionPerformed(null));
                }
            }
            assertTrue(released.await(5, TimeUnit.SECONDS));
            SwingUtilities.invokeAndWait(() -> assertTrue(primary.get().isEnabled()));
        }

        private static void characterizeEligibility() throws Exception {
            com.rusefi.core.net.PropertiesHolder.INSTANCE.getProperties().setProperty("board_compatibility", "uaefi");
            for (String change : new String[]{"none", "missing", "multiple", "replaced", "capabilities", "target", "selection", "busy"}) {
                FakePortScanner scanner = new FakePortScanner();
                PortResult dfu = new PortResult("DFU", SerialPortType.Dfu);
                PortResult blt = new PortResult("BLT", SerialPortType.OpenBlt);
                scanner.fireHardwareChange(new AvailableHardware(java.util.Arrays.asList(dfu, blt), true, false, false));
                AtomicReference<ProgramSelector.ProgrammingCapabilities> capabilities = new AtomicReference<>(
                    new ProgramSelector.ProgrammingCapabilities(true, false, "universal", "universal", true));
                AtomicReference<DfuManualJob> job = new AtomicReference<>();
                AtomicReference<JComponent> picker = new AtomicReference<>();
                AtomicReference<JComboBox<PortResult>> ports = new AtomicReference<>();
                java.util.concurrent.atomic.AtomicBoolean busy = new java.util.concurrent.atomic.AtomicBoolean();
                CountDownLatch shown = new CountDownLatch(1);
                SwingUtilities.invokeAndWait(() -> {
                    JComboBox<PortResult> combo = new JComboBox<>(new PortResult[]{dfu, blt});
                    ports.set(combo);
                    ProgramSelector selector = new ProgramSelector(new ConnectivityContext(scanner), combo, panel -> {
                        assertInstanceOf(DfuBoardSelectionPanel.class, panel);
                        picker.set(panel);
                        shown.countDown();
                    }, () -> {});
                    selector.setCapabilityProviderForTests(capabilities::get);
                    selector.setExternalBusySupplier(busy::get);
                    selector.setDfuRecoveryConfirmationForTests(message -> true);
                    SingleAsyncJobExecutor capture = mock(SingleAsyncJobExecutor.class);
                    when(capture.isNotInProgress()).thenReturn(true);
                    doAnswer(invocation -> {
                        job.set((DfuManualJob) invocation.getArgument(0));
                        return true;
                    }).when(capture).startJob(any(AsyncJob.class), any(Component.class));
                    selector.setJobExecutor(capture);
                    selector.apply(scanner.getCurrentHardware());
                    findButton(selector.getControl(), "Recover ECU").doClick();
                });
                assertNotNull(job.get());
                ExecutorService worker = Executors.newSingleThreadExecutor();
                RecordingCallbacks callbacks = new RecordingCallbacks();
                try {
                    // Only preflight is run. Accepting this real board picker cannot download or flash here.
                    Future<Boolean> prepared = worker.submit(() -> job.get().prepare(callbacks));
                    assertTrue(shown.await(5, TimeUnit.SECONDS));
                    if (change.equals("missing")) {
                        scanner.fireHardwareChange(new AvailableHardware(Collections.emptyList(), false, false, false));
                    } else if (change.equals("multiple")) {
                        scanner.fireHardwareChange(new AvailableHardware(java.util.Arrays.asList(dfu,
                            new PortResult("DFU_2", SerialPortType.Dfu)), true, false, false));
                    } else if (change.equals("replaced")) {
                        scanner.fireHardwareChange(new AvailableHardware(Collections.singletonList(
                            new PortResult("DFU_2", SerialPortType.Dfu)), true, false, false));
                    } else if (change.equals("capabilities")) {
                        capabilities.set(new ProgramSelector.ProgrammingCapabilities(false, false, "universal", "universal", true));
                    } else if (change.equals("target")) {
                        capabilities.set(new ProgramSelector.ProgrammingCapabilities(true, false, "universal", "other-board", true));
                    } else if (change.equals("selection")) {
                        SwingUtilities.invokeAndWait(() -> ports.get().setSelectedIndex(1));
                    } else if (change.equals("busy")) {
                        busy.set(true);
                    }
                    SwingUtilities.invokeAndWait(() -> {
                        findList(picker.get()).setSelectedIndex(0);
                        findButton(picker.get(), "Download firmware and recover ECU").doClick();
                    });
                    assertEquals(change.equals("none"), prepared.get(5, TimeUnit.SECONDS));
                    assertEquals(0, callbacks.errors.get());
                    assertEquals(0, callbacks.done.get());
                } finally {
                    if (picker.get() != null) {
                        SwingUtilities.invokeAndWait(() -> picker.get()
                            .getActionForKeyStroke(KeyStroke.getKeyStroke("ESCAPE")).actionPerformed(null));
                    }
                    worker.shutdownNow();
                }
            }
            RecordingCallbacks callbacks = new RecordingCallbacks();
            java.util.concurrent.atomic.AtomicBoolean customEligible = new java.util.concurrent.atomic.AtomicBoolean(true);
            DfuManualJob custom = new DfuManualJob(ConnectedEcuTarget.forManualRecovery("uaefi"), "validated-custom.bin",
                suggested -> { throw new AssertionError("custom firmware must not open a board picker"); }, customEligible::get);
            assertTrue(custom.prepare(callbacks)); // The backend is deliberately not run.
            customEligible.set(false);
            assertFalse(custom.prepare(callbacks));
            assertEquals(0, callbacks.errors.get());
            assertEquals(0, callbacks.done.get());
        }

        private static JButton findButton(java.awt.Container parent, String label) {
            for (Component child : parent.getComponents()) {
                if (child instanceof JButton && label.equals(((JButton) child).getText())) {
                    return (JButton) child;
                }
                if (child instanceof java.awt.Container) {
                    JButton found = findButton((java.awt.Container) child, label);
                    if (found != null) {
                        return found;
                    }
                }
            }
            return null;
        }

        private static JList<?> findList(java.awt.Container parent) {
            for (Component child : parent.getComponents()) {
                if (child instanceof JList) {
                    return (JList<?>) child;
                }
                if (child instanceof java.awt.Container) {
                    JList<?> found = findList((java.awt.Container) child);
                    if (found != null) {
                        return found;
                    }
                }
            }
            return null;
        }

        private static void characterize(String mode) throws Exception {
            assertEquals("universal", com.rusefi.core.io.BundleUtil.getBundleTarget());
            ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.NOT_CONNECTED);
            RecordingCallbacks callbacks = new RecordingCallbacks();
            CountDownLatch pickerShown = new CountDownLatch(1);
            CountDownLatch finished = new CountDownLatch(1);
            CountDownLatch released = new CountDownLatch(1);
            AtomicReference<JComponent> picker = new AtomicReference<>();
            AtomicInteger pickerClosed = new AtomicInteger();
            AtomicInteger workerStarted = new AtomicInteger();
            DfuManualJob job = new DfuManualJob(ConnectedEcuTarget.forManualRecovery("uaefi"), null,
                suggested -> DfuBoardPicker.pick(suggested, component -> {
                    assertInstanceOf(DfuBoardSelectionPanel.class, component);
                    picker.set(component);
                    pickerShown.countDown();
                }, pickerClosed::incrementAndGet, Collections.singletonList("uaefi")));
            AtomicReference<SingleAsyncJobExecutor> single = new AtomicReference<>();
            AtomicReference<DeviceSessionManager> session = new AtomicReference<>();
            AtomicReference<JTabbedPane> tabs = new AtomicReference<>();
            try {
                if ("single".equals(mode)) {
                    SwingUtilities.invokeAndWait(() -> {
                        FakePortScanner scanner = new FakePortScanner();
                        scanner.fireHardwareChange(new AvailableHardware(Collections.singletonList(
                            new PortResult("DFU", SerialPortType.Dfu)), true, false, false));
                        ConnectivityContext context = new ConnectivityContext(scanner);
                        DeviceSessionManager manager = new DeviceSessionManager(context, null, () -> {}, () -> {});
                        StatusPanelWithProgressBar status = new StatusPanelWithProgressBar();
                        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(new DoubleCallbacks(status, callbacks));
                        UIContext ui = mock(UIContext.class);
                        when(ui.getLinkManager()).thenReturn(mock(LinkManager.class));
                        JTabbedPane tabbedPane = new JTabbedPane();
                        DevicePane device = new DevicePane(ui, context, manager, tabbedPane, executor, status,
                            ignored -> {}, () -> {});
                        tabbedPane.addTab("Device", device.getContent());
                        single.set(executor);
                        session.set(manager);
                        tabs.set(tabbedPane);
                        executor.addOnJobWorkerAboutToStartListener(workerStarted::incrementAndGet);
                        executor.addOnJobInProgressFinishedListener(finished::countDown);
                        executor.addOnJobReleasedListener(released::countDown);
                        assertEquals(SessionState.DEVICE_IN_DFU, manager.getState());
                        assertTrue(executor.startJob(job, null));
                    });
                } else {
                    assertEquals("direct", mode);
                    AsyncJobExecutor.INSTANCE.executeJob(job, callbacks, workerStarted::incrementAndGet, finished::countDown);
                }

                assertTrue(pickerShown.await(5, TimeUnit.SECONDS), "the actual DfuManualJob never reached its board picker");
                SwingUtilities.invokeAndWait(() -> {}); // Drain DevicePane's production session-state render.
                assertEquals(0, workerStarted.get());
                assertEquals(1L, finished.getCount(), "the worker must remain blocked until board consent/cancellation");
                assertEquals(0, callbacks.done.get());
                assertEquals(0, callbacks.errors.get());
                assertEquals(0, callbacks.warnings.get());
                if (single.get() != null) {
                    SwingUtilities.invokeAndWait(() -> {
                        SingleAsyncJobExecutor executor = single.get();
                        assertFalse(executor.isNotInProgress(), "preparation must still reserve the exclusive job slot");
                        assertSame(job, executor.getJobInProgress().orElse(null));
                        assertEquals(SessionState.DEVICE_IN_DFU, session.get().getState());
                        assertEquals(Boolean.FALSE, tabs.get().getClientProperty("isUpdating"),
                            "this exact property drives both the orange Updating overlay and UPDATING title");
                        assertEquals(UpdateFirmwareResult.NONE, executor.getLastResult());
                        List<String> overlapErrors = new ArrayList<>();
                        assertFalse(executor.startJob(new AsyncJob("overlap must not run") {
                            @Override
                            public void doJob(UpdateOperationCallbacks ignored, Runnable complete) {
                                fail("another job started during board selection");
                            }
                        }, null, overlapErrors::add));
                        assertEquals(1, overlapErrors.size());
                    });
                }
            } finally {
                // Never click Download firmware and recover ECU: cancelling the real picker returns null before
                // ManualDfuRecovery can download firmware, inspect DFU hardware or execute a flash tool.
                if (picker.get() != null) {
                    SwingUtilities.invokeAndWait(() -> picker.get()
                        .getActionForKeyStroke(KeyStroke.getKeyStroke("ESCAPE")).actionPerformed(null));
                }
            }
            assertTrue(finished.await(5, TimeUnit.SECONDS));
            if (single.get() != null) {
                assertTrue(released.await(5, TimeUnit.SECONDS));
            }
            SwingUtilities.invokeAndWait(() -> {});
            assertEquals(1, pickerClosed.get());
            assertEquals(0, callbacks.done.get(), "cancellation must not manufacture a successful update");
            assertEquals(0, callbacks.errors.get(), "cancellation must remain neutral, not a failure");
            assertEquals(0, callbacks.warnings.get());
            if (single.get() != null) {
                assertTrue(single.get().isNotInProgress());
                assertEquals(UpdateFirmwareResult.NONE, single.get().getLastResult());
                SwingUtilities.invokeAndWait(() -> {
                    assertEquals(SessionState.DEVICE_IN_DFU, session.get().getState());
                    assertEquals(Boolean.FALSE, tabs.get().getClientProperty("isUpdating"));
                });
            }
        }
    }

    private static final class RecordingCallbacks implements UpdateOperationCallbacks {
        final AtomicInteger done = new AtomicInteger();
        final AtomicInteger errors = new AtomicInteger();
        final AtomicInteger warnings = new AtomicInteger();

        @Override
        public void log(String message, boolean breakLineOnTextArea, boolean sendToLogger) {}

        @Override
        public void done() {
            done.incrementAndGet();
        }

        @Override
        public void error() {
            errors.incrementAndGet();
        }

        @Override
        public void warning() {
            warnings.incrementAndGet();
        }

        @Override
        public void clear() {}
    }
}
