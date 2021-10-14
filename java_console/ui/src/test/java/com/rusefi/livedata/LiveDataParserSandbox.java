package com.rusefi.livedata;

import com.rusefi.core.ValueSource;
import com.rusefi.ui.livedata.LiveDataParserTest;
import com.rusefi.ui.livedata.VariableValueSource;
import com.rusefi.ui.util.FrameHelper;

import java.util.Map;
import java.util.TreeMap;

public class LiveDataParserSandbox {
    public static void main(String[] args) {
        Map<String, Object> values = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        values.put("engineTooSlow", Boolean.TRUE);
        values.put("engineTooFast", Boolean.FALSE);

        VariableValueSource valueSource = new VariableValueSource() {
            @Override
            public Object getValue(String name) {
                return values.get(name);
            }
        };


        new FrameHelper().showFrame(new LiveDataParserPanel(valueSource).getContent());
    }
}
