package com.rusefi.ui.livedata;

import java.awt.*;

public interface SourceCodePainter {
    SourceCodePainter VOID = new SourceCodePainter() {
        @Override
        public void paintBackground(Color color, Range range) {
        }

        @Override
        public void paintForeground(Color color, Range range) {
        }
    };

    void paintBackground(Color color, Range range);

    void paintForeground(Color color, Range range);
}
