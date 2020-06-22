package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.efiAnalytics.plugin.ecu.ControllerException;
import com.efiAnalytics.plugin.ecu.ControllerParameter;
import com.efiAnalytics.plugin.ecu.servers.ControllerParameterServer;
import com.rusefi.TsTuneReader;
import com.rusefi.autoupdate.AutoupdateUtil;
import com.rusefi.tools.online.Online;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.AuthTokenPanel;
import com.rusefi.ui.storage.PersistentConfiguration;
import com.rusefi.ui.util.URLLabel;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import javax.xml.bind.JAXBException;
import java.awt.event.ActionEvent;
import java.io.IOException;
import java.net.URL;
import java.util.Map;
import java.util.TreeMap;
import java.util.jar.Attributes;
import java.util.jar.Manifest;

/**
 * TsPlugin launcher creates an instance of this class via reflection.
 */
public class PluginEntry implements TsPluginBody {
    public static final String BUILT_DATE = "Built-Date";
    public static final String REO = "https://rusefi.com/online/";
    private final AuthTokenPanel tokenPanel = new AuthTokenPanel();
    private final JComponent content = new JPanel(new VerticalFlowLayout());
    private static final ImageIcon LOGO = AutoupdateUtil.loadIcon("/rusefi_online_color_300.png");

    public PluginEntry() {

        JButton upload = new JButton("Upload Current Tune");
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

                Msq tune = writeCurrentTune(ControllerAccess.getInstance(), configurationName);
                Online.uploadTune(tune, tokenPanel, content);
            }
        });

        content.add(upload);
        content.add(new JLabel(LOGO));
        content.add(tokenPanel.getContent());
        content.add(new URLLabel(REO));
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
        ControllerAccess controllerAccess = ControllerAccess.getInstance();
        if (controllerAccess == null) {
            System.out.println("No ControllerAccess");
            return null;
        }
        return controllerAccess.getEcuConfigurationNames()[0];
    }

    private static String toString(double scalarValue, int decimalPlaces) {
        // todo: start using decimalPlaces parameter!
        return Double.toString(scalarValue);
    }

    private static Msq writeCurrentTune(ControllerAccess controllerAccess, String configurationName) {
        Msq msq = new Msq();
        ControllerParameterServer controllerParameterServer = controllerAccess.getControllerParameterServer();

        Msq tsTune = TsTuneReader.readTsTune(configurationName);
        Map<String, Constant> byName = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        for (Constant c : tsTune.findPage().constant) {
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

                msq.findPage().constant.add(new Constant(parameterName, cp.getUnits(), value));
            }

            String fileName = Msq.outputXmlFileName;
            msq.writeXmlFile(fileName);
            return msq;
        } catch (JAXBException | IOException | ControllerException e) {
            System.out.println("Error writing XML: " + e);
            return null;
        }
    }

    /**
     * this method is invoked by refection
     *
     * @see TsPluginBody#GET_VERSION
     */
    @SuppressWarnings("unused")
    public static String getVersion() {
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
            System.out.println("Attributed " + attributes);
            System.out.println("Attributed " + attributes.keySet());
            System.out.println("Attributed " + attributes.getValue("Class-Path"));
            System.out.println("Attributed " + attributes.getValue("Main-Class"));

            String result = attributes.getValue(BUILT_DATE);
            System.out.println(BUILT_DATE + " " + result);
            return result == null ? "Unknown version" : result;
        } catch (IOException e) {
            e.printStackTrace();
            return "Unknown version";
        }
    }
}
