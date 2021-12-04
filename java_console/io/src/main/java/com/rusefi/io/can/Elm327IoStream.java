package com.rusefi.io.can;

import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IoHelper;
import com.rusefi.io.ByteReader;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.shared.FileUtil;
import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.nio.ByteBuffer;
import java.util.Arrays;

public class Elm327IoStream extends AbstractIoStream {
    private final String loggingPrefix;
    private final Elm327Connector con;
    private final DisconnectListener disconnectListener;
    @NotNull
    private final IncomingDataBuffer dataBuffer;
    private DataListener dataListener;

    // the buffer size is limited by CAN-TP protocol
    private final static int OUT_BUFFER_SIZE = 4095;
    private ByteBuffer inBuf;
    private ByteBuffer outBuf;

    // this should match the TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME in the firmware
    private final static boolean sendShortPacketsInOneFrame = true;
    private final static boolean receiveShortPacketsInOneFrame = false;

	private Elm327Connector.CanDecoder canDecoder = new Elm327Connector.CanDecoder();


    public Elm327IoStream(Elm327Connector con, String loggingPrefix) throws IOException {
        this(con, loggingPrefix, DisconnectListener.VOID);
    }

    public Elm327IoStream(Elm327Connector con, String loggingPrefix, DisconnectListener disconnectListener) throws IOException {
        this.con = con;
        this.loggingPrefix = loggingPrefix;
        this.disconnectListener = disconnectListener;
        this.dataBuffer = IncomingDataBuffer.createDataBuffer(loggingPrefix, this);

        inBuf = ByteBuffer.allocate(OUT_BUFFER_SIZE);
        outBuf = ByteBuffer.allocate(OUT_BUFFER_SIZE);
    }

    @Override
    public void close() {
        // we need to guarantee only one onDisconnect invocation for retry logic to be healthy
        synchronized (this) {
            if (!isClosed()) {
                super.close();
                disconnectListener.onDisconnect("on close");
            }
        }
    }

    @Override
    public String getLoggingPrefix() {
        return loggingPrefix;
    }

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

    		for (int i = 0; i < data.length; i++) {
	    		packet[i + 2] = data[i];
	    	}
	    	int crc = IoHelper.getCrc32(data);
	    	IoHelper.putInt(packet, 2 + data.length, crc);
		} else {
			// long packets are sent as-is
			packet = data;
		}

    	dataListener.onDataArrived(packet);
    }

    public void processCanPacket(byte [] data) throws Exception {
    	byte [] rawData = canDecoder.decodePacket(data);
    	sendDataToClient(rawData);
    }

    public interface DisconnectListener {
        DisconnectListener VOID = (String message) -> {

        };
        void onDisconnect(String message);
    }
}
