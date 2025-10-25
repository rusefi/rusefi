package com.rusefi.wizard;

import java.util.function.Supplier;

import com.devexperts.logging.Logging;
import com.efiAnalytics.plugin.ecu.ControllerAccess;

import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;

public class BackgroundWizard {
    private static final Logging log = Logging.getLogging(BackgroundWizard.class);
    private static Supplier<ControllerAccess> controllerAccessSupplier;

    public static void start(Supplier<ControllerAccess> controllerAccessSupplier) {
        BackgroundWizard.controllerAccessSupplier = controllerAccessSupplier;
        Thread thread = new Thread(() -> {
            while (true) {
                try {
                    periodicWizardLogic();
                } catch (Throwable e) {
                    log.error("error " + e, e);
                }
                sleep(300);
            }
        });
        thread.setName("wizard-thread");
        thread.setDaemon(true);
        thread.start();
    }

    private static void periodicWizardLogic() {
        // todo: check if ECU is connected
        // todo: run logic
        // todo: use something based on TunerStudioIntegration to actually open dialog!
    }
}
