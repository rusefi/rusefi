package com.rusefi.ui.basic;

import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;

public class TuneManagementTab {
    private final JPanel content = new JPanel(new VerticalFlowLayout());

    public TuneManagementTab(ImportTuneControl importTuneButton) {
        content.add(importTuneButton.getContent());
    }

    public Component getContent() {
        return content;
    }
}
