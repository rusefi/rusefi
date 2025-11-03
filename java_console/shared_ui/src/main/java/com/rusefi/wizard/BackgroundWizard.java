package com.rusefi.wizard;

import java.util.function.Supplier;

import com.devexperts.logging.Logging;
import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.efiAnalytics.plugin.ecu.ControllerException;
import com.efiAnalytics.plugin.ecu.ControllerParameter;
import com.efiAnalytics.plugin.ecu.OutputChannelClient;

import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;

public class BackgroundWizard {
    private static final String ECU_VIN_KEY = "vinNumber";

    private static final Logging log = Logging.getLogging(BackgroundWizard.class);
    private static Supplier<ControllerAccess> controllerAccessSupplier;
    static OutputChannelClient onlineListener = new EcuOnlineListener();

    private static int CURRENT_STATE_UNKNOWN = -1;
    private static int CURRENT_STATE_OFFLINE = 0;
    private static int CURRENT_STATE_ONLINE = 1;
    private static int currentState = -1;
    private static int lastState = -1;
    private static boolean pluginEnabled = false;
    private static boolean ecuVinToogle = true;
    private static String ecuVin = null;


    public static void start(Supplier<ControllerAccess> controllerAccessSupplier) {
        BackgroundWizard.controllerAccessSupplier = controllerAccessSupplier;
        try {
            BackgroundWizard.controllerAccessSupplier.get().getOutputChannelServer().subscribe("AppEvent", "controllerOnline", onlineListener);
        } catch(Exception e) {
            log.error("error on onlineListener " + e, e);
        }

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

    static class EcuOnlineListener implements OutputChannelClient {
        public void setCurrentOutputChannelValue(String string, double d) {
            int newState = (int)d;
            if(currentState == 0 ^ newState == 0){
                currentState = newState;
            }
        }
    }

    private static void periodicWizardLogic() throws ControllerException {
        // todo: check if ECU is connected
        // todo: run logic
        // todo: use something based on TunerStudioIntegration to actually open dialog!
        if (currentState != lastState) {
            if (currentState == CURRENT_STATE_UNKNOWN) {
                log.info("ECU is not connected / no updates from TS");
            } else if (currentState == CURRENT_STATE_OFFLINE) {
                log.info("ECU is offline");
            } else if (currentState == CURRENT_STATE_ONLINE) {
                log.info("ECU is online");
                ecuVinToogle = true;
            }
            lastState = currentState;
        }

         if (pluginEnabled && ecuVinToogle) {
            log.info("ECU is online and we can run the wizard");
            // weird way of getting the equivalent of "page = 1" on the ini file
            String mainConfigName = controllerAccessSupplier.get().getEcuConfigurationNames()[0];
            ControllerParameter currentVin = controllerAccessSupplier.get().getControllerParameterServer().getControllerParameter(mainConfigName, ECU_VIN_KEY);
            ecuVin = currentVin.getStringValue();
            log.info("ECU vin is " + ecuVin);
            ecuVinToogle = false;
        }
    }

    public static boolean displayPlugin(String serialSignature) {
        boolean signatureValid = serialSignature != null && !serialSignature.isEmpty();

        if (signatureValid){
            pluginEnabled = serialSignature.contains("hd81");
        } else {
            log.info("ECU signature invalid for this plugin: " + serialSignature);
        }

        return pluginEnabled;
    }
}
