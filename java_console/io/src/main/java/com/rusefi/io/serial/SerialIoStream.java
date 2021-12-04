package com.rusefi.io.serial;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;

public class SerialIoStream extends AbstractIoStream {
    protected final SerialPort sp;
    protected final String port;

    public SerialIoStream(SerialPort sp, String port) {
        this.sp = sp;
        this.port = port;
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        throw new UnsupportedOperationException();
    }

    @Override
    public String getLoggingPrefix() {
        return "";
    }

    @Override
    public void close() {
        log.info(port + ": Closing port...");
        super.close();
        sp.closePort();
        log.info(port + ": Closed port.");
    }

    @Override
    public void write(byte[] bytes) {
        sp.writeBytes(bytes, bytes.length);
    }

    @Override
    public void setInputListener(DataListener listener) {
        // datalistener can be redefined
        sp.removeDataListener();
        sp.addDataListener(new SerialPortDataListener() {
            private boolean isFirstEvent = true;

            @Override
            public int getListeningEvents() {
                return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
            }

            @Override
            public void serialEvent(SerialPortEvent event) {
                if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE)
                    return;
                if (isFirstEvent) {
                    // a hack to have explicit thread name, see https://github.com/Fazecast/jSerialComm/issues/308
                    Thread.currentThread().setName("Serial Port Event Thread");
                    isFirstEvent = false;
                }
                int bytesAvailable = sp.bytesAvailable();
                if (bytesAvailable <= 0)
                    return; // sometimes negative value is returned at least on Mac
                byte[] newData = new byte[bytesAvailable];
                int numRead = sp.readBytes(newData, newData.length);
                byte[] data = new byte[numRead];
                System.arraycopy(newData, 0, data, 0, numRead);
                listener.onDataArrived(data);
                //System.out.println("Read " + numRead + " bytes.");
            }
        });

    }

}
