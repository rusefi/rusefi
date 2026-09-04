package com.rusefi.maintenance;

import com.rusefi.io.can.CanAddress;
import com.rusefi.io.can.ClassicCanFrame;
import com.rusefi.io.can.RawCanPort;
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
        port.failProgramClear = true;

        assertThrows(IOException.class,
            () -> OpenBltFlasher.flashCan(firmware.toString(), port, callbacks(), 3, () -> { }));

        assertEquals(2, commands(port).stream().filter(command -> command == 0xFF).count());
        assertEquals(1, commands(port).stream().filter(command -> command == 0xD1).count());
        assertEquals(2, port.openCount);
        assertEquals(2, port.closeCount);
    }

    private Path firmwareFile() throws IOException {
        Path firmware = tempDir.resolve("test.srec");
        Files.write(firmware, record(0x08008000L, new byte[]{1, 2, 3, 4, 5, 6, 7, 8})
            .getBytes(StandardCharsets.US_ASCII));
        return firmware;
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

    private static class ScriptedCanPort implements RawCanPort {
        final List<ClassicCanFrame> sent = new ArrayList<>();
        final ArrayDeque<ClassicCanFrame> responses = new ArrayDeque<>();
        CanAddress openAddress;
        int openCount;
        int closeCount;
        int connectResponsesToSkip;
        boolean failProgramClear;

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
            } else if (command == 0xD1 && failProgramClear) {
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
