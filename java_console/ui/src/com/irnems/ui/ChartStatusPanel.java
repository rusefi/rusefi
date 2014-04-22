package com.irnems.ui;

import com.irnems.waves.TimeAxisTranslator;
import com.rusefi.waves.WaveReport;
import com.irnems.waves.ZoomProvider;
import com.rusefi.waves.RevolutionLog;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.util.Map;

/**
 * Status bar at the bottom of Digital Sniffer - {@link WavePanel}
 * <p/>
 * <p/>
 * Date: 12/26/13
 * Andrey Belomutskiy (c) 2012-2013
 */
public class ChartStatusPanel {
    public final JPanel infoPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
    private final JLabel xLabel = new JLabel();
    private final JLabel timeLabel = new JLabel();
    private final JLabel angleLabel = new JLabel();
    private final JLabel rpmLabel = new JLabel();
    private TimeAxisTranslator translator = WaveReport.MOCK;

    private RevolutionLog time2rpm = RevolutionLog.parseRevolutions(null);

    final MouseMotionAdapter motionAdapter = new MouseMotionAdapter() {
        @Override
        public void mouseMoved(MouseEvent event) {
            int x = event.getX();
            xLabel.setText("" + x);

            /**
             * Time which corresponds to the mouse cursor screen location
             */
            double time = translator.screenToTime(x, infoPanel.getWidth(), zoomProvider);
            timeLabel.setText("" + String.format("%.5f sec", time));

            String text = time2rpm == null ? "n/a" : time2rpm.getCrankAngleByTimeString(time);
            angleLabel.setText(text);

            Map.Entry<Integer, Integer> e = time2rpm.getTimeAndRpm(time);
            if (e == null)
                rpmLabel.setText("n/a");
            else
                rpmLabel.setText("" + e.getValue());
        }
    };

    private ZoomProvider zoomProvider;

    public ChartStatusPanel(ZoomProvider zoomProvider) {
        this.zoomProvider = zoomProvider;
        infoPanel.add(new JLabel("X: "));
        infoPanel.add(xLabel);
        infoPanel.add(new JLabel(" time: "));
        infoPanel.add(timeLabel);

        infoPanel.add(new JLabel(" angle: "));
        infoPanel.add(angleLabel);

        infoPanel.add(new JLabel(" RPM: "));
        infoPanel.add(rpmLabel);
    }

    public void setWaveReport(TimeAxisTranslator translator) {
        this.translator = translator;
    }

    public void setRevolutions(StringBuilder revolutions) {
        time2rpm = RevolutionLog.parseRevolutions(revolutions);
    }
}
