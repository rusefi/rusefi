package com.rusefi.sensor_logs;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniMemberNotFound;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.ui.UIContext;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 4/15/2016.
 */
public class SensorLogger {
    public static List<BinaryLogEntry> getSensors(UIContext uiContext) {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        if (bp == null)
            return Collections.emptyList();
        return getIniFileModel(bp.getIniFileNullable());
    }

    private static List<BinaryLogEntry> getIniFileModel(IniFileModel iniFileModel) {
        List<BinaryLogEntry> sensors = new ArrayList<>();
        for (GaugeModel gaugeModel : iniFileModel.getGauges().values()) {
            IniField field = null;
            try {
                field = iniFileModel.getOutputChannel(gaugeModel.getChannel());
            } catch (IniMemberNotFound e) {
                throw new RuntimeException(e);
            }
            if (field instanceof ScalarIniField) {
                sensors.add(new CustomBinaryLogEntry(gaugeModel, (ScalarIniField) field));
            }
        }
        return sensors;
    }

    private final List<SensorLog> sensorLogs;

    private boolean isInitialized;

    public SensorLogger(UIContext uiContext) {
        sensorLogs = Arrays.asList(new PlainTextSensorLog(uiContext), new BinarySensorLogRestarter(uiContext));
    }

    public synchronized void init() {
        if (isInitialized) {
            return;
        }
        isInitialized = true;
        SensorCentral.getInstance().addListener(Sensor.SECONDS,
            value -> {
                if (ConnectionStatusLogic.INSTANCE.getValue() != ConnectionStatusValue.CONNECTED)
                    return;
                for (SensorLog sensorLog : sensorLogs)
                    sensorLog.writeSensorLogLine();
            });
    }

}
