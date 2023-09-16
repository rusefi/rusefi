package com.rusefi.simulator;

import com.rusefi.io.LinkManager;
import etch.util.CircularByteBuffer;

import java.io.EOFException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static com.rusefi.binaryprotocol.IoHelper.swap16;
import static com.rusefi.config.generated.Fields.TS_SIMULATE_CAN;

public class SimulatorFunctionalTest {
    private final LinkManager linkManager;

    public SimulatorFunctionalTest(LinkManager linkManager) {
        this.linkManager = linkManager;
    }

    public void mainTestBody() throws InterruptedException {
        CountDownLatch gotCan = new CountDownLatch(1);
        linkManager.submit(new Runnable() {
            @Override
            public void run() {
                byte[] response = linkManager.getBinaryProtocol().executeCommand(TS_SIMULATE_CAN, "TS_SIMULATE_CAN");

                if (response == null) {
                    return;
                }
                CircularByteBuffer c = new CircularByteBuffer(response);
                try {
                    int count = swap16(c.getShort());

                    c.get();
                    int dataLength = c.get();
                    c.get();
                    int ide = c.get();
                    int eid = c.getInt();

                    System.out.println("Got " + count + " packets");
                    System.out.println(response);
                    gotCan.countDown();

                } catch (EOFException e) {
                    throw new IllegalStateException(e);
                }


            }
        });
        gotCan.await(1, TimeUnit.MINUTES);


    }
}
