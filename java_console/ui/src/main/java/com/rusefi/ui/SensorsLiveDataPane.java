package com.rusefi.ui;

import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class SensorsLiveDataPane {
    /**
     * this is the panel we expose to the outside world
     */
    private final JPanel content = new JPanel(new BorderLayout());

    public SensorsLiveDataPane(UIContext uiContext) {
        JPanel vertical = new JPanel(new VerticalFlowLayout());


        JScrollPane scroll = new JScrollPane(vertical, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        content.add(scroll, BorderLayout.CENTER);

    }

    public JPanel getContent() {
        return content;
    }
}
