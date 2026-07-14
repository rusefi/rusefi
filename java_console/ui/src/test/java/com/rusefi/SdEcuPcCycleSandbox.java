package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.LinkManager;

import java.nio.ByteBuffer;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;

/**
 * Headless soak sandbox: cycles the SD card between ECU/logging mode and PC/MSD mode
 * {@value #CYCLES} times, dwelling {@value #DWELL_MS}ms in each mode, to stress the mode-switch
 * firmware path (the PC/MSD -> ECU transition that historically failed with FR_DISK_ERR). Each
 * half-cycle sends the TS bench command (the same TS_SD_MOUNT_PC / TS_SD_MOUNT_ECU that the SD
 * Card dialog buttons issue), polls the SD status output channels (sd_present /
 * sd_logging_internal / sd_msd) to confirm the requested mode was reached, then holds for the
 * remainder of the 20s period. Reports a per-cycle pass/fail tally and exits non-zero if any
 * switch failed or the serial link dropped.
 *
 * Usage: run main() with the ECU serial port as the first argument, or set -Decu.port,
 * or let it auto-detect.
 */
public class SdEcuPcCycleSandbox {
    private static final Logging log = Logging.getLogging(SdEcuPcCycleSandbox.class);
    private static final int CYCLES = 10;
    /** Time spent in each mode before switching to the other, per the task spec. */
    private static final int DWELL_MS = 20_000;
    private static final int STATUS_READY_TIMEOUT_MS = 10_000;
    private static final int SWITCH_TIMEOUT_MS = 20_000;
    private static final int POLL_MS = 250;

    public static void main(String[] args) throws Exception {
        String port = args.length > 0 ? args[0] : System.getProperty("ecu.port");
        if (port == null) {
            port = PortDetector.autoDetectSerial(null).getSerialPort();
            if (port == null)
                throw new IllegalStateException("ECU serial not detected, pass port as an argument");
            log.info("Auto-detected ECU at " + port);
        }

        LinkManager linkManager = new LinkManager().setCompositeLogicEnabled(false);
        if (!linkManager.connect(port, false).await(60, TimeUnit.SECONDS))
            throw new IllegalStateException("Not connected to " + port + " in time");
        log.info("Connected to " + port);

        int failures = 0;
        try {
            waitForStatusData();
            logStatus("initial");

            for (int cycle = 1; cycle <= CYCLES; cycle++) {
                log.info(String.format("===== cycle %d/%d =====", cycle, CYCLES));

                // ECU/logging -> PC/MSD
                if (!switchAndDwell(linkManager, true, cycle))
                    failures++;
                // PC/MSD -> ECU/logging (the historically failure-prone direction)
                if (!switchAndDwell(linkManager, false, cycle))
                    failures++;
            }
        } catch (Exception e) {
            logStatus("last");
            log.error("ABORTED: " + e.getMessage());
            failures++;
        } finally {
            linkManager.close();
        }

        int switches = CYCLES * 2;
        if (failures == 0) {
            log.info(String.format("SUCCESS: all %d mode switches over %d cycles completed", switches, CYCLES));
        } else {
            log.error(String.format("FAILED: %d/%d mode switches did not complete", failures, switches));
        }
        // non-daemon communication threads would keep the JVM alive
        System.exit(failures == 0 ? 0 : 1);
    }

    /**
     * Sends the mount command for the requested mode, confirms the card reached it, then holds for
     * the remainder of the {@link #DWELL_MS} period. Returns false if the switch was not confirmed
     * (but still dwells so the soak timing stays regular).
     */
    private static boolean switchAndDwell(LinkManager linkManager, boolean wantPc, int cycle)
            throws InterruptedException, ExecutionException {
        String modeName = wantPc ? "PC/MSD" : "ECU/logging";
        int ts14 = wantPc
                ? VariableRegistryValues.ts_14_command_TS_SD_MOUNT_PC
                : VariableRegistryValues.ts_14_command_TS_SD_MOUNT_ECU;

        long periodStart = System.currentTimeMillis();
        log.info(String.format("cycle %d: switching -> %s", cycle, modeName));
        sendSdCommand(linkManager, ts14, "mount SD to " + modeName);

        boolean ok = waitForMode(linkManager, wantPc);
        logStatus(String.format("cycle %d after -> %s", cycle, modeName));
        if (ok) {
            log.info(String.format("cycle %d: reached %s", cycle, modeName));
        } else {
            log.error(String.format("cycle %d: did NOT reach %s within %dms", cycle, modeName, SWITCH_TIMEOUT_MS));
        }

        // hold the remainder of the 20s period in this mode
        long elapsed = System.currentTimeMillis() - periodStart;
        long remaining = DWELL_MS - elapsed;
        if (remaining > 0)
            Thread.sleep(remaining);
        return ok;
    }

    /** Poll until the SD status bits report the requested mode, the link drops, or we time out. */
    private static boolean waitForMode(LinkManager linkManager, boolean wantPc) throws InterruptedException {
        SensorCentral sc = SensorCentral.getInstance();
        long deadline = System.currentTimeMillis() + SWITCH_TIMEOUT_MS;
        while (System.currentTimeMillis() < deadline) {
            BinaryProtocol bp = linkManager.getBinaryProtocol();
            if (bp == null || bp.isClosed()) {
                log.error("serial link dropped while waiting for the mode switch - the failed mount stalled "
                        + "the firmware and SerialIoStream treated the full TX buffer as a disconnect");
                return false;
            }
            double present = sc.getValue(VariableRegistryValues.OUTPUT_CHANNEL_SD_PRESENT);
            double ecu = sc.getValue(VariableRegistryValues.OUTPUT_CHANNEL_SD_LOGGING_INTERNAL);
            double pc = sc.getValue(VariableRegistryValues.OUTPUT_CHANNEL_SD_MSD);
            boolean reached = wantPc
                    ? (present > 0.5 && pc > 0.5 && ecu < 0.5)
                    : (present > 0.5 && ecu > 0.5 && pc < 0.5);
            if (reached)
                return true;
            Thread.sleep(POLL_MS);
        }
        return false;
    }

    private static void waitForStatusData() throws InterruptedException {
        SensorCentral sc = SensorCentral.getInstance();
        long deadline = System.currentTimeMillis() + STATUS_READY_TIMEOUT_MS;
        while (System.currentTimeMillis() < deadline) {
            if (!Double.isNaN(sc.getValue(VariableRegistryValues.OUTPUT_CHANNEL_SD_PRESENT)))
                return;
            Thread.sleep(POLL_MS);
        }
        log.warn("SD status output channels never arrived; mode detection may not work");
    }

    private static void logStatus(String when) {
        SensorCentral sc = SensorCentral.getInstance();
        log.info(String.format("SD status %s: %s=%s %s(ECU)=%s %s(PC)=%s",
                when,
                VariableRegistryValues.OUTPUT_CHANNEL_SD_PRESENT, sc.getValue(VariableRegistryValues.OUTPUT_CHANNEL_SD_PRESENT),
                VariableRegistryValues.OUTPUT_CHANNEL_SD_LOGGING_INTERNAL, sc.getValue(VariableRegistryValues.OUTPUT_CHANNEL_SD_LOGGING_INTERNAL),
                VariableRegistryValues.OUTPUT_CHANNEL_SD_MSD, sc.getValue(VariableRegistryValues.OUTPUT_CHANNEL_SD_MSD)));
    }

    /** Sends the same TS bench command as the .ini SD Card dialog issues. */
    private static void sendSdCommand(LinkManager linkManager, int ts14command, String what)
            throws InterruptedException, ExecutionException {
        byte[] payload = ByteBuffer.allocate(4)
                .putShort((short) VariableRegistryValues.ts_command_e_TS_X14)
                .putShort((short) ts14command)
                .array();
        linkManager.submit(() -> {
            BinaryProtocol bp = linkManager.getBinaryProtocol();
            byte[] response = bp.executeCommand(Integration.TS_IO_TEST_COMMAND, payload, what);
            if (response == null || response.length == 0 || response[0] != (byte) Integration.TS_RESPONSE_OK)
                throw new IllegalStateException(what + ": no/bad response from firmware");
        }).get();
    }
}
