package com.rusefi.ui;

import com.rusefi.ui.engine.EngineSnifferPanel;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.widgets.AnyCommand;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;

/**
 * One of console top level tabs
 * @see MessagesPanel
 */
public class MessagesPane {
    private static final String HELP_URL = "http://rusefi.com/wiki/index.php?title=Manual:Software:dev_console_commands";

    private final JPanel content = new JPanel(new BorderLayout()) {
        @Override
        public Dimension getPreferredSize() {
            Dimension size = super.getPreferredSize();
            return new Dimension(250, size.height);
        }
    };
    private final AnyCommand command;

    public MessagesPane(UIContext uiContext, final Node config) {
        JPanel topPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 0));

        command = AnyCommand.createArea(uiContext, config, config.getProperty(AnyCommand.KEY), true, false);
        final MessagesPanel upperPanel = new MessagesPanel(command.getContent(), config);
        upperPanel.loadFont(config);

        JPanel middlePanel = new JPanel(new BorderLayout());
        middlePanel.add(upperPanel.getMessagesScroll(), BorderLayout.CENTER);
        middlePanel.add(new RecentCommands(uiContext).getContent(), BorderLayout.EAST);

        content.add(middlePanel, BorderLayout.CENTER);

        upperPanel.getButtonPanel().add(new RpmLabel(uiContext, 2).getContent());
        topPanel.add(upperPanel.getButtonPanel());
//        JButton fontButton = new JButton("Font");
//        topPanel.add(fontButton);
        topPanel.add(new URLLabel(EngineSnifferPanel.HELP_TEXT, HELP_URL));
        content.add(topPanel, BorderLayout.NORTH);

        JPanel statsPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));

        statsPanel.add(new RpmLabel(uiContext).getContent());
        statsPanel.add(new WarningPanel(config).getPanel(config));

        content.add(statsPanel, BorderLayout.SOUTH);

        /*fontButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFontChooser fc = new JFontChooser(ConsoleUI.getFrame());
                fc.setLocationRelativeTo(fontButton);
                if (fc.showDialog(upperPanel.getFont()) == JFontChooser.OK_OPTION) {
                    upperPanel.setFont(fc.getFont(), config);
                }
            }
        });*/
    }

    public JComponent getContent() {
        return content;
    }

    public ActionListener getTabSelectedListener() {
        return e -> command.requestFocus();
    }
}
