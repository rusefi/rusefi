package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.TsTuneReader;
import com.rusefi.autoupdate.AutoupdateUtil;
import com.rusefi.tools.online.Online;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.AuthTokenPanel;
import com.rusefi.ui.storage.PersistentConfiguration;
import com.rusefi.ui.util.URLLabel;
import org.apache.http.concurrent.FutureCallback;
import org.jetbrains.annotations.NotNull;
import org.json.simple.JSONArray;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.Map;
import java.util.function.Supplier;
import java.util.jar.Attributes;
import java.util.jar.Manifest;

/**
 * TsPlugin launcher creates an instance of this class via reflection.
 */
public class PluginEntry implements TsPluginBody {
    private static final String BUILT_DATE = "Built-Date";
    private static final String BUILT_TIMESTAMP = "Built-Timestamp";
    public static final String REO = "https://rusefi.com/online/";
    private static final String NO_PROJECT = "Please open project";
    private final AuthTokenPanel tokenPanel = new AuthTokenPanel();
    private final JComponent content = new JPanel(new VerticalFlowLayout());
    private final ImageIcon LOGO = AutoupdateUtil.loadIcon("/rusefi_online_color_300.png");

    private final JButton upload = new JButton("Upload Current Tune");
    private final JLabel uploadState = new JLabel();
    private final JLabel projectWarning = new JLabel(NO_PROJECT);
    private final JLabel tuneInfo = new JLabel();
    private final Supplier<ControllerAccess> controllerAccessSupplier;

    private String currentConfiguration;
    private boolean tuneIsOk;
    private boolean projectIsOk;

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
                                projectWarning.setText(NO_PROJECT);
                                projectIsOk = false;
                            } else if (!new File(TsTuneReader.getTsTuneFileName(configurationName)).exists()) {
                                projectWarning.setText("Tune not found " + configurationName);
                                projectIsOk = false;
                            } else {
                                projectIsOk = true;
                            }
                            projectWarning.setVisible(!projectIsOk);
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

        content.add(new JLabel(getAttribute(BUILT_TIMESTAMP)));
//        content.add(new JLabel("Active project: " + getConfigurationName()));

        uploadState.setVisible(false);

        content.add(projectWarning);
        content.add(tuneInfo);
        content.add(upload);
        content.add(uploadState);
        content.add(new JLabel(LOGO));
        content.add(tokenPanel.getContent());
        content.add(new URLLabel(REO));
    }

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
            tuneInfo.setText(engineMake.getValue() + " " + engineCode.getValue() + " " + vehicleName.getValue());
            tuneIsOk = true;
            updateUploadEnabled();
        } else {
            tuneInfo.setText("<html>Please set " + warning + " on Base Settings tab<br>and reopen Project");
            tuneInfo.setForeground(Color.red);
            tuneIsOk = false;
            updateUploadEnabled();
        }
        currentConfiguration = configurationName;
    }

    private boolean isEmpty(Constant constant) {
        if (constant == null)
            return true;
        return isEmpty(constant.getValue());
    }

    private void updateUploadEnabled() {
        upload.setEnabled(tuneIsOk && projectIsOk);
    }

    private boolean isEmpty(String value) {
        return value == null || value.trim().length() == 0;
    }

    @Override
    public JComponent getContent() {
        return content;
    }

    public void close() {
        PersistentConfiguration.getConfig().save();
    }

    private void printEcuConfigurationNames(ControllerAccess controllerAccess) {
        for (String config : controllerAccess.getEcuConfigurationNames()) {
            System.out.println("EcuConfigurationName " + config);
        }
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
        return getAttribute(BUILT_DATE);
    }

    @NotNull
    private static String getAttribute(String attributeName) {
        // all this magic below to make sure we are reading manifest of the *our* jar file not TS main jar file
        Class clazz = PluginEntry.class;
        String className = clazz.getSimpleName() + ".class";
        String classPath = clazz.getResource(className).toString();
        if (!classPath.startsWith("jar")) {
            // Class not from JAR
            return "Local Run";
        }
        String manifestPath = classPath.substring(0, classPath.lastIndexOf("!") + 1) +
                "/META-INF/MANIFEST.MF";
        try {
            Manifest manifest = new Manifest(new URL(manifestPath).openStream());
            Attributes attributes = manifest.getMainAttributes();

            String result = attributes.getValue(attributeName);
            System.out.println(BUILT_DATE + " " + result);
            return result == null ? "Unknown version" : result;
        } catch (IOException e) {
            e.printStackTrace();
            return "Unknown version";
        }
    }
}
