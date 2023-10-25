package com.rusefi.simulator;

import com.rusefi.config.generated.Fields;
import com.rusefi.enums.bench_mode_e;
import com.rusefi.enums.bench_test_magic_numbers_e;
import com.rusefi.enums.bench_test_packet_ids_e;
import com.rusefi.enums.bench_test_io_control_e;
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
    private int pinToggleCounter = 0;
    private int [] durationsInStateMs = { 0, 0 };

    public SimulatorFunctionalTest(LinkManager linkManager) {
        this.linkManager = linkManager;
    }

    public void mainTestBody() throws InterruptedException {
        assertRawAnalogPackets();
        testOutputPin(bench_mode_e.BENCH_MAIN_RELAY, Fields.BENCH_MAIN_RELAY_DURATION);
        testOutputPin(bench_mode_e.BENCH_FUEL_PUMP, Fields.BENCH_FUEL_PUMP_DURATION);
        testOutputPin(bench_mode_e.BENCH_FAN_RELAY, Fields.BENCH_FAN_DURATION);
        testOutputPin(bench_mode_e.BENCH_AC_COMPRESSOR_RELAY, Fields.BENCH_AC_RELAY_DURATION);
        testOutputPin(bench_mode_e.BENCH_STARTER_ENABLE_RELAY, Fields.BENCH_STARTER_DURATION);
// todo: fix me as well!        testOutputPin(bench_mode_e.BENCH_VVT0_VALVE, Fields.BENCH_VVT_DURATION);
    }

    private int store8bit(byte [] buf, int offset, int int8) {
        buf[offset++] = (byte)int8;
        return offset;
    }

    private int store32bit(byte [] buf, int offset, int int32) {
        buf[offset++] = (byte)(int32 >> 24);
        buf[offset++] = (byte)(int32 >> 16);
        buf[offset++] = (byte)(int32 >> 8);
        buf[offset++] = (byte)(int32);
        return offset;
    }

    private void exchangeCanPackets(CountDownLatch gotCan,
                                    bench_test_packet_ids_e [] expectedEids,
                                    byte[][] packets) throws InterruptedException {
        linkManager.submit(new Runnable() {
            @Override
            public void run() {
                // calc total number of bytes
                int numBytes = 1 + (4 * expectedEids.length) + 1;
                for (byte [] packet : packets) {
                    numBytes += packet.length;
                }
                byte [] allData = new byte [numBytes];
                // store EID list
                int offset = store8bit(allData, 0, expectedEids.length);
                for (bench_test_packet_ids_e eid  : expectedEids) {
                    offset = store32bit(allData, offset, eid.get());
                }
                // store the number of packets
                offset = store8bit(allData, offset, packets.length);

                // copy the packet data
                for (byte [] packet : packets) {
                    System.arraycopy(packet, 0, allData, offset, packet.length);
                    offset += packet.length;
                }
                // exchange data
                byte[] response = linkManager.getBinaryProtocol().executeCommand(TS_SIMULATE_CAN, allData, "TS_SIMULATE_CAN");

                if (response == null) {
                    return;
                }
                CircularByteBuffer c = new CircularByteBuffer(response);
                try {
                    int count = swap16(c.getShort());

                    c.get();
                    for (int idx = 0; idx < count; idx++) {
                        int dataLength = c.get() & 0xf;
                        c.get();
                        c.get();
                        c.get();
                        int eid = c.get();
                        eid |= c.get() << 8;
                        eid |= c.get() << 16;
                        c.get();
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
    }

    private byte [] getCanFrameData(int eid, byte [] msg) {
        byte [] packet  = {
                0, // FMI
                0, 0, // TIME
                (1 << 5) | 8, // bitfields: 0..3: DLC=8, 4: RTR=0, 5: IDE=1
                (byte)(eid & 0xff),
                (byte)((eid >> 8) & 0xff),
                (byte)((eid >> 16) & 0xff),
                (byte)((eid >> 24) & 0xff),
                // data (reserve space for arraycopy)
                0, 0, 0, 0, 0, 0, 0, 0
        };

        if (msg.length > 8) {
            throw new RuntimeException("msg.length > 8");
        }

        System.arraycopy(msg, 0, packet, packet.length - 8, msg.length);
        return packet;
    }

    private void executeIoControlCommand(bench_test_io_control_e command,
                                         bench_test_packet_ids_e [] expectedEids,
                                         byte subCommand) throws InterruptedException {
        CountDownLatch gotCan = new CountDownLatch(1);
        byte [][] packets = new byte [1][];
        packets[0] = getCanFrameData(bench_test_packet_ids_e.IO_CONTROL.get(),
                new byte [] {
                        (byte)bench_test_magic_numbers_e.BENCH_HEADER.get(),
                        (byte)command.get(),
                        subCommand,
                });
        exchangeCanPackets(gotCan, expectedEids, packets);
        gotCan.await(1, TimeUnit.MINUTES);

    }

    private void assertRawAnalogPackets() throws InterruptedException {
        CountDownLatch gotCan1 = new CountDownLatch(1);
        exchangeCanPackets(gotCan1,
                new bench_test_packet_ids_e[] { bench_test_packet_ids_e.RAW_ANALOG_1, bench_test_packet_ids_e.RAW_ANALOG_2 },
                new byte [0][]);
        gotCan1.await(1, TimeUnit.MINUTES);

        // assert RAW_ANALOG can packets have arrived
        if (!gotCanPacketAnalog1 || !gotCanPacketAnalog2) {
            throw new IllegalStateException("Didn't receive analog CAN packets! "
                    + gotCanPacketAnalog1 + " " + gotCanPacketAnalog2);
        }
    }

private void testOutputPin(bench_mode_e pinId, int stateToggleTimeMs) throws InterruptedException {
        // gain pin control and reset pin stats (otherwise pin can be randomly toggled before or during the test)
        executeIoControlCommand(bench_test_io_control_e.CAN_BENCH_START_PIN_TEST,
                new bench_test_packet_ids_e[] { }, (byte)pinId.ordinal());

        // request "default" pin state for the fuel pump
        executeIoControlCommand(bench_test_io_control_e.CAN_BENCH_QUERY_PIN_STATE,
                new bench_test_packet_ids_e[] { bench_test_packet_ids_e.PIN_STATE },
                (byte)pinId.ordinal());

        int defaultPinToggleCounter = pinToggleCounter;
// todo: unused variable?       int [] defaultDurationsInStateMs = { durationsInStateMs[0], durationsInStateMs[1] };

        // execute the test (toggle the pin)
        executeIoControlCommand(bench_test_io_control_e.CAN_BENCH_EXECUTE_BENCH_TEST,
                new bench_test_packet_ids_e[] { }, (byte)pinId.ordinal());

        // sleep 2 cycles of BENCH_FUEL_PUMP_DURATION + extra half second
        Thread.sleep(stateToggleTimeMs * 2 + 500);

        // request the pin state saved right after the test
        executeIoControlCommand(bench_test_io_control_e.CAN_BENCH_END_PIN_TEST,
                new bench_test_packet_ids_e[] { bench_test_packet_ids_e.PIN_STATE }, (byte)pinId.ordinal());

        // this check assumes that simulator makes 2 iterations for each pin
        // (this magic is needed because we don't know the previous pin state before starting the test)
        if (pinToggleCounter < defaultPinToggleCounter + 3 || pinToggleCounter > defaultPinToggleCounter + 4) {
            throw new IllegalStateException(pinId + ": Unexpected pin toggle counter: before="
                    + defaultPinToggleCounter + " after=" + pinToggleCounter);
        }

        if (!nearEq(durationsInStateMs[1], stateToggleTimeMs, 10)) {
            throw new IllegalStateException(pinId + ": Unexpected pin state duration: [0]="
                    + durationsInStateMs[0] + " [1]=" + durationsInStateMs[1]
                    + " toggleTimeMs=" + stateToggleTimeMs);
        }
    }
    
    private int readInt(byte[] data, int startIdx, int endIdx) {
        int v = 0;
        for (int i = startIdx; i <= endIdx; i++) {
            v |= (data[i] & 0xff) << (8 * (endIdx - i));
        }
        return v;
    }

    private void processCanPacket(int eid, byte[] data) {
        if (eid == bench_test_packet_ids_e.RAW_ANALOG_1.get()) {
            gotCanPacketAnalog1 = true;
        } else if (eid == bench_test_packet_ids_e.RAW_ANALOG_2.get()) {
            gotCanPacketAnalog2 = true;
        } else if (eid == bench_test_packet_ids_e.PIN_STATE.get()) {
            pinToggleCounter = readInt(data, 0, 1);
            durationsInStateMs[0] = readInt(data, 2, 4);
            durationsInStateMs[1] = readInt(data, 5, 7);
        }
    }

    private boolean nearEq(int value1, int value2, int tolerance) {
        return Math.abs(value1 - value2) <= tolerance;
    }
}
