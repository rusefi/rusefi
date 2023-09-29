package com.rusefi.simulator;

import com.rusefi.enums.bench_test_packet_ids_e;
import com.rusefi.io.LinkManager;
import etch.util.CircularByteBuffer;

import java.io.EOFException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static com.rusefi.binaryprotocol.IoHelper.swap16;
import static com.rusefi.config.generated.Fields.TS_SIMULATE_CAN;

public class SimulatorFunctionalTest {
    private final LinkManager linkManager;
    private boolean gotCanPacketAnalog1 = false;
    private boolean gotCanPacketAnalog2 = false;

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

                    for (int idx = 0; idx < count; idx++) {
                        c.get();
                        int dataLength = c.get() & 0xf;
                        c.get();
                        c.get();
                        c.get();
                        int eid = c.get();
                        eid |= c.get() << 8;
                        eid |= c.get() << 16;
                        byte[] data = new byte[dataLength];
                        c.get(data);
                        processCanPacket(eid, data);
                    }


                    System.out.println("Got " + count + " packets");
                    gotCan.countDown();

                } catch (EOFException e) {
                    throw new IllegalStateException(e);
                }


            }
        });
        gotCan.await(1, TimeUnit.MINUTES);

        // assert RAW_ANALOG can packets have arrived
        if (!gotCanPacketAnalog1 || !gotCanPacketAnalog2) {
            throw new IllegalStateException("Didn't receive analog CAN packets! "
                    + gotCanPacketAnalog1 + " " + gotCanPacketAnalog2);
        }


        // todo send new CAN command "request pin state for bench_mode_e pin BENCH_FUEL_PUMP

        // todo ecu.sendCommand(CMD_FUEL_PUMP_BENCH




    }

    private void processCanPacket(int eid, byte[] data) {
        if (eid == bench_test_packet_ids_e.RAW_ANALOG_1.get()) {
            gotCanPacketAnalog1 = true;
        } else if (eid == bench_test_packet_ids_e.RAW_ANALOG_2.get()) {
            gotCanPacketAnalog2 = true;
        }
    }
}
