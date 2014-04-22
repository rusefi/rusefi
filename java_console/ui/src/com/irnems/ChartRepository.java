package com.irnems;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * 7/27/13
 * (c) Andrey Belomutskiy
 */
public class ChartRepository {
    private static final ChartRepository instance = new ChartRepository();

    private final List<String> charts = new ArrayList<String>();

    private ChartRepository() {
    }

    public static ChartRepository getInstance() {
        return instance;
    }

    public String getChart(int index) {
        return charts.get(index);
    }

    public int getSize() {
        return charts.size();
    }

    public void addChart(String value) {
        charts.add(value);
    }

    public void clear() {
        charts.clear();
    }

    public interface CRListener {
        public void onDigitalChart(String chart);
    }

    public JComponent createControls(final CRListener listener) {

        final AtomicInteger index = new AtomicInteger();

        JPanel result = new JPanel(new FlowLayout(FlowLayout.RIGHT, 5, 0));
        result.setBorder(BorderFactory.createLineBorder(Color.red));

        final JLabel info = new JLabel();

        setInfoText(index, info);

        JButton prev = new JButton("<");
        prev.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (index.intValue() > 0) {
                    index.decrementAndGet();
                    listener.onDigitalChart(charts.get(index.get()));
                    setInfoText(index, info);
                }
            }
        });


        JButton next = new JButton(">");
        next.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (index.intValue() < getSize() - 1) {
                    index.incrementAndGet();
                    listener.onDigitalChart(charts.get(index.get()));
                    setInfoText(index, info);
                }
            }
        });

        result.add(prev);
        result.add(info);
        result.add(next);

        return result;
    }

    private void setInfoText(AtomicInteger index, JLabel info) {
        info.setText(index.get() + "/" + getSize());
    }
}
