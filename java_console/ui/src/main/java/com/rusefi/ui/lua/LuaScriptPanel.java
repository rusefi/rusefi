package com.rusefi.ui.lua;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.ConnectionTab;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.MessagesPanel;
import com.rusefi.ui.UIContext;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.widgets.AnyCommand;
import neoe.formatter.lua.LuaFormatter;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.Objects;

import static com.devexperts.logging.Logging.getLogging;

public class LuaScriptPanel {
    private static final Logging log = getLogging(LuaScriptPanel.class);

    private static final String SCRIPT_FOLDER_CONFIG_KEY = "SCRIPT_FOLDER";
    private final UIContext context;
    private final Node config;
    private final JPanel mainPanel = new JPanel(new BorderLayout());
    private final AnyCommand command;
    private final LuaTextEditor scriptText;
    private final MessagesPanel mp;

    public LuaScriptPanel(UIContext context, Node config) {
        this.context = context;
        this.config = config;
        ConnectionTab.installConnectAndDisconnect(context, mainPanel);
        command = AnyCommand.createField(context, config, true, true);

        // Upper panel: command entry, etc
        JPanel upperPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 0));

        JButton writeButton = new JButton("Write to ECU");
        JButton burnButton = new JButton("Burn to ECU");

        mp = new MessagesPanel(null, config);

        mp.getMessagesView().listener = message -> {
            if (message.contains("BEEP"))
                Toolkit.getDefaultToolkit().beep();
        };

        writeButton.addActionListener(e -> {
            writeScriptToEcu();
        });

        burnButton.addActionListener(e -> {
            LinkManager linkManager = context.getLinkManager();

            linkManager.submit(() -> {
                BinaryProtocol bp = linkManager.getBinaryProtocol();
                bp.burn();
            });
        });

        JButton moreButton = new JButton("More...");
        moreButton.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseReleased(MouseEvent me) {
                JPopupMenu menu = new JPopupMenu();
                JMenuItem format = new JMenuItem("Format Script");
                format.addActionListener(e -> formatScript());
                JMenuItem reset = new JMenuItem("Reset Lua");
                reset.addActionListener(e -> resetLua());


                String scriptName = LuaIncludeSyntax.getScriptName(getScript());
                JMenuItem loadFromDisc;
                if (scriptName == null) {
                    loadFromDisc = new JMenuItem("Script name not specified");
                    loadFromDisc.setEnabled(false);
                } else if (!new File(getScriptFullFileName()).exists()) {
                    loadFromDisc = new JMenuItem(scriptName + " not found in " + getWorkingFolder());
                    loadFromDisc.setEnabled(false);
                } else {
                    loadFromDisc = new JMenuItem("Reload " + scriptName);
                    loadFromDisc.addActionListener(e -> reloadFromDisc());
                }

                JMenuItem selectFolder = createSelectFolderMenuItem();


                menu.add(format);
                menu.add(reset);
                menu.add(new JSeparator());
                menu.add(loadFromDisc);
                menu.add(selectFolder);


                menu.show(moreButton, me.getX(), me.getY());
            }
        });


        upperPanel.add(writeButton);
        upperPanel.add(burnButton);
        upperPanel.add(moreButton);
        upperPanel.add(command.getContent());
        upperPanel.add(new URLLabel("Lua Wiki", "https://wiki.rusefi.com/Lua-Scripting"));

        // Center panel - script editor and log
        JPanel scriptPanel = new JPanel(new BorderLayout());

        scriptText = new LuaTextEditor(context);
        JComponent editorComponent = scriptText.getControl();
        // enforce monospaced font for editor
        editorComponent.setFont(new Font(Font.MONOSPACED, Font.PLAIN, editorComponent.getFont().getSize()));
        scriptPanel.add(editorComponent, BorderLayout.CENTER);

        //centerPanel.add(, BorderLayout.WEST);
        JPanel messagesPanel = new JPanel(new BorderLayout());
        messagesPanel.add(BorderLayout.NORTH, mp.getButtonPanel());
        // enforce monospaced font for log view
        Font current = mp.getFont();
        Font mono = new Font(Font.MONOSPACED, Font.PLAIN, current.getSize());
        mp.setFont(mono, config);
        messagesPanel.add(BorderLayout.CENTER, mp.getMessagesScroll());

        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> {
            SwingUtilities.invokeLater(new Runnable() {
                @Override
                public void run() {
                    try {
                        readFromECU();
                    } catch (Throwable e) {
                        System.out.println(e);
                    }
                }
            });
        });

        JSplitPane centerPanel = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, scriptPanel, messagesPanel);
        centerPanel.setResizeWeight(0.5);
        centerPanel.setDividerLocation(0.5);

        mainPanel.add(upperPanel, BorderLayout.NORTH);
        mainPanel.add(centerPanel, BorderLayout.CENTER);

        AutoupdateUtil.trueLayoutAndRepaint(mainPanel);
    }

    private String getScriptFullFileName() {
        String scriptName = LuaIncludeSyntax.getScriptName(getScript());
        if (scriptName == null)
            return null;
        return getWorkingFolder() + File.separator + scriptName;
    }

    private void reloadFromDisc() {
        String fullFileName = getScriptFullFileName();
        if (fullFileName == null)
            return;
        System.out.println("Reading " + fullFileName);

        try {
            File file = new File(fullFileName);
            log.info("Reloading " + file.getAbsolutePath());
            String discContent = CompatibilityFiles.readString(file.toPath());

            String newLua = LuaIncludeSyntax.reloadScript(discContent, name -> {
                String includeFullName = getWorkingFolder() + File.separator + name;
                File includeFile = new File(includeFullName);
                log.info("Reading " + includeFile.getAbsolutePath());
                try {
                    String string = CompatibilityFiles.readString(includeFile.toPath());
                    log.info("Got " + string.length() + " bytes");
                    return string;
                } catch (IOException e) {
                    log.error("ERROR ", e);
                    return "ERROR reading " + name + ": " + e.getMessage();
                }
            });

            BinaryProtocol bp = context.getBinaryProtocol();
            StringIniField luaScript = getLuaScriptField(bp);

            if (newLua.length() >= luaScript.getSize()) {
                setText(newLua.length() + " bytes would not fit sorry current limit " + luaScript.getSize());
            } else {
                setText(newLua);
                // and send to ECU (without burn!)
                writeScriptToEcu();
            }
        } catch (IOException e) {
            System.err.println("Error " + e);
        }
    }

    @NotNull
    private JMenuItem createSelectFolderMenuItem() {
        JMenuItem selectFolder = new JMenuItem("Select Working Folder");
        selectFolder.addActionListener(e -> {
            JFileChooser fc = new JFileChooser();
            fc.setCurrentDirectory(new File(getWorkingFolder()));
            fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
            int returnVal = fc.showSaveDialog(selectFolder);
            if (returnVal == JFileChooser.APPROVE_OPTION) {
                String newWorkingFolder = fc.getSelectedFile().getPath();
                config.setProperty(SCRIPT_FOLDER_CONFIG_KEY, newWorkingFolder);
            }

        });
        return selectFolder;
    }

    private void formatScript() {
        String sourceCode = getScript();
        try {
            String formatted = new LuaFormatter().format(sourceCode, new LuaFormatter.Env());
            setText(formatted);
        } catch (Exception ignored) {
            // todo: fix luaformatter no reason for exception
        }
    }

    private void setText(String luaScript) {
        scriptText.setText(luaScript);
    }

    private String getWorkingFolder() {
        return config.getProperty(SCRIPT_FOLDER_CONFIG_KEY, System.getProperty("user.home"));
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
        BinaryProtocol bp = context.getBinaryProtocol();

        if (bp == null) {
            setText("No ECU located");
            return;
        }

        ConfigurationImage image = bp.getControllerConfiguration();
        if (image == null) {
            setText("No configuration image");
            return;
        }
        StringIniField luaScript = getLuaScriptField(bp);
        ByteBuffer luaScriptBuffer = image.getByteBuffer(luaScript.getOffset(), luaScript.getSize());

        byte[] scriptArr = new byte[luaScript.getSize()];
        luaScriptBuffer.get(scriptArr);

        int i = findNullTerminator(scriptArr);
        setText(new String(scriptArr, 0, i, StandardCharsets.US_ASCII));
    }

    static StringIniField getLuaScriptField(BinaryProtocol bp) {
        Objects.requireNonNull(bp, "BinaryProtocol");
        // todo: do we have "luaScript" as code-generated constant anywhere?
        IniFileModel iniFile = bp.getIniFile();
        Objects.requireNonNull(iniFile, "iniFile");
        return (StringIniField) iniFile.getIniField("LUASCRIPT");
    }

    @SuppressWarnings("StatementWithEmptyBody")
    private static int findNullTerminator(byte[] scriptArr) {
        int i;
        for (i = 0; i < scriptArr.length && scriptArr[i] != 0; i++) ;
        return i;
    }

    private void writeScriptToEcu() {
        String script = getScript();

        LinkManager linkManager = context.getLinkManager();

        linkManager.submit(() -> {
            BinaryProtocol bp = linkManager.getBinaryProtocol();

            StringIniField field = getLuaScriptField(bp);

            byte[] paddedScript = getScriptBytes(field, script);

            log.info("Sending " + field);
            bp.writeInBlocks(paddedScript, 0, field.getOffset(), paddedScript.length);

// need a way to modify script on the fly with shorter execution gaps to keep E65 CAN network happy
// todo: auto-burn on console close check box in case of Lua changes?
// todo: check box for auto-burn?
//            bp.burn();

            // Burning doesn't reload lua script, so we have to do it manually
            resetLua();
        });
        // resume messages on 'write new script to ECU'
        mp.setPaused(false);
    }

    private static byte @NotNull [] getScriptBytes(StringIniField luaScript, String script) {
        byte[] paddedScript = new byte[luaScript.getSize()];
        byte[] scriptBytes = script.getBytes(StandardCharsets.US_ASCII);
        System.arraycopy(scriptBytes, 0, paddedScript, 0, scriptBytes.length);
        return paddedScript;
    }

    private String getScript() {
        String script = scriptText.getText();
        return script;
    }

    void resetLua() {
        this.context.getCommandQueue().write("luareset");
    }
}
