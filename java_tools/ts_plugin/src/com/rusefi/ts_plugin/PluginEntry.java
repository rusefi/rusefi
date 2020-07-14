package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.TsTuneReader;
import com.rusefi.autoupdate.AutoupdateUtil;
import com.rusefi.tools.online.Online;
import com.rusefi.ts_plugin.util.ManifestHelper;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.AuthTokenPanel;
import com.rusefi.ui.storage.PersistentConfiguration;
import com.rusefi.ui.util.URLLabel;
import org.apache.http.concurrent.FutureCallback;
import org.json.simple.JSONArray;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.File;
import java.util.Map;
import java.util.function.Supplier;

/**
 * TsPlugin launcher creates an instance of this class via reflection.
 */
public class PluginEntry implements TsPluginBody {
    private static final String REO_URL = "https://rusefi.com/online/";
    private static final String NO_PROJECT = "Please open project";
    private final AuthTokenPanel tokenPanel = new AuthTokenPanel();
    private final JComponent content = new JPanel(new VerticalFlowLayout());

    private final JButton upload = new JButton("Upload Current Tune");
    private final JLabel uploadState = new JLabel();
    private final JLabel projectWarning = new JLabel(NO_PROJECT);
    private final JLabel tuneInfo = new JLabel();
    private final Supplier<ControllerAccess> controllerAccessSupplier;

    private String currentConfiguration;

    private UploaderStatus uploaderStatus = new UploaderStatus();


    /**
     * the real constructor - this one is invoked via reflection
     */
    public PluginEntry() {
        this(ControllerAccess::getInstance);
    }

    public PluginEntry(Supplier<ControllerAccess> controllerAccessSupplier) {
        this.controllerAccessSupplier = controllerAccessSupplier;
        upload.setBackground(new Color(0x90EE90));

        new Thread(new Runnable() {
            @Override
            public void run() {
                while (true) {
                    String configurationName = getConfigurationName();
                    if ((currentConfiguration == null && configurationName != null)
                            || !currentConfiguration.equals(configurationName)) {
                        handleConfigurationChange(configurationName);
                    }

                    boolean isProjectActive = configurationName != null;
                    SwingUtilities.invokeLater(new Runnable() {
                        @Override
                        public void run() {
                            if (!isProjectActive) {
                                uploaderStatus.projectWarning = NO_PROJECT;
                            } else if (!new File(TsTuneReader.getTsTuneFileName(configurationName)).exists()) {
                                uploaderStatus.projectWarning = "Tune not found " + configurationName;
                            } else {
                                uploaderStatus.projectWarning = null;
                            }

                            updateUploadEnabled();
                        }
                    });

                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();

        upload.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (!tokenPanel.hasToken()) {
                    tokenPanel.showError(content);
                    return;
                }
                String configurationName = getConfigurationName();
                if (configurationName == null) {
                    JOptionPane.showMessageDialog(content, "No project opened");
                    return;
                }

                Msq tune = TuneUploder.writeCurrentTune(ControllerAccess.getInstance(), configurationName);
                Online.uploadTune(tune, tokenPanel, content, new FutureCallback<JSONArray>() {
                    @Override
                    public void completed(JSONArray array) {
                        SwingUtilities.invokeLater(new Runnable() {
                            @Override
                            public void run() {
                                uploadState.setText(array.get(0).toString());
                                uploadState.setVisible(true);
                            }
                        });
                    }

                    @Override
                    public void failed(Exception e) {
                    }

                    @Override
                    public void cancelled() {
                    }
                });
            }
        });

        content.add(new JLabel(ManifestHelper.getBuildTimestamp()));
//        content.add(new JLabel("Active project: " + getConfigurationName()));

        uploadState.setVisible(false);

        content.add(projectWarning);
        content.add(tuneInfo);
        content.add(upload);
        content.add(uploadState);
        ImageIcon LOGO = AutoupdateUtil.loadIcon("/rusefi_online_color_300.png");
        content.add(new JLabel(LOGO));
        content.add(tokenPanel.getContent());
        content.add(new URLLabel(REO_URL));
    }

    /**
     * This method is invoked every time we defect a switch between projects
     */
    private void handleConfigurationChange(String configurationName) {
        Map<String, Constant> fileSystemValues = TuneUploder.getFileSystemValues(configurationName);
        Constant engineMake = fileSystemValues.get("enginemake");
        Constant engineCode = fileSystemValues.get("enginecode");
        Constant vehicleName = fileSystemValues.get("VEHICLENAME");
        String warning = "";
        if (isEmpty(engineMake)) {
            warning += " engine make";
        }
        if (isEmpty(engineCode)) {
            warning += " engine code";
        }
        if (isEmpty(vehicleName)) {
            warning += " vehicle name";
        }
        if (warning.isEmpty()) {
            uploaderStatus.tuneInfo = engineMake.getValue() + " " + engineCode.getValue() + " " + vehicleName.getValue();
            uploaderStatus.tuneWarning = null;
        } else {
            uploaderStatus.tuneInfo = null;
            uploaderStatus.tuneWarning = "<html>Please set " + warning + " on Base Settings tab<br>and reopen Project";
        }
        updateUploadEnabled();

        currentConfiguration = configurationName;
    }

    private boolean isEmpty(Constant constant) {
        if (constant == null)
            return true;
        return isEmpty(constant.getValue());
    }

    private void updateUploadEnabled() {
        if (uploaderStatus.isTuneOk()) {
            tuneInfo.setText(uploaderStatus.tuneInfo);
            tuneInfo.setForeground(Color.black);
        } else {
            tuneInfo.setText(uploaderStatus.tuneWarning);
            tuneInfo.setForeground(Color.red);
        }

        if (uploaderStatus.isProjectIsOk()) {
            projectWarning.setVisible(false);
        } else {
            projectWarning.setVisible(true);
            projectWarning.setText(uploaderStatus.projectWarning);
        }

        upload.setEnabled(uploaderStatus.isTuneOk() && uploaderStatus.isProjectIsOk());
    }

    private static boolean isEmpty(String value) {
        return value == null || value.trim().length() == 0;
    }

    @Override
    public JComponent getContent() {
        return content;
    }

    public void close() {
        PersistentConfiguration.getConfig().save();
    }

    private String getConfigurationName() {
        ControllerAccess controllerAccess = controllerAccessSupplier.get();
        if (controllerAccess == null) {
            System.out.println("No ControllerAccess");
            return null;
        }
        String[] configurationNames = controllerAccess.getEcuConfigurationNames();
        if (configurationNames.length == 0)
            return null;
        return configurationNames[0];
    }

    /**
     * this method is invoked by refection
     *
     * @see TsPluginBody#GET_VERSION
     */
    @SuppressWarnings("unused")
    public static String getVersion() {
        return ManifestHelper.getVersion();
    }
}
