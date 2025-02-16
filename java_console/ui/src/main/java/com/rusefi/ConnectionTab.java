package com.rusefi;

import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.lua.LuaTextEditor;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;

public class ConnectionTab {
    private final UIContext uiContext;
    private final JPanel content = new JPanel(new BorderLayout());

    public ConnectionTab(UIContext uiContext) {
        this.uiContext = uiContext;

        JPanel vertical = new JPanel(new VerticalFlowLayout());

        content.add(vertical, BorderLayout.CENTER);

        JButton connect = new JButton("Connect");

        JButton disconnect = new JButton("Disconnect");

        vertical.add(new JLabel("Same shortcuts work while on Lua tab"));
        vertical.add(new JLabel("Ctrl+R connect"));
        vertical.add(connect);
        vertical.add(new JLabel("Ctrl+D disconnect"));
        vertical.add(disconnect);

        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> {
            connect.setEnabled(!isConnected);
            disconnect.setEnabled(isConnected);
        });

        installConnectAndDisconnect(uiContext, content);
        disconnect.addActionListener(e -> disconnect(uiContext));
        connect.addActionListener(e -> reconnect(uiContext));
    }

    public static void installConnectAndDisconnect(UIContext uiContext, JComponent control) {
        KeyStroke disconnectKeyStroke = KeyStroke.getKeyStroke(KeyEvent.VK_D, InputEvent.CTRL_MASK);
        KeyStroke connectKeyStroke = KeyStroke.getKeyStroke(KeyEvent.VK_R, InputEvent.CTRL_MASK);
        LuaTextEditor.installKeyAction(connectKeyStroke, "connectCommand", control, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                reconnect(uiContext);
            }
        });

        LuaTextEditor.installKeyAction(disconnectKeyStroke, "disconnectCommand", control, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                disconnect(uiContext);
            }
        });
    }

    private static void reconnect(UIContext uiContext) {
        uiContext.getLinkManager().reconnect();
    }

    private static void disconnect(UIContext uiContext) {
        uiContext.getLinkManager().disconnect();
    }

    public JPanel getContent() {
        return content;
    }
}
