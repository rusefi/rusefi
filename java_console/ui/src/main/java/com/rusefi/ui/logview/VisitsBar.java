package com.rusefi.ui.logview;

import com.rusefi.ui.widgets.JoinedButtons;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Font;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import static com.rusefi.ui.logview.ThemedStrip.themed;

/**
 * The strip above the timeline after "Show on timeline" from a histogram cell: which cell, how often and how long
 * the engine was in it, and which stay is in view, with Previous / Next to step through the stays, Back to the
 * histogram, and × to clear the shading. Opens on the longest stay. [tag:console_theme]
 */
final class VisitsBar extends ThemedStrip {
    final JLabel titleLabel = new JLabel();
    final JLabel positionLabel = ChartToolbar.muted(new JLabel());
    final JButton previousButton = new JButton("‹ Previous");
    final JButton nextButton = new JButton("Next ›");
    final JButton backButton = new JButton("Back to histogram");
    final JButton closeButton = new JButton("×");

    private final LogChart chart;
    private List<double[]> visits = new ArrayList<>();
    private int current = -1;

    VisitsBar(LogChart chart, Runnable onBack) {
        super(new BorderLayout(12, 0), "Console.Banner.info.background", false);
        this.chart = chart;
        setBorder(BorderFactory.createEmptyBorder(6, 12, 6, 6));
        titleLabel.setForeground(themed("Console.Banner.info.accent", new Color(0x1d4ed8)));
        titleLabel.setFont(titleLabel.getFont().deriveFont(Font.BOLD));

        previousButton.setToolTipText("The stay before this one");
        nextButton.setToolTipText("The stay after this one");
        backButton.setToolTipText("Back to the histogram; the shading stays until you clear it");
        closeButton.setToolTipText("Clear the shading");
        closeButton.setBorderPainted(false);
        closeButton.setContentAreaFilled(false);
        closeButton.setFocusPainted(false);
        previousButton.addActionListener(e -> step(-1));
        nextButton.addActionListener(e -> step(1));
        backButton.addActionListener(e -> onBack.run());
        closeButton.addActionListener(e -> clear());

        JPanel text = new JPanel(new FlowLayout(FlowLayout.LEFT, 14, 0));
        text.setOpaque(false);
        text.add(titleLabel);
        text.add(positionLabel);
        JPanel buttons = new JPanel(new FlowLayout(FlowLayout.RIGHT, JoinedButtons.ROW_GAP, 0));
        buttons.setOpaque(false);
        buttons.add(new JoinedButtons(previousButton, nextButton));
        buttons.add(new JoinedButtons(backButton));
        buttons.add(closeButton);
        add(text, BorderLayout.CENTER);
        add(buttons, BorderLayout.EAST);
        setVisible(false);
    }

    /** Shades the stays and shows the longest; nothing happens for an empty list. */
    void show(String cell, List<double[]> stays) {
        if (stays.isEmpty()) {
            return;
        }
        visits = new ArrayList<>(stays);
        double total = 0;
        int longest = 0;
        for (int i = 0; i < visits.size(); i++) {
            total += visits.get(i)[2];
            if (visits.get(i)[2] > visits.get(longest)[2]) {
                longest = i;
            }
        }
        titleLabel.setText(String.format(Locale.ROOT, "%s  ·  %,d stay%s, %s in total", cell, visits.size(),
                visits.size() == 1 ? "" : "s", HistogramView.duration(total)));
        setVisible(true);
        go(longest);
    }

    private void step(int delta) {
        go(Math.floorMod(current + delta, visits.size()));
    }

    /** Shows one stay: stronger shading, in view with room around it. */
    void go(int index) {
        current = index;
        double[] visit = visits.get(index);
        chart.setHighlights(visits, index);
        chart.reveal(visit[0], visit[1]);
        positionLabel.setText(String.format(Locale.ROOT, "Stay %,d of %,d  ·  %.2f–%.2f s (%s)", index + 1,
                visits.size(), visit[0], visit[1], HistogramView.duration(visit[2])));
        previousButton.setEnabled(visits.size() > 1);
        nextButton.setEnabled(visits.size() > 1);
    }

    int current() {
        return current;
    }

    /** "Back to histogram" or "Back to scatter": where the stays came from. */
    void setBackLabel(String label) {
        backButton.setText(label);
        backButton.setToolTipText(label + "; the shading stays until you clear it");
    }

    void clear() {
        visits = new ArrayList<>();
        current = -1;
        chart.setHighlights(visits, -1);
        setVisible(false);
    }
}
