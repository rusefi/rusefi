package com.rusefi.binaryprotocol.test;

import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.can.Elm327Connector;
import com.rusefi.io.serial.BaudRateHolder;
import com.rusefi.io.serial.SerialIoStream;

import java.io.IOException;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;
import static com.rusefi.io.can.Elm327Connector.ELM327_DEFAULT_BAUDRATE;

public class Elm327Sandbox {
    public static void main(String[] args) throws InterruptedException, IOException {
        BaudRateHolder.INSTANCE.baudRate = ELM327_DEFAULT_BAUDRATE;
        String serialPort = "COM7";
        Elm327Connector connector = new Elm327Connector(SerialIoStream.openPort(serialPort));
        boolean initConnection = connector.start(serialPort);
        if (!initConnection)
            return;

        IoStream tsStream = connector.getTsStream();

        IncomingDataBuffer dataBuffer = tsStream.getDataBuffer();
        System.out.println("Hello new ELM327 connection, pending=" + dataBuffer.getPendingCount());

        /*
        runFcommand("First time", tsStream);
        Elm327Connector.whyDoWeNeedToSleepBetweenCommands();

        runFcommand("Second time", tsStream);
        Elm327Connector.whyDoWeNeedToSleepBetweenCommands();
*/

        SandboxCommon.verifySignature(tsStream, "", "ELM");
        Elm327Connector.whyDoWeNeedToSleepBetweenCommands();

        SandboxCommon.verifySignature(tsStream, "Let's do it again! ", "ELM");
        Elm327Connector.whyDoWeNeedToSleepBetweenCommands();

        {
            tsStream.sendPacket(new byte[]{Fields.TS_HELLO_COMMAND});
            byte[] response = dataBuffer.getPacket("[hello command]");
            if (!checkResponseCode(response, (byte) Fields.TS_RESPONSE_OK))
                return;
            String signature = new String(response, 1, response.length - 1);
            System.out.println(Fields.TS_HELLO_COMMAND + " returned " + signature);

            if (!signature.startsWith(Fields.PROTOCOL_SIGNATURE_PREFIX))
                throw new IllegalStateException("Unexpected S " + signature);
        }

        Elm327Connector.whyDoWeNeedToSleepBetweenCommands();
        System.out.println("****************************************");
        System.out.println("********  ELM327 LOOKS GREAT  **********");
        System.out.println("****************************************");
        System.exit(-1);

        /*
        {
            tsStream.sendPacket(BinaryProtocol.createCrcCommand(1000));
            byte[] fResponse = new byte[3];
            dataBuffer.waitForBytes("CRC", System.currentTimeMillis(), fResponse.length);
            dataBuffer.getData(fResponse);
            System.out.println(" Got CRC response " + IoStream.printByteArray(fResponse));
        }

        LinkManager linkManager = new LinkManager();
        StreamConnector streamConnector = new StreamConnector(linkManager, () -> tsStream);
        linkManager.setConnector(streamConnector);
        streamConnector.connectAndReadConfiguration(new ConnectionStateListener() {
            @Override
            public void onConnectionEstablished() {
                System.out.println("onConnectionEstablished");
            }

            @Override
            public void onConnectionFailed() {
                System.out.println("onConnectionFailed");
            }
        });
*/
    }

    private static void runFcommand(String prefix, IoStream tsStream) throws IOException {
        IncomingDataBuffer dataBuffer = tsStream.getDataBuffer();
        tsStream.write(new byte[]{Fields.TS_COMMAND_F});
        tsStream.flush();
        byte[] fResponse = new byte[3];
        dataBuffer.waitForBytes("hello", System.currentTimeMillis(), fResponse.length);
        dataBuffer.getData(fResponse);
        System.out.println(prefix + " Got F response " + IoStream.printByteArray(fResponse));
    }

}
