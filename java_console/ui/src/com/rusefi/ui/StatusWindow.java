package com.rusefi.ui;

import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;

/**
 * (c) Andrey Belomutskiy 2013-2015
 * 3/7/2015
 */
public class StatusWindow {
    // todo: extract driver from console bundle? find a separate driver bundle?
    private static final String CONSOLE_DRIVER_URI = "http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/utility/stsw-link004.zip";
    private final JTextArea log = new JTextArea();
    private final JPanel content = new JPanel(new BorderLayout());
    private final JScrollPane messagesScroll = new JScrollPane(log, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED) {
        @Override
        public Dimension getPreferredSize() {
            return new Dimension(400, 400);
        }
    };

    public StatusWindow() {
        log.setLineWrap(true);
    }

    // todo: make this method more abstract. table editor use-case does not need stm32 driver
    public void showFrame(String title) {
        content.add(new URLLabel("stm32 driver", CONSOLE_DRIVER_URI), BorderLayout.NORTH);
        content.add(messagesScroll, BorderLayout.CENTER);

        FrameHelper f = new FrameHelper();
        f.getFrame().setTitle(title);
        f.showFrame(content, false);
        UiUtils.centerWindow(f.getFrame());
        log.setText(""); // let's remove stuff from previous invocation
    }

    public void appendMsg(final String s) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                log.append(s + "\r\n");
                UiUtils.trueLayout(log);
            }
        });
    }
}
