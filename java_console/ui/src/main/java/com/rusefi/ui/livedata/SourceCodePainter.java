package com.rusefi.ui.livedata;

import java.awt.*;

public interface SourceCodePainter {
    void paintBackground(Color color, Range range);

    void paintForeground(Color color, Range range);
}
