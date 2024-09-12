package com.rusefi.io.can.elm;

import com.devexperts.logging.Logging;
import com.opensr5.io.DataListener;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.can.HexUtil;
import com.rusefi.io.can.isotp.IsoTpConnector;
import com.rusefi.util.HexBinary;
import com.rusefi.io.IoStream;
import org.jetbrains.annotations.NotNull;

import java.io.Closeable;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static com.rusefi.Timeouts.SECOND;
/*
public class Elm327Connector implements Closeable {
	private final static Logging log = Logging.getLogging(Elm327Connector.class);

	static {
		log.configureDebugEnabled(false);
	}

	//	public final static int ELM327_DEFAULT_BAUDRATE = 115200; // OBDlink SX, 1.3a STN1110
    public final static int ELM327_DEFAULT_BAUDRATE = 38400;
    private final static int BIG_TIMEOUT = 2 * SECOND;
    private final static int TIMEOUT = 70;
	public static final String HELLO = "ATZ";
	public static final String ELM_EOL = "\r";
	// those are inverted between ECU side and PC side
	private static final int CAN_PC_SERIAL_RX_ID = Fields.CAN_ECU_SERIAL_TX_ID;
	private static final int CAN_PC_SERIAL_TX_ID = Fields.CAN_ECU_SERIAL_RX_ID;

	private final Object lock = new Object();

//	 * Connection to ELM327 device where text based ELM protocol happens
//
	private final IoStream underlyingStream;
//	 * Binary serial stream for TS traffic
	private final Elm327IoStream tsStream;
	private String partialLine = "";
	private final List<String> completeLines = new ArrayList<>();
	private boolean isCommandMode = false;

	public Elm327Connector(IoStream underlyingStream) {
		this.underlyingStream = underlyingStream;
		underlyingStream.setInputListener(listener);
		tsStream = new Elm327IoStream(this);
	}

//	 * TODO: HUH? what's that about?!
	public static void whyDoWeNeedToSleepBetweenCommands() {
		try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
			throw new IllegalStateException(e);
		}
	}

	public Elm327IoStream getTsStream() {
		return tsStream;
	}

	public boolean start(String msg) {
    	log.info("* Elm327.start()");

		boolean initConnection = initConnection(msg);
		if (initConnection) {
        	// reset to defaults
        	sendCommand("ATD", "OK");

        	// Echo off
        	sendCommand("ATE0", "OK");
        	//waitForEcho = false;

        	// protocol #6 - ISO 15765-4 CAN (11 bit ID, 500 kbaud)
			sendCommand("ATSP6", "OK");

			// set rx ID
			sendCommand("ATCF " + Integer.toHexString(CAN_PC_SERIAL_RX_ID), "OK");

			// rx ID mask = "all bits set"
			sendCommand("ATCM FFF", "OK");

			// set tx ID
			sendCommand("ATSH " + Integer.toHexString(CAN_PC_SERIAL_TX_ID), "OK");

			// set FC tx ID (should match our tx ID)
			sendCommand("ATFCSH " + Integer.toHexString(CAN_PC_SERIAL_TX_ID), "OK");
			// set FC data
			sendCommand("ATFCSD 30 00 00", "OK");
			// use custom FC ID & data
			sendCommand("ATFCSM 1", "OK");

			// disable data auto-formatting (less bytes in a stream)
			sendCommand("ATCAF 0", "OK");

			// allow "long" 8-byte messages
			sendCommand("ATAL", "OK");

			// disable spaces ("00 11 22" => "001122")
			sendCommand("ATS 0", "OK");

			// disable silent monitoring (ELM will send ACK)
			sendCommand("ATCSM 0", "OK");

			// read the ignition voltage
			String voltage = sendCommand("ATRV", "([0-9\\.]+)V");
			log.info("* Ignition voltage = " + voltage);
        }
        return initConnection;
    }

    @Override
    public void close() {
    	log.info("* Elm327.close()");
    	if (underlyingStream != null)
    		underlyingStream.close();
    }

	private final DataListener listener = freshData -> {
    	// ELM327 uses a text protocol, so we convert the data to a string
    	String freshStr = new String(freshData);
    	while (true) {
	    	int newL = freshStr.indexOf('\r');
    	    //log.info("* onData: " + newL + " [" + freshStr + "]");

	        // split the stream into separate lines
    	    if (newL >= 0) {
        		String curLine = this.partialLine;
	        	this.partialLine = "";
    	    	if (newL > 0)
        		    curLine += freshStr.substring(0, newL);
    	        if (curLine.length() > 0)
        	    	processLine(curLine);
            	freshStr = freshStr.substring(newL + 1);
            	continue;
	        }
			this.partialLine += freshStr;
			break;
		}
	};

	private final IsoTpConnector connector = new IsoTpConnector(Fields.CAN_ECU_SERIAL_RX_ID) {
		@Override
		public void sendCanData(byte[] total) {
			Elm327Connector.this.sendCanData(total);
		}

		@Override
		public void receiveData() {
			Elm327Connector.this.receiveData();
		}
	};

	public void sendBytesToSerial(byte [] bytes) {
        IsoTpConnector.sendStrategy(bytes, connector);
    }

    ///////////////////////////////////////////////////////

    private boolean initConnection(String msg) {
        if (sendCommand(HELLO, "ELM327 v[0-9]+\\.[0-9]+", BIG_TIMEOUT) != null) {
        	log.info("ELM DETECTED on " + msg + "! " + ELM327_DEFAULT_BAUDRATE);
        	return true;
        }
		log.info("ELM NOT FOUND on " + msg + "! " + ELM327_DEFAULT_BAUDRATE);
		return false;
    }

    private String sendCommand(String command, String responseFilter) {
    	return sendCommand(command, responseFilter, TIMEOUT);
    }

    private String sendCommand(String command, String responseFilter, int timeout) {
    	log.info("* Elm327.cmd: " + command);
    	isCommandMode = true;
    	this.completeLines.clear();
       	try {
       		underlyingStream.write((command + ELM_EOL).getBytes());
       		waitForResponse(timeout);
	    } catch (IOException | InterruptedException ignore) {
	        return null;
	    } finally {
	    	isCommandMode = false;
	    }

	    if (this.completeLines.size() < 1) {
	    	return null;
	    }

        int responseIdx = 0;
		// the first line can repeat the command (if echo is on)
        if (this.completeLines.get(0).equals(command)) {
        	// then the response is in the 2nd line
        	responseIdx = 1;
        }
        // return the response only if it matches the given regexp
        Pattern pattern = Pattern.compile(responseFilter);
		String inputLine = completeLines.get(responseIdx);
		Matcher matcher = pattern.matcher(inputLine);
		if (matcher.find()) {
        	// store the echo mode
        	//this.waitForEcho = responseIdx != 0;

        	return (matcher.groupCount() > 0) ? matcher.group(1) : matcher.group();
        }
        return null;
    }

    private void sendCanData(byte [] data) {
        if (log.debugEnabled()) {
            log.debug("sendCanData total  " + HexBinary.printByteArray(data));
        }
    	//log.info("--------sendData offset="+Integer.toString(offset) + " len=" + Integer.toString(len) + "hdr.len=" + Integer.toString(hdr.length));

		byte[] hexData = byteToString(data);

   		//log.info("* Elm327.data: " + (new String(hexData)));

		try {
       		underlyingStream.write(hexData);
	    } catch (IOException ignore) {
			// ignore
	    }
    }

	@NotNull
	public static byte[] byteToString(byte[] data) {
		int length = data.length;
		byte[] hexData = new byte[length * 2 + 1];
		for (int i = 0; i < length; i++) {
			int j = i * 2;
			int v = data[i] & 0xFF;
			hexData[j] = HexUtil.HEX_BYTE_ARRAY[v >>> 4];
			hexData[j + 1] = HexUtil.HEX_BYTE_ARRAY[v & 0x0F];
		}
		hexData[length * 2] = '\r';
		return hexData;
	}

	private byte[] receiveData() {
        synchronized (lock) {
            try {
                waitForResponse(TIMEOUT);
                //log.info("Elm327Connector.receiveData(): size=" + this.completeLines.size());
                return null;
            } catch (InterruptedException ignore) {
                return null;
            }
        }
    }

    private void waitForResponse(int timeout) throws InterruptedException {
        synchronized (lock) {
            // multiple lines can be sent, we need to wait for them all
            while (true) {
                int numLines = completeLines.size();
                lock.wait(timeout);
                // if nothing changed
                if (completeLines.size() == numLines)
                    break;
            }
        }
    }

    private void processLine(String line) {
        synchronized (lock) {
            log.info("Elm327Connector.processLine(): {" + line + "}");

            // remove the 'cursor'
            if (line.charAt(0) == '>')
                line = line.substring(1);

            if (isCommandMode) {
                // store the output as a response to the command (for verification)
                this.completeLines.add(line);
                lock.notifyAll();
            } else {
                // just send it back to the proxy
                sendDataBack(line);
            }
        }
    }

    private void sendDataBack(String line) {
		byte [] canPacket = HexUtil.asBytes(line);
		tsStream.processCanPacket(canPacket);
    }

//	public static boolean checkConnection(String serialPort, IoStream stream) {
//		Elm327Connector con = new Elm327Connector(stream);
//		boolean found = con.initConnection(serialPort);
//		con.close();
//		return found;
//	}
}
*/
