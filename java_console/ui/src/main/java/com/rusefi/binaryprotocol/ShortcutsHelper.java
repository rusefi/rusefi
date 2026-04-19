package com.rusefi.binaryprotocol;

import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;

public class ShortcutsHelper {
    public static void installConnectAndDisconnect(UIContext uiContext, JComponent control) {
        KeyStroke disconnectKeyStroke = KeyStroke.getKeyStroke(KeyEvent.VK_D, InputEvent.CTRL_MASK);
        KeyStroke connectKeyStroke = KeyStroke.getKeyStroke(KeyEvent.VK_R, InputEvent.CTRL_MASK);
        installKeyAction(connectKeyStroke, "connectCommand", control, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                reconnect(uiContext);
            }
        });

        installKeyAction(disconnectKeyStroke, "disconnectCommand", control, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                disconnect(uiContext);
            }
        });
    }

    public static void reconnect(UIContext uiContext) {
        uiContext.getLinkManager().reconnect();
    }

    public static void disconnect(UIContext uiContext) {
        uiContext.getLinkManager().disconnect();
    }

    public static void installKeyAction(KeyStroke undoKeyStroke, String actionName, JComponent control, AbstractAction action) {
        control.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(undoKeyStroke, actionName);
        control.getActionMap().put(actionName, action);
    }
}
