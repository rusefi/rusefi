package com.rusefi.wizard;

import java.awt.Frame;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;
import java.util.function.Supplier;

import javax.swing.AbstractButton;
import javax.swing.JDialog;
import javax.swing.JMenuItem;
import javax.swing.SwingUtilities;

import com.devexperts.logging.Logging;
import com.efiAnalytics.plugin.ecu.*;

import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;

public class BackgroundWizard {
    private static final String ECU_VIN_KEY = "vinNumber";

    private static final Logging log = Logging.getLogging(BackgroundWizard.class);
    private static final int CURRENT_STATE_UNKNOWN = -1;
    private static final int CURRENT_STATE_OFFLINE = 0;
    private static final int CURRENT_STATE_ONLINE = 1;
    // setting to search on wizard
    private static final String BASE_ENGINE_SETTINGS_TITLE = "Base Engine Settings";
    private static final String BASE_ENGINE_BUTTON_TITLE = "Base engine";
    // listener for ecu online state
    static OutputChannelClient onlineListener = new EcuOnlineListener();
    private static Supplier<ControllerAccess> controllerAccessSupplier;
    private static int currentState = CURRENT_STATE_UNKNOWN;
    private static int lastState = CURRENT_STATE_UNKNOWN;
    private static boolean pluginEnabled = false;
    private static boolean WizardVinToggle = true;

    public static void start(Supplier<ControllerAccess> controllerAccessSupplier) {
        BackgroundWizard.controllerAccessSupplier = controllerAccessSupplier;
        try {
            BackgroundWizard.controllerAccessSupplier.get().getOutputChannelServer().subscribe("AppEvent", "controllerOnline", onlineListener);
        } catch (Exception e) {
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

    private static void periodicWizardLogic() throws ControllerException, MathException {
        if (currentState != lastState) {
            if (currentState == CURRENT_STATE_UNKNOWN) {
                log.info("ECU is not connected / no updates from TS");
            } else if (currentState == CURRENT_STATE_OFFLINE) {
                log.info("ECU is offline");
            } else if (currentState == CURRENT_STATE_ONLINE) {
                log.info("ECU is online");
                WizardVinToggle = true;
            }
            lastState = currentState;
        }

        if (currentState == CURRENT_STATE_ONLINE && pluginEnabled && WizardVinToggle) {
            log.info("ECU is online and we can run the wizard");
            // weird way of getting the equivalent of "page = 1" on the ini file
            String mainConfigName = controllerAccessSupplier.get().getEcuConfigurationNames()[0];
            ControllerParameter currentVin = controllerAccessSupplier.get().getControllerParameterServer().getControllerParameter(mainConfigName, ECU_VIN_KEY);

            String ecuVin = currentVin.getStringValue();
            if (ecuVin == null || ecuVin.isEmpty()) {
                launchVinUI();
            }
            log.info("ECU vin is " + ecuVin);
            WizardVinToggle = false;
        }
    }

    private static void launchVinUI() {
        log.info("Launching VIN UI");
        try {
            Frame mainFrame = TsReflectionHelper.findTsMainFrame(10, TimeUnit.SECONDS);
            if (mainFrame == null) {
                throw new IllegalStateException("Could not find TunerStudio frame to launch wizard");
            }

            // Discover top level menu buttons first (TS shows menu items only after a top level is selected)
            List<AbstractButton> topButtons = TsReflectionHelper.findTopLevelButtons(mainFrame);
            if (topButtons.isEmpty()) {
                throw new IllegalStateException("no TS top-level buttons");
            }

            List<AbstractButton> buttonsToTry = new ArrayList<>(topButtons);
            // Always include a 'no button' pass at the end to catch globally visible menus
            int attempts = Math.max(1, buttonsToTry.size() + 1);
            for (int i = 0; i < attempts; i++) {
                AbstractButton topButton = (i < buttonsToTry.size()) ? buttonsToTry.get(i) : null;
                if (topButton != null) {
                    try {
                        SwingUtilities.invokeAndWait(topButton::doClick);
                        sleep(200);
                    } catch (Exception e) {
                        log.info("Unable to click top-level menu: " + e);
                    }
                }

                JMenuItem engineMetaItem = TsReflectionHelper.findMenuItem(mainFrame, BASE_ENGINE_BUTTON_TITLE);
                if (engineMetaItem != null) {
                    try {
                        SwingUtilities.invokeAndWait(engineMetaItem::doClick);
                        sleep(200);
                        JDialog dialog = TsReflectionHelper.waitForDialogWithTitle(BASE_ENGINE_SETTINGS_TITLE, 3, TimeUnit.SECONDS);
                        if (dialog != null) {
                            TsReflectionHelper.bringToFront(dialog);
                            log.info("Engine Metadata dialog opened");
                            return;
                        }
                    } catch (Exception e) {
                        log.info("Failed to invoke Engine Metadata menu: " + e);
                    }
                }
            }
        } catch (Throwable t) {
            log.error("Error launching VIN UI: " + t, t);
        }
    }

    public static boolean displayPlugin(String serialSignature) {
        boolean signatureValid = serialSignature != null && !serialSignature.isEmpty();

        if (signatureValid) {
            pluginEnabled = serialSignature.contains("hd81");
        } else {
            log.info("ECU signature invalid for this plugin: " + serialSignature);
        }

        return pluginEnabled;
    }

    static class EcuOnlineListener implements OutputChannelClient {
        public void setCurrentOutputChannelValue(String string, double d) {
            int newState = (int) d;
            if (currentState == 0 ^ newState == 0) {
                currentState = newState;
            }
        }
    }
}
