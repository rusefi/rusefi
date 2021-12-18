package com.rusefi.livedata;

import com.rusefi.ui.UIContext;
import com.rusefi.ui.livedata.VariableValueSource;
import com.rusefi.ui.util.FrameHelper;

import javax.swing.*;
import java.util.Map;
import java.util.TreeMap;

/**
 * @see LiveDataParserPanelSandbox
 */
public class LiveDataParserSandbox {
    public static void main(String[] args) {
        Map<String, Object> values = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        values.put("engineTooSlow", Boolean.TRUE);
        values.put("engineTooFast", Boolean.FALSE);

        VariableValueSource valueSource = name -> values.get(name);

        new FrameHelper(JDialog.EXIT_ON_CLOSE).showFrame(new LiveDataParserPanel(new UIContext(), valueSource, "ac_control.cpp").getContent());
    }
}
