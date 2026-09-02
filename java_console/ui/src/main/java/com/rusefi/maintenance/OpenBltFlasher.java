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

    private static final Logging log = getLogging(OpenBltFlasher.class);

    private final XcpLoader mLoader;
    private final OpenbltJni.OpenbltCallbacks mCallbacks;

    private List<SrecParser.SRecord> mSegments;
    private int mTotalFileSize;

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
        OpenBltFlasher f = OpenBltFlasher.makeCan(port, new XcpSettings(), callbacks);
        f.flash(fileName);
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

    private void loadFile(String filename) throws IOException {
        mCallbacks.setPhase("Load firmware file", false);
//        mCallbacks.log("Parsing firmware file...");

        SrecParser file = new SrecParser();
        file.parse(new File(filename));
        setSegments(file.getSegments(), "Firmware file");
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
