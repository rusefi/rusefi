package com.rusefi.maintenance;

import com.rusefi.AvailableHardware;
import com.rusefi.ConnectivityContext;
import com.rusefi.FakePortScanner;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.LinkManager;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.OpenBltSwitchJob;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import org.junit.jupiter.api.Test;
import org.mockito.ArgumentCaptor;

import javax.swing.*;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * A detected PCAN adapter must offer the same bootloader workflow as serial: command a live CAN ECU
 * into OpenBLT ("Switch to OpenBLT Mode"), then flash it over CAN ("Manual OpenBLT Update via CAN").
 */
class ProgramSelectorPcanTest {
    private final PortResult pcanEcu = new PortResult(LinkManager.PCAN, SerialPortType.Ecu);

    private static ProgramSelector.ProgrammingCapabilities windowsCapabilities() {
        return new ProgramSelector.ProgrammingCapabilities(false, false, "proteus_f7", "proteus_f7", true);
    }

    private ProgramSelector newSelector(ConnectivityContext context, JComboBox<PortResult> ports) {
        ProgramSelector selector = new ProgramSelector(context, ports, ignored -> {}, () -> {});
        selector.setCapabilityProviderForTests(ProgramSelectorPcanTest::windowsCapabilities);
        return selector;
    }

    private static ConnectivityContext contextFor(AvailableHardware hardware) {
        ConnectivityContext context = mock(ConnectivityContext.class);
        when(context.getCurrentHardware()).thenReturn(hardware);
        when(context.getConnectedEcuTarget()).thenReturn(ConnectedEcuTarget.forManualRecovery("proteus_f7"));
        when(context.getPortScanner()).thenReturn(new FakePortScanner());
        return context;
    }

    private static LinkManager liveLink(String port) {
        LinkManager linkManager = mock(LinkManager.class);
        when(linkManager.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class));
        when(linkManager.getLastTriedPort()).thenReturn(port);
        return linkManager;
    }

    @Test
    void livePcanEcuOffersSwitchToOpenBltAndManualCanUpdate() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AvailableHardware hardware = new AvailableHardware(
                Collections.singletonList(pcanEcu), false, false, true);
            ProgramSelector selector = newSelector(contextFor(hardware),
                new JComboBox<>(new PortResult[]{pcanEcu}));
            selector.setLinkManager(liveLink(LinkManager.PCAN));

            List<String> labels = menuLabels(
                selector.apply(hardware, false, false, "proteus_f7", "proteus_f7"));

            assertTrue(labels.contains(UpdateMode.OPENBLT_SWITCH.displayText), labels.toString());
            assertTrue(labels.contains(UpdateMode.OPENBLT_CAN.displayText), labels.toString());
            assertFalse(labels.contains(UpdateMode.OPENBLT_MANUAL.displayText),
                "the serial manual flasher cannot program a CAN target");
        });
    }

    @Test
    void pcanAdapterWithoutAnEcuStillOffersManualCanUpdateButNoSwitch() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            // The board sits silent in OpenBLT on the bus: no ECU port entry, only the adapter.
            AvailableHardware hardware = new AvailableHardware(
                Collections.emptyList(), false, false, true);
            ProgramSelector selector = newSelector(contextFor(hardware), new JComboBox<>());

            List<String> labels = menuLabels(
                selector.apply(hardware, false, false, "proteus_f7", "proteus_f7"));

            assertTrue(labels.contains(UpdateMode.OPENBLT_CAN.displayText), labels.toString());
            assertFalse(labels.contains(UpdateMode.OPENBLT_SWITCH.displayText),
                "there is no live ECU to command into the bootloader");
        });
    }

    @Test
    void clickingSwitchOnALivePcanConnectionStartsTheSwitchJob() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AvailableHardware hardware = new AvailableHardware(
                Collections.singletonList(pcanEcu), false, false, true);
            ConnectivityContext context = contextFor(hardware);
            ProgramSelector selector = newSelector(context, new JComboBox<>(new PortResult[]{pcanEcu}));
            selector.setLinkManager(liveLink(LinkManager.PCAN));
            SingleAsyncJobExecutor executor = mock(SingleAsyncJobExecutor.class);
            when(executor.isNotInProgress()).thenReturn(true);
            selector.setJobExecutor(executor);

            JPopupMenu menu = selector.apply(hardware, false, false, "proteus_f7", "proteus_f7");
            menuItem(menu, UpdateMode.OPENBLT_SWITCH.displayText).doClick();

            ArgumentCaptor<AsyncJob> job = ArgumentCaptor.forClass(AsyncJob.class);
            verify(executor).startJob(job.capture(), any());
            assertTrue(job.getValue() instanceof OpenBltSwitchJob, String.valueOf(job.getValue()));
        });
    }

    @Test
    void switchIsRejectedWithoutAMatchingLiveCanConnection() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AvailableHardware hardware = new AvailableHardware(
                Collections.singletonList(pcanEcu), false, false, true);
            ConnectivityContext context = contextFor(hardware);
            ProgramSelector selector = newSelector(context, new JComboBox<>(new PortResult[]{pcanEcu}));
            // Scanner sees an ECU over PCAN, but the console is not connected to it.
            SingleAsyncJobExecutor executor = mock(SingleAsyncJobExecutor.class);
            when(executor.isNotInProgress()).thenReturn(true);
            selector.setJobExecutor(executor);

            JPopupMenu menu = selector.apply(hardware, false, false, "proteus_f7", "proteus_f7");
            menuItem(menu, UpdateMode.OPENBLT_SWITCH.displayText).doClick();

            verify(executor, never()).startJob(any(), any());
        });
    }

    @Test
    void liveSocketCanEcuOffersSwitchWithoutWindows() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult socketCan = new PortResult(LinkManager.SOCKET_CAN, SerialPortType.Ecu);
            AvailableHardware hardware = new AvailableHardware(
                Collections.singletonList(socketCan), false, false, false, true);
            ProgramSelector selector = new ProgramSelector(contextFor(hardware),
                new JComboBox<>(new PortResult[]{socketCan}), ignored -> {}, () -> {});
            selector.setCapabilityProviderForTests(() -> new ProgramSelector.ProgrammingCapabilities(
                false, false, "proteus_f7", "proteus_f7", false));
            selector.setLinkManager(liveLink(LinkManager.SOCKET_CAN));

            List<String> labels = menuLabels(
                selector.apply(hardware, false, false, "proteus_f7", "proteus_f7"));

            assertTrue(labels.contains(UpdateMode.OPENBLT_SWITCH.displayText), labels.toString());
            assertFalse(labels.contains(UpdateMode.OPENBLT_CAN.displayText),
                "the PCAN flasher entry stays Windows-only");
        });
    }

    private static JMenuItem menuItem(JPopupMenu menu, String text) {
        return Arrays.stream(menu.getComponents())
            .filter(JMenuItem.class::isInstance)
            .map(JMenuItem.class::cast)
            .filter(item -> text.equals(item.getText()))
            .findFirst()
            .orElseThrow(() -> new AssertionError(text + " not found in " + menuLabels(menu)));
    }

    private static List<String> menuLabels(JPopupMenu menu) {
        return Arrays.stream(menu.getComponents())
            .filter(JMenuItem.class::isInstance)
            .map(component -> ((JMenuItem) component).getText())
            .collect(java.util.stream.Collectors.toList());
    }
}
