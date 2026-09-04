package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.rusefi.io.can.CanAddress;
import com.rusefi.io.can.RawCanPort;
import com.rusefi.libopenblt.XcpLoader;
import com.rusefi.libopenblt.XcpSettings;
import com.rusefi.libopenblt.file.SrecParser;
import com.rusefi.libopenblt.transport.IXcpTransport;
import com.rusefi.libopenblt.transport.XcpCan;
import com.rusefi.libopenblt.transport.XcpNet;
import com.rusefi.libopenblt.transport.XcpSerial;

import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.List;

import static com.devexperts.logging.Logging.getLogging;

public class OpenBltFlasher {
    // Defaults from firmware/hw_layer/openblt/efi_blt_ids.h, represented without OS-specific EFF bits.
    private static final CanAddress RUSEFI_OPENBLT_REQUEST = new CanAddress(0x10667, true);
    private static final CanAddress RUSEFI_OPENBLT_RESPONSE = new CanAddress(0x107E1, true);

    // 16 full PROGRAM_MAX packets with a 240-byte CTO, while still updating progress often.
    private static final int PROGRAM_CHUNK_SIZE = 16 * 239;
    // A failed probe can consume the 50 ms XCP T6 timeout plus this 200 ms pause.
    private static final int CAN_BOOTLOADER_ATTEMPTS = 240;
    private static final long CAN_BOOTLOADER_RETRY_DELAY_MS = 200;

    private static final Logging log = getLogging(OpenBltFlasher.class);

    private final XcpLoader mLoader;
    private final OpenbltJni.OpenbltCallbacks mCallbacks;

    private List<SrecParser.SRecord> mSegments;
    private int mTotalFileSize;

    static final class PreparedFirmware {
        private final List<SrecParser.SRecord> segments;
        private final int totalFileSize;

        private PreparedFirmware(List<SrecParser.SRecord> segments, int totalFileSize) {
            this.segments = segments;
            this.totalFileSize = totalFileSize;
        }
    }

    interface RetryPause {
        void pause() throws InterruptedException;
    }

    OpenBltFlasher(IXcpTransport transport, XcpSettings settings, OpenbltJni.OpenbltCallbacks callbacks) {
        mLoader = new XcpLoader(transport, settings);
        mCallbacks = callbacks;
    }

    public static OpenBltFlasher makeSerial(String portName, XcpSettings settings, OpenbltJni.OpenbltCallbacks callbacks) {
        log.info("makeSerial " + portName);
        IXcpTransport transport = new XcpSerial(portName);
        return new OpenBltFlasher(transport, settings, callbacks);
    }

    public static OpenBltFlasher makeTcp(String hostname, int port, XcpSettings settings, OpenbltJni.OpenbltCallbacks callbacks) {
        IXcpTransport transport = new XcpNet(hostname, port);
        return new OpenBltFlasher(transport, settings, callbacks);
    }

    public static OpenBltFlasher makeCan(RawCanPort port, XcpSettings settings, OpenbltJni.OpenbltCallbacks callbacks) {
        IXcpTransport transport = new XcpCan(port, RUSEFI_OPENBLT_REQUEST, RUSEFI_OPENBLT_RESPONSE);
        return new OpenBltFlasher(transport, settings, callbacks);
    }

    public static void flashSerial(String fileName, String port, OpenbltJni.OpenbltCallbacks callbacks) throws IOException {
        OpenBltFlasher f = OpenBltFlasher.makeSerial(port, new XcpSettings(), callbacks);
        f.flash(fileName);
    }

    public static void flashCan(String fileName, RawCanPort port, OpenbltJni.OpenbltCallbacks callbacks) throws IOException {
        flashCan(fileName, port, callbacks, CAN_BOOTLOADER_ATTEMPTS,
            () -> Thread.sleep(CAN_BOOTLOADER_RETRY_DELAY_MS));
    }

    static void flashCan(PreparedFirmware firmware, RawCanPort port,
                         OpenbltJni.OpenbltCallbacks callbacks) throws IOException {
        flashCan(firmware, port, callbacks, CAN_BOOTLOADER_ATTEMPTS,
            () -> Thread.sleep(CAN_BOOTLOADER_RETRY_DELAY_MS));
    }

    static void flashCan(String fileName, RawCanPort port, OpenbltJni.OpenbltCallbacks callbacks,
                         int bootloaderAttempts, RetryPause retryPause) throws IOException {
        flashCan(prepareFirmware(fileName, callbacks), port, callbacks, bootloaderAttempts, retryPause);
    }

    static void flashCan(PreparedFirmware firmware, RawCanPort port, OpenbltJni.OpenbltCallbacks callbacks,
                         int bootloaderAttempts, RetryPause retryPause) throws IOException {
        OpenBltFlasher f = OpenBltFlasher.makeCan(port, new XcpSettings(), callbacks);
        f.usePreparedFirmware(firmware);
        f.awaitBootloader(bootloaderAttempts, retryPause);
        f.execute(true);
    }

    static PreparedFirmware prepareFirmware(String fileName, OpenbltJni.OpenbltCallbacks callbacks) throws IOException {
        callbacks.setPhase("Load firmware file", false);

        SrecParser file = new SrecParser();
        file.parse(new File(fileName));
        List<SrecParser.SRecord> segments = file.getSegments();
        if (segments.isEmpty()) {
            throw new IOException("Firmware file contains no data records");
        }

        int totalFileSize = segments.stream().map(s -> s.data.length).reduce(0, Integer::sum);
        callbacks.log("Firmware file parsed:");
        callbacks.log(String.format("\tfirst address: 0x%08X", segments.get(0).address));
        callbacks.log("\ttotal size: " + totalFileSize);
        return new PreparedFirmware(segments, totalFileSize);
    }

    public static void eraseSerial(OpenBltWipeArtifact artifact, String port,
                                   OpenbltJni.OpenbltCallbacks callbacks) throws IOException {
        OpenBltFlasher f = OpenBltFlasher.makeSerial(port, new XcpSettings(), callbacks);
        f.erase(artifact);
    }

    public void flash(String filename) throws IOException {
        loadFile(filename);
        execute(true);
    }

    void erase(OpenBltWipeArtifact artifact) throws IOException {
        mCallbacks.setPhase("Load emergency wipe image", false);
        setSegments(artifact.getSegments(), "Emergency wipe image");
        mLoader.requireStationIdCheck(artifact.getReportedStationId());
        execute(false);
    }

    private void execute(boolean writeData) throws IOException {
        mCallbacks.setPhase("Connect to target", false);
        boolean started = false;
        Throwable failure = null;
        try {
            mLoader.start();
            started = true;
            erase();
            if (writeData) {
                write();
            }
            mCallbacks.setPhase("Cleanup", false);
        } catch (IOException | RuntimeException | Error e) {
            failure = e;
            throw e;
        } finally {
            if (started) {
                try {
                    mLoader.stop();
                } catch (IOException | RuntimeException e) {
                    if (failure != null) {
                        failure.addSuppressed(e);
                    } else {
                        throw e;
                    }
                }
            }
        }
    }

    private void awaitBootloader(int attempts, RetryPause retryPause) throws IOException {
        if (attempts <= 0) {
            throw new IllegalArgumentException("Bootloader attempt count must be positive");
        }

        mCallbacks.setPhase("Wait for CAN bootloader", false);
        IOException lastFailure = null;
        for (int attempt = 1; attempt <= attempts; attempt++) {
            try {
                mLoader.probeAvailability();
                return;
            } catch (IOException e) {
                lastFailure = e;
            }

            if (attempt < attempts) {
                try {
                    retryPause.pause();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    throw new IOException("Interrupted while waiting for OpenBLT over CAN", e);
                }
            }
        }

        throw new IOException("OpenBLT did not respond over CAN after " + attempts + " attempts", lastFailure);
    }

    private void loadFile(String filename) throws IOException {
        usePreparedFirmware(prepareFirmware(filename, mCallbacks));
    }

    private void usePreparedFirmware(PreparedFirmware firmware) {
        mSegments = firmware.segments;
        mTotalFileSize = firmware.totalFileSize;
    }

    private void setSegments(List<SrecParser.SRecord> segments, String description) throws IOException {
        if (segments.isEmpty()) {
            throw new IOException(description + " contains no data records");
        }

        mSegments = segments;

        mTotalFileSize = mSegments.stream()
            .map(s -> s.data.length).reduce(0, Integer::sum);

        mCallbacks.log(description + " parsed:");
        mCallbacks.log(String.format("\tfirst address: 0x%08X", mSegments.get(0).address));
        mCallbacks.log("\ttotal size: " + mTotalFileSize);
    }

    private class ProgressUpdater {
        private int mTotalProcessed = 0;

        private int mLastPercent = -1;

        void processBytes(int thisChunk) {
            mTotalProcessed += thisChunk;

            int percent = (int)(100.0 * mTotalProcessed / mTotalFileSize);

            if (percent != mLastPercent) {
                mLastPercent = percent;

                mCallbacks.updateProgress(percent);
            }
        }
    }

    private void erase() throws IOException {
        mCallbacks.setPhase("Erase", true);
        final ProgressUpdater pu = new ProgressUpdater();

        // Some bootloaders in the wild (F7 1MB, in particular) have a bug that they might not
        // erase the 2nd page if an erase request is made across a page boundary. Because of
        // that, erase very small chunks, which will result in an aligned erase request with the
        // beginning of every page. Most of these requests will instantly return as it was already
        // erased by a previous request erasing a full page.
        forEachFirmwareChunk(4096, (Chunk c) -> {
            mLoader.clearMemory(c.address, c.data.length);

            pu.processBytes(c.data.length);
        });
    }

    private void write() throws IOException {
        mCallbacks.setPhase("Program", true);
        final ProgressUpdater pu = new ProgressUpdater();

        forEachFirmwareChunk(PROGRAM_CHUNK_SIZE, (Chunk c) -> {
            mLoader.writeData(c.address, c.data);

            pu.processBytes(c.data.length);
        });
    }

    private static class Chunk {
        public int address;
        public byte[] data;
    }

    private interface ChunkHandler {
        void handle(Chunk chunk) throws IOException;
    }

    private void forEachFirmwareChunk(int maxChunk, ChunkHandler func) throws IOException {
        for (SrecParser.SRecord segment : mSegments) {
            int segmentRemain = segment.data.length;
            int segmentOffset = 0;

            while (segmentRemain > 0) {
                int chunkSize = Math.min(segmentRemain, maxChunk);

                Chunk c = new Chunk();
                c.address = segment.address + segmentOffset;
                c.data = Arrays.copyOfRange(segment.data, segmentOffset, segmentOffset + chunkSize);

                func.handle(c);

                segmentRemain -= chunkSize;
                segmentOffset += chunkSize;
            }
        }
    }
}
