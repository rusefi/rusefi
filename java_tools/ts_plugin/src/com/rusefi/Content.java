package com.rusefi;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.efiAnalytics.plugin.ecu.ControllerException;
import com.efiAnalytics.plugin.ecu.ControllerParameter;
import com.efiAnalytics.plugin.ecu.servers.ControllerParameterServer;
import com.rusefi.tools.online.Online;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.AuthTokenPanel;
import com.rusefi.ui.storage.PersistentConfiguration;

import javax.swing.*;
import javax.xml.bind.JAXBException;
import java.awt.event.ActionEvent;
import java.io.IOException;
import java.util.Map;
import java.util.TreeMap;

public class Content {
    private final AuthTokenPanel tokenPanel = new AuthTokenPanel();
    private final JComponent content = new JPanel();

    private ControllerAccess controllerAccess;

    public Content() {
        content.add(tokenPanel.getContent());

        JButton upload = new JButton("Upload Tune");
        upload.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Msq tune = writeCurrentTune(controllerAccess);
                Online.uploadTune(tune, tokenPanel, content);
            }
        });
        content.add(upload);

    }

    public void close() {
        PersistentConfiguration.getConfig().save();
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


    private static String toString(double scalarValue, int decimalPlaces) {
        // todo: start using decimalPlaces parameter!
        return Double.toString(scalarValue);
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
}
