package com.rusefi.ui;

import com.rusefi.ui.livedocs.LiveDocPanel;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;

/**
 * (c) Andrey Belomutskiy 2013-2019
 */
public class SensorsLiveDataPane {
    private JPanel liveDocs = LiveDocPanel.createSensorsLiveDataPanel();
    /**
     * this is the panel we expose to the outside world
     */
    private final JPanel content = new JPanel(new BorderLayout());

    public SensorsLiveDataPane() {
        JPanel vertical = new JPanel(new VerticalFlowLayout());

        vertical.add(liveDocs);

        JScrollPane scroll = new JScrollPane(vertical, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        content.add(scroll, BorderLayout.CENTER);

    }

    public JPanel getContent() {
        return content;
    }
}
