package com.rusefi.test;

import com.rusefi.*;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;

import java.util.List;

public class BltSwitchSandbox {
    public static void main(String[] args) throws InterruptedException {
        SerialPortScanner serialPortScanner = ConnectivityContext.INSTANCE.getSerialPortScanner();

        ConnectionAndMeta.getProperties().setProperty(UiProperties.SKIP_ECU_TYPE_DETECTION, "true");

        serialPortScanner.addListener(new SerialPortScanner.Listener() {
            @Override
            public void onChange(AvailableHardware currentHardware) {
                System.out.println(currentHardware);

                List<PortResult> knownPorts = currentHardware.getKnownPorts();
                if (knownPorts.isEmpty())
                    return;
                DfuFlasher.rebootToDfu(null, knownPorts.get(0).port, UpdateOperationCallbacks.LOGGER, Integration.CMD_REBOOT_OPENBLT);


            }
        });

        while (true) {
            Thread.sleep(200);
        }
    }
}
