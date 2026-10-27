package com.rusefi.maintenance;

import com.rusefi.AvailableHardware;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.UiProperties;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.core.FindFileHelper;
import com.rusefi.maintenance.jobs.DfuAutoJob;
import com.rusefi.maintenance.jobs.DfuManualJob;
import com.rusefi.maintenance.jobs.DfuSwitchJob;
import com.rusefi.maintenance.jobs.OpenBltAutoJob;
import com.rusefi.maintenance.jobs.StLinkJob;
import com.rusefi.maintenance.jobs.OpenBltManualJob;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import com.rusefi.ui.wizard.DfuBoardSelectionPanel;
import com.rusefi.ui.wizard.DfuRecoveryConfirmationPanel;
import com.rusefi.ui.wizard.FirmwareProgrammingPanel;
import com.rusefi.updater.OpenbltDetectorStrategy.OpenbltInfo;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.*;
import java.awt.Component;
import java.awt.Container;
import java.awt.event.ActionEvent;
import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Consumer;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

class ProgramSelectorDfuUiTest {
    @Test
    void singlePrimaryButtonAdaptsBetweenRecoveryAndUpdateWithoutProtocolLabels() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult dfu = new PortResult("DFU", SerialPortType.Dfu);
            PortResult openBlt = new PortResult("BLT", SerialPortType.OpenBlt);
            PortResult ecu = new PortResult("ECU", SerialPortType.Ecu);
            AvailableHardware hardware = new AvailableHardware(Arrays.asList(dfu, openBlt, ecu), true, false, false);
            ConnectivityContext context = mock(ConnectivityContext.class);
            when(context.getCurrentHardware()).thenReturn(hardware);
            when(context.getConnectedEcuTarget()).thenReturn(ConnectedEcuTarget.forManualRecovery("proteus_f7"));
            JComboBox<PortResult> ports = new JComboBox<>(new PortResult[]{dfu, openBlt, ecu});
            ProgramSelector selector = new ProgramSelector(context, ports, ignored -> {}, () -> {});
            LinkManager linkManager = mock(LinkManager.class);
            when(linkManager.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class));
            when(linkManager.getLastTriedPort()).thenReturn("ECU");
            selector.setLinkManager(linkManager);
            selector.apply(hardware, true, false, "proteus_f7", "proteus_f7");

            JButton mainButton = findButton(selector.getControl(), "Recover ECU");
            assertNotNull(mainButton);
            assertTrue(mainButton.isEnabled(), "the selected detected DFU offers explicitly labelled recovery");
            assertEquals(1, countFirmwareActionButtons(selector.getControl()));
            ports.setSelectedItem(openBlt);
            assertTrue(mainButton.isEnabled(), "a currently detected OpenBLT device remains a normal update target");
            assertEquals("Update ECU firmware", findButton(selector.getControl(), "Update ECU firmware").getText());
            assertNull(findButton(selector.getControl(), "Recover ECU"), "there is no separate recovery control");
            ports.setSelectedItem(ecu);
            assertEquals("Update ECU firmware", findButton(selector.getControl(), "Update ECU firmware").getText());
            assertEquals(1, countFirmwareActionButtons(selector.getControl()));
        });
    }

    @Test
    void specialistActionsAreGroupedUnderAdvancedProgrammingTools() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult dfu = new PortResult("DFU", SerialPortType.Dfu);
            PortResult ecu = new PortResult("ECU", SerialPortType.Ecu);
            AvailableHardware hardware = new AvailableHardware(Arrays.asList(dfu, ecu), true, false, false);
            ConnectivityContext context = mock(ConnectivityContext.class);
            when(context.getCurrentHardware()).thenReturn(hardware);
            when(context.getConnectedEcuTarget()).thenReturn(ConnectedEcuTarget.forManualRecovery("proteus_f7"));
            ProgramSelector selector = new ProgramSelector(context, new JComboBox<>(new PortResult[]{dfu, ecu}),
                ignored -> {}, () -> {});

            JPopupMenu menu = selector.apply(hardware, true, false, "proteus_f7", "proteus_f7");
            assertEquals(1, menu.getComponentCount());
            assertTrue(menu.getComponent(0) instanceof JMenu);
            JMenu advancedTools = (JMenu) menu.getComponent(0);
            assertEquals("Advanced programming tools", advancedTools.getText());
            List<String> advancedLabels = Arrays.stream(advancedTools.getMenuComponents())
                .filter(JMenuItem.class::isInstance)
                .map(component -> ((JMenuItem) component).getText())
                .collect(java.util.stream.Collectors.toList());

            assertTrue(advancedLabels.containsAll(Arrays.asList(
                UpdateMode.DFU_MANUAL.displayText,
                "Pick Custom File [DFU]",
                UpdateMode.DFU_ERASE.displayText,
                UpdateMode.OPENBLT_SWITCH.displayText,
                UpdateMode.OPENBLT_MANUAL.displayText
            )), advancedLabels.toString());
            assertFalse(advancedLabels.contains(UpdateMode.DFU_AUTO.displayText),
                "the selected port is DFU, not a live ECU eligible for automatic DFU");
            assertFalse(advancedLabels.contains(UpdateMode.DFU_SWITCH.displayText));
            assertNotNull(findButton(selector.getControl(), "Recover ECU"),
                "the selected DFU changes the primary default action, while advanced tools remain available");
            assertEquals(1, countFirmwareActionButtons(selector.getControl()));
        });
    }

    @Test
    void recoveryPrimaryConfirmsAndStartsManualJobOnlyWhileAvailableAndIdle() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult dfu = new PortResult("DFU", SerialPortType.Dfu);
            AvailableHardware hardware = new AvailableHardware(Collections.singletonList(dfu), true, false, false);
            ConnectivityContext context = mock(ConnectivityContext.class);
            when(context.getCurrentHardware()).thenReturn(hardware);
            when(context.getConnectedEcuTarget()).thenReturn(ConnectedEcuTarget.forManualRecovery("proteus_f7"));
            ProgramSelector selector = new ProgramSelector(context, new JComboBox<>(new PortResult[]{dfu}),
                ignored -> {}, () -> {});
            SingleAsyncJobExecutor executor = mock(SingleAsyncJobExecutor.class);
            when(executor.isNotInProgress()).thenReturn(true);
            selector.setJobExecutor(executor);

            selector.setCapabilityProviderForTests(() -> capabilities(true, true, "universal", "proteus_f7"));
            JPopupMenu menu = selector.apply(hardware, true, true, "universal", "proteus_f7");
            assertEquals(1, menu.getComponentCount(), "manual DFU remains available under advanced tools");
            JMenu advanced = (JMenu) menu.getComponent(0);
            assertEquals(UpdateMode.DFU_MANUAL.displayText, ((JMenuItem) advanced.getMenuComponent(0)).getText());
            JButton recover = findButton(selector.getControl(), "Recover ECU");
            assertNotNull(recover);
            assertTrue(recover.isVisible());
            assertTrue(recover.isEnabled());
            selector.setDfuRecoveryConfirmationForTests(message -> {
                assertTrue(message.contains("exact board"), message);
                assertTrue(message.contains("stable power"), message);
                return true;
            });
            recover.doClick();
            verify(executor).startJob(isA(DfuManualJob.class), any(Component.class));

            when(executor.isNotInProgress()).thenReturn(false);
            selector.apply(hardware, true, false, "universal", "proteus_f7");
            assertFalse(recover.isEnabled());
            JButton arrow = findButton(selector.getControl(), "");
            assertNotNull(arrow);
            assertFalse(arrow.isEnabled(), "advanced actions are disabled while a job is running");
            recover.doClick();
            verify(executor).startJob(isA(DfuManualJob.class), any(Component.class));
        });
    }

    @Test
    void normalUpdateShortcutDoesNotDispatchOrInterceptDetectedDfuRecovery() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult dfu = new PortResult("DFU", SerialPortType.Dfu);
            AvailableHardware hardware = new AvailableHardware(Collections.singletonList(dfu), true, false, false);
            ConnectivityContext context = mock(ConnectivityContext.class);
            when(context.getCurrentHardware()).thenReturn(hardware);
            when(context.getConnectedEcuTarget()).thenReturn(ConnectedEcuTarget.forManualRecovery("proteus_f7"));
            ProgramSelector selector = new ProgramSelector(context, new JComboBox<>(new PortResult[]{dfu}),
                ignored -> {}, () -> {});
            SingleAsyncJobExecutor executor = mock(SingleAsyncJobExecutor.class);
            when(executor.isNotInProgress()).thenReturn(true);
            selector.setJobExecutor(executor);
            selector.apply(hardware, true, false, "universal", "proteus_f7");
            JButton main = findButton(selector.getControl(), "Recover ECU");
            assertNotNull(main);
            assertTrue(main.isEnabled(), "only the adaptive mouse action offers recovery; the shortcut stays normal-only");
            selector.setDfuRecoveryConfirmationForTests(message -> fail("the normal shortcut must not offer recovery"));
            java.util.concurrent.atomic.AtomicBoolean intercepted = new java.util.concurrent.atomic.AtomicBoolean();
            selector.setFirmwareUpdateInterceptor(source -> {
                intercepted.set(true);
                return true;
            });

            selector.triggerUpdateFirmware();

            assertFalse(intercepted.get(), "the LTS interceptor must not turn the normal shortcut into recovery");
            verify(executor, never()).startJob(any(), any());
        });
    }

    @Test
    void startupEcuNeedsMatchingLiveConnectionThenUsesAutomaticDfuUpdate() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult ecu = new PortResult("ECU", SerialPortType.Ecu);
            AvailableHardware hardware = new AvailableHardware(Collections.singletonList(ecu), false, false, false);
            ConnectivityContext context = mock(ConnectivityContext.class);
            ConnectedEcuTarget target = new ConnectedEcuTarget();
            target.setConnectedTargetForUnitTest("proteus_f7");
            when(context.getCurrentHardware()).thenReturn(hardware);
            when(context.getConnectedEcuTarget()).thenReturn(target);
            ProgramSelector selector = new ProgramSelector(context, new JComboBox<>(new PortResult[]{ecu}),
                ignored -> {}, () -> {});
            SingleAsyncJobExecutor executor = mock(SingleAsyncJobExecutor.class);
            when(executor.isNotInProgress()).thenReturn(true);
            selector.setJobExecutor(executor);

            selector.setCapabilityProviderForTests(() -> capabilities(true, false, "proteus_f7", "proteus_f7"));
            selector.apply(hardware, true, false, "proteus_f7", "proteus_f7");
            JButton main = findButton(selector.getControl(), "Update ECU firmware");
            assertNotNull(main);
            assertFalse(main.isEnabled(), "startup selection is not a programming target before connection");
            selector.triggerUpdateFirmware();
            verify(executor, never()).startJob(any(), any());

            LinkManager linkManager = mock(LinkManager.class);
            when(linkManager.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class));
            when(linkManager.getLastTriedPort()).thenReturn(ecu.port);
            selector.setLinkManager(linkManager);
            JPopupMenu menu = selector.apply(hardware, true, false, "proteus_f7", "proteus_f7");
            assertTrue(main.isEnabled());
            assertNotNull(menuItem(menu, UpdateMode.DFU_AUTO.displayText));
            assertNotNull(menuItem(menu, UpdateMode.DFU_SWITCH.displayText));
            selector.setNormalUpdateConfirmationForTests(message -> {
                assertTrue(message.contains("ECU board: proteus_f7 (reported by connected ECU)"), message);
                assertTrue(message.contains("Firmware channel:"), message);
                assertTrue(message.contains("engine stopped"), message);
                assertTrue(message.contains("stable power"), message);
                return true;
            });

            selector.triggerUpdateFirmware();

            verify(executor).startJob(isA(DfuAutoJob.class), any(Component.class));
        });
    }

    @Test
    void ltsInterceptorRunsOnlyForTheValidatedSelectedLiveEcu() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult ecu = new PortResult("ECU", SerialPortType.Ecu);
            AvailableHardware hardware = new AvailableHardware(Collections.singletonList(ecu), false, false, false);
            ConnectivityContext context = mock(ConnectivityContext.class);
            when(context.getCurrentHardware()).thenReturn(hardware);
            when(context.getConnectedEcuTarget()).thenReturn(ConnectedEcuTarget.forManualRecovery("proteus_f7"));
            ProgramSelector selector = new ProgramSelector(context, new JComboBox<>(new PortResult[]{ecu}),
                ignored -> {}, () -> {});
            SingleAsyncJobExecutor executor = mock(SingleAsyncJobExecutor.class);
            when(executor.isNotInProgress()).thenReturn(true);
            selector.setJobExecutor(executor);
            LinkManager linkManager = mock(LinkManager.class);
            when(linkManager.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class));
            when(linkManager.getLastTriedPort()).thenReturn("OTHER");
            selector.setLinkManager(linkManager);
            selector.setCapabilityProviderForTests(() -> capabilities(true, false, "proteus_f7", "proteus_f7"));
            selector.apply(hardware, true, false, "proteus_f7", "proteus_f7");
            java.util.concurrent.atomic.AtomicBoolean intercepted = new java.util.concurrent.atomic.AtomicBoolean();
            selector.setFirmwareUpdateInterceptor(source -> {
                intercepted.set(true);
                return true;
            });
            selector.setNormalUpdateConfirmationForTests(message -> true);

            selector.triggerUpdateFirmware();
            assertFalse(intercepted.get(), "mismatched connection identity fails before the interceptor");
            verify(executor, never()).startJob(any(), any());

            when(linkManager.getLastTriedPort()).thenReturn(ecu.port);
            selector.apply(hardware, true, false, "proteus_f7", "proteus_f7");
            selector.triggerUpdateFirmware();
            assertTrue(intercepted.get());
            verify(executor, never()).startJob(any(), any());
        });
    }

    @Test
    void recoveryRequiresAnIdentifiedUnambiguousCurrentDfuAndRevalidatesAfterConfirmation() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult first = new PortResult("DFU_1", SerialPortType.Dfu);
            PortResult second = new PortResult("DFU_2", SerialPortType.Dfu);
            AvailableHardware multiple = new AvailableHardware(Arrays.asList(first, second), true, false, false);
            ConnectivityContext context = mock(ConnectivityContext.class);
            when(context.getCurrentHardware()).thenReturn(multiple);
            when(context.getConnectedEcuTarget()).thenReturn(ConnectedEcuTarget.forManualRecovery("proteus_f7"));
            JComboBox<PortResult> ports = new JComboBox<>(new PortResult[]{first, second});
            ports.setSelectedItem(null);
            ProgramSelector selector = new ProgramSelector(context, ports, ignored -> {}, () -> {});
            SingleAsyncJobExecutor executor = mock(SingleAsyncJobExecutor.class);
            when(executor.isNotInProgress()).thenReturn(true);
            selector.setJobExecutor(executor);
            selector.apply(multiple, true, false, "universal", "proteus_f7");
            JButton recover = findButton(selector.getControl(), "Update ECU firmware");
            assertNotNull(recover);
            assertFalse(recover.isEnabled(), "multiple DFU entries do not identify an unambiguous default");

            ports.setSelectedItem(first);
            selector.apply(multiple, true, false, "universal", "proteus_f7");
            assertEquals("Recover ECU", recover.getText());
            assertFalse(recover.isEnabled(), "selection cannot bind the global DFU backend to one device");
            selector.setCapabilityProviderForTests(() -> capabilities(true, false, "universal", "proteus_f7"));
            AvailableHardware single = new AvailableHardware(Collections.singletonList(first), true, false, false);
            when(context.getCurrentHardware()).thenReturn(single);
            selector.apply(single, true, false, "universal", "proteus_f7");
            assertTrue(recover.isEnabled());
            selector.setDfuRecoveryConfirmationForTests(message -> {
                assertTrue(message.contains("cannot identify the board or choose between connected ECUs"), message);
                assertTrue(message.contains("may erase firmware and settings"), message);
                assertTrue(message.contains("connect only the ECU you want to recover"), message);
                when(context.getCurrentHardware()).thenReturn(
                    new AvailableHardware(Collections.singletonList(second), true, false, false));
                return true;
            });

            recover.doClick();

            verify(executor, never()).startJob(any(), any());
        });
    }

    @Test
    void recoveryPrimaryEligibilityRespectsPlatformAndBundleGatesAndRequiresAListedDevice() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult dfu = new PortResult("DFU", SerialPortType.Dfu);
            AvailableHardware hardware = new AvailableHardware(Collections.singletonList(dfu), true, false, false);
            ConnectivityContext context = mock(ConnectivityContext.class);
            when(context.getCurrentHardware()).thenReturn(hardware);
            when(context.getConnectedEcuTarget()).thenReturn(ConnectedEcuTarget.forManualRecovery("proteus_f7"));
            ProgramSelector selector = new ProgramSelector(context, new JComboBox<>(new PortResult[]{dfu}),
                ignored -> {}, () -> {});
            JButton recover = findButton(selector.getControl(), "Recover ECU");
            assertNotNull(recover);

            selector.apply(hardware, false, false, "universal", "proteus_f7");
            assertFalse(recover.isEnabled(), "platform capability gates recovery");
            selector.apply(hardware, true, true, "proteus_f7", "uaefi");
            assertFalse(recover.isEnabled(), "foreign non-universal bundles do not offer manual DFU");
            selector.apply(new AvailableHardware(Collections.emptyList(), true, false, false),
                true, false, "universal", "proteus_f7");
            assertFalse(recover.isEnabled(), "the DFU-found flag alone does not identify a device");
            assertEquals("Update ECU firmware", recover.getText(), "a stale selection is not a recovery default");
        });
    }

    @Test
    void unselectedSingleDfuOnlyDetectionOffersConfirmedRecoveryOnThePrimaryButton() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Harness h = new Harness(new PortResult("DFU", SerialPortType.Dfu));
            h.ports.setSelectedItem(null);
            h.selector.apply(h.hardware.get());
            JButton primary = findButton(h.selector.getControl(), "Recover ECU");
            assertNotNull(primary);
            assertTrue(primary.isEnabled());
            assertEquals(1, countFirmwareActionButtons(h.selector.getControl()));
            AtomicBoolean confirmed = new AtomicBoolean();
            h.selector.setDfuRecoveryConfirmationForTests(message -> {
                confirmed.set(true);
                return true;
            });

            primary.doClick();

            assertTrue(confirmed.get());
            verify(h.executor).startJob(isA(DfuManualJob.class), any(Component.class));
        });
    }

    @Test
    void independentDfuBesideSelectedLiveOrBootloaderEcuDoesNotReplaceTheUpdateDefault() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (SerialPortType type : Arrays.asList(SerialPortType.Ecu, SerialPortType.OpenBlt)) {
                Harness h = new Harness(new PortResult("ECU", type), new PortResult("DFU", SerialPortType.Dfu));
                JPopupMenu menu = h.selector.apply(h.hardware.get(), true, false, "proteus_f7", "proteus_f7");
                JButton primary = findButton(h.selector.getControl(), "Update ECU firmware");
                assertNotNull(primary);
                assertTrue(primary.isEnabled());
                assertNull(findButton(h.selector.getControl(), "Recover ECU"));
                assertEquals(1, countFirmwareActionButtons(h.selector.getControl()));
                h.selector.setDfuRecoveryConfirmationForTests(message -> fail("the default must not recover another device"));

                primary.doClick();

                if (type == SerialPortType.Ecu) {
                    verify(h.executor).startJob(isA(DfuAutoJob.class), any(Component.class));
                } else {
                    verify(h.executor).startJob(isA(OpenBltManualJob.class), any(Component.class));
                }
                // Recovery remains possible as an explicit advanced choice, still requiring confirmation.
                AtomicBoolean confirmed = new AtomicBoolean();
                h.selector.setDfuRecoveryConfirmationForTests(message -> {
                    confirmed.set(true);
                    return true;
                });
                menuItem(menu, UpdateMode.DFU_MANUAL.displayText).doClick();
                assertTrue(confirmed.get());
                verify(h.executor).startJob(isA(DfuManualJob.class), any(Component.class));
            }
        });
    }

    @Test
    void unknownStaleOrMixedUnselectedPortsNeverFallBackToImplicitRecovery() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult dfu = new PortResult("DFU", SerialPortType.Dfu);
            for (SerialPortType staleType : Arrays.asList(SerialPortType.Ecu, SerialPortType.EcuUnknown,
                SerialPortType.Dfu)) {
                Harness h = new Harness(new PortResult("STALE", staleType));
                h.hardware.set(new AvailableHardware(Collections.singletonList(dfu), true, false, false));
                h.selector.apply(h.hardware.get());
                JButton primary = findButton(h.selector.getControl(), "Update ECU firmware");
                assertNotNull(primary);
                assertFalse(primary.isEnabled());
                assertNull(findButton(h.selector.getControl(), "Recover ECU"));
                verify(h.executor, never()).startJob(any(), any());
            }
            Harness unknown = new Harness(new PortResult("UNKNOWN", SerialPortType.EcuUnknown), dfu);
            unknown.selector.apply(unknown.hardware.get());
            assertFalse(findButton(unknown.selector.getControl(), "Update ECU firmware").isEnabled());
            unknown.ports.setSelectedItem(null);
            unknown.selector.apply(unknown.hardware.get());
            assertFalse(findButton(unknown.selector.getControl(), "Update ECU firmware").isEnabled(),
                "an unselected DFU beside another device is not a DFU-only default");
            assertNull(findButton(unknown.selector.getControl(), "Recover ECU"));
        });
    }

    @Test
    void primaryRecoveryCancelsIfSelectionChangesToAnUpdateTargetDuringConfirmation() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult dfu = new PortResult("DFU", SerialPortType.Dfu);
            PortResult bootloader = new PortResult("BLT", SerialPortType.OpenBlt);
            Harness h = new Harness(dfu, bootloader);
            h.selector.apply(h.hardware.get());
            JButton primary = findButton(h.selector.getControl(), "Recover ECU");
            assertNotNull(primary);
            h.selector.setDfuRecoveryConfirmationForTests(message -> {
                h.ports.setSelectedItem(bootloader);
                return true;
            });

            primary.doClick();

            assertEquals("Update ECU firmware", primary.getText());
            verify(h.executor, never()).startJob(any(), any());
        });
    }

    @Test
    void staleRecoveryLabelCannotDispatchWhenItsDetectedDeviceHasGone() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Harness h = new Harness(new PortResult("DFU", SerialPortType.Dfu));
            h.selector.apply(h.hardware.get());
            JButton primary = findButton(h.selector.getControl(), "Recover ECU");
            assertNotNull(primary);
            h.hardware.set(new AvailableHardware(Collections.emptyList(), false, false, false));
            h.selector.setDfuRecoveryConfirmationForTests(message -> fail("a vanished device must not offer recovery"));

            primary.doClick();

            assertEquals("Update ECU firmware", primary.getText());
            assertFalse(primary.isEnabled());
            verify(h.executor, never()).startJob(any(), any());
        });
    }

    @Test
    void advancedSerialDfuActionsRejectEveryCanTransportEvenWithAnotherSerialEcu() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (String transport : Arrays.asList(LinkManager.PCAN, LinkManager.SOCKET_CAN, "SLCAN:COM7")) {
                for (SerialPortType type : Arrays.asList(SerialPortType.Ecu, SerialPortType.EcuWithOpenblt)) {
                    PortResult can = new PortResult(transport, type);
                    Harness h = new Harness(can, new PortResult("COM8", SerialPortType.Ecu));
                    JPopupMenu menu = h.selector.apply(h.hardware.get(), true, false, "proteus_f7", "proteus_f7");
                    JMenu advanced = (JMenu) menu.getComponent(0);
                    List<String> labels = Arrays.stream(advanced.getMenuComponents())
                        .filter(JMenuItem.class::isInstance)
                        .map(component -> ((JMenuItem) component).getText())
                        .collect(java.util.stream.Collectors.toList());
                    assertFalse(labels.contains(UpdateMode.DFU_AUTO.displayText));
                    assertFalse(labels.contains(UpdateMode.DFU_SWITCH.displayText));
                    verify(h.executor, never()).startJob(any(), any());
                }
            }
        });
    }

    @Test
    void openBltOnlySelectionHidesIneligibleAutomaticDfuActions() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Harness h = new Harness(bootloader("proteus_f7"));
            JPopupMenu menu = h.selector.apply(h.hardware.get(), true, false, "proteus_f7", "proteus_f7");
            List<String> labels = Arrays.stream(((JMenu) menu.getComponent(0)).getMenuComponents())
                .filter(JMenuItem.class::isInstance)
                .map(component -> ((JMenuItem) component).getText())
                .collect(java.util.stream.Collectors.toList());

            assertFalse(labels.contains(UpdateMode.DFU_AUTO.displayText), labels.toString());
            assertFalse(labels.contains(UpdateMode.DFU_SWITCH.displayText), labels.toString());
        });
    }

    @Test
    void dfuSwitchCannotUseLiveEcuAWhenEcuBIsSelected() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult a = new PortResult("ECU_A", SerialPortType.Ecu);
            PortResult b = new PortResult("ECU_B", SerialPortType.Ecu);
            Harness h = new Harness(a, b);
            h.ports.setSelectedItem(b);
            when(h.linkManager.getLastTriedPort()).thenReturn(b.port);
            JPopupMenu menu = h.selector.apply(h.hardware.get(), true, false, "proteus_f7", "proteus_f7");
            when(h.linkManager.getLastTriedPort()).thenReturn(a.port);
            menuItem(menu, UpdateMode.DFU_SWITCH.displayText).doClick();
            verify(h.executor, never()).startJob(any(), any());
            when(h.linkManager.getLastTriedPort()).thenReturn(b.port);
            menuItem(menu, UpdateMode.DFU_SWITCH.displayText).doClick();
            verify(h.executor).startJob(isA(DfuSwitchJob.class), any(Component.class));
        });
    }

    @Test
    void targetedInterceptorReceivesTheConfirmedCanonicalSelectedPort() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult a = new PortResult("ECU_A", SerialPortType.Ecu);
            PortResult selectedB = new PortResult("ECU_B", SerialPortType.Ecu);
            PortResult currentB = new PortResult("ECU_B", SerialPortType.Ecu);
            Harness h = new Harness(a, selectedB);
            h.ports.setSelectedItem(selectedB);
            when(h.linkManager.getLastTriedPort()).thenReturn(selectedB.port);
            h.hardware.set(new AvailableHardware(Arrays.asList(a, currentB), false, false, false));
            AtomicReference<PortResult> intercepted = new AtomicReference<>();
            h.selector.setTargetedFirmwareUpdateInterceptor((source, expectedPort) -> {
                intercepted.set(expectedPort);
                return true;
            });
            h.selector.triggerUpdateFirmware();
            assertSame(currentB, intercepted.get());
            verify(h.executor, never()).startJob(any(), any());
        });
    }

    @Test
    void pcanNormalUpdateDoesNotEnterTheSerialLtsInterceptor() throws Exception {
        String previous = System.getProperty(UiProperties.SHOW_CAN_OPENBLT);
        System.setProperty(UiProperties.SHOW_CAN_OPENBLT, "true");
        try {
            SwingUtilities.invokeAndWait(() -> {
                Harness h = new Harness(new PortResult(LinkManager.PCAN, SerialPortType.Ecu));
                AtomicBoolean intercepted = new AtomicBoolean();
                h.selector.setTargetedFirmwareUpdateInterceptor((source, expectedPort) -> {
                    intercepted.set(true);
                    return true;
                });
                h.selector.triggerUpdateFirmware();
                assertFalse(intercepted.get());
                verify(h.executor).startJob(isA(OpenBltAutoJob.class), any(Component.class));
            });
        } finally {
            if (previous == null) {
                System.clearProperty(UiProperties.SHOW_CAN_OPENBLT);
            } else {
                System.setProperty(UiProperties.SHOW_CAN_OPENBLT, previous);
            }
        }
    }

    @Test
    void normalInvocationUsesFreshCapabilitiesNotRenderedEligibility() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Harness h = new Harness(new PortResult("ECU", SerialPortType.Ecu));
            h.selector.apply(h.hardware.get(), true, false, "proteus_f7", "proteus_f7");
            h.capabilities.set(capabilities(false, false, "proteus_f7", "proteus_f7"));
            h.selector.setNormalUpdateConfirmationForTests(message -> fail("stale render must not authorize confirmation"));
            h.selector.triggerUpdateFirmware();
            verify(h.executor, never()).startJob(any(), any());
        });
    }

    @Test
    void normalUpdateRechecksCapabilityAndTargetChangesDuringConfirmation() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (ProgramSelector.ProgrammingCapabilities changed : invalidatedDfuCapabilities()) {
                Harness h = new Harness(new PortResult("ECU", SerialPortType.Ecu));
                h.selector.setNormalUpdateConfirmationForTests(message -> {
                    h.capabilities.set(changed);
                    return true;
                });
                h.selector.triggerUpdateFirmware();
                verify(h.executor, never()).startJob(any(), any());
            }
            // Even an OpenBLT update, whose mode is unchanged, must not use a different confirmed board target.
            Harness h = new Harness(new PortResult("ECU", SerialPortType.EcuWithOpenblt));
            h.selector.setNormalUpdateConfirmationForTests(message -> {
                h.capabilities.set(capabilities(true, false, "proteus_f7", "uaefi"));
                return true;
            });
            h.selector.triggerUpdateFirmware();
            verify(h.executor, never()).startJob(any(), any());
        });
    }

    @Test
    void recoveryUsesFreshCapabilitiesBeforeAndAfterConfirmation() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (ProgramSelector.ProgrammingCapabilities changed : invalidatedDfuCapabilities()) {
                Harness h = new Harness(new PortResult("DFU", SerialPortType.Dfu));
                h.selector.apply(h.hardware.get(), true, false, "proteus_f7", "proteus_f7");
                h.selector.setDfuRecoveryConfirmationForTests(message -> {
                    h.capabilities.set(changed);
                    return true;
                });
                findButton(h.selector.getControl(), "Recover ECU").doClick();
                verify(h.executor, never()).startJob(any(), any());
            }
            Harness h = new Harness(new PortResult("DFU", SerialPortType.Dfu));
            h.selector.apply(h.hardware.get(), true, false, "proteus_f7", "proteus_f7");
            h.capabilities.set(capabilities(false, false, "proteus_f7", "proteus_f7"));
            h.selector.setDfuRecoveryConfirmationForTests(message -> fail("stale render must not authorize recovery"));
            findButton(h.selector.getControl(), "Recover ECU").doClick();
            verify(h.executor, never()).startJob(any(), any());
        });
    }

    @Test
    void customDfuRechecksCapabilitiesAfterFinalConfirmation(@TempDir Path directory) throws Exception {
        Path firmware = Files.createFile(directory.resolve("custom.bin"));
        SwingUtilities.invokeAndWait(() -> {
            for (ProgramSelector.ProgrammingCapabilities changed : invalidatedDfuCapabilities()) {
                Harness h = new Harness(new PortResult("DFU", SerialPortType.Dfu));
                h.selector.setDfuRecoveryConfirmationForTests(message -> {
                    h.capabilities.set(changed);
                    return true;
                });
                h.selector.startCustomDfuRecovery(firmware.toFile());
                verify(h.executor, never()).startJob(any(), any());
            }
            Harness valid = new Harness(new PortResult("DFU", SerialPortType.Dfu));
            valid.selector.setDfuRecoveryConfirmationForTests(message -> true);
            valid.selector.startCustomDfuRecovery(firmware.toFile());
            verify(valid.executor).startJob(isA(DfuManualJob.class), any(Component.class));
        });
    }

    @Test
    void allGlobalDfuActionsRejectMultipleEntriesDespiteExplicitSelection(@TempDir Path directory) throws Exception {
        Path firmware = Files.createFile(directory.resolve("custom.bin"));
        SwingUtilities.invokeAndWait(() -> {
            PortResult first = new PortResult("DFU_1", SerialPortType.Dfu);
            PortResult second = new PortResult("DFU_2", SerialPortType.Dfu);
            Harness h = new Harness(first);
            JPopupMenu staleMenu = h.selector.apply(h.hardware.get(), true, false, "proteus_f7", "proteus_f7");
            h.hardware.set(new AvailableHardware(Arrays.asList(first, second), true, false, false));
            h.selector.setDfuRecoveryConfirmationForTests(message -> fail("multiple DFU entries must fail closed"));
            findButton(h.selector.getControl(), "Recover ECU").doClick();
            menuItem(staleMenu, UpdateMode.DFU_MANUAL.displayText).doClick();
            menuItem(staleMenu, UpdateMode.DFU_ERASE.displayText).doClick();
            h.selector.startCustomDfuRecovery(firmware.toFile());
            verify(h.executor, never()).startJob(any(), any());
        });
    }

    @Test
    void stLinkRechecksBltRestrictionButNotDfuPlatformSupport() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Harness permitted = new Harness(new PortResult("ECU", SerialPortType.Ecu));
            permitted.hardware.set(new AvailableHardware(Collections.emptyList(), false, true, false));
            permitted.capabilities.set(capabilities(false, false, "proteus_f7", "proteus_f7"));
            JPopupMenu menu = permitted.selector.apply(permitted.hardware.get(), false, false, "proteus_f7", "proteus_f7");
            menuItem(menu, UpdateMode.ST_LINK.displayText).doClick();
            verify(permitted.executor).startJob(isA(StLinkJob.class), any(Component.class));
            for (ProgramSelector.ProgrammingCapabilities changed : Arrays.asList(
                capabilities(false, true, "proteus_f7", "proteus_f7"),
                capabilities(false, false, "proteus_f7", "uaefi"))) {
                Harness denied = new Harness(new PortResult("ECU", SerialPortType.Ecu));
                denied.hardware.set(new AvailableHardware(Collections.emptyList(), false, true, false));
                JPopupMenu staleMenu = denied.selector.apply(denied.hardware.get(), false, false, "proteus_f7", "proteus_f7");
                denied.capabilities.set(changed);
                menuItem(staleMenu, UpdateMode.ST_LINK.displayText).doClick();
                verify(denied.executor, never()).startJob(any(), any());
            }
        });
    }

    @Test
    void bootloaderConfirmationUsesCurrentBoardMetadataAndCancelsSamePortBoardReplacement() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult stale = bootloader("proteus_f7");
            PortResult current = bootloader("uaefi");
            Harness h = new Harness(stale);
            h.hardware.set(new AvailableHardware(Collections.singletonList(current), false, false, false));
            assertSame(current, ProgramSelector.resolveNormalFlashPort(stale, false, null,
                h.hardware.get().getKnownPorts(), false));
            h.selector.setNormalUpdateConfirmationForTests(message -> {
                assertTrue(message.contains("uaefi (reported by selected bootloader)"), message);
                assertFalse(message.contains("proteus_f7"), message);
                h.hardware.set(new AvailableHardware(Collections.singletonList(bootloader("proteus_f7")), false, false, false));
                return true;
            });
            h.selector.triggerUpdateFirmware();
            verify(h.executor, never()).startJob(any(), any());
        });
    }

    @Test
    void normalDispatchUsesLatestEquivalentCanonicalPortObject() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult old = bootloader("uaefi");
            PortResult replacement = bootloader("uaefi");
            Harness h = new Harness(old);
            AtomicReference<PortResult> dispatched = new AtomicReference<>();
            ProgramSelector selector = new ProgramSelector(h.context, h.ports, ignored -> {}, () -> {}) {
                @Override
                void executeJob(JComponent parent, UpdateMode mode, PortResult selected) {
                    assertEquals(UpdateMode.OPENBLT_MANUAL, mode);
                    dispatched.set(selected);
                }
            };
            selector.setCapabilityProviderForTests(h.capabilities::get);
            selector.setNormalUpdateConfirmationForTests(message -> {
                h.hardware.set(new AvailableHardware(Collections.singletonList(replacement), false, false, false));
                return true;
            });
            selector.triggerUpdateFirmware();
            assertSame(replacement, dispatched.get());
        });
    }

    @Test
    void advancedManualOpenBltDispatchesCurrentBoardMetadataRatherThanStaleComboObject() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult stale = bootloader("proteus_f7");
            PortResult current = bootloader("uaefi");
            Harness h = new Harness(stale);
            AtomicReference<PortResult> dispatched = new AtomicReference<>();
            ProgramSelector selector = new ProgramSelector(h.context, h.ports, ignored -> {}, () -> {}) {
                @Override
                void executeJob(JComponent parent, UpdateMode mode, PortResult selected) {
                    assertEquals(UpdateMode.OPENBLT_MANUAL, mode);
                    dispatched.set(selected);
                }
            };
            selector.setCapabilityProviderForTests(h.capabilities::get);
            JPopupMenu menu = selector.apply(h.hardware.get(), true, false, "proteus_f7", "proteus_f7");
            h.hardware.set(new AvailableHardware(Collections.singletonList(current), false, false, false));

            menuItem(menu, UpdateMode.OPENBLT_MANUAL.displayText).doClick();

            assertSame(stale, h.ports.getSelectedItem(), "the combo still holds the previous detection object");
            assertSame(current, dispatched.get(), "the job must use the current board metadata at this port");
        });
    }

    @Test
    void wipeConfirmationRejectsSamePortBoardReplacementAndAcceptsUnchangedIdentity(@TempDir Path directory)
        throws Exception {
        writeWipeArtifact(directory);
        String previousInputPath = FindFileHelper.INPUT_FILES_PATH;
        FindFileHelper.INPUT_FILES_PATH = directory.toString();
        try {
            SwingUtilities.invokeAndWait(() -> {
                for (String replacementBoard : Arrays.asList("proteus_f7", "uaefi")) {
                    PortResult confirmed = bootloader("uaefi");
                    PortResult replacement = bootloader(replacementBoard);
                    Harness h = new Harness(confirmed);
                    AtomicReference<JComponent> confirmation = new AtomicReference<>();
                    ProgramSelector selector = new ProgramSelector(h.context, h.ports, confirmation::set, () -> {});
                    selector.setCapabilityProviderForTests(h.capabilities::get);
                    selector.setJobExecutor(h.executor);
                    JPopupMenu menu = selector.apply(h.hardware.get(), true, false, "proteus_f7", "proteus_f7");
                    menuItem(menu, UpdateMode.OPENBLT_EMERGENCY_WIPE.displayText).doClick();
                    assertNotNull(confirmation.get(), "the real artifact must pass validation and show confirmation");

                    h.hardware.set(new AvailableHardware(Collections.singletonList(replacement), false, false, false));
                    JButton erase = findButton(confirmation.get(), "Erase Internal Flash");
                    assertNotNull(erase);
                    erase.doClick();

                    assertSame(confirmed, h.ports.getSelectedItem(), "selection equality must not hide metadata changes");
                    if (replacementBoard.equals("uaefi")) {
                        verify(h.executor).startJob(isA(OpenBltManualJob.class), any(Component.class));
                    } else {
                        verify(h.executor, never()).startJob(any(), any());
                    }
                }
            });
        } finally {
            FindFileHelper.INPUT_FILES_PATH = previousInputPath;
        }
    }

    private static void writeWipeArtifact(Path directory) throws IOException {
        Path wipeDirectory = Files.createDirectories(directory.resolve("bin/wipe"));
        String name = "rusefi_development_test_uaefi_wipe.srec";
        Files.write(wipeDirectory.resolve("openblt_wipe.properties"), (
            "format=1\nbundleTarget=uaefi\nbootloaderTarget=uaefi\nmcuFamily=ARCH_STM32F4\n"
                + "startAddress=0x08008000\nendAddressExclusive=0x08100000\nsrec=" + name + "\n")
            .getBytes(StandardCharsets.US_ASCII));
        try (BufferedWriter writer = Files.newBufferedWriter(wipeDirectory.resolve(name), StandardCharsets.US_ASCII)) {
            writer.write("S0030000FC\n");
            for (long address = 0x08008000L; address < 0x08100000L; address += 240) {
                int size = (int) Math.min(240, 0x08100000L - address);
                int sum = size + 5 + size * 255;
                for (int shift = 0; shift < 32; shift += 8) {
                    sum += (address >> shift) & 255;
                }
                StringBuilder record = new StringBuilder(String.format("S3%02X%08X", size + 5, address));
                for (int i = 0; i < size; i++) {
                    record.append("FF");
                }
                writer.write(record.append(String.format("%02X%n", (~sum) & 255)).toString());
            }
            writer.write("S7050800800072\n");
        }
    }

    @Test
    void fullScreenRecoveryWaitsForConsentAndClosesBeforeJobCanOpenBoardPicker() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicReference<JComponent> shown = new AtomicReference<>();
            List<String> events = new ArrayList<>();
            Harness h = new Harness(panel -> {
                events.add("confirmation");
                shown.set(panel);
            }, () -> events.add("close"), new PortResult("DFU", SerialPortType.Dfu));
            DfuBoardSelectionPanel boardPicker = new DfuBoardSelectionPanel(ignored -> {});
            doAnswer(invocation -> {
                events.add("job");
                shown.set(boardPicker);
                events.add("board picker");
                return true;
            }).when(h.executor).startJob(any(AsyncJob.class), any(Component.class));

            openRecoveryPrimary(h);
            assertTrue(shown.get() instanceof DfuRecoveryConfirmationPanel);
            assertEquals(Collections.singletonList("confirmation"), events);
            verify(h.executor, never()).startJob(any(), any());

            JButton confirm = findButton(shown.get(), "Recover ECU");
            assertNotNull(confirm);
            confirm.doClick();
            confirm.doClick();

            assertEquals(Arrays.asList("confirmation", "close", "job", "board picker"), events);
            assertSame(boardPicker, shown.get(), "the confirmation must not close a board picker opened by its job");
            verify(h.executor).startJob(isA(DfuManualJob.class), any(Component.class));
        });
    }

    @Test
    void fullScreenCancelAndEscapeCloseOnceAndNeverProgramEvenWithStaleConfirm() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (boolean escape : new boolean[]{false, true}) {
                AtomicReference<JComponent> shown = new AtomicReference<>();
                AtomicInteger closes = new AtomicInteger();
                Harness h = new Harness(shown::set, closes::incrementAndGet, new PortResult("DFU", SerialPortType.Dfu));
                openRecoveryPrimary(h);
                assertTrue(shown.get() instanceof DfuRecoveryConfirmationPanel);
                JButton confirm = findButton(shown.get(), "Recover ECU");
                assertNotNull(confirm);

                if (escape) {
                    pressEscape(shown.get());
                } else {
                    JButton cancel = findButton(shown.get(), "Cancel");
                    assertNotNull(cancel);
                    cancel.doClick();
                }
                assertFalse(confirm.isEnabled());
                confirm.setEnabled(true);
                confirm.doClick();
                pressEscape(shown.get());

                assertEquals(1, closes.get());
                verify(h.executor, never()).startJob(any(), any());
            }
        });
    }

    @Test
    void fullScreenConfirmationRechecksMissingMultipleAndReplacedDfuDevices() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult dfu = new PortResult("DFU", SerialPortType.Dfu);
            PortResult replacement = new PortResult("OTHER_DFU", SerialPortType.Dfu);
            for (AvailableHardware changed : Arrays.asList(
                new AvailableHardware(Collections.emptyList(), false, false, false),
                new AvailableHardware(Arrays.asList(dfu, replacement), true, false, false),
                new AvailableHardware(Collections.singletonList(replacement), true, false, false))) {
                AtomicReference<JComponent> shown = new AtomicReference<>();
                AtomicInteger closes = new AtomicInteger();
                Harness h = new Harness(shown::set, closes::incrementAndGet, dfu);
                openRecoveryPrimary(h);
                verify(h.executor, never()).startJob(any(), any());
                h.hardware.set(changed);

                findButton(shown.get(), "Recover ECU").doClick();

                assertEquals(1, closes.get());
                verify(h.executor, never()).startJob(any(), any());
            }
        });
    }

    @Test
    void fullScreenConfirmationRechecksCapabilityAndFirmwareTargetChanges() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (ProgramSelector.ProgrammingCapabilities changed : invalidatedDfuCapabilities()) {
                AtomicReference<JComponent> shown = new AtomicReference<>();
                AtomicInteger closes = new AtomicInteger();
                Harness h = new Harness(shown::set, closes::incrementAndGet, new PortResult("DFU", SerialPortType.Dfu));
                openRecoveryPrimary(h);
                h.capabilities.set(changed);

                findButton(shown.get(), "Recover ECU").doClick();

                assertEquals(1, closes.get());
                verify(h.executor, never()).startJob(any(), any());
            }
        });
    }

    @Test
    void fullScreenConfirmationRechecksPrimarySelectionAndBusyState() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<Consumer<Harness>> changes = Arrays.asList(
                h -> h.ports.setSelectedIndex(1),
                h -> when(h.executor.isNotInProgress()).thenReturn(false),
                h -> h.selector.setExternalBusySupplier(() -> true));
            for (Consumer<Harness> change : changes) {
                AtomicReference<JComponent> shown = new AtomicReference<>();
                AtomicInteger closes = new AtomicInteger();
                Harness h = new Harness(shown::set, closes::incrementAndGet,
                    new PortResult("DFU", SerialPortType.Dfu), new PortResult("BLT", SerialPortType.OpenBlt));
                openRecoveryPrimary(h);
                change.accept(h);

                findButton(shown.get(), "Recover ECU").doClick();

                assertEquals(1, closes.get());
                verify(h.executor, never()).startJob(any(), any());
            }
        });
    }

    @Test
    void customFirmwareRecoveryUsesTheSameAsyncPanelAndFreshValidation(@TempDir Path directory) throws Exception {
        Path firmware = Files.createFile(directory.resolve("custom.bin"));
        SwingUtilities.invokeAndWait(() -> {
            for (boolean invalidate : new boolean[]{false, true}) {
                AtomicReference<JComponent> shown = new AtomicReference<>();
                AtomicInteger closes = new AtomicInteger();
                Harness h = new Harness(shown::set, closes::incrementAndGet, new PortResult("DFU", SerialPortType.Dfu));

                h.selector.startCustomDfuRecovery(firmware.toFile());

                assertTrue(shown.get() instanceof DfuRecoveryConfirmationPanel);
                verify(h.executor, never()).startJob(any(), any());
                if (invalidate) {
                    h.capabilities.set(capabilities(true, true, "proteus_f7", "proteus_f7"));
                }
                findButton(shown.get(), "Recover ECU").doClick();
                assertEquals(1, closes.get());
                if (invalidate) {
                    verify(h.executor, never()).startJob(any(), any());
                } else {
                    verify(h.executor).startJob(isA(DfuManualJob.class), any(Component.class));
                }
            }
        });
    }

    @Test
    void supersededFullScreenConfirmationCannotProgramOrCloseTheNewPanel() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicReference<JComponent> shown = new AtomicReference<>();
            AtomicInteger closes = new AtomicInteger();
            Harness h = new Harness(shown::set, closes::incrementAndGet, new PortResult("DFU", SerialPortType.Dfu));
            openRecoveryPrimary(h);
            JComponent oldPanel = shown.get();
            openRecoveryPrimary(h);
            JComponent currentPanel = shown.get();
            assertNotSame(oldPanel, currentPanel);

            findButton(oldPanel, "Recover ECU").doClick();

            assertEquals(0, closes.get());
            assertSame(currentPanel, shown.get());
            verify(h.executor, never()).startJob(any(), any());
            findButton(currentPanel, "Recover ECU").doClick();
            assertEquals(1, closes.get());
            verify(h.executor).startJob(isA(DfuManualJob.class), any(Component.class));
        });
    }

    @Test
    void normalFullScreenConsentClosesBeforeDispatchAndCannotConfirmTwice() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicReference<JComponent> shown = new AtomicReference<>();
            List<String> events = new ArrayList<>();
            Harness h = new Harness(shown::set, () -> events.add("close"), new PortResult("ECU", SerialPortType.Ecu));
            doAnswer(invocation -> {
                events.add("job");
                return true;
            }).when(h.executor).startJob(any(AsyncJob.class), any(Component.class));

            openNormalPrimary(h);

            assertTrue(shown.get() instanceof FirmwareProgrammingPanel);
            String summary = findTextArea(shown.get()).getText();
            assertTrue(summary.contains("ECU board:"), summary);
            assertTrue(summary.contains("Firmware channel:"), summary);
            assertTrue(summary.contains("Selected device: ECU"), summary);
            verify(h.executor, never()).startJob(any(), any());
            assertTrue(events.isEmpty());
            JButton confirm = findButton(shown.get(), "Update ECU firmware");
            assertNotNull(confirm);
            confirm.doClick();
            confirm.setEnabled(true);
            confirm.doClick();
            pressEscape(shown.get());

            assertEquals(Arrays.asList("close", "job"), events);
            verify(h.executor).startJob(isA(DfuAutoJob.class), any(Component.class));
        });
    }

    @Test
    void normalFullScreenClosesBeforeInvokingTheTargetedLtsInterceptor() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicReference<JComponent> shown = new AtomicReference<>();
            List<String> events = new ArrayList<>();
            PortResult ecu = new PortResult("ECU", SerialPortType.Ecu);
            Harness h = new Harness(shown::set, () -> events.add("close"), ecu);
            h.selector.setTargetedFirmwareUpdateInterceptor((source, expected) -> {
                assertSame(ecu, expected);
                events.add("interceptor");
                return true;
            });
            openNormalPrimary(h);
            assertTrue(events.isEmpty());

            findButton(shown.get(), "Update ECU firmware").doClick();

            assertEquals(Arrays.asList("close", "interceptor"), events);
            verify(h.executor, never()).startJob(any(), any());
        });
    }

    @Test
    void normalFullScreenCancelAndEscapeNeverDispatchOrIntercept() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (boolean escape : new boolean[]{false, true}) {
                AtomicReference<JComponent> shown = new AtomicReference<>();
                AtomicInteger closes = new AtomicInteger();
                Harness h = new Harness(shown::set, closes::incrementAndGet, new PortResult("ECU", SerialPortType.Ecu));
                h.selector.setTargetedFirmwareUpdateInterceptor((source, expected) -> fail("cancel must not intercept"));
                openNormalPrimary(h);
                JButton confirm = findButton(shown.get(), "Update ECU firmware");
                assertNotNull(confirm);
                if (escape) {
                    pressEscape(shown.get());
                } else {
                    findButton(shown.get(), "Cancel").doClick();
                }
                confirm.setEnabled(true);
                confirm.doClick();
                pressEscape(shown.get());
                assertEquals(1, closes.get());
                verify(h.executor, never()).startJob(any(), any());
            }
        });
    }

    @Test
    void normalFullScreenRechecksCapabilitiesAndTargetAfterConsent() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (ProgramSelector.ProgrammingCapabilities changed : invalidatedDfuCapabilities()) {
                AtomicReference<JComponent> shown = new AtomicReference<>();
                AtomicInteger closes = new AtomicInteger();
                Harness h = new Harness(shown::set, closes::incrementAndGet, new PortResult("ECU", SerialPortType.Ecu));
                openNormalPrimary(h);
                h.capabilities.set(changed);

                findButton(shown.get(), "Update ECU firmware").doClick();

                assertEquals(1, closes.get());
                verify(h.executor, never()).startJob(any(), any());
            }
        });
    }

    @Test
    void normalFullScreenRechecksSelectionLiveIdentityHardwareAndBusyState() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<Consumer<Harness>> changes = Arrays.asList(
                h -> h.ports.setSelectedIndex(1),
                h -> when(h.linkManager.getLastTriedPort()).thenReturn("OTHER"),
                h -> when(h.linkManager.getBinaryProtocol()).thenReturn(null),
                h -> when(h.executor.isNotInProgress()).thenReturn(false),
                h -> h.selector.setExternalBusySupplier(() -> true),
                h -> h.hardware.set(new AvailableHardware(Collections.emptyList(), false, false, false)),
                h -> h.hardware.set(new AvailableHardware(Collections.singletonList(
                    new PortResult("ECU", SerialPortType.EcuWithOpenblt)), false, false, false)));
            for (Consumer<Harness> change : changes) {
                AtomicReference<JComponent> shown = new AtomicReference<>();
                AtomicInteger closes = new AtomicInteger();
                Harness h = new Harness(shown::set, closes::incrementAndGet,
                    new PortResult("ECU", SerialPortType.Ecu), new PortResult("OTHER", SerialPortType.Ecu));
                openNormalPrimary(h);
                change.accept(h);

                findButton(shown.get(), "Update ECU firmware").doClick();

                assertEquals(1, closes.get());
                verify(h.executor, never()).startJob(any(), any());
            }
        });
    }

    @Test
    void normalFullScreenCancelsSamePortBootloaderBoardMetadataChange() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicReference<JComponent> shown = new AtomicReference<>();
            AtomicInteger closes = new AtomicInteger();
            Harness h = new Harness(shown::set, closes::incrementAndGet, bootloader("uaefi"));
            openNormalPrimary(h);
            assertTrue(findTextArea(shown.get()).getText().contains("uaefi (reported by selected bootloader)"));
            h.hardware.set(new AvailableHardware(Collections.singletonList(bootloader("proteus_f7")), false, false, false));

            findButton(shown.get(), "Update ECU firmware").doClick();

            assertEquals(1, closes.get());
            verify(h.executor, never()).startJob(any(), any());
        });
    }

    @Test
    void normalAndRecoveryConfirmationsCannotActivateOrCloseEachOthersNewerView() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (boolean recoveryFirst : new boolean[]{false, true}) {
                AtomicReference<JComponent> shown = new AtomicReference<>();
                AtomicInteger closes = new AtomicInteger();
                Harness h = new Harness(shown::set, closes::incrementAndGet,
                    new PortResult("ECU", SerialPortType.Ecu), new PortResult("DFU", SerialPortType.Dfu));
                if (recoveryFirst) {
                    h.ports.setSelectedIndex(1);
                    openRecoveryPrimary(h);
                } else {
                    openNormalPrimary(h);
                }
                JComponent old = shown.get();
                h.ports.setSelectedIndex(recoveryFirst ? 0 : 1);
                if (recoveryFirst) {
                    openNormalPrimary(h);
                } else {
                    openRecoveryPrimary(h);
                }
                JComponent current = shown.get();

                findButton(old, recoveryFirst ? "Recover ECU" : "Update ECU firmware").doClick();
                pressEscape(old);

                assertSame(current, shown.get());
                assertEquals(0, closes.get());
                verify(h.executor, never()).startJob(any(), any());
                findButton(current, "Cancel").doClick();
                assertEquals(1, closes.get());
            }
        });
    }

    @Test
    void invalidSelectedCustomFirmwareShowsFullScreenErrorWithoutAJob(@TempDir Path directory) throws Exception {
        Path wrongType = Files.createFile(directory.resolve("firmware.txt"));
        SwingUtilities.invokeAndWait(() -> {
            for (Path invalid : Arrays.asList(wrongType, directory.resolve("missing.bin"))) {
                AtomicReference<JComponent> shown = new AtomicReference<>();
                AtomicInteger closes = new AtomicInteger();
                Harness h = new Harness(shown::set, closes::incrementAndGet, new PortResult("DFU", SerialPortType.Dfu));

                // This is the same selected-file handler used after JFileChooser approval.
                h.selector.startCustomDfuRecovery(invalid.toFile());

                assertTrue(shown.get() instanceof FirmwareProgrammingPanel);
                assertEquals("Please select a .bin firmware file.", findTextArea(shown.get()).getText());
                assertNull(findButton(shown.get(), "Update ECU firmware"));
                findButton(shown.get(), "Back").doClick();
                pressEscape(shown.get());
                assertEquals(1, closes.get());
                verify(h.executor, never()).startJob(any(), any());
            }
        });
    }

    @Test
    void wipeArtifactIOExceptionShowsFullScreenErrorAndNeverQueuesAJob(@TempDir Path directory) throws Exception {
        writeWipeArtifact(directory);
        Files.write(directory.resolve("bin/wipe/rusefi_development_test_uaefi_wipe.srec"),
            "not an S-record\n".getBytes(StandardCharsets.US_ASCII));
        String previous = FindFileHelper.INPUT_FILES_PATH;
        FindFileHelper.INPUT_FILES_PATH = directory.toString();
        try {
            SwingUtilities.invokeAndWait(() -> {
                AtomicReference<JComponent> shown = new AtomicReference<>();
                AtomicInteger closes = new AtomicInteger();
                Harness h = new Harness(shown::set, closes::incrementAndGet, bootloader("uaefi"));
                JPopupMenu menu = h.selector.apply(h.hardware.get(), true, false, "proteus_f7", "proteus_f7");

                menuItem(menu, UpdateMode.OPENBLT_EMERGENCY_WIPE.displayText).doClick();

                assertTrue(shown.get() instanceof FirmwareProgrammingPanel);
                assertTrue(findTextArea(shown.get()).getText().contains("Invalid S-record"));
                assertNull(findButton(shown.get(), "Erase Internal Flash"));
                verify(h.executor, never()).startJob(any(), any());
                pressEscape(shown.get());
                findButton(shown.get(), "Back").doClick();
                assertEquals(1, closes.get());
                verify(h.executor, never()).startJob(any(), any());
            });
        } finally {
            FindFileHelper.INPUT_FILES_PATH = previous;
        }
    }

    @Test
    void errorSupersedesNormalConsentAndItsStaleBackCannotCloseRecovery(@TempDir Path directory) throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicReference<JComponent> shown = new AtomicReference<>();
            AtomicInteger closes = new AtomicInteger();
            Harness h = new Harness(shown::set, closes::incrementAndGet,
                new PortResult("ECU", SerialPortType.Ecu), new PortResult("DFU", SerialPortType.Dfu));
            openNormalPrimary(h);
            JComponent normal = shown.get();
            h.selector.startCustomDfuRecovery(directory.resolve("missing.bin").toFile());
            JComponent error = shown.get();
            findButton(normal, "Update ECU firmware").doClick();
            assertEquals(0, closes.get());
            h.ports.setSelectedIndex(1);
            openRecoveryPrimary(h);
            JComponent recovery = shown.get();

            findButton(error, "Back").doClick();
            pressEscape(error);

            assertSame(recovery, shown.get());
            assertEquals(0, closes.get());
            verify(h.executor, never()).startJob(any(), any());
            findButton(recovery, "Cancel").doClick();
            assertEquals(1, closes.get());
        });
    }

    @Test
    void normalConsentCannotDispatchIfClosingItsViewPresentsANewerError(@TempDir Path directory) throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicReference<JComponent> shown = new AtomicReference<>();
            AtomicReference<Harness> current = new AtomicReference<>();
            AtomicBoolean presentErrorOnClose = new AtomicBoolean(true);
            Harness h = new Harness(shown::set, () -> {
                if (presentErrorOnClose.getAndSet(false)) {
                    current.get().selector.startCustomDfuRecovery(directory.resolve("missing.bin").toFile());
                }
            }, new PortResult("ECU", SerialPortType.Ecu));
            current.set(h);
            openNormalPrimary(h);

            findButton(shown.get(), "Update ECU firmware").doClick();

            assertEquals("Please select a .bin firmware file.", findTextArea(shown.get()).getText());
            assertNotNull(findButton(shown.get(), "Back"));
            verify(h.executor, never()).startJob(any(), any());
        });
    }

    @Test
    void wipeAndErrorCallbacksCannotActOnEachOthersNewerView(@TempDir Path directory) throws Exception {
        writeWipeArtifact(directory);
        String previous = FindFileHelper.INPUT_FILES_PATH;
        FindFileHelper.INPUT_FILES_PATH = directory.toString();
        try {
            SwingUtilities.invokeAndWait(() -> {
                for (boolean errorFirst : new boolean[]{false, true}) {
                    AtomicReference<JComponent> shown = new AtomicReference<>();
                    AtomicInteger closes = new AtomicInteger();
                    Harness h = new Harness(shown::set, closes::incrementAndGet, bootloader("uaefi"));
                    JPopupMenu menu = h.selector.apply(h.hardware.get(), true, false, "proteus_f7", "proteus_f7");
                    if (errorFirst) {
                        h.selector.startCustomDfuRecovery(directory.resolve("missing.bin").toFile());
                    } else {
                        menuItem(menu, UpdateMode.OPENBLT_EMERGENCY_WIPE.displayText).doClick();
                    }
                    JComponent old = shown.get();
                    if (errorFirst) {
                        menuItem(menu, UpdateMode.OPENBLT_EMERGENCY_WIPE.displayText).doClick();
                    } else {
                        h.selector.startCustomDfuRecovery(directory.resolve("missing.bin").toFile());
                    }
                    JComponent current = shown.get();

                    findButton(old, errorFirst ? "Back" : "Erase Internal Flash").doClick();
                    pressEscape(old);

                    assertSame(current, shown.get());
                    assertEquals(0, closes.get());
                    verify(h.executor, never()).startJob(any(), any());
                    findButton(current, errorFirst ? "Cancel" : "Back").doClick();
                    assertEquals(1, closes.get());
                }
            });
        } finally {
            FindFileHelper.INPUT_FILES_PATH = previous;
        }
    }

    private static void openNormalPrimary(Harness h) {
        h.selector.setNormalUpdateConfirmationForTests(null);
        h.selector.apply(h.hardware.get());
        JButton primary = findButton(h.selector.getControl(), "Update ECU firmware");
        assertNotNull(primary);
        assertTrue(primary.isEnabled());
        primary.doClick();
    }

    private static JTextArea findTextArea(Container container) {
        for (Component component : container.getComponents()) {
            if (component instanceof JTextArea) {
                return (JTextArea) component;
            }
            if (component instanceof Container) {
                JTextArea found = findTextArea((Container) component);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static void openRecoveryPrimary(Harness h) {
        h.selector.apply(h.hardware.get());
        JButton primary = findButton(h.selector.getControl(), "Recover ECU");
        assertNotNull(primary);
        assertTrue(primary.isEnabled());
        primary.doClick();
    }

    private static void pressEscape(JComponent panel) {
        Object key = panel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).get(KeyStroke.getKeyStroke("ESCAPE"));
        assertNotNull(key);
        panel.getActionMap().get(key).actionPerformed(new ActionEvent(panel, ActionEvent.ACTION_PERFORMED, "escape"));
    }

    private static PortResult bootloader(String board) {
        return new PortResult("BLT", SerialPortType.OpenBlt, null, new OpenbltInfo(true, "rusEFI." + board));
    }

    private static ProgramSelector.ProgrammingCapabilities capabilities(boolean supported, boolean obfuscated,
                                                                       String bundle, String target) {
        return new ProgramSelector.ProgrammingCapabilities(supported, obfuscated, bundle, target, true);
    }

    private static List<ProgramSelector.ProgrammingCapabilities> invalidatedDfuCapabilities() {
        return Arrays.asList(capabilities(false, false, "proteus_f7", "proteus_f7"),
            capabilities(true, true, "proteus_f7", "proteus_f7"),
            capabilities(true, false, "proteus_f7", "uaefi"),
            capabilities(true, false, "uaefi", "uaefi"));
    }

    private static JMenuItem menuItem(JPopupMenu menu, String text) {
        for (Component component : menu.getComponents()) {
            if (component instanceof JMenu) {
                for (Component child : ((JMenu) component).getMenuComponents()) {
                    if (child instanceof JMenuItem && text.equals(((JMenuItem) child).getText())) {
                        return (JMenuItem) child;
                    }
                }
            }
        }
        return fail("Missing menu item " + text);
    }

    private static final class Harness {
        final ConnectivityContext context = mock(ConnectivityContext.class);
        final AtomicReference<AvailableHardware> hardware;
        final AtomicReference<ProgramSelector.ProgrammingCapabilities> capabilities = new AtomicReference<>(
            capabilities(true, false, "proteus_f7", "proteus_f7"));
        final JComboBox<PortResult> ports;
        final LinkManager linkManager = mock(LinkManager.class);
        final SingleAsyncJobExecutor executor = mock(SingleAsyncJobExecutor.class);
        final ProgramSelector selector;

        Harness(PortResult... detected) {
            this(ignored -> {}, () -> {}, detected);
        }

        Harness(Consumer<JComponent> showPanel, Runnable closePanel, PortResult... detected) {
            hardware = new AtomicReference<>(new AvailableHardware(Arrays.asList(detected),
                Arrays.stream(detected).anyMatch(port -> port.type == SerialPortType.Dfu), false, false));
            ports = new JComboBox<>(detected);
            when(context.getCurrentHardware()).thenAnswer(ignored -> hardware.get());
            when(context.getConnectedEcuTarget()).thenReturn(ConnectedEcuTarget.forManualRecovery("proteus_f7"));
            when(linkManager.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class));
            when(linkManager.getLastTriedPort()).thenReturn(detected[0].port);
            when(executor.isNotInProgress()).thenReturn(true);
            selector = new ProgramSelector(context, ports, showPanel, closePanel);
            selector.setLinkManager(linkManager);
            selector.setJobExecutor(executor);
            selector.setCapabilityProviderForTests(capabilities::get);
            selector.setNormalUpdateConfirmationForTests(message -> true);
        }
    }

    private static int countFirmwareActionButtons(Container container) {
        int count = 0;
        for (Component component : container.getComponents()) {
            if (component instanceof JButton && ("Update ECU firmware".equals(((JButton) component).getText())
                || "Recover ECU".equals(((JButton) component).getText()))) {
                count++;
            }
            if (component instanceof Container) {
                count += countFirmwareActionButtons((Container) component);
            }
        }
        return count;
    }

    private static JButton findButton(Container container, String text) {
        for (Component component : container.getComponents()) {
            if (component instanceof JButton && text.equals(((JButton) component).getText())) {
                return (JButton) component;
            }
            if (component instanceof Container) {
                JButton found = findButton((Container) component, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
