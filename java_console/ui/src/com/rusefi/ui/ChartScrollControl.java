package com.rusefi.ui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * 4/7/14
 * (c) Andrey Belomutskiy
 */
public class ChartScrollControl {
    private final JPanel content = new JPanel(new FlowLayout(FlowLayout.RIGHT, 5, 0));
    private final AtomicInteger index = new AtomicInteger();
    private static final JLabel info = new JLabel();

    public ChartScrollControl(final ChartRepository.CRListener listener) {

        content.setBorder(BorderFactory.createLineBorder(Color.red));


        setInfoText(index);

        JButton prev = new JButton("<");
        prev.setToolTipText("Previous page");
        prev.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (index.intValue() > 0) {
                    index.decrementAndGet();
                    listener.onDigitalChart(ChartRepository.getInstance().getChart(index.get()));
                    setInfoText(index);
                }
            }
        });


        JButton next = new JButton(">");
        next.setToolTipText("Next page");
        next.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (index.intValue() < ChartRepository.getInstance().getSize() - 1) {
                    index.incrementAndGet();
                    listener.onDigitalChart(ChartRepository.getInstance().getChart(index.get()));
                    setInfoText(index);
                }
            }
        });

        content.add(prev);
        content.add(info);
        content.add(next);

    }

    public JPanel getContent() {
        return content;
    }

    private void setInfoText(AtomicInteger index) {
        info.setText(index.get() + "/" + ChartRepository.getInstance().getSize());
    }

    public void reset() {
        index.set(0);
        setInfoText(index);
    }
}

