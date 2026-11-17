package com.rusefi.ui;

import com.rusefi.KeyStrokeShortcut;
import com.rusefi.ui.widgets.JoinedButtons;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * 4/7/14
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class ChartScrollControl {
    private final JPanel content = new JPanel(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, 0));
    private final AtomicInteger index = new AtomicInteger();
    private static final JLabel info = new JLabel();
    private ChartRepository.ChartRepositoryListener listener;

    public ChartScrollControl(final ChartRepository.ChartRepositoryListener listener) {

        this.listener = listener;
        info.setToolTipText("Current page index / total pages count");

        // toolbar style [tag:console_theme]: outlined arrows around the page index
        content.setOpaque(false);

        setInfoText(index);

        JButton prev = new JButton("<");
        prev.setToolTipText("Previous page (Left arrow)");
        prev.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                previousPage();
            }
        });


        JButton next = new JButton(">");
        next.setToolTipText("Next page (Right arrow)");
        next.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                nextPage();
            }
        });

        content.add(new JoinedButtons(prev));
        content.add(info);
        content.add(new JoinedButtons(next));

        bindKeyStrokeActions();
    }

    private void previousPage() {
        if (ChartRepository.getInstance().getSize() < index.get()) {
            index.set(0);
        }
        if (ChartRepository.getInstance().getSize() == 0)
            return;
        if (index.intValue() > 0) {
            index.decrementAndGet();
            showChart();
        } else if (index.intValue() == 0) {
            // let's go to the last chart if chart repository is not empty
            index.set(ChartRepository.getInstance().getSize() - 1);
            showChart();
        }
    }

    private void showChart() {
        listener.onDigitalChart(ChartRepository.getInstance().getChart(index.get()));
        setInfoText(index);
    }

    private void nextPage() {
        if (index.intValue() < ChartRepository.getInstance().getSize() - 1) {
            index.incrementAndGet();
            showChart();
        }
    }

    private void bindKeyStrokeActions() {
        InputMap inputMap = content.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);

        inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_LEFT, 0), KeyStrokeShortcut.PREVIOUS_PAGE);
        inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_RIGHT, 0), KeyStrokeShortcut.NEXT_PAGE);

        content.getActionMap().put(KeyStrokeShortcut.PREVIOUS_PAGE, new AbstractAction() {
            public void actionPerformed(ActionEvent event) {
                previousPage();
            }
        });
        content.getActionMap().put(KeyStrokeShortcut.NEXT_PAGE, new AbstractAction() {
            public void actionPerformed(ActionEvent event) {
                nextPage();
            }
        });
    }

    public JPanel getContent() {
        return content;
    }

    private void setInfoText(AtomicInteger index) {
        int humanIndex = index.get() + 1;
        info.setText(humanIndex + "/" + ChartRepository.getInstance().getSize());
    }

    public void reset() {
        index.set(0);
        setInfoText(index);
    }
}
