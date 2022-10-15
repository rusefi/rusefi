package com.rusefi.livedata;

import com.rusefi.config.Field;
import com.rusefi.config.FieldType;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.livedata.VariableValueSource;
import com.rusefi.core.ui.FrameHelper;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.util.Map;
import java.util.TreeMap;

/**
 *
 */
public class LiveDataParserSandbox {
    public static void main(String[] args) {
        Map<String, Double> values = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        values.put("engineTooSlow", 1.0);
        values.put("engineTooFast", 0.0);

        VariableValueSource valueSource = getVariableValueSource(values);

        new FrameHelper(JDialog.EXIT_ON_CLOSE).showFrame(new LiveDataParserPanel(new UIContext(), valueSource, "ac_control.cpp").getContent());
    }

    @Nullable
    public static VariableValueSource getVariableValueSource(Map<String, Double> values) {
        return name -> {
            Double value = values.get(name);
            if (value == null)
                return null;
            return new VariableValueSource.VariableState(new Field(name, 0, FieldType.BIT), value);
        };
    }
}
