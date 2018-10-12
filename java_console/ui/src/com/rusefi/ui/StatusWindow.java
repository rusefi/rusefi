package com.rusefi.ui;

import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;

/**
 * (c) Andrey Belomutskiy 2013-2018
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
    @NotNull
    protected final FrameHelper frameHelper = new FrameHelper();

    public StatusWindow() {
        log.setLineWrap(true);
        // todo: move this somewhere else. table editor use-case does not need stm32 driver
        content.add(new URLLabel("stm32 driver", CONSOLE_DRIVER_URI), BorderLayout.NORTH);
        content.add(messagesScroll, BorderLayout.CENTER);
    }

    @NotNull
    public FrameHelper getFrameHelper() {
        return frameHelper;
    }

    public JPanel getContent() {
        return content;
    }

    public JFrame getFrame() {
        return frameHelper.getFrame();
    }

    public void showFrame(String title) {
        frameHelper.getFrame().setTitle(title);
        frameHelper.showFrame(content, false);
        UiUtils.centerWindow(frameHelper.getFrame());
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
