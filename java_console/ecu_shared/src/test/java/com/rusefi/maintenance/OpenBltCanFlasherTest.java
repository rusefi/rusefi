package com.rusefi.maintenance;

import com.rusefi.io.can.CanAddress;
import com.rusefi.io.can.ClassicCanFrame;
import com.rusefi.io.can.RawCanPort;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class OpenBltCanFlasherTest {
    private static final CanAddress REQUEST = new CanAddress(0x10667, true);
    private static final CanAddress RESPONSE = new CanAddress(0x107E1, true);

    @TempDir
    Path tempDir;

    @Test
    void flashesThroughSharedExtendedCanTransportWithEightByteCto() throws IOException {
        Path firmware = tempDir.resolve("test.srec");
        Files.write(firmware, record(0x08008000L, new byte[]{1, 2, 3, 4, 5, 6, 7, 8})
            .getBytes(StandardCharsets.US_ASCII));
        ScriptedCanPort port = new ScriptedCanPort();

        OpenBltFlasher.flashCan(firmware.toString(), port, callbacks());

        assertEquals(RESPONSE, port.openAddress);
        assertEquals(2, port.openCount);
        assertEquals(2, port.closeCount);
        assertTrue(port.sent.stream().allMatch(frame -> REQUEST.equals(frame.getAddress())));
        assertEquals(Arrays.asList(0xFF, 0xFF, 0xD2, 0xF6, 0xD1, 0xF6, 0xC9, 0xD0, 0xD0, 0xCF),
            port.sent.stream()
                .map(frame -> frame.getPayload()[0] & 0xFF)
                .collect(Collectors.toList()));
    }

    @Test
    void updateProgressIsContinuousAcrossFirmwareStages() throws IOException {
        Path firmware = firmwareFile(9_000);
        List<Integer> progress = new ArrayList<>();

        OpenBltFlasher.flashCan(firmware.toString(), new ScriptedCanPort(), callbacks(progress), 3, () -> { });

        assertFalse(hasBackwardStep(progress), "firmware progress must not move backward between stages");
        assertTrue(progress.get(0) >= 25);
        assertEquals(95, progress.get(progress.size() - 1));
    }

    @Test
    void openBltPhaseCallbacksReachTypedFirmwareStages() throws IOException {
        List<UpdateOperationCallbacks.FirmwareUpdateStage> phases = new ArrayList<>();
        UpdateOperationCallbacks callbacks = progressCallbacks(new ArrayList<>(), phases);

        OpenBltFlasher.flashCan(firmwareFile().toString(), new ScriptedCanPort(),
            FirmwareOperations.makeOpenbltCallbacks(callbacks), 3, () -> { });

        assertEquals(Arrays.asList(UpdateOperationCallbacks.FirmwareUpdateStage.ERASE,
            UpdateOperationCallbacks.FirmwareUpdateStage.PROGRAM), phases);
    }

    @Test
    void fullProgressSequenceRunsFromDownloadThroughRestoreAndSuccessfulDone() throws IOException {
        Path firmware = firmwareFile(9_000);
        List<Integer> progress = new ArrayList<>();
        UpdateOperationCallbacks callbacks = progressCallbacks(progress);

        callbacks.updateFirmwareProgress(UpdateOperationCallbacks.FirmwareUpdateStage.DOWNLOAD, 100);
        callbacks.updateFirmwareProgress(UpdateOperationCallbacks.FirmwareUpdateStage.CALIBRATION_BACKUP, 100);
        OpenBltFlasher.flashCan(firmware.toString(), new ScriptedCanPort(), callbacks(progress), 3, () -> { });
        callbacks.updateFirmwareProgress(UpdateOperationCallbacks.FirmwareUpdateStage.CALIBRATION_RESTORE, 0);
        callbacks.done();

        assertFalse(hasBackwardStep(progress));
        assertEquals(15, progress.get(0));
        assertEquals(25, progress.get(1));
        assertEquals(95, progress.get(progress.size() - 2));
        assertEquals(100, progress.get(progress.size() - 1));
    }

    @Test
    void rejectsInvalidFirmwareBeforeOpeningCan() throws IOException {
        Path firmware = tempDir.resolve("invalid.srec");
        Files.write(firmware, "not an S-record".getBytes(StandardCharsets.US_ASCII));
        ScriptedCanPort port = new ScriptedCanPort();

        assertThrows(IOException.class,
            () -> OpenBltFlasher.flashCan(firmware.toString(), port, callbacks(), 3, () -> { }));

        assertEquals(0, port.openCount);
        assertTrue(port.sent.isEmpty());
    }

    @Test
    void preparedFirmwareIsNotReadAgainAfterHandoffStarts() throws IOException {
        Path firmware = firmwareFile();
        OpenbltJni.OpenbltCallbacks callbacks = callbacks();
        OpenBltFlasher.PreparedFirmware prepared =
            OpenBltFlasher.prepareFirmware(firmware.toString(), callbacks);
        Files.delete(firmware);
        ScriptedCanPort port = new ScriptedCanPort();

        OpenBltFlasher.flashCan(prepared, port, callbacks, 3, () -> { });

        assertEquals(2, port.openCount);
        assertTrue(commands(port).contains(0xD1));
    }

    @Test
    void retriesOnlyConnectWhileLiveFirmwareReboots() throws IOException {
        Path firmware = firmwareFile();
        ScriptedCanPort port = new ScriptedCanPort();
        port.connectResponsesToSkip = 1;

        OpenBltFlasher.flashCan(firmware.toString(), port, callbacks(), 3, () -> { });

        assertEquals(Arrays.asList(0xFF, 0xFF, 0xFF, 0xD2),
            commands(port).subList(0, 4));
        assertEquals(3, port.openCount);
        assertEquals(3, port.closeCount);
    }

    @Test
    void givesUpBeforeProgrammingWhenBootloaderDoesNotReply() throws IOException {
        Path firmware = firmwareFile();
        ScriptedCanPort port = new ScriptedCanPort();
        port.connectResponsesToSkip = Integer.MAX_VALUE;

        assertThrows(IOException.class,
            () -> OpenBltFlasher.flashCan(firmware.toString(), port, callbacks(), 3, () -> { }));

        assertEquals(Arrays.asList(0xFF, 0xFF, 0xFF), commands(port));
        assertEquals(3, port.openCount);
        assertEquals(3, port.closeCount);
    }

    @Test
    void doesNotRetryAfterEraseBegins() throws IOException {
        Path firmware = firmwareFile();
        ScriptedCanPort port = new ScriptedCanPort();
        port.failEraseClear = true;

        assertThrows(IOException.class,
            () -> OpenBltFlasher.flashCan(firmware.toString(), port, callbacks(), 3, () -> { }));

        assertEquals(2, commands(port).stream().filter(command -> command == 0xFF).count());
        assertEquals(1, commands(port).stream().filter(command -> command == 0xD1).count());
        assertEquals(2, port.openCount);
        assertEquals(2, port.closeCount);
    }

    @Test
    void failedProgramWriteDoesNotReportCompletion() throws IOException {
        Path firmware = firmwareFile(9_000);
        ScriptedCanPort port = new ScriptedCanPort();
        port.failProgramWrite = true;
        List<Integer> progress = new ArrayList<>();

        assertThrows(IOException.class,
            () -> OpenBltFlasher.flashCan(firmware.toString(), port, callbacks(progress), 3, () -> { }));

        assertTrue(commands(port).stream().anyMatch(command -> command == 0xC9 || command == 0xD0));
        assertTrue(progress.get(progress.size() - 1) < 100, "failed programming must not report completion");
    }

    private Path firmwareFile() throws IOException {
        return firmwareFile(8);
    }

    private Path firmwareFile(int size) throws IOException {
        Path firmware = tempDir.resolve("test.srec");
        StringBuilder contents = new StringBuilder();
        for (int offset = 0; offset < size; offset += 200) {
            int chunk = Math.min(200, size - offset);
            byte[] data = new byte[chunk];
            for (int i = 0; i < chunk; i++) {
                data[i] = (byte) (offset + i);
            }
            contents.append(record(0x08008000L + offset, data));
        }
        Files.write(firmware, contents.toString().getBytes(StandardCharsets.US_ASCII));
        return firmware;
    }

    private static boolean hasBackwardStep(List<Integer> progress) {
        for (int i = 1; i < progress.size(); i++) {
            if (progress.get(i) < progress.get(i - 1)) {
                return true;
            }
        }
        return false;
    }

    private static UpdateOperationCallbacks progressCallbacks(List<Integer> progress) {
        return progressCallbacks(progress, new ArrayList<>());
    }

    private static UpdateOperationCallbacks progressCallbacks(List<Integer> progress,
                                                               List<UpdateOperationCallbacks.FirmwareUpdateStage> phases) {
        return new UpdateOperationCallbacks() {
            @Override
            public void log(String message, boolean breakLineOnTextArea, boolean sendToLogger) {
            }

            @Override
            public void done() {
                progress.add(100);
            }

            @Override
            public void warning() {
            }

            @Override
            public void error() {
            }

            @Override
            public void clear() {
            }

            @Override
            public void updateProgress(int percent) {
                progress.add(percent);
            }

            @Override
            public void updateFirmwarePhase(UpdateOperationCallbacks.FirmwareUpdateStage stage) {
                phases.add(stage);
            }
        };
    }

    private static List<Integer> commands(ScriptedCanPort port) {
        return port.sent.stream()
            .map(frame -> frame.getPayload()[0] & 0xFF)
            .collect(Collectors.toList());
    }

    private static String record(long address, byte[] data) {
        byte[] body = new byte[4 + data.length];
        for (int i = 0; i < 4; i++) {
            body[i] = (byte) (address >> (8 * (3 - i)));
        }
        System.arraycopy(data, 0, body, 4, data.length);
        int count = body.length + 1;
        int sum = count;
        StringBuilder result = new StringBuilder(String.format("S3%02X", count));
        for (byte value : body) {
            sum += value & 0xFF;
            result.append(String.format("%02X", value & 0xFF));
        }
        return result.append(String.format("%02X%n", (~sum) & 0xFF)).toString();
    }

    private static OpenbltJni.OpenbltCallbacks callbacks() {
        return callbacks(new ArrayList<>());
    }

    private static OpenbltJni.OpenbltCallbacks callbacks(List<Integer> progress) {
        return new OpenbltJni.OpenbltCallbacks() {
            @Override
            public void log(String line) {
            }

            @Override
            public void updateProgress(int percent) {
                progress.add(percent);
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

    private static class ScriptedCanPort implements RawCanPort {
        final List<ClassicCanFrame> sent = new ArrayList<>();
        final ArrayDeque<ClassicCanFrame> responses = new ArrayDeque<>();
        CanAddress openAddress;
        int openCount;
        int closeCount;
        int connectResponsesToSkip;
        boolean failEraseClear;
        boolean failProgramWrite;

        @Override
        public void open(CanAddress receiveAddress) {
            openAddress = receiveAddress;
            openCount++;
        }

        @Override
        public void send(ClassicCanFrame frame) {
            sent.add(frame);
            int command = frame.getPayload()[0] & 0xFF;
            if (command == 0xFF) {
                if (connectResponsesToSkip > 0) {
                    connectResponsesToSkip--;
                    return;
                }
                responses.add(new ClassicCanFrame(RESPONSE,
                    new byte[]{(byte) 0xFF, 0, 0, 8, 8, 0, 0, 0}));
            } else if (command == 0xD2) {
                responses.add(new ClassicCanFrame(RESPONSE,
                    new byte[]{(byte) 0xFF, 0, 0, 8, 0, 0, 0, 0}));
            } else if (command == 0xD1 && failEraseClear) {
                return;
            } else if ((command == 0xC9 || command == 0xD0) && failProgramWrite) {
                return;
            } else {
                responses.add(new ClassicCanFrame(RESPONSE,
                    new byte[]{(byte) 0xFF, 0, 0, 0, 0, 0, 0, 0}));
            }
        }

        @Override
        public Optional<ClassicCanFrame> receive(int timeoutMs) {
            return responses.isEmpty() ? Optional.empty() : Optional.of(responses.remove());
        }

        @Override
        public void close() {
            closeCount++;
        }
    }
}
