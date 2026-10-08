package com.rusefi.maintenance;

import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.core.FindFileHelper;
import com.rusefi.libopenblt.XcpSettings;
import com.rusefi.libopenblt.transport.IXcpTransport;
import com.rusefi.updater.OpenbltDetectorStrategy.OpenbltInfo;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class OpenBltEmergencyWipeTest {
    private static final String TARGET = "stm32f429_nucleo";
    private static final String UAEFI_TARGET = "uaefi";
    private static final long START = 0x08008000L;
    private static final long UAEFI_END = 0x08100000L;
    private static final long END = 0x08200000L;
    private static final int RECORD_DATA_SIZE = 240;

    @TempDir
    Path tempDir;

    private String previousInputFilesPath;

    @BeforeEach
    void setUp() throws IOException {
        previousInputFilesPath = FindFileHelper.INPUT_FILES_PATH;
        FindFileHelper.INPUT_FILES_PATH = tempDir.toString();
        writeArtifact();
    }

    @AfterEach
    void tearDown() {
        FindFileHelper.INPUT_FILES_PATH = previousInputFilesPath;
    }

    @Test
    void erasesEveryChunkWithoutProgrammingData() throws IOException {
        OpenBltWipeArtifact artifact = OpenBltWipeArtifact.loadAndValidate(signedPort(TARGET));
        RecordingTransport transport = new RecordingTransport();
        OpenBltFlasher flasher = new OpenBltFlasher(transport, new XcpSettings(), callbacks());

        flasher.erase(artifact);

        assertEquals(1, transport.connectCount);
        assertEquals(1, transport.disconnectCount);
        assertEquals(504, transport.commandCount(0xD1));
        assertEquals(1, transport.commandCount(0xD0));
        assertEquals(0, transport.commandCount(0xC9));
        assertEquals(1, transport.commandCount(0xCF));

        List<byte[]> setMtaRequests = transport.commands(0xF6);
        List<byte[]> clearRequests = transport.commands(0xD1);
        for (int i = 0; i < clearRequests.size(); i++) {
            assertEquals(START + i * 4096L, Integer.toUnsignedLong(ByteBuffer.wrap(setMtaRequests.get(i))
                .order(ByteOrder.LITTLE_ENDIAN).getInt(4)));
            assertEquals(4096, ByteBuffer.wrap(clearRequests.get(i)).order(ByteOrder.LITTLE_ENDIAN).getInt(4));
        }
        assertEquals(END, START + clearRequests.size() * 4096L);

        byte[] finalProgram = transport.first(0xD0);
        assertEquals(2, finalProgram.length);
        assertEquals(0, finalProgram[1]);
        assertEquals(0xCF, transport.requests.get(transport.requests.size() - 1)[0] & 0xFF);
    }

    @Test
    void eraseFailureStillDisconnectsAndNeverProgramsPayload() throws IOException {
        OpenBltWipeArtifact artifact = OpenBltWipeArtifact.loadAndValidate(signedPort(TARGET));
        RecordingTransport transport = new RecordingTransport();
        transport.failFirstClear = true;
        OpenBltFlasher flasher = new OpenBltFlasher(transport, new XcpSettings(), callbacks());

        assertThrows(IOException.class, () -> flasher.erase(artifact));

        assertEquals(1, transport.disconnectCount);
        assertEquals(0, transport.commandCount(0xC9));
        assertTrue(transport.requests.stream()
            .filter(request -> (request[0] & 0xFF) == 0xD0)
            .allMatch(request -> request.length == 2 && request[1] == 0));
    }

    @Test
    void signedTargetMismatchFailsBeforeAnyTransportExists() {
        IOException error = assertThrows(IOException.class,
            () -> OpenBltWipeArtifact.loadAndValidate(signedPort("different_board")));
        assertTrue(error.getMessage().contains("does not match"));
        assertFalse(OpenBltWipeArtifact.isAvailableFor(signedPort("different_board")));
        assertTrue(OpenBltWipeArtifact.isAvailableFor(signedPort(TARGET)));
    }

    @Test
    void uaefiProfileUsesOneMiBGeometry() throws IOException {
        Files.delete(tempDir.resolve("bin/wipe/rusefi_development_test_" + TARGET + "_wipe.srec"));
        writeArtifact(UAEFI_TARGET, UAEFI_END, false);

        OpenBltWipeArtifact artifact = OpenBltWipeArtifact.loadAndValidate(signedPort(UAEFI_TARGET));

        assertEquals(1024, artifact.getFlashSizeKiB());
        assertTrue(OpenBltWipeArtifact.isAvailableFor(signedPort(UAEFI_TARGET)));
    }

    @Test
    void liveTargetChangeDisconnectsBeforeProgrammingStarts() throws IOException {
        OpenBltWipeArtifact artifact = OpenBltWipeArtifact.loadAndValidate(signedPort(TARGET));
        RecordingTransport transport = new RecordingTransport();
        transport.stationId = "rusEFI.different_board";
        OpenBltFlasher flasher = new OpenBltFlasher(transport, new XcpSettings(), callbacks());

        IOException error = assertThrows(IOException.class, () -> flasher.erase(artifact));

        assertTrue(error.getMessage().contains("target changed"));
        assertEquals(1, transport.disconnectCount);
        assertEquals(0, transport.commandCount(0xD2));
        assertEquals(0, transport.commandCount(0xD1));
    }

    @Test
    void confirmationStatesDestructiveAndExternalStoragePolicy() throws IOException {
        String message = OpenBltWipeArtifact.loadAndValidate(signedPort(TARGET)).confirmationMessage();
        assertTrue(message.contains("permanently erase the ECU firmware and all internal settings"));
        assertTrue(message.contains("remain in recovery mode"));
        assertTrue(message.contains("previous tune will not be restored"));
        assertTrue(message.contains("SD card and external storage will not be erased"));
        assertFalse(message.contains("MCU:"));
        assertFalse(message.contains("KiB"));
    }

    @Test
    void emptyDataRecordIsRejectedForWipeUse() throws IOException {
        writeArtifact(true);
        IOException error = assertThrows(IOException.class,
            () -> OpenBltWipeArtifact.loadAndValidate(signedPort(TARGET)));
        assertTrue(error.getMessage().contains("empty data record"));
    }

    private static PortResult signedPort(String target) {
        return new PortResult("COM_TEST", SerialPortType.OpenBlt, null,
            new OpenbltInfo(true, "rusEFI." + target, Collections.emptyList()));
    }

    private void writeArtifact() throws IOException {
        writeArtifact(false);
    }

    private void writeArtifact(boolean includeEmptyRecord) throws IOException {
        writeArtifact(TARGET, END, includeEmptyRecord);
    }

    private void writeArtifact(String target, long end, boolean includeEmptyRecord) throws IOException {
        Path wipeDir = tempDir.resolve("bin/wipe");
        Files.createDirectories(wipeDir);
        String srecName = "rusefi_development_test_" + target + "_wipe.srec";
        Files.write(wipeDir.resolve("openblt_wipe.properties"), (
            "format=1\n"
                + "bundleTarget=" + target + "\n"
                + "bootloaderTarget=" + target + "\n"
                + "mcuFamily=ARCH_STM32F4\n"
                + "startAddress=0x08008000\n"
                + String.format("endAddressExclusive=0x%08X\n", end)
                + "srec=" + srecName + "\n").getBytes(StandardCharsets.US_ASCII));

        try (BufferedWriter writer = Files.newBufferedWriter(wipeDir.resolve(srecName), StandardCharsets.US_ASCII)) {
            writer.write(record('0', 0, 2, "wipe".getBytes(StandardCharsets.US_ASCII)));
            if (includeEmptyRecord) {
                writer.write(record('3', START, 4, new byte[0]));
            }
            int recordCount = 0;
            for (long address = START; address < end; address += RECORD_DATA_SIZE) {
                int size = (int) Math.min(RECORD_DATA_SIZE, end - address);
                byte[] data = new byte[size];
                java.util.Arrays.fill(data, (byte) 0xFF);
                writer.write(record('3', address, 4, data));
                recordCount++;
            }
            writer.write(record('5', recordCount, 2, new byte[0]));
            writer.write(record('7', START, 4, new byte[0]));
        }
    }

    private static String record(char type, long address, int addressSize, byte[] data) {
        byte[] body = new byte[addressSize + data.length];
        for (int i = 0; i < addressSize; i++) {
            body[i] = (byte) (address >> (8 * (addressSize - i - 1)));
        }
        System.arraycopy(data, 0, body, addressSize, data.length);
        int count = body.length + 1;
        int sum = count;
        StringBuilder result = new StringBuilder(String.format("S%c%02X", type, count));
        for (byte value : body) {
            sum += value & 0xFF;
            result.append(String.format("%02X", value & 0xFF));
        }
        return result.append(String.format("%02X%n", (~sum) & 0xFF)).toString();
    }

    private static OpenbltJni.OpenbltCallbacks callbacks() {
        return new OpenbltJni.OpenbltCallbacks() {
            @Override
            public void log(String line) {
            }

            @Override
            public void updateProgress(int percent) {
            }

            @Override
            public void error(String line) {
                throw new AssertionError(line);
            }

            @Override
            public void setPhase(String title, boolean hasProgress) {
            }
        };
    }

    private static class RecordingTransport implements IXcpTransport {
        final List<byte[]> requests = new ArrayList<>();
        int connectCount;
        int disconnectCount;
        boolean failFirstClear;
        String stationId = "rusEFI." + TARGET;

        @Override
        public void connect() {
            connectCount++;
        }

        @Override
        public void disconnect() {
            disconnectCount++;
        }

        @Override
        public byte[] sendPacket(byte[] request, int timeoutMs, int expectResponseBytes) throws IOException {
            requests.add(request.clone());
            int command = request[0] & 0xFF;
            if (command == 0xFF) {
                return new byte[]{(byte) 0xFF, 0, 0, (byte) 240, (byte) 240, 0, 0, 0};
            }
            if (command == 0xD2) {
                return new byte[]{(byte) 0xFF, 0, 0, (byte) 240, 0, 0, 0};
            }
            if (command == 0xBF) {
                byte[] station = stationId.getBytes(StandardCharsets.US_ASCII);
                byte[] response = new byte[station.length + 1];
                response[0] = (byte) 0xFF;
                System.arraycopy(station, 0, response, 1, station.length);
                return response;
            }
            if (command == 0xD1 && failFirstClear) {
                failFirstClear = false;
                throw new IOException("erase failed");
            }
            return new byte[]{(byte) 0xFF};
        }

        int commandCount(int command) {
            return (int) requests.stream().filter(request -> (request[0] & 0xFF) == command).count();
        }

        byte[] first(int command) {
            return requests.stream().filter(request -> (request[0] & 0xFF) == command).findFirst().orElseThrow();
        }

        List<byte[]> commands(int command) {
            List<byte[]> result = new ArrayList<>();
            for (byte[] request : requests) {
                if ((request[0] & 0xFF) == command) {
                    result.add(request);
                }
            }
            return result;
        }
    }
}
