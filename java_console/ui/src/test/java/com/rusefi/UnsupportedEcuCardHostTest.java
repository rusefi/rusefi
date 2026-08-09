package com.rusefi;

import com.rusefi.core.io.UnsupportedEcuInfo;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.io.LinkManager;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.Arrays;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class UnsupportedEcuCardHostTest {
    private static final String PORT = "COM7";

    @Test
    public void unsupportedAlwaysBlocksUntilThatPortIsResolved() throws Exception {
        FakePortScanner scanner = new FakePortScanner();
        LinkManager linkManager = new LinkManager();
        UnsupportedEcuCardHost host = createHost(scanner, linkManager);
        UnsupportedEcuInfo info = new UnsupportedEcuInfo("board-a", "universal");

        scanner.fireHardwareChange(hardware(
            new PortResult("COM1", SerialPortType.Ecu),
            PortResult.unsupportedEcu(PORT, info)));
        flushEdt();
        assertTrue(host.isBlocking(), "an unsupported second ECU must interrupt a supported session");

        host.onCompatibleEcu("COM1", null);
        flushEdt();
        assertTrue(host.isBlocking(), "a compatible identity on another port must not dismiss the modal");

        AtomicReference<PortResult> recovered = new AtomicReference<>();
        host.addCompatiblePortListener(recovered::set);
        host.onCompatibleEcu(PORT, null);
        flushEdt();
        assertFalse(host.isBlocking());
        assertTrue(recovered.get().type == SerialPortType.EcuUnknown);
    }

    @Test
    public void typedWatchdogRejectionSurvivesStaleSupportedSnapshot() throws Exception {
        FakePortScanner scanner = new FakePortScanner();
        UnsupportedEcuCardHost host = createHost(scanner, new LinkManager());

        scanner.fireHardwareChange(hardware(new PortResult(PORT, SerialPortType.Ecu)));
        host.onUnsupportedEcu(PORT, new UnsupportedEcuInfo("board-a", "universal"));
        scanner.fireHardwareChange(hardware(new PortResult(PORT, SerialPortType.Ecu)));
        flushEdt();

        assertTrue(host.isBlocking());
        assertTrue(scanner.invalidatedPorts.contains(PORT));

        scanner.fireHardwareChange(hardware());
        flushEdt();
        assertFalse(host.isBlocking(), "physical removal is the hardware-driven dismissal");
    }

    @Test
    public void modalPreservesNormalContentAndHasOnlyDownloadAction() throws Exception {
        FakePortScanner scanner = new FakePortScanner();
        UnsupportedEcuCardHost host = createHost(scanner, new LinkManager());
        JPanel normal = new JPanel();
        SwingUtilities.invokeAndWait(() -> host.setNormalContent(normal));

        scanner.fireHardwareChange(hardware(new PortResult(PORT, SerialPortType.Ecu)));
        host.onUnsupportedEcu(PORT, new UnsupportedEcuInfo("board-a", "universal"));
        flushEdt();
        host.onCompatibleEcu(PORT, null);
        flushEdt();

        assertSame(normal, host.getNormalContent().getComponent(0));
        assertTrue(containsButton(host.getContent(), "Download compatible bundle"));
        assertFalse(containsButton(host.getContent(), "Dismiss"));
        assertFalse(containsButton(host.getContent(), "Continue"));
    }

    @Test
    public void lateRejectionAfterRemovalDoesNotReopenModal() throws Exception {
        FakePortScanner scanner = new FakePortScanner();
        UnsupportedEcuCardHost host = createHost(scanner, new LinkManager());

        scanner.fireHardwareChange(hardware(new PortResult(PORT, SerialPortType.Ecu)));
        scanner.fireHardwareChange(hardware());
        host.onUnsupportedEcu(PORT, new UnsupportedEcuInfo("board-a", "universal"));
        flushEdt();

        assertFalse(host.isBlocking());
    }

    @Test
    public void differentCompatibleIdentityClearsStaleSessionMetadata() throws Exception {
        FakePortScanner scanner = new FakePortScanner();
        UnsupportedEcuCardHost host = createHost(scanner, new LinkManager());
        host.onCompatibleEcu(PORT, signature("board-a", "hash-a"));
        AtomicReference<PortResult> recovered = new AtomicReference<>();
        host.addCompatiblePortListener(recovered::set);

        host.onCompatibleEcu(PORT, signature("board-a", "hash-b"));

        assertTrue(recovered.get().type == SerialPortType.EcuUnknown);
    }

    private static UnsupportedEcuCardHost createHost(FakePortScanner scanner, LinkManager linkManager)
        throws Exception {
        AtomicReference<UnsupportedEcuCardHost> result = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> result.set(
            new UnsupportedEcuCardHost(new ConnectivityContext(scanner), linkManager)));
        flushEdt();
        return result.get();
    }

    private static AvailableHardware hardware(PortResult... ports) {
        return new AvailableHardware(Arrays.asList(ports), false, false, false);
    }

    private static RusEfiSignature signature(String target, String hash) {
        return new RusEfiSignature("master", "2026", "07", "30", target, hash, false);
    }

    private static void flushEdt() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
        });
    }

    private static boolean containsButton(Component component, String text) {
        if (component instanceof AbstractButton) {
            return text.equals(((AbstractButton) component).getText());
        }
        if (component instanceof Container) {
            return Arrays.stream(((Container) component).getComponents())
                .anyMatch(child -> containsButton(child, text));
        }
        return false;
    }
}
