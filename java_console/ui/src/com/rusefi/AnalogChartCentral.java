package com.rusefi;

import com.irnems.core.EngineState;
import com.rusefi.io.LinkManager;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class AnalogChartCentral {
    private static final String KEY = "analog_chart";

    static List<AnalogChartListener> listeners = new CopyOnWriteArrayList<>();

    static {
        LinkManager.engineState.registerStringValueAction(KEY, new EngineState.ValueCallback<String>() {
                    @Override
                    public void onUpdate(String message) {
                        for (AnalogChartListener listener : listeners)
                            listener.onAnalogChart(message);
                    }
                }
        );
    }

    interface AnalogChartListener {
        void onAnalogChart(String analogChart);
    }
}