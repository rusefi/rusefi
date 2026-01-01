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
import com.rusefi.output.HashUtil;

import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;
import static com.rusefi.config.generated.VariableRegistryValues.DIALOG_NAME_VEHICLE_INFORMATION;

public class BackgroundWizard {
    private static final String ECU_WIZARD_KEY = "wizardPanelToShow";

    private static final Logging log = Logging.getLogging(BackgroundWizard.class);
    private static final int CURRENT_STATE_UNKNOWN = -1;
    private static final int CURRENT_STATE_OFFLINE = 0;
    private static final int CURRENT_STATE_ONLINE = 1;

    // listener for ecu online state
    static OutputChannelClient onlineListener = new EcuOnlineListener();
    private static Supplier<ControllerAccess> controllerAccessSupplier;
    private static int currentState = CURRENT_STATE_UNKNOWN;
    private static int lastState = CURRENT_STATE_UNKNOWN;
    private static boolean pluginEnabled = false;
    private static boolean WizardRunToogle = true;


    /**
     * Represents the panels enabled for use in the BackgroundWizard
     */
    private static final String[] WizardEnabledPanels = {DIALOG_NAME_VEHICLE_INFORMATION};

    public static void start(Supplier<ControllerAccess> controllerAccessSupplier) {
        BackgroundWizard.controllerAccessSupplier = controllerAccessSupplier;

        Thread thread = new Thread(() -> {
            boolean subscribed = false;
            // due to TS 3.2.03 and prior unable to subscribe to AppEvent event, we have to busy-wait (or retry) until we get and ecu online status
            // triggered by onEcuDiscovery, also TS tries to load the outputChannel server *before* reading the .ini file, so we can't also do the
            // getEcuConfigurationNames()[0] like on the wizard loop logic
            while (!subscribed) {
                try {
                    BackgroundWizard.controllerAccessSupplier.get().getOutputChannelServer().subscribe("AppEvent", "controllerOnline", onlineListener);
                    subscribed = true;
                } catch (Exception e) {
                    log.error("Error subscribing to controllerOnline event: " + e, e);
                    if (currentState == CURRENT_STATE_ONLINE) {
                        subscribed = true;
                    } else {
                        sleep(5000);
                    }
                }
            }

            while (true) {
                try {
                    periodicWizardLogic();
                } catch (Throwable e) {
                    log.error("Wizard crash, error " + e, e);
                    WizardRunToogle = true;
                }
                sleep(300);
            }
        });
        thread.setName("wizard-thread");
        thread.setDaemon(true);
        thread.start();
    }

    private static void periodicWizardLogic() throws ControllerException {
        if (currentState != lastState) {
            if (currentState == CURRENT_STATE_UNKNOWN) {
                log.info("ECU is not connected / no updates from TS");
            } else if (currentState == CURRENT_STATE_OFFLINE) {
                log.info("ECU is offline");
            } else if (currentState == CURRENT_STATE_ONLINE) {
                log.info("ECU is online");
                WizardRunToogle = true;
            }
            lastState = currentState;
        }

        if (currentState == CURRENT_STATE_ONLINE && pluginEnabled && WizardRunToogle) {
            log.info("ECU is online and we can run the wizard");

            // weird way of getting the equivalent of "page = 1" on the ini file
            String[] mainConfigName = controllerAccessSupplier.get().getEcuConfigurationNames();
            ControllerParameter currentVin = controllerAccessSupplier.get().getControllerParameterServer().getControllerParameter(mainConfigName[0], ECU_WIZARD_KEY);
            int panelToShow = (int)currentVin.getScalarValue();

            if(panelToShow == -1){
                WizardRunToogle = false;
                return;
            }

            for(String enabledPanel : WizardEnabledPanels){
                int hash = HashUtil.hash(enabledPanel);

                if (panelToShow == hash) {
                    launchVinUI(enabledPanel);
                    WizardRunToogle = false;
                    return;
                }
            }

            log.info("FW requested a panel to show, but we can't find it in the list of enabled panels");
            WizardRunToogle = false;
        }
    }

    private static void launchVinUI(String panelToOpen) {
        log.info("Launching VIN UI");
        try {
            Frame mainFrame = TsAccess.findTsMainFrame();
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

                JMenuItem engineMetaItem = TsReflectionHelper.findMenuItem(mainFrame, panelToOpen);
                if (engineMetaItem != null) {
                    try {
                        SwingUtilities.invokeAndWait(engineMetaItem::doClick);
                        sleep(200);
                        JDialog dialog = TsReflectionHelper.waitForDialogWithTitle(panelToOpen, 3, TimeUnit.SECONDS);
                        if (dialog != null) {
                            TsReflectionHelper.bringToFront(dialog);
                            log.info(panelToOpen + " dialog opened");
                            return;
                        }
                    } catch (Exception e) {
                        log.info("Failed to invoke" + panelToOpen + e);
                    }
                }
            }
        } catch (Throwable t) {
            log.error("Error launching VIN UI: " + t, t);
            sleep(5000);
            launchVinUI(panelToOpen);
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

    public static void onEcuDiscovery(String serialSignature){
        displayPlugin(serialSignature);
        currentState = CURRENT_STATE_ONLINE;
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
