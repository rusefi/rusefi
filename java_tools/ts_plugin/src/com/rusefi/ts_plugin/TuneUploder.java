package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.efiAnalytics.plugin.ecu.ControllerException;
import com.efiAnalytics.plugin.ecu.ControllerParameter;
import com.efiAnalytics.plugin.ecu.servers.ControllerParameterServer;
import com.rusefi.TsTuneReader;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;
import org.jetbrains.annotations.NotNull;

import javax.xml.bind.JAXBException;
import java.io.IOException;
import java.util.Collections;
import java.util.Map;
import java.util.TreeMap;

public class TuneUploder {
    static Msq writeCurrentTune(ControllerAccess controllerAccess, String configurationName) {
        Msq msq = Msq.create();
        ControllerParameterServer controllerParameterServer = controllerAccess.getControllerParameterServer();

        Map<String, Constant> fileSystemValues = getFileSystemValues(configurationName);

        try {
            String[] parameterNames = controllerParameterServer.getParameterNames(configurationName);
            for (String parameterName : parameterNames) {
                handleParameter(configurationName, msq, controllerParameterServer, fileSystemValues, parameterName);
            }

            String fileName = Msq.outputXmlFileName;
            msq.writeXmlFile(fileName);
            return msq;
        } catch (JAXBException | IOException | ControllerException e) {
            System.out.println("Error writing XML: " + e);
            return null;
        }
    }

    @NotNull
    static Map<String, Constant> getFileSystemValues(String configurationName) {
        if (configurationName == null)
            return Collections.emptyMap();
        Msq tsTune;
        try {
            tsTune = TsTuneReader.readTsTune(configurationName);
        } catch (Exception e) {
            return Collections.emptyMap();
        }
        Map<String, Constant> byName = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        for (Constant c : tsTune.findPage().constant) {
            byName.put(c.getName(), c);
        }
        return byName;
    }

    private static void handleParameter(String configurationName, Msq msq, ControllerParameterServer controllerParameterServer, Map<String, Constant> byName, String parameterName) throws ControllerException {
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

        Page page = msq.findPage();
        page.constant.add(new Constant(parameterName, cp.getUnits(), value));
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

    private static String toString(double scalarValue, int decimalPlaces) {
        // todo: start using decimalPlaces parameter!
        return Double.toString(scalarValue);
    }
}
