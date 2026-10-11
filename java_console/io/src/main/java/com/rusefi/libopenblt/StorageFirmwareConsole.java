package com.rusefi.libopenblt;

import com.fazecast.jSerialComm.SerialPort;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.zip.CRC32;

/** Minimal framed console client: no UI, tune loading, or network access. */
final class StorageFirmwareConsole implements AutoCloseable {
    private final SerialPort port;

    private StorageFirmwareConsole(String name) throws IOException {
        port = SerialPort.getCommPort(name);
        port.setBaudRate(115200);
        port.setComPortTimeouts(SerialPort.TIMEOUT_READ_BLOCKING | SerialPort.TIMEOUT_WRITE_BLOCKING, 3000, 3000);
        if (!port.openPort()) {
            throw new IOException("Cannot open " + name);
        }
    }

    static void run(String port, String operation) throws Exception {
        try (StorageFirmwareConsole console = new StorageFirmwareConsole(port)) {
            String signature = new String(console.request("S"), StandardCharsets.US_ASCII);
            if (!signature.contains(".premium-quick-test.")) {
                throw new IOException("Wrong firmware target: " + signature);
            }
            System.out.println(signature);
            if (operation.equals("openblt")) {
                // Firmware may disconnect USB before sending its acknowledgement.
                console.writeFrame("Ereboot_openblt");
                System.out.println("OpenBLT reboot requested; reconnect using the bootloader serial port.");
                return;
            }
            String command = operation.equals("qspitest") ? "qspitest"
                : operation.equals("expose") ? "sdmode pc" : "sdinfo";
            console.request("E" + command);
            long deadline = System.nanoTime() + 10_000_000_000L;
            while (System.nanoTime() < deadline) {
                Thread.sleep(200);
                String text = new String(console.request("G"), StandardCharsets.US_ASCII).replace("`msg`", "\n");
                System.out.println(text);
                if (operation.equals("qspitest") && text.contains("confirmation_qspitest")) {
                    if (!text.contains("QSPI: pc=0x9010") || !text.contains("PASS")) {
                        throw new IOException("QSPI execution test did not pass");
                    }
                    return;
                }
                if (operation.equals("status") && text.contains("confirmation_sdinfo")) {
                    return;
                }
                if (operation.equals("expose")) {
                    if (text.contains("status=MSD")) {
                        return;
                    }
                    console.request("Esdinfo");
                }
            }
            throw new IOException("Timed out waiting for " + command);
        }
    }

    private void writeFrame(String command) throws IOException {
        byte[] data = command.getBytes(StandardCharsets.US_ASCII);
        CRC32 crc = new CRC32();
        crc.update(data);
        byte[] packet = ByteBuffer.allocate(data.length + 6).putShort((short) data.length)
            .put(data).putInt((int) crc.getValue()).array();
        if (port.writeBytes(packet, packet.length) != packet.length) {
            throw new IOException("Short console write");
        }
    }

    private byte[] request(String command) throws IOException {
        writeFrame(command);
        int length = Short.toUnsignedInt(ByteBuffer.wrap(readExactly(2)).getShort());
        if (length < 1 || length > 32768) {
            throw new IOException("Invalid console response length: " + length);
        }
        byte[] response = readExactly(length);
        CRC32 crc = new CRC32();
        crc.update(response);
        long received = Integer.toUnsignedLong(ByteBuffer.wrap(readExactly(4)).getInt());
        if (received != crc.getValue() || response[0] != 0) {
            throw new IOException("Invalid console response CRC/status");
        }
        return response;
    }

    private byte[] readExactly(int count) throws IOException {
        byte[] data = new byte[count];
        int received = port.readBytes(data, count);
        if (received != count) {
            throw new IOException("Short console read: " + received + " / " + count);
        }
        return data;
    }

    @Override public void close() {
        port.closePort();
    }
}
