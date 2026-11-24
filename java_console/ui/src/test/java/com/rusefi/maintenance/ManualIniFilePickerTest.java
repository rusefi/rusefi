package com.rusefi.maintenance;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.AfterEach;

import com.rusefi.AvailableHardware;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.opensr5.ini.IniLocator;
import com.rusefi.binaryprotocol.IniNotFoundException;
import com.rusefi.binaryprotocol.RealIniFileProvider;
import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReference;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class ManualIniFilePickerTest {
    @Test
    void queuedLossCannotCloseNewerSameTargetPicker() throws Exception {
        AtomicReference<JTabbedPane> tabsRef = new AtomicReference<>();
        AtomicReference<JPanel> connectRef = new AtomicReference<>();
        AtomicReference<Component> pickerRef = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            JRootPane rootPane = new JRootPane();
            JPanel connect = new JPanel();
            JTabbedPane tabs = new JTabbedPane();
            tabs.addTab("Firmware", new JPanel());
            tabs.addTab("Connect", connect);
            rootPane.setContentPane(tabs);
            ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {}, ignored -> {});
            ManualIniFilePicker.StartupHost host = (ManualIniFilePicker.StartupHost)
                rootPane.getClientProperty(ManualIniFilePicker.class.getName() + ".startupHost");
            ManualIniFilePicker.applySearchProgressForTests(host, "same-signature", "COM9", 1);
            ManualIniFilePicker.showPicker(rootPane, rootPane, "same-signature");
            tabsRef.set(tabs);
            connectRef.set(connect);
            pickerRef.set(tabs.getComponentAt(1));
            ManualIniFilePicker.targetLost("COM9");
            ManualIniFilePicker.applySearchProgressForTests(host, "same-signature", "COM9", 2);
        });
        SwingUtilities.invokeAndWait(() -> {
            assertNotSame(connectRef.get(), pickerRef.get());
            assertSame(pickerRef.get(), tabsRef.get().getComponentAt(1),
                "queued loss must retain its original generation when a same-target retry advances recovery");
        });
    }

    @AfterEach
    void resetRecovery() {
        ManualIniFilePicker.resetRecoveryForTests();
        RealIniFileProvider.manualPicker = null;
        RealIniFileProvider.clearMissingIniStateForTests();
    }

    @Test
    void pickerAcceptsOnlyIniAndCompressedArchives() {
        JRootPane rootPane = new JRootPane();
        ManualIniFilePicker.showPicker(rootPane, rootPane, "test-signature");
        JFileChooser chooser = findChooser(rootPane.getGlassPane());

        assertNotNull(chooser);
        assertTrue(chooser.getFileFilter().accept(new File("board.ini")));
        assertTrue(chooser.getFileFilter().accept(new File("board.7Z")));
        assertFalse(chooser.getFileFilter().accept(new File("firmware.bin")));
    }

    @Test
    void startupPickerLayoutOnAWideWindow() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JRootPane rootPane = new JRootPane();
            JPanel connect = new JPanel();
            JTabbedPane tabs = new JTabbedPane();
            tabs.addTab("Update ECU firmware", new JPanel());
            tabs.addTab("Connect", connect);
            rootPane.setContentPane(tabs);
            ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {});

            ManualIniFilePicker.showPicker(rootPane, rootPane,
                "rusEFI noini-10540-20261119.2026.11.19.uaefi_pro.1254041395");
            tabs.setSize(2000, 1100);
            layoutRecursively(tabs);

            JFileChooser chooser = findChooser(tabs.getComponentAt(1));
            assertNotNull(chooser);
            assertTrue(chooser.getWidth() < 1000, "retain the compact chooser width");
            assertTrue(chooser.getHeight() < 700, "retain the compact chooser height");
            Container content = chooser.getParent();
            assertTrue(content.getX() > 0);
            assertTrue(content.getY() > 0);
            assertEquals(content.getParent().getWidth() / 2.0,
                content.getX() + content.getWidth() / 2.0, 1.0);
            assertEquals(content.getParent().getHeight() / 2.0,
                content.getY() + content.getHeight() / 2.0, 1.0);
            assertFalse(rootPane.getGlassPane().isVisible());
            assertNotNull(findButton(tabs.getComponentAt(1), "Go to Update ECU firmware"));
        });
    }

    @Test
    void nonStartupPickerRetainsGlassPaneFallbackAndCancelRestoresIt() {
        JRootPane rootPane = new JRootPane();
        JPanel previousGlassPane = new JPanel();
        rootPane.setGlassPane(previousGlassPane);
        previousGlassPane.setVisible(true);

        ManualIniFilePicker.showPicker(rootPane, rootPane, "test-signature");

        assertNotSame(previousGlassPane, rootPane.getGlassPane());
        assertTrue(rootPane.getGlassPane().isVisible());
        clickCancel(rootPane.getGlassPane());
        assertSame(previousGlassPane, rootPane.getGlassPane());
        assertTrue(previousGlassPane.isVisible());
    }

    @Test
    void startupPickerLeavesTabsAccessibleAndCancelRestoresConnectContent() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Update ECU firmware", new JPanel());
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        boolean[] revealed = {false};
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> revealed[0] = true);

        ManualIniFilePicker.showPicker(rootPane, rootPane, "test-signature");

        assertFalse(rootPane.getGlassPane().isVisible());
        assertTrue(revealed[0]);
        assertEquals(1, tabs.getSelectedIndex());
        Component picker = tabs.getComponentAt(1);
        assertNotSame(connect, picker);
        assertNotNull(findButton(picker, "Go to Update ECU firmware"));

        clickCancel(picker);
        assertSame(connect, tabs.getComponentAt(1));
        assertEquals(0, tabs.getSelectedIndex());
    }

    @Test
    void firmwareNavigationSelectsTheExistingTabWithoutStartingAnUpdate() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JPanel firmware = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Update ECU firmware", firmware);
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {});

        ManualIniFilePicker.showPicker(rootPane, rootPane, "test-signature");
        findButton(tabs.getComponentAt(1), "Go to Update ECU firmware").doClick();

        assertEquals(0, tabs.getSelectedIndex());
        assertSame(firmware, tabs.getComponentAt(0));
        clickCancel(tabs.getComponentAt(1));
        assertSame(connect, tabs.getComponentAt(1));
        assertEquals(0, tabs.getSelectedIndex());
    }

    @Test
    void secondStartupPickerReplacesActivePickerAndRestoresOriginalConnectContent() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Update ECU firmware", new JPanel());
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {});

        ManualIniFilePicker.showPicker(rootPane, rootPane, "first-signature");
        Component firstPicker = tabs.getComponentAt(1);
        ManualIniFilePicker.showPicker(rootPane, rootPane, "second-signature");
        Component secondPicker = tabs.getComponentAt(1);

        assertFalse(rootPane.getGlassPane().isVisible());
        assertNotSame(firstPicker, secondPicker);
        clickCancel(firstPicker);
        assertSame(secondPicker, tabs.getComponentAt(1), "stale picker actions must not restore over a newer picker");
        clickCancel(secondPicker);
        assertSame(connect, tabs.getComponentAt(1));
        assertEquals(0, tabs.getSelectedIndex());
    }

    @Test
    void approvalOfReplacementPickerImportsItsOwnSignatureAndRestoresConnectTab() throws Exception {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        CountDownLatch connectRestored = new CountDownLatch(1);
        JTabbedPane tabs = new JTabbedPane() {
            @Override
            public void setComponentAt(int index, Component component) {
                super.setComponentAt(index, component);
                if (component == connect) {
                    connectRestored.countDown();
                }
            }
        };
        tabs.addTab("Update ECU firmware", new JPanel());
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {});
        ManualIniFilePicker.showPicker(rootPane, rootPane, "first-signature");
        AtomicReference<String> importedSignature = new AtomicReference<>();
        CountDownLatch importFinished = new CountDownLatch(1);

        ManualIniFilePicker.showPicker(rootPane, rootPane, "second-signature",
            (signature, file) -> {
                importedSignature.set(signature);
                importFinished.countDown();
                return new Object();
            });
        SwingUtilities.invokeAndWait(() -> findChooser(tabs.getComponentAt(1)).approveSelection());

        assertTrue(importFinished.await(2, TimeUnit.SECONDS), "the injected import should complete");
        assertTrue(connectRestored.await(2, TimeUnit.SECONDS), "the completion callback should restore the tab");
        SwingUtilities.invokeAndWait(() -> {
            assertEquals("second-signature", importedSignature.get());
            assertSame(connect, tabs.getComponentAt(1));
            assertEquals(0, tabs.getSelectedIndex());
        });
    }

    @Test
    void manualImportRunsOffEdtAndIsCancelledWhenTargetIsLost() throws Exception {
        String signature = "target-signature";
        String port = "COM14";
        CountDownLatch importStarted = new CountDownLatch(1);
        CountDownLatch releaseImporter = new CountDownLatch(1);
        CountDownLatch importerFinished = new CountDownLatch(1);
        AtomicBoolean importerOnEdt = new AtomicBoolean();
        AtomicBoolean targetLost = new AtomicBoolean();
        AtomicBoolean importerCompletedAfterTargetLoss = new AtomicBoolean();
        AtomicBoolean importerInterrupted = new AtomicBoolean();
        AtomicBoolean edtResponsive = new AtomicBoolean();
        AtomicReference<ManualIniFilePicker.StartupHost> hostRef = new AtomicReference<>();
        AtomicReference<JFileChooser> chooserRef = new AtomicReference<>();

        SwingUtilities.invokeAndWait(() -> {
            JRootPane rootPane = new JRootPane();
            JPanel connect = new JPanel();
            JTabbedPane tabs = new JTabbedPane();
            tabs.addTab("Firmware", new JPanel());
            tabs.addTab("Connect", connect);
            rootPane.setContentPane(tabs);
            ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {});
            ManualIniFilePicker.StartupHost host = (ManualIniFilePicker.StartupHost)
                rootPane.getClientProperty(ManualIniFilePicker.class.getName() + ".startupHost");
            hostRef.set(host);
            ManualIniFilePicker.applySearchProgressForTests(host, signature, port, 40);
            ManualIniFilePicker.showPicker(rootPane, rootPane, signature, (requestedSignature, file) -> {
                importerOnEdt.set(SwingUtilities.isEventDispatchThread());
                importStarted.countDown();
                try {
                    if (!releaseImporter.await(2, TimeUnit.SECONDS)) {
                        throw new IOException("bounded test importer wait expired");
                    }
                    importerCompletedAfterTargetLoss.set(targetLost.get());
                    return new Object();
                } catch (InterruptedException e) {
                    importerInterrupted.set(true);
                    Thread.currentThread().interrupt();
                    throw new IOException("test importer interrupted", e);
                } finally {
                    importerFinished.countDown();
                }
            });
            JFileChooser chooser = findChooser(tabs.getComponentAt(1));
            assertNotNull(chooser);
            chooser.setSelectedFile(new File("selected.ini"));
            chooserRef.set(chooser);
        });

        try {
            SwingUtilities.invokeLater(chooserRef.get()::approveSelection);
            assertTrue(importStarted.await(2, TimeUnit.SECONDS), "the injected importer should start");

            targetLost.set(true);
            SwingUtilities.invokeAndWait(() -> edtResponsive.set(true));
            SwingUtilities.invokeAndWait(
                () -> ManualIniFilePicker.targetLostForTests(hostRef.get(), port, false));
            assertTrue(importerFinished.await(2, TimeUnit.SECONDS), "the injected importer should finish");
            SwingUtilities.invokeAndWait(() -> {});

            assertFalse(importerOnEdt.get(), "archive reading must not run on the EDT");
            assertTrue(edtResponsive.get(), "the EDT must remain available while the importer runs");
            assertTrue(importerInterrupted.get(), "losing the recovery target must cancel the import worker");
            assertFalse(importerCompletedAfterTargetLoss.get(),
                "the import must not proceed after target ownership is lost");
        } finally {
            releaseImporter.countDown();
            if (importStarted.getCount() == 0) {
                importerFinished.await(2, TimeUnit.SECONDS);
            }
        }
    }

    @Test
    void driveSearchProgressPrecedesManualFallback() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Update ECU firmware", new JPanel());
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        List<String> progress = new ArrayList<>();
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {}, progress::add);

        ManualIniFilePicker.applySearchProgressForTests(
            (ManualIniFilePicker.StartupHost) rootPane.getClientProperty(
                ManualIniFilePicker.class.getName() + ".startupHost"),
            "target-signature", "COM4", 1);
        assertEquals(List.of("Searching Windows drives for the matching INI…"), progress);

        ManualIniFilePicker.showPicker(rootPane, rootPane, "target-signature");
        assertNotSame(connect, tabs.getComponentAt(1));
        assertNotNull(findChooser(tabs.getComponentAt(1)));
        assertEquals(List.of("Searching Windows drives for the matching INI…", ""), progress);
    }

    @Test
    void automaticSuccessClearsProgressAndDoesNotShowManualPicker() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Update ECU firmware", new JPanel());
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        List<String> progress = new ArrayList<>();
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {}, progress::add);
        ManualIniFilePicker.StartupHost host = (ManualIniFilePicker.StartupHost) rootPane.getClientProperty(
            ManualIniFilePicker.class.getName() + ".startupHost");

        ManualIniFilePicker.applySearchProgressForTests(host, "target-signature", "COM4", 1);
        ManualIniFilePicker.applySearchResolvedForTests(host, "target-signature", "COM4", 1);

        assertSame(connect, tabs.getComponentAt(1));
        assertNull(findChooser(tabs));
        assertEquals(List.of("Searching Windows drives for the matching INI…", ""), progress);
    }

    @Test
    void staleTargetProgressAndSuccessCannotReplaceCurrentRecovery() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Update ECU firmware", new JPanel());
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        List<String> progress = new ArrayList<>();
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {}, progress::add);
        ManualIniFilePicker.StartupHost host = (ManualIniFilePicker.StartupHost) rootPane.getClientProperty(
            ManualIniFilePicker.class.getName() + ".startupHost");

        ManualIniFilePicker.applySearchProgressForTests(host, "current-signature", "COM5", 2);
        ManualIniFilePicker.applySearchProgressForTests(host, "stale-signature", "COM4", 1);
        ManualIniFilePicker.applySearchResolvedForTests(host, "stale-signature", "COM4", 1);
        ManualIniFilePicker.applySearchResolvedForTests(host, "stale-signature", "COM4", 3);

        assertEquals(List.of("Searching Windows drives for the matching INI…"), progress);
        assertSame(connect, tabs.getComponentAt(1));
    }

    @Test
    void cancellingManualPickerDoesNotReopenOnTheNextProbe() {
        JRootPane rootPane = new JRootPane();
        AtomicInteger pickerRequests = new AtomicInteger();
        RealIniFileProvider.manualPicker = signature -> {
            pickerRequests.incrementAndGet();
            ManualIniFilePicker.showPicker(rootPane, rootPane, signature);
        };
        RealIniFileProvider provider = new RealIniFileProvider();

        assertThrows(IniNotFoundException.class, () -> provider.provide("not a real signature"));
        Component cancelledPicker = rootPane.getGlassPane();
        assertTrue(cancelledPicker.isVisible());
        clickCancel(cancelledPicker);
        assertFalse(cancelledPicker.isVisible());

        assertThrows(IniNotFoundException.class, () -> provider.provide("not a real signature"));
        assertEquals(1, pickerRequests.get());
        assertNotSame(cancelledPicker, rootPane.getGlassPane());
        assertFalse(rootPane.getGlassPane().isVisible());
    }

    @Test
    void lateProgressCannotResurrectAfterRecoveryResolves() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Update ECU firmware", new JPanel());
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        List<String> progress = new ArrayList<>();
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {}, progress::add);
        ManualIniFilePicker.StartupHost host = (ManualIniFilePicker.StartupHost) rootPane.getClientProperty(
            ManualIniFilePicker.class.getName() + ".startupHost");

        ManualIniFilePicker.applySearchProgressForTests(host, "current-signature", "COM5", 2);
        ManualIniFilePicker.applySearchResolvedForTests(host, "current-signature", "COM5", 2);
        ManualIniFilePicker.applySearchProgressForTests(host, "stale-signature", "COM4", 1);

        assertEquals(List.of(
            "Searching Windows drives for the matching INI…",
            ""),
            progress,
            "a resolved generation remains closed to late progress callbacks");
    }

    @Test
    void completedRootWorkerClearsProgressWithoutAnotherProbe() throws Exception {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Update ECU firmware", new JPanel());
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        List<String> progress = new ArrayList<>();
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {}, progress::add);
        ManualIniFilePicker.StartupHost host = (ManualIniFilePicker.StartupHost) rootPane.getClientProperty(
            ManualIniFilePicker.class.getName() + ".startupHost");
        CountDownLatch workerFinished = new CountDownLatch(1);

        ManualIniFilePicker.applySearchProgressForTests(host, "target-signature", "COM4", 3);
        ManualIniFilePicker.applySearchFinishedForTests(host, "target-signature", "COM4", 3);
        Thread worker = new Thread(workerFinished::countDown, "test completed root scan");
        worker.setDaemon(true);
        worker.start();

        assertTrue(workerFinished.await(1, TimeUnit.SECONDS));
        assertEquals(List.of("Searching Windows drives for the matching INI…", ""), progress,
            "terminal search completion clears the startup progress directly");
    }

    @Test
    void lateRootProgressCannotReplaceLaterStageOrCompletion() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        List<IniLocator.LookupProgress> progress = new ArrayList<>();
        ManualIniFilePicker.registerStartupHostWithLookupProgress(rootPane, tabs, connect, () -> {},
            progress::add, ignored -> true, null);
        ManualIniFilePicker.StartupHost host = (ManualIniFilePicker.StartupHost) rootPane.getClientProperty(
            ManualIniFilePicker.class.getName() + ".startupHost");

        ManualIniFilePicker.applyLookupProgressForTests(host, "same-target", "COM9", 6,
            new IniLocator.LookupProgress(IniLocator.LookupStage.BUNDLED_INI, 0, 0, 0, 1));
        ManualIniFilePicker.applyLookupProgressForTests(host, "same-target", "COM9", 6,
            new IniLocator.LookupProgress(IniLocator.LookupStage.DRIVE_ROOTS, 1, 12, 3, 2));
        ManualIniFilePicker.applyLookupProgressForTests(host, "same-target", "COM9", 6,
            new IniLocator.LookupProgress(IniLocator.LookupStage.CONSOLE_CACHE_DOWNLOAD, 0, 0, 0, 3));
        ManualIniFilePicker.applyLookupProgressForTests(host, "same-target", "COM9", 6,
            new IniLocator.LookupProgress(IniLocator.LookupStage.DRIVE_ROOTS, 2, 12, 2, 4));
        assertEquals(3, progress.size(),
            "a root worker update cannot roll progress back from a later lookup source");

        ManualIniFilePicker.applyLookupProgressForTests(host, "same-target", "COM9", 6,
            new IniLocator.LookupProgress(IniLocator.LookupStage.COMPLETE, 0, 0, 0, 5));
        ManualIniFilePicker.applyLookupProgressForTests(host, "same-target", "COM9", 6,
            new IniLocator.LookupProgress(IniLocator.LookupStage.DRIVE_ROOTS, 12, 12, 0, 6));
        assertEquals(4, progress.size(),
            "no late root callback can revive progress after terminal completion");
        assertEquals(IniLocator.LookupStage.COMPLETE, progress.get(3).stage);
    }

    @Test
    void unrelatedProbeDoesNotDismissTheActivePicker() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Update ECU firmware", new JPanel());
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        List<String> progress = new ArrayList<>();
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {}, progress::add,
            port -> "COM-A".equals(port));
        ManualIniFilePicker.StartupHost host = (ManualIniFilePicker.StartupHost) rootPane.getClientProperty(
            ManualIniFilePicker.class.getName() + ".startupHost");

        ManualIniFilePicker.applySearchProgressForTests(host, "selected-signature", "COM-A", 1);
        ManualIniFilePicker.showPicker(rootPane, rootPane, "selected-signature");
        Component selectedPicker = tabs.getComponentAt(1);
        ManualIniFilePicker.applySearchProgressForTests(host, "incidental-signature", "COM-B", 2);

        assertNotSame(connect, selectedPicker);
        assertSame(selectedPicker, tabs.getComponentAt(1),
            "an unrelated probe cannot take over target-owned recovery");
    }

    @Test
    void initialDiscoveryClaimsRecoveryButCannotReplaceAnExistingTarget() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        List<String> progress = new ArrayList<>();
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {}, progress::add,
            port -> ManualIniFilePicker.ownsRecoveryPort(port, null, "COM8", true));
        ManualIniFilePicker.StartupHost host = (ManualIniFilePicker.StartupHost) rootPane.getClientProperty(
            ManualIniFilePicker.class.getName() + ".startupHost");

        ManualIniFilePicker.applySearchProgressForTests(host, "discovered-ecu", "COM9", 1);
        ManualIniFilePicker.applySearchProgressForTests(host, "unrelated-ecu", "COM-B", 2);

        assertEquals("COM9", ManualIniFilePicker.activeRecoveryPort(),
            "initial discovery can supersede a stale remembered COM8, but the first recovery owns the UI");
        assertEquals(List.of("Searching Windows drives for the matching INI…"), progress);
    }

    @Test
    void expectedMissingIniClosePreservesRecoveryButTargetLossClosesIt() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        List<String> progress = new ArrayList<>();
        AvailableHardware initialHardware = new AvailableHardware(List.of(), false, false, false);
        AtomicReference<AvailableHardware> currentHardware = new AtomicReference<>(initialHardware);
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {}, progress::add,
            ignored -> true, currentHardware::get);
        ManualIniFilePicker.StartupHost host = (ManualIniFilePicker.StartupHost) rootPane.getClientProperty(
            ManualIniFilePicker.class.getName() + ".startupHost");

        ManualIniFilePicker.applySearchProgressForTests(host, "missing-ini", "COM9", 4);
        ManualIniFilePicker.targetLostForTests(host, "COM9", true);
        assertEquals("COM9", ManualIniFilePicker.activeRecoveryPort(),
            "the expected connection close after IniNotFoundException must preserve its recovery");

        ManualIniFilePicker.hardwareUpdatedForTests(host, initialHardware);
        assertEquals("COM9", ManualIniFilePicker.activeRecoveryPort(),
            "the empty snapshot from before discovery must not invalidate the newly identified target");

        AvailableHardware stillPresent = new AvailableHardware(
            List.of(new PortResult("COM9", SerialPortType.Unknown)), false, false, false);
        currentHardware.set(stillPresent);
        ManualIniFilePicker.hardwareUpdatedForTests(host, stillPresent);
        assertEquals("COM9", ManualIniFilePicker.activeRecoveryPort(),
            "a present Unknown port is still being probed, not unplugged");

        AvailableHardware removed = new AvailableHardware(List.of(), false, false, false);
        currentHardware.set(removed);
        ManualIniFilePicker.hardwareUpdatedForTests(host, removed);
        assertNull(ManualIniFilePicker.activeRecoveryPort(),
            "a fresh hardware snapshot which omits the target invalidates recovery");
        assertTrue(ManualIniFilePicker.ownsRecoveryPort("COM9", null, "COM8", true),
            "a retained stale remembered COM8 must not block a newly discovered COM9 after removal");
        assertEquals(List.of("Searching Windows drives for the matching INI…", ""), progress);
    }

    @Test
    void sameTargetRetryAdvancesGenerationWithoutClosingTheManualChooser() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        List<String> progress = new ArrayList<>();
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {}, progress::add);
        ManualIniFilePicker.StartupHost host = (ManualIniFilePicker.StartupHost) rootPane.getClientProperty(
            ManualIniFilePicker.class.getName() + ".startupHost");

        ManualIniFilePicker.applySearchProgressForTests(host, "same-target", "COM9", 7);
        ManualIniFilePicker.showPicker(rootPane, rootPane, "same-target");
        Component pickerView = tabs.getComponentAt(0);
        JFileChooser chooser = findChooser(pickerView);

        ManualIniFilePicker.applySearchProgressForTests(host, "same-target", "COM9", 8);
        int progressEvents = progress.size();
        ManualIniFilePicker.applySearchFinishedForTests(host, "same-target", "COM9", 7);
        ManualIniFilePicker.targetLostGenerationForTests(host, "COM9", 7);

        assertSame(pickerView, tabs.getComponentAt(0),
            "a newer retry of the same target must not restore or replace the open chooser");
        assertEquals(progressEvents, progress.size(),
            "a stale completion must not clear the newer retry's progress");
        assertSame(chooser, findChooser(tabs.getComponentAt(0)),
            "the existing chooser and its user actions stay live while automatic lookup retries");
        clickCancel(chooser);
        assertSame(connect, tabs.getComponentAt(0),
            "the original chooser's cancel action remains connected after generation advance");
    }

    @Test
    void explicitlySelectedNewTargetCanReplaceRetainedRecovery() {
        JRootPane rootPane = new JRootPane();
        JPanel connect = new JPanel();
        JTabbedPane tabs = new JTabbedPane();
        tabs.addTab("Connect", connect);
        rootPane.setContentPane(tabs);
        AtomicReference<String> selectedPort = new AtomicReference<>("COM-A");
        ManualIniFilePicker.registerStartupHost(rootPane, tabs, connect, () -> {}, ignored -> {},
            port -> ManualIniFilePicker.ownsRecoveryPort(port, null, selectedPort.get(), false));
        ManualIniFilePicker.StartupHost host = (ManualIniFilePicker.StartupHost) rootPane.getClientProperty(
            ManualIniFilePicker.class.getName() + ".startupHost");

        ManualIniFilePicker.applySearchProgressForTests(host, "signature-a", "COM-A", 11);
        ManualIniFilePicker.showPicker(rootPane, rootPane, "signature-a");
        selectedPort.set("COM-B");
        ManualIniFilePicker.applySearchProgressForTests(host, "signature-b", "COM-B", 12);

        assertEquals("COM-B", ManualIniFilePicker.activeRecoveryPort(),
            "a selected newer target may take ownership from the retained old target");
        assertSame(connect, tabs.getComponentAt(0),
            "replacing a target restores the old chooser before starting the new recovery");
    }

    @Test
    void statusAnimationStillEmitsGenericStatusForUnownedConsumers() throws Exception {
        JLabel status = new JLabel("Searching Windows drives for the matching INI…");
        CountDownLatch animationUpdated = new CountDownLatch(1);
        StatusAnimation animation = new StatusAnimation(text -> {
            status.setText(text);
            animationUpdated.countDown();
        }, "Scanning ports");

        try {
            assertTrue(animationUpdated.await(2, TimeUnit.SECONDS));
            assertTrue(status.getText().startsWith("Scanning ports"),
                "the owning UI decides whether to accept this generic status update");
        } finally {
            animation.stop();
        }
    }

    private static JButton findButton(Component component, String text) {
        if (component instanceof JButton && text.equals(((JButton) component).getText())) {
            return (JButton) component;
        }
        if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                JButton found = findButton(child, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static void clickCancel(Component component) {
        if (component instanceof JFileChooser) {
            ((JFileChooser) component).cancelSelection();
        } else if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                clickCancel(child);
            }
        }
    }

    private static JFileChooser findChooser(Component component) {
        if (component instanceof JFileChooser) {
            return (JFileChooser) component;
        }
        if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                JFileChooser found = findChooser(child);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static void layoutRecursively(Container container) {
        container.doLayout();
        for (Component component : container.getComponents()) {
            if (component instanceof Container) {
                layoutRecursively((Container) component);
            }
        }
    }
}
