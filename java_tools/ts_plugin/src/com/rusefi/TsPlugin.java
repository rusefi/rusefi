package com.rusefi;

import com.efiAnalytics.plugin.ApplicationPlugin;
import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.efiAnalytics.plugin.ecu.ControllerException;
import com.efiAnalytics.plugin.ecu.ControllerParameter;
import com.efiAnalytics.plugin.ecu.servers.ControllerParameterServer;
import com.rusefi.config.Field;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.util.Misc;

import javax.swing.*;
import javax.xml.bind.JAXBException;
import java.awt.*;
import java.io.IOException;

public class TsPlugin implements ApplicationPlugin {
    private ControllerAccess controllerAccess;
    private JPanel content = new JPanel(new BorderLayout());

    public TsPlugin() {
        content.add(Misc.getRusEFI_online_manual());
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
        for (String config : controllerAccess.getEcuConfigurationNames()) {
            System.out.println("EcuConfigurationName " + config);
        }

        Msq msq = new Msq();

        String configurationName = getConfigurationName();
        ControllerParameterServer controllerParameterServer = controllerAccess.getControllerParameterServer();
        String[] parameterNames = controllerParameterServer.getParameterNames(configurationName);
        try {
            for (String parameterName : parameterNames) {
                ControllerParameter cp = controllerParameterServer.getControllerParameter(configurationName, parameterName);
                String type = cp.getParamClass();
                String value;
                if (ControllerParameter.PARAM_CLASS_BITS.equals(type)) {
                    value = cp.getStringValue();
                    System.out.println("bits " + parameterName + ": " + value);
                } else if (ControllerParameter.PARAM_CLASS_SCALAR.equals(type)) {
                    value = cp.getStringValue();
                    System.out.println("scalar " + parameterName + ": " + cp.getScalarValue() + "/" + cp.getStringValue());

                } else if (ControllerParameter.PARAM_CLASS_ARRAY.equals(type)) {
                    value = getArrayValue(cp.getArrayValues());
                } else {
                    System.out.println("name=" + parameterName + " type " + type + "/" + cp.getStringValue());
                    value = cp.getStringValue();
                }

                msq.getPage().constant.add(new Constant(parameterName, cp.getUnits(), value));
            }

            try {
                msq.writeXmlFile("plugin.xml");
            } catch (JAXBException | IOException e) {
                System.out.println("Error writing XML: " + e);
            }

        } catch (ControllerException e) {
            e.printStackTrace();
        }
    }

    private String getArrayValue(double[][] arrayValues) {
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

    }

    @Override
    public String getHelpUrl() {
        return "https://rusefi.com";
    }

    @Override
    public String getVersion() {
        return "0.00000001";
    }

    @Override
    public double getRequiredPluginSpec() {
        return PLUGIN_API_VERSION;
    }
}
