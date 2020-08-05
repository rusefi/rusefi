package com.rusefi.ts_plugin;

import javax.swing.*;
import java.awt.*;

public class SdCardReader {
    private final JPanel content = new JPanel();

    public SdCardReader() {
        content.add(new ConnectPanel().getContent());
    }

    public Component getContent() {
        return content;
    }
}
