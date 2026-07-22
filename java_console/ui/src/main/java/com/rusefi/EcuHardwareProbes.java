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
import com.rusefi.updater.OpenbltDetectorStrategy.OpenbltInfo;

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

    // The DFU driver-state query differs between H7 and F4/F7 boards, so the probe needs the
    // connected-board identity (shared with ConnectivityContext, see ProductionConnectivity).
    private final com.rusefi.core.io.ConnectedEcuTarget connectedEcuTarget;

    public EcuHardwareProbes(com.rusefi.core.io.ConnectedEcuTarget connectedEcuTarget) {
        this.connectedEcuTarget = connectedEcuTarget;
    }

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
        return DfuFlasher.detectSTM32BootloaderDriverState(UpdateOperationCallbacks.DUMMY, connectedEcuTarget);
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
     * The three per-port hardware operations behind {@link #inspect(String, PortProbe)} plus the
     * retry backoff, extracted so the classification/retry decision tree can be unit-tested with
     * scripted probes — the same seam shape as {@link SerialPortScanner.HardwareProbes} one level up.
     * The production implementation ({@link #REAL_PROBE}) opens real serial ports.
     */
    interface PortProbe {
        /** @throws com.fazecast.jSerialComm.SerialPortInvalidPortException for a stale OS node */
        OpenbltInfo getOpenbltInfo(String port);

        /** @throws com.fazecast.jSerialComm.SerialPortInvalidPortException when the port vanishes mid-probe */
        Optional<CalibrationsInfo> readEcuCalibrations(String port);

        boolean ecuHasOpenblt(String port);

        /** Backoff between ECU-detection attempts; a test fake counts calls instead of sleeping. */
        void sleepBetweenAttempts() throws InterruptedException;
    }

    static final PortProbe REAL_PROBE = new PortProbe() {
        @Override
        public OpenbltInfo getOpenbltInfo(String port) {
            return EcuHardwareProbes.getOpenbltInfo(port);
        }

        @Override
        public Optional<CalibrationsInfo> readEcuCalibrations(String port) {
            return EcuHardwareProbes.readEcuCalibrations(port);
        }

        @Override
        public boolean ecuHasOpenblt(String port) {
            return EcuHardwareProbes.ecuHasOpenblt(port);
        }

        @Override
        public void sleepBetweenAttempts() throws InterruptedException {
            Thread.sleep(200);
        }
    };

    /**
     * Determines the type of a serial port: OpenBLT bootloader, ECU (with or without OpenBLT), or Unknown.
     * Unlike {@link com.rusefi.autodetect.PortDetector#autoDetectSerial} which only finds ECUs,
     * this method also recognises OpenBLT bootloaders via the XCP protocol probe.
     */
    static PortResult inspect(String serialPort) {
        return inspect(serialPort, REAL_PROBE);
    }

    static PortResult inspect(String serialPort, PortProbe probe) {
        log.info("Determining type of serial port: " + serialPort);

        OpenbltInfo openbltInfo;
        try {
            openbltInfo = probe.getOpenbltInfo(serialPort);
        } catch (com.fazecast.jSerialComm.SerialPortInvalidPortException e) {
            // Return null so inspectPorts filters them out entirely. [tag:better_ux_for_flashing]
            log.info("Port " + serialPort + " is not actually available (stale OS node): " + e.getMessage());
            return null;
        }
        log.info("Port " + serialPort + (openbltInfo.isOpenblt ? " looks like" : " does not look like") + " an OpenBLT bootloader");
        if (openbltInfo.isOpenblt) {
            return new PortResult(serialPort, SerialPortType.OpenBlt, null, openbltInfo);
        }

        for (int attempt = 1; attempt <= DETECT_MAX_ATTEMPTS; attempt++) {
            final Optional<CalibrationsInfo> ecuCalibrations;
            try {
                ecuCalibrations = probe.readEcuCalibrations(serialPort);
            } catch (com.fazecast.jSerialComm.SerialPortInvalidPortException e) {
                log.info("Port " + serialPort + " vanished during ECU detection: " + e.getMessage());
                return null;
            }
            final boolean isEcu = ecuCalibrations.isPresent();
            log.info("Port " + serialPort + " ECU detection attempt " + attempt + "/" + DETECT_MAX_ATTEMPTS
                + ": " + (isEcu ? "found" : "not found"));
            if (isEcu) {
                final boolean hasOpenblt = probe.ecuHasOpenblt(serialPort);
                log.info("ECU at " + serialPort + (hasOpenblt ? " has" : " does not have") + " an OpenBLT bootloader");
                return new PortResult(
                    serialPort,
                    hasOpenblt ? SerialPortType.EcuWithOpenblt : SerialPortType.Ecu,
                    ecuCalibrations.get()
                );
            }
            if (attempt < DETECT_MAX_ATTEMPTS) {
                try {
                    probe.sleepBetweenAttempts();
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

    private static OpenbltInfo getOpenbltInfo(String port) {
        try (IoStream stream = BufferedSerialIoStream.openPort(port)) {
            return OpenbltDetectorStrategy.detectOpenbltWithSignature(stream);
        } catch (com.fazecast.jSerialComm.SerialPortInvalidPortException e) {
            // Port is truly dead — let it propagate so inspectPort returns null.
            throw e;
        } catch (Exception e) {
            log.info("OpenBLT probe failed for " + port + ": " + e);
            return new OpenbltInfo(false, null);
        }
    }
}
