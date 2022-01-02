package com.rusefi.ui;

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
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class LiveDataPane {
    /**
     * this is the panel we expose to the outside world
     */
    private final JPanel content = new JPanel(new BorderLayout());
    private boolean isPaused;

    public LiveDataPane(UIContext uiContext) {


        JPanel vertical = new JPanel(new VerticalFlowLayout());
        JScrollPane scroll = new JScrollPane(vertical, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

        JPanel leftList = new JPanel(new VerticalFlowLayout());
        for (LiveDataView view : LiveDataView.values()) {
            JPanel liveDataParserContent = LiveDataParserPanel.createLiveDataParserContent(uiContext, view);

            JButton shortCut = new JButton(view.getFileName());
            shortCut.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    scroll.getVerticalScrollBar().setValue(liveDataParserContent.getLocation().y);
                }
            });
            leftList.add(shortCut);

            vertical.add(liveDataParserContent);
        }

        content.add(leftList, BorderLayout.WEST);
        content.add(scroll, BorderLayout.CENTER);

/*
        JButton saveImage = UiUtils.createSaveImageButton();
        saveImage.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String fileName = Logger.getDate() + "_formulas.png";

                UiUtils.saveImageWithPrompt(fileName, formulaProxy, formulaProxy);
            }
        });
*/

        final JButton pauseButton = UiUtils.createPauseButton();
        pauseButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                isPaused = !isPaused;
                UiUtils.setPauseButtonText(pauseButton, isPaused);
            }
        });
        JPanel topButtonsPanel = new JPanel(new FlowLayout());
// todo: implement this        topButtonsPanel.add(pauseButton);
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