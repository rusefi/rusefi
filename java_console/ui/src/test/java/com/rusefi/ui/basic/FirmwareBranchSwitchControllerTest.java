package com.rusefi.ui.basic;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.ui.wizard.FirmwareBranchSwitchPanel;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class FirmwareBranchSwitchControllerTest {
    @Test
    void limaDevOffersBranchSwitch() throws Exception {
        Fixture f = new Fixture("lts-26lima-dev");
        SwingUtilities.invokeAndWait(() -> {
            assertTrue(f.controller.getButton().isVisible());
        });
    }

    @Test
    void limaDevRequiresWarningConfirmationBeforeSwitchingToDev() throws Exception {
        Fixture f = new Fixture("lts-26lima-dev");
        SwingUtilities.invokeAndWait(() -> {
            f.controller.getButton().doClick();
            assertNotNull(f.panel());
            assertEquals(0, f.jobsStarted.get());
            f.panel().clickCancelForTests();
            assertEquals(0, f.jobsStarted.get());

            f.controller.getButton().doClick();
            FirmwareBranchSwitchPanel panel = f.panel();
            panel.clickLimaDevForTests();
            assertEquals(0, f.jobsStarted.get(), "The current branch must not be a switch option");
            panel.clickDevelopmentForTests();
            panel.clickDevelopmentForTests();
        });
        assertEquals(1, f.jobsStarted.get());
        assertTrue(FirmwareBranchSwitchController.isBranchSwitchJob(f.startedJob.get()));
        assertEquals(0, f.factoryCalls.get());
        assertTrue(prepareOffEdt(f.startedJob.get()));
        assertEquals(1, f.factoryCalls.get());
        assertEquals("development", f.requestedBranch.get());
    }

    @Test
    void chooserCancelStartsNoJobAndConfirmationCanOnlyBeConsumedOnce() throws Exception {
        Fixture f = new Fixture("lts-26lima");
        SwingUtilities.invokeAndWait(() -> {
            f.controller.getButton().doClick();
            f.panel().clickCancelForTests();
            assertEquals(0, f.jobsStarted.get());

            f.controller.getButton().doClick();
            FirmwareBranchSwitchPanel panel = f.panel();
            panel.clickLimaDevForTests();
            panel.clickLimaDevForTests();
        });

        assertEquals(1, f.jobsStarted.get());
        assertTrue(FirmwareBranchSwitchController.isBranchSwitchJob(f.startedJob.get()));
        assertEquals(0, f.factoryCalls.get(), "selection/download starts only in reserved job preparation");
        assertTrue(prepareOffEdt(f.startedJob.get()));
        assertEquals(1, f.factoryCalls.get());
        assertEquals("lts-26lima-dev", f.requestedBranch.get());
    }

    @Test
    void liveSourceChangeOnSamePortWhileWarningOpenCancels() throws Exception {
        Fixture f = new Fixture("lts-26lima");
        SwingUtilities.invokeAndWait(() -> {
            f.controller.getButton().doClick();
            FirmwareBranchSwitchPanel panel = f.panel();
            f.liveSignature.set("rusEFI lts-26lima.2026.02.01.board.newhash");
            panel.clickDevelopmentForTests();
            panel.clickDevelopmentForTests();
        });
        assertEquals(0, f.jobsStarted.get());
        assertEquals(0, f.factoryCalls.get());
    }

    @Test
    void onlyLiveSupportedSourceOnSelectedPortWithIdleExecutorOffersChoices() throws Exception {
        Fixture f = new Fixture("lts-26lima");
        SwingUtilities.invokeAndWait(() -> {
            assertTrue(f.controller.getButton().isVisible());
            f.liveSignature.set("rusEFI development.2026.01.01.board.hash");
            f.controller.refreshButton();
            assertFalse(f.controller.getButton().isVisible());
            f.liveSignature.set("rusEFI lts-26lima-dev.2026.01.01.board.hash");
            f.controller.refreshButton();
            assertTrue(f.controller.getButton().isVisible());
            f.liveSignature.set("rusEFI lts-25kansas.2026.01.01.board.hash");
            f.controller.refreshButton();
            assertFalse(f.controller.getButton().isVisible());
            f.liveSignature.set("rusEFI lts-26lima.2026.01.01.board.hash");
            f.lastTriedPort.set("COM5");
            f.controller.refreshButton();
            assertFalse(f.controller.getButton().isVisible());
            f.lastTriedPort.set("COM4");
            f.linkPresent.set(false);
            f.controller.refreshButton();
            assertFalse(f.controller.getButton().isVisible());
            f.linkPresent.set(true);
            f.protocolClosed.set(true);
            f.controller.refreshButton();
            assertFalse(f.controller.getButton().isVisible());
            f.protocolClosed.set(false);
            f.currentPort.set(new PortResult("SLCAN:COM4", SerialPortType.EcuWithOpenblt));
            f.lastTriedPort.set("SLCAN:COM4");
            f.controller.refreshButton();
            assertFalse(f.controller.getButton().isVisible());
            f.currentPort.set(new PortResult("COM4", SerialPortType.Ecu));
            f.lastTriedPort.set("COM4");
            f.executorIdle.set(false);
            f.controller.refreshButton();
            assertFalse(f.controller.getButton().isVisible());
        });
        assertEquals(0, f.jobsStarted.get());
    }

    @Test
    void stalePortAfterDownloadBlocksPreparation() throws Exception {
        Fixture f = new Fixture("lts-26lima");
        f.afterFactory = () -> f.currentPort.set(new PortResult("COM5", SerialPortType.Ecu));
        AsyncJob[] wrapper = new AsyncJob[1];
        SwingUtilities.invokeAndWait(() -> {
            f.controller.getButton().doClick();
            f.panel().clickDevelopmentForTests();
            wrapper[0] = f.startedJob.get();
        });

        assertFalse(prepareOffEdt(wrapper[0]));
        assertEquals(1, f.factoryCalls.get());
        assertEquals(0, f.delegateCalls.get());
    }

    @Test
    void flashingUiStateAfterPrepareDoesNotAbortDelegatedJob() throws Exception {
        Fixture f = new Fixture("lts-26lima");
        AsyncJob[] wrapper = new AsyncJob[1];
        SwingUtilities.invokeAndWait(() -> {
            f.controller.getButton().doClick();
            f.panel().clickLimaDevForTests();
            wrapper[0] = f.startedJob.get();
        });

        assertTrue(prepareOffEdt(wrapper[0]));
        f.connected.set(false); // SingleAsyncJobExecutor has now transitioned the ECU UI to FLASHING.
        Thread worker = new Thread(() -> wrapper[0].doJob(UpdateOperationCallbacks.DUMMY, () -> {}));
        worker.start();
        worker.join();
        assertEquals(1, f.delegateCalls.get());
    }

    private static boolean prepareOffEdt(AsyncJob job) throws Exception {
        AtomicBoolean result = new AtomicBoolean();
        AtomicReference<Throwable> failure = new AtomicReference<>();
        Thread worker = new Thread(() -> {
            try {
                assertFalse(SwingUtilities.isEventDispatchThread());
                result.set(job.prepare(UpdateOperationCallbacks.DUMMY));
            } catch (Throwable t) {
                failure.set(t);
            }
        });
        worker.start();
        worker.join();
        assertNull(failure.get());
        return result.get();
    }

    private static class Fixture {
        final AtomicReference<PortResult> currentPort = new AtomicReference<>(new PortResult("COM4", SerialPortType.Ecu));
        final AtomicReference<String> liveSignature = new AtomicReference<>(signature("lts-26lima", "hash-a"));
        final AtomicReference<String> requestedBranch = new AtomicReference<>();
        final AtomicReference<String> lastTriedPort = new AtomicReference<>("COM4");
        final AtomicBoolean linkPresent = new AtomicBoolean(true);
        final AtomicBoolean protocolClosed = new AtomicBoolean();
        final AtomicBoolean connected = new AtomicBoolean(true);
        final AtomicBoolean executorIdle = new AtomicBoolean(true);
        final AtomicInteger jobsStarted = new AtomicInteger();
        final AtomicInteger factoryCalls = new AtomicInteger();
        final AtomicInteger delegateCalls = new AtomicInteger();
        final AtomicReference<AsyncJob> startedJob = new AtomicReference<>();
        Runnable afterFactory = () -> {};
        final AtomicReference<JComponent> shown = new AtomicReference<>();
        final FirmwareBranchSwitchController controller;

        Fixture(String branch) {
            liveSignature.set(signature(branch, "hash-a"));
            LinkManager lm = mock(LinkManager.class);
            com.rusefi.binaryprotocol.BinaryProtocol protocol = mock(com.rusefi.binaryprotocol.BinaryProtocol.class);
            when(protocol.isClosed()).thenAnswer(call -> protocolClosed.get());
            when(lm.getBinaryProtocol()).thenAnswer(call -> linkPresent.get()
                ? protocol : null);
            when(lm.getLastTriedPort()).thenAnswer(call -> lastTriedPort.get());
            // The test provider is the live protocol signature; the production provider reads BinaryProtocol.signature.
            SingleAsyncJobExecutor executor = mock(SingleAsyncJobExecutor.class);
            when(executor.isNotInProgress()).thenAnswer(call -> executorIdle.get());
            FirmwareBranchSwitchController.JobFactory factory = (port, source, target, callbacks) -> {
                factoryCalls.incrementAndGet();
                requestedBranch.set(target);
                afterFactory.run();
                return new AsyncJob("selected") {
                    @Override public void doJob(UpdateOperationCallbacks callbacks, Runnable done) {
                        delegateCalls.incrementAndGet();
                        done.run();
                    }
                };
            };
            controller = new FirmwareBranchSwitchController(mock(ConnectivityContext.class),
                UpdateOperationCallbacks.DUMMY, executor, () -> Optional.ofNullable(currentPort.get()),
                connected::get, shown::set, () -> shown.set(null), factory, job -> {
                    jobsStarted.incrementAndGet();
                    startedJob.set(job);
                }, liveSignature::get);
            controller.setLinkManager(lm);
        }

        FirmwareBranchSwitchPanel panel() { return (FirmwareBranchSwitchPanel) shown.get(); }

        private static String signature(String branch, String hash) {
            return "rusEFI " + branch + ".2026.01.01.board." + hash;
        }
    }
}
