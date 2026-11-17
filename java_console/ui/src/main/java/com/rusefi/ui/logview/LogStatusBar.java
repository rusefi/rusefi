package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import com.rusefi.ui.widgets.ConsoleFonts;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.Font;
import java.util.Locale;

/**
 * Status strip below the log viewer, styled like the Gauges status strip: what is loaded, which part
 * of it is visible, the cursor time, and how to navigate. [tag:console_theme]
 */
final class LogStatusBar extends ThemedStrip {
    static final String HELP = "Wheel: zoom  ·  Drag: zoom to range  ·  Right-drag or Shift+wheel: pan  ·  Double-click: whole log"
            + "  ·  Hover or click a legend item: min and max";

    final JLabel fileLabel = new JLabel();
    final JLabel viewLabel = new JLabel();
    final JLabel cursorLabel = new JLabel();
    final JLabel helpLabel = ChartToolbar.muted(new JLabel(HELP));

    LogStatusBar(String fileName, CsvLogReader log) {
        super(new BorderLayout(), "Console.StatusBar.background", true);
        setBorder(BorderFactory.createEmptyBorder(7, 12, 7, 12));
        String info = String.format(Locale.ROOT, "%,d samples  ·  %,d signals", log.sampleCount(), log.fieldCount() - 1);
        fileLabel.setText(fileName == null ? info : fileName + "  ·  " + info);
        cursorLabel.setFont(ConsoleFonts.mono(Font.PLAIN, ConsoleFonts.baseSize()));

        JPanel left = new JPanel(new FlowLayout(FlowLayout.LEFT, 14, 0));
        left.setOpaque(false);
        left.add(fileLabel);
        left.add(viewLabel);
        left.add(cursorLabel);
        add(left, BorderLayout.CENTER);
        add(helpLabel, BorderLayout.EAST);
    }

    void update(TimeWindow window, double cursorTime) {
        String format = window.span() < 10 ? "%.3f" : "%.2f";
        String range = String.format(Locale.ROOT, format + "–" + format + " s", window.start(), window.end());
        String full = String.format(Locale.ROOT, format + "–" + format + " s", window.fullStart(), window.fullEnd());
        viewLabel.setText(window.isFull() ? "Whole log  " + full : "Showing " + range + " of " + full);
        cursorLabel.setText(Double.isFinite(cursorTime)
                ? String.format(Locale.ROOT, "Cursor " + format + " s", cursorTime) : "");
    }
}
