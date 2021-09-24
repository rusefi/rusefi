package com.rusefi.ui.lua;

import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.ui.MessagesPanel;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.widgets.AnyCommand;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

import static com.rusefi.ui.util.UiUtils.trueLayout;

public class LuaScriptPanel {
    private final UIContext context;
    private final JPanel mainPanel = new JPanel(new BorderLayout());
    private final AnyCommand command;
    private final JTextArea scriptText = new JTextArea();
    private boolean isFirstRender = true;

    public LuaScriptPanel(UIContext context, Node config) {
        this.context = context;
        command = AnyCommand.createField(context, config, true, true);

        // Upper panel: command entry, etc
        JPanel upperPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 0));

        JButton readButton = new JButton("Read from ECU");
        JButton writeButton = new JButton("Write to ECU");
        JButton resetButton = new JButton("Reset/Reload Lua");

        readButton.addActionListener(e -> readFromECU());
        writeButton.addActionListener(e -> write());
        resetButton.addActionListener(e -> resetLua());

        upperPanel.add(readButton);
        upperPanel.add(writeButton);
        upperPanel.add(resetButton);
        upperPanel.add(command.getContent());

        // Center panel - script editor and log
        JPanel scriptPanel = new JPanel(new BorderLayout());
        scriptText.setTabSize(2);
        scriptPanel.add(scriptText, BorderLayout.CENTER);

        //centerPanel.add(, BorderLayout.WEST);
        JPanel messagesPanel = new JPanel(new BorderLayout());
        MessagesPanel mp = new MessagesPanel(null);
        messagesPanel.add(BorderLayout.NORTH, mp.getButtonPanel());
        messagesPanel.add(BorderLayout.CENTER, mp.getMessagesScroll());

        JSplitPane centerPanel = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, scriptPanel, messagesPanel) {
            @Override
            public void paint(Graphics g) {
                super.paint(g);
                if (isFirstRender) {
                    readFromECU();
                    isFirstRender = true;
                }
            }
        };

        mainPanel.add(upperPanel, BorderLayout.NORTH);
        mainPanel.add(centerPanel, BorderLayout.CENTER);

        trueLayout(mainPanel);
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                centerPanel.setDividerLocation(centerPanel.getSize().width / 2);
            }
        });
    }

    public JPanel getPanel() {
        return mainPanel;
    }

    public ActionListener getTabSelectedListener() {
        return e -> {
            if (command != null)
                command.requestFocus();
        };
    }

    void readFromECU() {
        BinaryProtocol bp = context.getLinkManager().getCurrentStreamState();

        if (bp == null) {
            scriptText.setText("No ECU located");
            return;
        }

        ConfigurationImage image = bp.getControllerConfiguration();
        if (image == null) {
            scriptText.setText("No configuration image");
            return;
        }
        ByteBuffer luaScriptBuffer = image.getByteBuffer(Fields.luaScript_offset, Fields.LUA_SCRIPT_SIZE);

        byte[] scriptArr = new byte[Fields.LUA_SCRIPT_SIZE];
        luaScriptBuffer.get(scriptArr);

        int i = findNullTerminator(scriptArr);
        scriptText.setText(new String(scriptArr, 0, i, StandardCharsets.US_ASCII));
    }

    @SuppressWarnings("StatementWithEmptyBody")
    private static int findNullTerminator(byte[] scriptArr) {
        int i;
        for (i = 0; i < scriptArr.length && scriptArr[i] != 0; i++) ;
        return i;
    }

    void write() {
        BinaryProtocol bp = this.context.getLinkManager().getCurrentStreamState();

        String script = scriptText.getText();

        byte[] paddedScript = new byte[Fields.LUA_SCRIPT_SIZE];
        byte[] scriptBytes = script.getBytes(StandardCharsets.US_ASCII);
        System.arraycopy(scriptBytes, 0, paddedScript, 0, scriptBytes.length);

        int idx = 0;
        int remaining;

        do {
            remaining = paddedScript.length - idx;
            int thisWrite = Math.min(remaining, Fields.BLOCKING_FACTOR);

            bp.writeData(paddedScript, idx, Fields.luaScript_offset + idx, thisWrite);

            idx += thisWrite;

            remaining -= thisWrite;
        } while (remaining > 0);

        bp.burn();

        // Burning doesn't reload lua script, so we have to do it manually
        resetLua();
    }

    void resetLua() {
        this.context.getCommandQueue().write("luareset");
    }
}
