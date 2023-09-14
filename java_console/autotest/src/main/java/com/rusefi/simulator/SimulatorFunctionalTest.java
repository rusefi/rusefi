package com.rusefi.simulator;

import com.rusefi.io.LinkManager;

import static com.rusefi.config.generated.Fields.TS_SIMULATE_CAN;

public class SimulatorFunctionalTest {
    private final LinkManager linkManager;

    public SimulatorFunctionalTest(LinkManager linkManager) {
        this.linkManager = linkManager;
    }

    public void mainTestBody() throws InterruptedException {
        linkManager.submit(new Runnable() {
            @Override
            public void run() {
                byte[] response = linkManager.getBinaryProtocol().executeCommand(TS_SIMULATE_CAN, "TS_SIMULATE_CAN");

                System.out.println(response);

            }
        });
        Thread.sleep(10000);


    }
}
