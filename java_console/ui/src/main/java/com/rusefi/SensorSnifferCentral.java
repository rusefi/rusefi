package com.rusefi;

import com.rusefi.core.EngineState;
import com.rusefi.io.LinkManager;

import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class SensorSnifferCentral {
    private final List<AnalogChartListener> listeners = new CopyOnWriteArrayList<>();

    public SensorSnifferCentral(LinkManager linkManager) {
        linkManager.getEngineState().registerStringValueAction(AverageAnglesUtil.KEY, new EngineState.ValueCallback<String>() {
                    @Override
                    public void onUpdate(String message) {
                        for (AnalogChartListener listener : listeners)
                            listener.onAnalogChart(message);
                    }
                }
        );
    }

    public void addListener(AnalogChartListener listener) {
        listeners.add(listener);
    }

    interface AnalogChartListener {
        void onAnalogChart(String analogChart);
    }
}