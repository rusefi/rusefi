package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.rusefi.libopenblt.XcpLoader;
import com.rusefi.libopenblt.XcpSettings;
import com.rusefi.libopenblt.file.SrecParser;
import com.rusefi.libopenblt.transport.IXcpTransport;
import com.rusefi.libopenblt.transport.XcpNet;
import com.rusefi.libopenblt.transport.XcpSerial;

import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.List;

import static com.devexperts.logging.Logging.getLogging;

public class OpenBltFlasher {
    private static final Logging log = getLogging(OpenBltFlasher.class);

    private final XcpLoader mLoader;
    private final OpenbltJni.OpenbltCallbacks mCallbacks;

    private List<SrecParser.SRecord> mSegments;
    private int mTotalFileSize;

    private OpenBltFlasher(IXcpTransport transport, XcpSettings settings, OpenbltJni.OpenbltCallbacks callbacks) {
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

    public static void flashSerial(String fileName, String port, OpenbltJni.OpenbltCallbacks callbacks) throws IOException {
        OpenBltFlasher f = OpenBltFlasher.makeSerial(port, new XcpSettings(), callbacks);
        f.flash(fileName);
    }

    public void flash(String filename) throws IOException {
        loadFile(filename);

        mCallbacks.setPhase("Connect to target", false);
        // Prepare loader
        mLoader.start();

        // Erase memory
        erase();

        // Write new data
        write();

        mCallbacks.setPhase("Cleanup", false);
        // Done, stop the session!
        mLoader.stop();
    }

    private void loadFile(String filename) throws IOException {
        mCallbacks.setPhase("Load firmware file", false);
//        mCallbacks.log("Parsing firmware file...");

        SrecParser file = new SrecParser();
        file.parse(new File(filename));

        mSegments = file.getSegments();

        mTotalFileSize = mSegments.stream()
            .map(s -> s.data.length).reduce(0, Integer::sum);

        mCallbacks.log("Firmware file parsed:");
        mCallbacks.log("\tfirst address: 0x" + Integer.toString(mSegments.get(0).address, 16));
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

        forEachFirmwareChunk(1024, (Chunk c) -> {
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
