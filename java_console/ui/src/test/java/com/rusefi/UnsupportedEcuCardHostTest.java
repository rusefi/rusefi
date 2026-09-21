package com.rusefi;

import com.opensr5.ConfigurationImageMeta;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IniFileProvider;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.io.UnsupportedEcuInfo;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.mockito.Mockito.*;

public class UnsupportedEcuCardHostTest {
    private static final String PORT = "COM7";

    @Test
    public void cachedIniDateMustNotChangeLiveBootloaderIdentity() throws Exception {
        String liveSignature = "rusEFI lts-25kansas.2026.06.27.proteus_f7.12345";
        String cachedSignature = "rusEFI lts-25kansas.2026.06.24.proteus_f7.12345";
        IniFileModel ini = mock(IniFileModel.class, RETURNS_DEEP_STUBS);
        when(ini.getSignature()).thenReturn(cachedSignature);
        when(ini.getMetaInfo().getSignature()).thenReturn(cachedSignature);
        when(ini.getMetaInfo().getPageSize(0)).thenReturn(16);
        when(ini.getMetaInfo().getnPages()).thenReturn(2);
        when(ini.getMetaInfo().getPageIdentifier(1)).thenReturn(0x0400);
        when(ini.getMetaInfo().getPageSize(1)).thenReturn(8);
        BinaryProtocol bp = mock(BinaryProtocol.class);
        IoStream stream = mock(IoStream.class);
        IncomingDataBuffer buffer = mock(IncomingDataBuffer.class);
        when(bp.getStream()).thenReturn(stream);
        when(stream.getDataBuffer()).thenReturn(buffer);
        when(buffer.getPacket("[hello]")).thenReturn(("\u0000" + liveSignature + "\u0000").getBytes(StandardCharsets.US_ASCII));
        when(bp.readFullImageFromController(any(ConfigurationImageMeta.class))).thenAnswer(call ->
            new ConfigurationImageWithMeta(call.getArgument(0), new byte[16]));
        when(bp.readFromPage(0x0400, 0, 8)).thenReturn(new byte[8]);
        IniFileProvider previousProvider = BinaryProtocol.iniFileProvider;
        CalibrationsInfo calibrations;
        try {
            BinaryProtocol.iniFileProvider = signature -> ini;
            calibrations = CalibrationsHelper.readCurrentCalibrations(bp, UpdateOperationCallbacks.DUMMY).orElseThrow();
        } finally {
            BinaryProtocol.iniFileProvider = previousProvider;
        }
        FakePortScanner scanner = new FakePortScanner();
        UnsupportedEcuCardHost host = createHost(scanner, new LinkManager());
        PortResult detected = new PortResult(PORT, SerialPortType.EcuWithOpenblt, calibrations);
        scanner.fireHardwareChange(hardware(detected));
        host.onCompatibleEcu(PORT, SignatureHelper.parse(liveSignature));
        flushEdt();

        // A delayed wizard exit must not lose BL detection just because the cached INI is older.
        assertEquals(liveSignature, calibrations.getImage().getMeta().getEcuSignature());
        assertEquals(liveSignature, calibrations.getPage(0x0400).getMeta().getEcuSignature());
        assertEquals(0, scanner.cachedPorts.size());
        assertEquals(SerialPortType.EcuWithOpenblt, scanner.getCurrentHardware().getKnownPorts().get(0).type);
    }

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
