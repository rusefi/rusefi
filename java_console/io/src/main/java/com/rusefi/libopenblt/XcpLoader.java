package com.rusefi.libopenblt;

import com.devexperts.logging.Logging;
import com.rusefi.libopenblt.transport.IXcpTransport;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;

import static com.devexperts.logging.Logging.getLogging;

public class XcpLoader {
    private static final Logging log = getLogging(XcpLoader.class);

    // XCP command codes as defined by the protocol currently supported by this module
    private static final byte XCPLOADER_CMD_CONNECT = (byte)0xFF;    // XCP connect command code
    private static final byte XCPLOADER_CMD_GET_STATUS = (byte)0xFD;    // XCP get status command code
    private static final byte XCPLOADER_CMD_GET_SEED = (byte)0xF8;    // XCP get seed command code
    private static final byte XCPLOADER_CMD_UNLOCK = (byte)0xF7;    // XCP unlock command code
    private static final byte XCPLOADER_CMD_SET_MTA = (byte)0xF6;    // XCP set mta command code
    private static final byte XCPLOADER_CMD_UPLOAD = (byte)0xF5;    // XCP upload command code
    private static final byte XCPLOADER_CMD_PROGRAM_START = (byte)0xD2;    // XCP program start command code
    private static final byte XCPLOADER_CMD_PROGRAM_CLEAR = (byte)0xD1;    // XCP program clear command code
    private static final byte XCPLOADER_CMD_PROGRAM = (byte)0xD0;    // XCP program command code
    private static final byte XCPLOADER_CMD_PROGRAM_RESET = (byte)0xCF;    // XCP program reset command code
    private static final byte XCPLOADER_CMD_PROGRAM_MAX = (byte)0xC9;    // XCP program max command code

    private static final byte XCPLOADER_CMD_PID_RES = (byte)0xFF;   // positive response
    private static final byte XCPLOADER_CMD_PID_ERR = (byte)0xFE;   // negative response

    private static final byte XCPLOADER_ERR_CMD_UNKNOWN = (byte)0x20;   // unknown command

    private static final int XCPLOADER_PACKET_SIZE_MAX = 255;

    private final IXcpTransport mTransport;

    private final XcpSettings mSettings;

    private static class TargetInfo {
        ByteOrder byteOrder;

        // The max number of bytes in the command transmit object (master->slave).
        int maxCto;

        // The max number of bytes in the data transmit object (slave->master).
        int maxDto;

        // The max number of bytes in the command transmit object (master->slave)
        // during a programming session.
        int maxProgCto;
    }

    private TargetInfo mTargetInfo;

    public XcpLoader(IXcpTransport transport, XcpSettings settings) {
        mTransport = transport;
        mSettings = settings;
    }

    public void start() throws IOException {
        mTransport.connect();
        connect();
    }

    public void stop() throws IOException {
        // Send a null program to conclude the programming session
        sendCmdProgram(new byte[0]);
        sendCmdProgramReset();
        mTransport.disconnect();
    }

    private void connect() throws IOException {
        byte[] request =
            ByteBuffer.allocate(2)
                .put(XCPLOADER_CMD_CONNECT)
                .put((byte) 0)  // connection mode?
                .array();

        byte[] response = mTransport.sendPacket(request, mSettings.timeoutT6, 8);

        if (response.length != 8) {
            throw new IOException("connect() failed due to incorrect length received: " + response.length);
        }

        if (response[0] != XCPLOADER_CMD_PID_RES) {
            throw new IOException("connect() failed due to bad response ID received: " + response[0]);
        }

        TargetInfo ti = new TargetInfo();
        ti.byteOrder = ((response[2] & 0x01) == 0) ? ByteOrder.LITTLE_ENDIAN : ByteOrder.BIG_ENDIAN;
        ti.maxCto = response[3] & 0xFF;
        if (ti.byteOrder == ByteOrder.LITTLE_ENDIAN) {
            ti.maxDto = (response[4] & 0xFF) + (response[5] << 8);
        } else {
            ti.maxDto = (response[5] & 0xFF) + (response[4] << 8);
        }

        // Validate CTO & DTO configuration
        if (ti.maxCto == 0) {
            throw new IllegalStateException("Controller returned invalid max CTO configuration: " + ti.maxDto);
        } else if (ti.maxCto > XCPLOADER_PACKET_SIZE_MAX) {
            ti.maxCto = XCPLOADER_PACKET_SIZE_MAX;
        }

        if (ti.maxDto == 0 || ti.maxDto > XCPLOADER_PACKET_SIZE_MAX) {
            throw new IllegalStateException("Controller returned invalid max DTO configuration: " + ti.maxDto);
        }

        // Place the target in programming mode
        ti.maxProgCto = programStart();

        mTargetInfo = ti;
    }

    // Place the target in programming mode, and return the maximum programming CTO
    private int programStart() throws IOException {
        byte[] request =
            ByteBuffer.allocate(1)
                .put(XCPLOADER_CMD_PROGRAM_START)
                .array();

        byte[] response = mTransport.sendPacket(request, mSettings.timeoutT6, 7);

        if (response.length != 7) {
            throw new IOException("sendCmdProgramStart() failed due to incorrect length received: " + response.length);
        }

        if (response[0] != XCPLOADER_CMD_PID_RES) {
            throw new IOException("sendCmdProgramStart() failed due to bad response ID received: " + response[0]);
        }

        int maxProgCto = response[3] & 0xFF;
        if (maxProgCto > XCPLOADER_PACKET_SIZE_MAX) {
            throw new IllegalStateException("Controller returned invalid max prog CTO configuration: " + maxProgCto);
        }

        return maxProgCto;
    }

    /**
     * Clear the specified range of memory
     * @param address First address to erase
     * @param len Number of bytes to erase
     */
    public void clearMemory(int address, int len) throws IOException {
//        System.out.println("clearMemory " + address + " len " + len);

        setMta(address);

        byte[] request =
            ByteBuffer.allocate(8)
                .order(mTargetInfo.byteOrder)
                .put(XCPLOADER_CMD_PROGRAM_CLEAR)
                .put((byte)0)
                .put((byte)0)
                .put((byte)0)
                .putInt(len)
                .array();

        byte[] response = mTransport.sendPacket(request, mSettings.timeoutT4, 1);

        if (response.length != 1 || response[0] != XCPLOADER_CMD_PID_RES) {
            throw new IOException("programClear failed for address=0x" + Integer.toString(address, 16) + " length=" + len);
        }
    }

    public void writeData(int address, byte[] data) throws IOException {
        setMta(address);

        int len = data.length;
        int offset = 0;

        while (len > 0) {
            final int maxChunkSize = mTargetInfo.maxProgCto - 1;
            int chunkSize = Math.min(maxChunkSize, len);

            byte[] chunk = Arrays.copyOfRange(data, offset, offset + chunkSize);
            if (chunkSize < maxChunkSize)
                sendCmdProgram(chunk);
            else
                sendCmdProgramMax(chunk);

            offset += chunkSize;
            len -= chunkSize;
        }
    }

    private void sendCmdProgram(byte[] data) throws IOException {

        if (data.length > mTargetInfo.maxProgCto - 2 ||
            mTargetInfo.maxProgCto > XCPLOADER_PACKET_SIZE_MAX) {
            throw new IllegalStateException("CmdProgram: too much data: " + data.length);
        }

        byte[] request =
            ByteBuffer.allocate(2 + data.length)
                .put(XCPLOADER_CMD_PROGRAM)
                .put((byte)data.length)
                .put(data)
                .array();

        byte[] response = mTransport.sendPacket(request, mSettings.timeoutT5, 1);

        if (response.length == 2 && response[0] == XCPLOADER_CMD_PID_ERR && response[1] == XCPLOADER_ERR_CMD_UNKNOWN) {
            throw new IOException("Non secure bootloader?!");
        }

        if (response.length == 1 && response[0] == XCPLOADER_CMD_PID_ERR) {
            throw new IOException("Error programming ECU. Non secure bootloader?!");
        }

        if (response.length != 1 || response[0] != XCPLOADER_CMD_PID_RES) {
            throw new IOException("sendCmdProgram failed");
        }
    }

    private void sendCmdProgramMax(byte[] data) throws IOException {

        if (data.length != mTargetInfo.maxProgCto - 1 ||
            mTargetInfo.maxProgCto > XCPLOADER_PACKET_SIZE_MAX) {
            throw new IllegalStateException("CmdProgramMax: wrong data length: " + data.length);
        }

        byte[] request =
            ByteBuffer.allocate(1 + data.length)
                .put(XCPLOADER_CMD_PROGRAM_MAX)
                .put(data)
                .array();

        byte[] response = mTransport.sendPacket(request, mSettings.timeoutT5, 1);

        if (response.length == 2 && response[0] == XCPLOADER_CMD_PID_ERR && response[1] == XCPLOADER_ERR_CMD_UNKNOWN) {
            throw new IOException("Non secure bootloader?!");
        }

        if (response.length == 1 && response[0] == XCPLOADER_CMD_PID_ERR) {
            throw new IOException("Error programming ECU. Non secure bootloader?!");
        }

        if (response.length != 1 || response[0] != XCPLOADER_CMD_PID_RES) {
            throw new IOException("sendCmdProgramMax failed");
        }
    }

    private void sendCmdProgramReset() throws IOException {
        byte[] request =
            ByteBuffer.allocate(1)
                .put(XCPLOADER_CMD_PROGRAM_RESET)
                .array();

        byte[] response;

        try {
            response = mTransport.sendPacket(request, mSettings.timeoutT6, 1);
        } catch (IOException e) {
            log.info("sendCmdProgramReset not a problem: " + e);
            // Eat any exception, it's fine if this fails
            return;
        }

        if (response.length != 1) {
            throw new IOException("sendCmdProgramReset() failed due to incorrect length received: " + response.length);
        }

        if (response[0] != XCPLOADER_CMD_PID_RES) {
            throw new IOException("sendCmdProgramReset() failed due to bad response ID received: " + response[0]);
        }
    }

    private void setMta(int address) throws IOException {
        byte[] request =
            ByteBuffer.allocate(8)
                .order(mTargetInfo.byteOrder)
                .put(XCPLOADER_CMD_SET_MTA)
                .put((byte)0)
                .put((byte)0)
                .put((byte)0)
                .putInt(address)
                .array();

        byte[] response = mTransport.sendPacket(request, mSettings.timeoutT1, 1);

        if (response.length != 1 || response[0] != XCPLOADER_CMD_PID_RES) {
            throw new IOException("setMta failed for address " + address);
        }
    }
}
