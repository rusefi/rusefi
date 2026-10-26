package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.widgets.AnyCommand;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.ToolbarStrip;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;

/**
 * One of console top level tabs. Laid out like the Gauges tab [tag:console_theme]: a toolbar with
 * RPM, the command box and Clear/Pause, the messages beside the quick commands, warnings below.
 * @see MessagesPanel
 */
public class MessagesPane {
    private static final String HELP_URL = "http://rusefi.com/wiki/index.php?title=Manual:Software:dev_console_commands";
    static final String HELP_TEXT = "Command reference";

    private final JPanel content = new JPanel(new BorderLayout()) {
        @Override
        public Dimension getPreferredSize() {
            Dimension size = super.getPreferredSize();
            return new Dimension(250, size.height);
        }
    };
    private final AnyCommand command;

    public MessagesPane(UIContext uiContext, final Node config) {
        command = AnyCommand.createArea(uiContext, config, config.getProperty(AnyCommand.KEY), true, false);
        final MessagesPanel messages = new MessagesPanel(null, config);
        messages.loadFont(config);
        messages.getMessagesScroll().setBorder(BorderFactory.createEmptyBorder());

        JPanel actions = new JPanel(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, 0));
        actions.setOpaque(false);
        actions.add(new RpmLabel(uiContext, 2).getContent());
        actions.add(command.getContent());
        actions.add(new JoinedButtons(messages.getButtons()));
        // GridBagLayout centres the link vertically in the toolbar
        JPanel help = new JPanel(new GridBagLayout());
        help.setOpaque(false);
        help.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, JoinedButtons.ROW_GAP));
        help.add(new URLLabel(HELP_TEXT, HELP_URL));
        JPanel toolbar = new ToolbarStrip(new BorderLayout());
        toolbar.setBorder(BorderFactory.createEmptyBorder(8, 0, 9, JoinedButtons.ROW_END_INSET - JoinedButtons.ROW_GAP));
        toolbar.add(actions, BorderLayout.CENTER);
        toolbar.add(help, BorderLayout.EAST);
        content.add(toolbar, BorderLayout.NORTH);

        JPanel middlePanel = new JPanel(new BorderLayout());
        middlePanel.add(messages.getMessagesScroll(), BorderLayout.CENTER);
        middlePanel.add(new RecentCommands(uiContext).getContent(), BorderLayout.EAST);
        content.add(middlePanel, BorderLayout.CENTER);

        content.add(new WarningPanel(config, uiContext).getPanel(config), BorderLayout.SOUTH);
    }

    public JComponent getContent() {
        return content;
    }

    public ActionListener getTabSelectedListener() {
        return e -> command.requestFocus();
    }
}
