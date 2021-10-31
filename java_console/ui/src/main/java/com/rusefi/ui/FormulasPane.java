package com.rusefi.ui;

import com.opensr5.Logger;
import com.rusefi.core.Sensor;
import com.rusefi.livedata.LiveDataParserPanel;
import com.rusefi.livedata.LiveDataView;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.IntGaugeLabel;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;


/**
 * todo: it was a nice prototype of jlatexmath usage but it's time to remove it soon!
 * Lua nad live_data_e seems much more promising at the moment
 *
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class FormulasPane {
    /**
     * this is the panel we expose to the outside world
     */
    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel formulaProxy = new JPanel(new BorderLayout());
    private boolean isPaused;

    public FormulasPane(UIContext uiContext) {

        JPanel vertical = new JPanel(new VerticalFlowLayout());
        for (LiveDataView view : LiveDataView.values())
            vertical.add(LiveDataParserPanel.createLiveDataParserContent(uiContext, view));
        vertical.add(formulaProxy);

        JScrollPane scroll = new JScrollPane(vertical, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        content.add(scroll, BorderLayout.CENTER);

        formulaProxy.add(new JLabel("Waiting for data..."), BorderLayout.CENTER);

        JButton saveImage = UiUtils.createSaveImageButton();
        saveImage.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String fileName = Logger.getDate() + "_formulas.png";

                UiUtils.saveImageWithPrompt(fileName, formulaProxy, formulaProxy);
            }
        });

        final JButton pauseButton = UiUtils.createPauseButton();
        pauseButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                isPaused = !isPaused;
                UiUtils.setPauseButtonText(pauseButton, isPaused);
            }
        });
        JPanel topButtonsPanel = new JPanel(new FlowLayout());
        topButtonsPanel.add(saveImage);
        topButtonsPanel.add(pauseButton);
        content.add(topButtonsPanel, BorderLayout.NORTH);

        JPanel bottomPanel = new JPanel(new FlowLayout());
        bottomPanel.add(new IntGaugeLabel("count", Sensor.totalTriggerErrorCounter));
        bottomPanel.add(new IntGaugeLabel("error", Sensor.lastErrorCode));

        content.add(bottomPanel, BorderLayout.SOUTH);
    }

    public JPanel getContent() {
        return content;
    }
}