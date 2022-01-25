package com.rusefi.ui;

import com.rusefi.CodeWalkthrough;
import com.rusefi.config.Field;
import com.rusefi.core.Sensor;
import com.rusefi.enums.live_data_e;
import com.rusefi.ldmp.StateDictionary;
import com.rusefi.livedata.LiveDataParserPanel;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.IntGaugeLabel;
import org.jetbrains.annotations.NotNull;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;


/**
 * Andrey Belomutskiy, (c) 2013-2020
 * @see LiveDataParserPanel
 * See LiveDataPaneSandbox
 */
public class LiveDataPane {
    public static final String CPP_SUFFIX = ".cpp";

    /**
     * this is the panel we expose to the outside world
     */
    private final JPanel content = new JPanel(new BorderLayout());
    private boolean isPaused;

    public LiveDataPane(UIContext uiContext) {


        JPanel vertical = new JPanel(new VerticalFlowLayout());
        JScrollPane scroll = new JScrollPane(vertical, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        JPanel legend = populateLegend();


        JPanel leftList = new JPanel(new VerticalFlowLayout());
        for (live_data_e view : live_data_e.values()) {
            String fileName = StateDictionary.INSTANCE.getFileName(view) + CPP_SUFFIX;
            Field[] values = StateDictionary.INSTANCE.getFields(view);
            JPanel liveDataParserContent = LiveDataParserPanel.createLiveDataParserPanel(uiContext, view, values, fileName).getContent();

            JButton shortCut = new JButton(fileName);
            shortCut.addActionListener(e -> {
                scroll.getVerticalScrollBar().setValue(liveDataParserContent.getLocation().y);
                // we want focus there so that mouse wheel scrolling would be active
                scroll.requestFocus();
            });
            leftList.add(shortCut);

            vertical.add(liveDataParserContent);
        }

        content.add(leftList, BorderLayout.WEST);
        content.add(scroll, BorderLayout.CENTER);
        content.add(legend, BorderLayout.EAST);

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

    @NotNull
    private JPanel populateLegend() {
        JPanel legend = new JPanel(new VerticalFlowLayout());
        legend.add(new JLabel("Legend:"));
        legend.add(createLabel(CodeWalkthrough.TRUE_CONDITION, "'true' condition"));
        legend.add(createLabel(CodeWalkthrough.FALSE_CONDITION, "'false' condition"));
        legend.add(createLabel(CodeWalkthrough.PASSIVE_CODE, "inactive code"));
        legend.add(createLabel(CodeWalkthrough.ACTIVE_STATEMENT, "active branch"));
        legend.add(createLabel(CodeWalkthrough.BROKEN_CODE, "No live data"));

        return legend;
    }

    @NotNull
    private JLabel createLabel(Color background, String text) {
        JLabel label = new JLabel(text);
        label.setOpaque(true);
        label.setBackground(background);
        return label;
    }

    public JPanel getContent() {
        return content;
    }
}