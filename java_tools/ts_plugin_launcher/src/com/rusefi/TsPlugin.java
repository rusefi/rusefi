package com.rusefi;

import com.efiAnalytics.plugin.ApplicationPlugin;
import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.efiAnalytics.plugin.ecu.ControllerException;
import com.efiAnalytics.plugin.ecu.ControllerParameter;
import com.efiAnalytics.plugin.ecu.servers.ControllerParameterServer;
import com.rusefi.tools.online.Online;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.AuthTokenPanel;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.storage.PersistentConfiguration;
import com.rusefi.ui.util.Misc;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import javax.xml.bind.JAXBException;
import java.awt.event.ActionEvent;
import java.io.IOException;
import java.util.Map;
import java.util.TreeMap;

/**
 * by the way TS installs stuff into %user%\.efianalytics\TunerStudio\plugins folder
 */
public class TsPlugin implements ApplicationPlugin {
    private ControllerAccess controllerAccess;
    private final JPanel content = new JPanel(new VerticalFlowLayout());

    private final AuthTokenPanel tokenPanel = new AuthTokenPanel();

    public TsPlugin() {
        content.add(new JLabel("" + rusEFIVersion.CONSOLE_VERSION));
        content.add(Misc.getRusEFI_online_manual());
        content.add(tokenPanel.getContent());

        JButton upload = new JButton("Upload");
        upload.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Msq tune = writeCurrentTune(controllerAccess);
                Online.uploadTune(tune, tokenPanel, content);
            }
        });
        content.add(upload);
    }

    @Override
    public String getIdName() {
        return "rusEFI_plugin";
    }

    @Override
    public int getPluginType() {
        return PERSISTENT_DIALOG_PANEL;
    }

    @Override
    public String getDisplayName() {
        return "rusEFI Plugin";
    }

    @Override
    public String getDescription() {
        return "A plugin for rusEFI integration";
    }

    @Override
    public void initialize(ControllerAccess controllerAccess) {
        this.controllerAccess = controllerAccess;
        printEcuConfigurationNames(controllerAccess);
    }

    private static Msq writeCurrentTune(ControllerAccess controllerAccess) {
        Msq msq = new Msq();
        String configurationName = getConfigurationName();
        ControllerParameterServer controllerParameterServer = controllerAccess.getControllerParameterServer();

        Msq tsTune = TsTuneReader.readTsTune(configurationName);
        Map<String, Constant> byName = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        for (Constant c : tsTune.getPage().constant) {
            byName.put(c.getName(), c);
        }

        try {
            String[] parameterNames = controllerParameterServer.getParameterNames(configurationName);
            for (String parameterName : parameterNames) {
                ControllerParameter cp = controllerParameterServer.getControllerParameter(configurationName, parameterName);
                String type = cp.getParamClass();
                String value;
                if (ControllerParameter.PARAM_CLASS_BITS.equals(type)) {
                    value = cp.getStringValue();
                    System.out.println("TsPlugin bits " + parameterName + ": " + value);
                } else if (ControllerParameter.PARAM_CLASS_SCALAR.equals(type)) {
                    value = toString(cp.getScalarValue(), cp.getDecimalPlaces());
                    System.out.println("TsPlugin scalar " + parameterName + ": " + cp.getScalarValue() + "/" + cp.getStringValue());

                } else if (ControllerParameter.PARAM_CLASS_ARRAY.equals(type)) {
                    value = getArrayValue(cp.getArrayValues());
                } else if ("string".equals(type)) {
                    //value = cp.getStringValue();
                    // WOW hack
                    // TS does not provide values for string parameters?! so we read the file directly
                    Constant constant = byName.get(parameterName);
                    if (constant == null) {
                        System.out.println("Not found in TS tune " + parameterName);
                        value = null;
                    } else {
                        value = constant.getValue();
                        System.out.println("TsPlugin name=" + parameterName + " string=" + cp.getStringValue() + "/h=" + value);
                    }
                } else {
                    System.out.println("TsPlugin name=" + parameterName + " unexpected type " + type + "/" + cp.getStringValue());
                    value = cp.getStringValue();
                }

                msq.getPage().constant.add(new Constant(parameterName, cp.getUnits(), value));
            }

            String fileName = Msq.outputXmlFileName;
            msq.writeXmlFile(fileName);
            return msq;
        } catch (JAXBException | IOException | ControllerException e) {
            System.out.println("Error writing XML: " + e);
            return null;
        }
    }

    private static String toString(double scalarValue, int decimalPlaces) {
        // todo: start using decimalPlaces parameter!
        return Double.toString(scalarValue);
    }

    private void printEcuConfigurationNames(ControllerAccess controllerAccess) {
        for (String config : controllerAccess.getEcuConfigurationNames()) {
            System.out.println("EcuConfigurationName " + config);
        }
    }

    private static String getArrayValue(double[][] arrayValues) {
        StringBuilder sb = new StringBuilder();
        for (int rowIndex = 0; rowIndex < arrayValues.length; rowIndex++) {
            double[] array = arrayValues[rowIndex];
            sb.append("\n\t");
            for (int colIndex = 0; colIndex < array.length; colIndex++) {
                double value = array[colIndex];
                sb.append(' ');
                sb.append(value);
            }
        }
        sb.append("\n");
        return sb.toString();
    }

    public static String getConfigurationName() {
        return ControllerAccess.getInstance().getEcuConfigurationNames()[0];
    }

    @Override
    public boolean displayPlugin(String signature) {
        // todo: smarter implementation one day
        return true;
    }

    @Override
    public boolean isMenuEnabled() {
        return true;
    }

    @Override
    public String getAuthor() {
        return "rusEFI LLC";
    }

    @Override
    public JComponent getPluginPanel() {
        return content;
    }

    @Override
    public void close() {
        System.out.printf("TsPlugin#close");
        PersistentConfiguration.getConfig().save();
    }

    @Override
    public String getHelpUrl() {
        return "https://rusefi.com";
    }

    @Override
    public String getVersion() {
        return Integer.toString(rusEFIVersion.CONSOLE_VERSION);
    }

    @Override
    public double getRequiredPluginSpec() {
        return PLUGIN_API_VERSION;
    }
}
