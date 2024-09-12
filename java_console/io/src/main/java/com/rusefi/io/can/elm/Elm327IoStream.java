package com.rusefi.io.can.elm;

import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IoHelper;
import com.rusefi.io.can.isotp.IsoTpCanDecoder;
import com.rusefi.io.serial.AbstractIoStream;
import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.nio.ByteBuffer;
import java.util.Arrays;

/**
 *
 */
/*
public class Elm327IoStream extends AbstractIoStream {
    private final Elm327Connector con;
    @NotNull
    private final IncomingDataBuffer dataBuffer;
    private DataListener dataListener;

    // the buffer size is limited by CAN-TP protocol
    private final static int OUT_BUFFER_SIZE = 4095;
    private final ByteBuffer outBuf;

    // this should match the TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME in the firmware
    // todo: move this to rusefi_config.txt / prepend.txt?
    private final static boolean sendShortPacketsInOneFrame = false;
    //private final static boolean receiveShortPacketsInOneFrame = sendShortPacketsInOneFrame;
    private final static boolean receiveShortPacketsInOneFrame = false;

	private final IsoTpCanDecoder canDecoder = new IsoTpCanDecoder();

    public Elm327IoStream(Elm327Connector con) {
        this.con = con;
        dataBuffer = createDataBuffer();

//        ByteBuffer inBuf = ByteBuffer.allocate(OUT_BUFFER_SIZE);
        outBuf = ByteBuffer.allocate(OUT_BUFFER_SIZE);
    }

    @Override
    public void close() {
        // we need to guarantee only one onDisconnect invocation for retry logic to be healthy
        synchronized (this) {
            if (!isClosed()) {
                super.close();
            }
        }
    }

    @NotNull
    @Override
    public IncomingDataBuffer getDataBuffer() {
        return dataBuffer;
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        super.write(bytes);

    	int offset = 0;
    	int numBytes = bytes.length;
    	// split the data if it doesn't fit in our out-buffer
    	while (numBytes > outBuf.remaining()) {
    		int remaining = outBuf.remaining();
    		//log.info("* remaining= "+remaining + " numBytes="+numBytes+" offset="+offset);
    		outBuf.put(bytes, offset, remaining);
    		numBytes -= remaining;
    		offset += remaining;
			flush();
    	}
    	//log.info("* rest numBytes="+numBytes+" offset="+offset);
        outBuf.put(bytes, offset, numBytes);
    }

    @Override
    public void flush() throws IOException {
        super.flush();
        byte [] bytes;
        // for smaller packets, send them in one 'simple' frame by stripping the header+footer off
        // i.e. un-pack CRC32 TS protocol
        // (2 = 16-bit length, 4 = 32-bit crc)
        if (sendShortPacketsInOneFrame && outBuf.position() >= 2 + 1 + 4 && outBuf.position() <= 2 + 7 + 4) {
        	bytes = Arrays.copyOfRange(outBuf.array(), 2, outBuf.position() - 4);
        } else {
        	bytes = Arrays.copyOf(outBuf.array(), outBuf.position());
        }
        con.sendBytesToSerial(bytes);
        outBuf.clear();
    }

    @Override
    public void setInputListener(final DataListener listener) {
    	dataListener = listener;
    }

    public void sendDataToClient(byte [] data) {
    	// reconstruct a TS packet from the raw data
    	byte [] packet;
    	if (receiveShortPacketsInOneFrame && data.length >= 1 && data.length <= 7) {
    		// short packets are sent without the header+footer, we need to reconstruct them
	    	packet = new byte [2 + data.length + 4];
			IoHelper.putShort(packet, 0, data.length);

            System.arraycopy(data, 0, packet, 2, data.length);
	    	int crc = IoHelper.getCrc32(data);
	    	IoHelper.putInt(packet, 2 + data.length, crc);
		} else {
			// long packets are sent as-is
			packet = data;
		}

    	dataListener.onDataArrived(packet);
    }

    public void processCanPacket(byte [] data) {
    	byte [] rawData = canDecoder.decodePacket(data);
        if (rawData.length != 0)
            sendDataToClient(rawData);
    }
}
*/
