package com.rusefi;

import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.CommandExecutor;
import com.rusefi.config.generated.Integration;

import static com.rusefi.binaryprotocol.IoHelper.swap16;

public class CommandExecutorMock implements CommandExecutor {
    private final ConfigurationImage testConfigurationImage;

    public CommandExecutorMock(final ConfigurationImage originalConfImage) {
        testConfigurationImage = originalConfImage.clone();
    }

    public ConfigurationImage getConfigurationImage() {
        return testConfigurationImage.clone();
    }

    @Override
    public byte[] executeCommand(final char opcode, final byte[] packet, final String loggingMsg) {
        switch (opcode) {
            case Integration.TS_CHUNK_WRITE_COMMAND: {
                writeChunk(packet);
                return new byte[] { Integration.TS_RESPONSE_OK };
            }
            case Integration.TS_BURN_COMMAND: {
                return new byte[] { Integration.TS_RESPONSE_BURN_OK };
            }
            default: {
                throw new UnsupportedOperationException();
            }
        }
    }

    private void writeChunk(final byte[] packet) {
        final int packetLength = packet.length;
        final int headerLength = 4;
        if (headerLength <= packetLength) {
            final int chunkOffset = swap16((int) packet[1] + (((int) packet[0]) << 8));
            final int chunkLength = swap16((int) packet[3] + (((int) packet[2]) << 8));
            if (chunkLength == packetLength - headerLength) {
                for (int i = 0; i < chunkLength; i++) {
                    testConfigurationImage.getContent()[chunkOffset + i] = packet[headerLength + i];
                }
            } else {
                throw new IllegalArgumentException(String.format(
                    "Packet length should be at least %d + %d bytes, but is %d bytes",
                    headerLength,
                    chunkLength,
                    packetLength
                ));
            }
        } else {
            throw new IllegalArgumentException(
                String.format("Packet length should be at least 4 bytes, but is %d bytes", packetLength)
            );
        }
    }
}
