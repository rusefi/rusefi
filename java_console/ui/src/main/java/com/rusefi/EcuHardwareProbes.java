package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.io.serial.BufferedSerialIoStream;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.DfuFlasher;
import com.rusefi.maintenance.MaintenanceUtil;
import com.rusefi.maintenance.StLinkFlasher;
import com.rusefi.updater.OpenbltDetectorStrategy;

import java.util.Collection;
import java.util.List;
import java.util.Optional;
import java.util.Set;

/**
 * Production {@link SerialPortScanner.HardwareProbes}: real OS/hardware detection behind the
 * connectivity-module scan policy. Lives in the ui module because ECU/OpenBLT detection needs the
 * flashing tools ({@link DfuFlasher}, {@link StLinkFlasher}, {@link CalibrationsHelper}).
 */
public class EcuHardwareProbes implements SerialPortScanner.HardwareProbes {
    private final static Logging log = Logging.getLogging(EcuHardwareProbes.class);

    // Number of ECU-detection attempts before giving up and labelling a port Unknown.
    // A freshly rebooted or reconnected ECU may not respond on the first try.
    private static final int DETECT_MAX_ATTEMPTS = 3;

    @Override
    public Set<String> listSerialPorts() {
        return LinkManager.getCommPorts();
    }

    @Override
    public PortResult inspectPort(String serialPort) {
        return EcuHardwareProbes.inspect(serialPort);
    }

    @Override
    public Collection<String> listTcpPorts() {
        return TcpConnector.getAvailablePorts();
    }

    @Override
    public Optional<CalibrationsInfo> getEcuCalibrations(String tcpPort) {
        return EcuHardwareProbes.readEcuCalibrations(tcpPort);
    }

    @Override
    public boolean isLiveEcuConnected() {
        return ConnectionStatusLogic.INSTANCE.isConnected();
    }

    @Override
    public boolean isDfuDeviceConnected() {
        return DfuFlasher.detectSTM32BootloaderDriverState(UpdateOperationCallbacks.DUMMY);
    }

    @Override
    public boolean isStLinkConnected() {
        return StLinkFlasher.detectStLink(UpdateOperationCallbacks.DUMMY);
    }

    @Override
    public boolean isPcanConnected() {
        return MaintenanceUtil.detectPcan(UpdateOperationCallbacks.DUMMY);
    }

    @Override
    public long now() {
        return System.currentTimeMillis();
    }

    /**
     * Determines the type of a serial port: OpenBLT bootloader, ECU (with or without OpenBLT), or Unknown.
     * Unlike {@link com.rusefi.autodetect.PortDetector#autoDetectSerial} which only finds ECUs,
     * this method also recognises OpenBLT bootloaders via the XCP protocol probe.
     */
    static PortResult inspect(String serialPort) {
        log.info("Determining type of serial port: " + serialPort);

        boolean isOpenblt;
        try {
            isOpenblt = isPortOpenblt(serialPort);
        } catch (com.fazecast.jSerialComm.SerialPortInvalidPortException e) {
            // Return null so inspectPorts filters them out entirely. [tag:better_ux_for_flashing]
            log.info("Port " + serialPort + " is not actually available (stale OS node): " + e.getMessage());
            return null;
        }
        log.info("Port " + serialPort + (isOpenblt ? " looks like" : " does not look like") + " an OpenBLT bootloader");
        if (isOpenblt) {
            return new PortResult(serialPort, SerialPortType.OpenBlt);
        }

        for (int attempt = 1; attempt <= DETECT_MAX_ATTEMPTS; attempt++) {
            final Optional<CalibrationsInfo> ecuCalibrations;
            try {
                ecuCalibrations = readEcuCalibrations(serialPort);
            } catch (com.fazecast.jSerialComm.SerialPortInvalidPortException e) {
                log.info("Port " + serialPort + " vanished during ECU detection: " + e.getMessage());
                return null;
            }
            final boolean isEcu = ecuCalibrations.isPresent();
            log.info("Port " + serialPort + " ECU detection attempt " + attempt + "/" + DETECT_MAX_ATTEMPTS
                + ": " + (isEcu ? "found" : "not found"));
            if (isEcu) {
                final boolean hasOpenblt = ecuHasOpenblt(serialPort);
                log.info("ECU at " + serialPort + (hasOpenblt ? " has" : " does not have") + " an OpenBLT bootloader");
                return new PortResult(
                    serialPort,
                    hasOpenblt ? SerialPortType.EcuWithOpenblt : SerialPortType.Ecu,
                    ecuCalibrations.get()
                );
            }
            if (attempt < DETECT_MAX_ATTEMPTS) {
                try {
                    Thread.sleep(200);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                }
            }
        }

        log.info("Port " + serialPort + " does not look like an ECU after " + DETECT_MAX_ATTEMPTS + " attempts");
        return new PortResult(serialPort, SerialPortType.Unknown);
    }

    /** Parallel probe of real ports, for manual harnesses like {@code InspectPortSandbox}. */
    static List<PortResult> inspectPorts(List<String> ports) {
        return SerialPortScanner.inspectPorts(ports, null, EcuHardwareProbes::inspect);
    }

    private static Optional<CalibrationsInfo> readEcuCalibrations(final String port) {
        log.info("getEcuCalibrations " + port);
        return CalibrationsHelper.readCurrentCalibrationsWithoutSuspendingPortScanner(
            port,
            UpdateOperationCallbacks.LOGGER
        );
    }

    private static boolean ecuHasOpenblt(String port) {
        try (IoStream stream = BufferedSerialIoStream.openPort(port)) {
            if (stream == null) {
                return false;
            }

            return OpenbltDetectorStrategy.streamHasOpenBlt(stream);
        } catch (Exception e) {
            return false;
        }
    }

    private static boolean isPortOpenblt(String port) {
        try (IoStream stream = BufferedSerialIoStream.openPort(port)) {
            return OpenbltDetectorStrategy.isPortOpenblt(stream);
        } catch (com.fazecast.jSerialComm.SerialPortInvalidPortException e) {
            // Port is truly dead — let it propagate so inspectPort returns null.
            throw e;
        } catch (Exception e) {
            log.info("isPortOpenblt probe failed for " + port + ": " + e);
            return false;
        }
    }
}
