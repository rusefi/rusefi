package com.rusefi.ui.lua;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.MessagesCentral;
import com.rusefi.binaryprotocol.ShortcutsHelper;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.MessagesPaneBackgroundColor;
import com.rusefi.ui.MessagesPanel;
import com.rusefi.ui.UIContext;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.util.URLLabel;
import com.rusefi.ui.widgets.AnyCommand;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.ToolbarStrip;
import com.formdev.flatlaf.FlatClientProperties;
import neoe.formatter.lua.LuaFormatter;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.Objects;
import java.util.function.Consumer;

import static com.devexperts.logging.Logging.getLogging;

public class LuaScriptPanel {
    private static final Logging log = getLogging(LuaScriptPanel.class);

    private static final String SCRIPT_FOLDER_CONFIG_KEY = "SCRIPT_FOLDER";
    static final String WRITE = "Write to ECU";
    static final String BURN = "Burn to ECU";
    static final String MORE = "More…";
    static final String SCRIPT = "Script";
    static final String OUTPUT = "Output";
    private final UIContext context;
    private final Node config;
    private final JPanel mainPanel = new JPanel(new BorderLayout());
    private final AnyCommand command;
    private final LuaTextEditor scriptText;
    private final MessagesPanel mp;
    private final Consumer<Runnable> submitCommunicationTask;
    private final Consumer<String> commandWriter;

    public LuaScriptPanel(UIContext context, Node config) {
        this(context, config, task -> context.getLinkManager().submit(task),
            ConnectionStatusLogic.INSTANCE::addAndFireListener, command -> context.getCommandQueue().write(command));
    }

    LuaScriptPanel(UIContext context, Node config, Consumer<Runnable> submitCommunicationTask,
                   Consumer<ConnectionStatusLogic.Listener> connectionListenerRegistrar,
                   Consumer<String> commandWriter) {
        this.context = context;
        this.config = config;
        this.submitCommunicationTask = submitCommunicationTask;
        this.commandWriter = commandWriter;
        command = AnyCommand.createField(context, config, true, false);

        JButton writeButton = new JButton(WRITE);
        writeButton.setToolTipText("Send the script to the ECU and restart Lua; lost at power off until burned");
        JButton burnButton = new JButton(BURN);
        burnButton.setToolTipText("Save the script on the ECU so it survives power off");
        burnButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        burnButton.setPreferredSize(new Dimension(burnButton.getPreferredSize().width + 24, JoinedButtons.HEIGHT));

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
            BinaryProtocol bp = linkManager.getBinaryProtocol();
            if (bp == null || context.isOfflineMode() || context.isTuneReviewInProgress()) {
                return;
            }

            submitCommunicationTask.accept(() -> {
                if (linkManager.getBinaryProtocol() != bp || bp.isClosed()
                        || context.isOfflineMode() || context.isTuneReviewInProgress()) {
                    return;
                }
                try {
                    StringIniField field = getLuaScriptField(bp);
                    if (field == null) {
                        throw new IllegalStateException("The ECU's INI does not define a Lua page.");
                    }
                    if (field.getPageIndex() == 0) {
                        bp.burn();
                    } else if (!bp.burnPage(field.getPageIndex())) {
                        throw new IllegalStateException("The ECU rejected the Lua page burn.");
                    } else {
                        SwingUtilities.invokeLater(() -> {
                            if (context.getBinaryProtocol() == bp) {
                                context.publishSecondaryTuneBurn(field.getPageIndex());
                            }
                        });
                    }
                } catch (RuntimeException ex) {
                    MessagesCentral.getInstance().postMessage(LuaScriptPanel.class,
                        "Failed to burn Lua: " + ex.getMessage());
                }
            });
        });

        JButton moreButton = new JButton(MORE);
        moreButton.setToolTipText("Format, reset Lua, reload the script from disk, working folder");
        moreButton.addActionListener(event -> createMoreMenu().show(moreButton, 0, moreButton.getHeight()));

        // toolbar in the Gauges tab style [tag:console_theme]: script actions, then the command box
        JPanel actions = new JPanel(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, 0));
        actions.setOpaque(false);
        actions.add(new JoinedButtons(writeButton));
        actions.add(burnButton);
        actions.add(new JoinedButtons(moreButton));
        actions.add(command.getContent());
        // GridBagLayout centres the link vertically in the toolbar
        JPanel help = new JPanel(new GridBagLayout());
        help.setOpaque(false);
        help.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, JoinedButtons.ROW_GAP));
        help.add(new URLLabel("Lua Wiki", "https://wiki.rusefi.com/Lua-Scripting"));
        JPanel upperPanel = new ToolbarStrip(new BorderLayout());
        upperPanel.setBorder(BorderFactory.createEmptyBorder(8, 0, 9, JoinedButtons.ROW_END_INSET - JoinedButtons.ROW_GAP));
        upperPanel.add(actions, BorderLayout.CENTER);
        upperPanel.add(help, BorderLayout.EAST);

        // Center panel - script editor and log, each under its own titled header
        scriptText = new LuaTextEditor(context);
        if (context.isOfflineMode()) {
            IniFileModel ini = context.iniFileState.getIniFileModel();
            StringIniField field = ini == null ? null : ini.findIniField("LUASCRIPT")
                .filter(f -> f instanceof StringIniField)
                .map(f -> (StringIniField) f)
                .orElse(null);
            if (field != null) {
                ConfigurationImage page = field.getPageIndex() == 0
                    ? context.getCurrentConfigurationImage()
                    : context.getWorkingSecondaryTunePages().get(field.getPageIndex());
                if (page != null) {
                    setTextIfUnmodified(scriptFrom(page, field));
                }
            }
        }
        JPanel scriptPanel = new JPanel(new BorderLayout());
        scriptPanel.add(ToolbarStrip.header(SCRIPT, null), BorderLayout.NORTH);
        scriptPanel.add(scriptText.getControl(), BorderLayout.CENTER);

        JPanel messagesPanel = new JPanel(new BorderLayout());
        messagesPanel.add(ToolbarStrip.header(OUTPUT, new JoinedButtons(mp.getButtons())), BorderLayout.NORTH);
        mp.getMessagesScroll().setBorder(BorderFactory.createEmptyBorder());
        messagesPanel.add(BorderLayout.CENTER, mp.getMessagesScroll());

        // addAndFireListener (not addListener): the Lua tab is usually opened AFTER the ECU is
        // already connected, so a plain listener would miss the connect event and never read the
        // script, leaving the editor blank. Firing immediately reads the current script on open.
        connectionListenerRegistrar.accept(isConnected -> {
            if (!isConnected) {
                return;
            }

            // readFromECU() does live ECU I/O (the Lua script lives on its own TS page now), which
            // must run on the communication thread — not the EDT — or executeCommand() throws
            // "Communication on wrong thread"
            submitCommunicationTask.accept(() -> {
                try {
                    readFromECU();
                } catch (Throwable e) {
                    log.error("Unexpected" + e, e);
                }
            });
        });

        JSplitPane centerPanel = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, scriptPanel, messagesPanel);
        centerPanel.setResizeWeight(0.5);
        centerPanel.setDividerLocation(0.5);
        centerPanel.setContinuousLayout(true);
        centerPanel.setBorder(BorderFactory.createEmptyBorder());

        mainPanel.add(upperPanel, BorderLayout.NORTH);
        mainPanel.add(centerPanel, BorderLayout.CENTER);

        AutoupdateUtil.trueLayoutAndRepaint(mainPanel);
    }

    /** The "More…" menu, built on each click so the reload entry reflects the current script name. */
    JPopupMenu createMoreMenu() {
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

        return menu;
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

            if (luaScript == null) {
                setText("luaScript field not found in .ini");
                return;
            }

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
        // May be called from the communication thread (readFromECU); Swing requires EDT.
        if (SwingUtilities.isEventDispatchThread()) {
            scriptText.setText(luaScript);
        } else {
            SwingUtilities.invokeLater(() -> scriptText.setText(luaScript));
        }
    }

    private void setTextIfUnmodified(String luaScript) {
        Runnable update = () -> scriptText.setTextIfUnmodified(luaScript);
        if (SwingUtilities.isEventDispatchThread()) {
            update.run();
        } else {
            SwingUtilities.invokeLater(update);
        }
    }

    private String getWorkingFolder() {
        return config.getProperty(SCRIPT_FOLDER_CONFIG_KEY, System.getProperty("user.home"));
    }

    /** The script and its output, the text Ctrl+wheel resizes. */
    public JComponent[] getZoomableText() {
        return new JComponent[]{scriptText.getTextArea(), mp.getText()};
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
            return;
        }

        StringIniField luaScript = getLuaScriptField(bp);
        if (luaScript == null) {
            return;
        }

        if (luaScript.getPageIndex() == 0 && bp.getControllerConfiguration() == null) {
            log.info("readFromECU: page 0 branch but no configuration image");
            return;
        }
        String script = readScript(bp, luaScript);
        if (script == null) {
            log.info("readFromECU: readFromPage returned null for page=" + luaScript.getDisplayPage());
            return;
        }
        setTextIfUnmodified(script);
    }

    /**
     * The tune's Lua script, or null when it cannot be read. On the main settings page it comes from the
     * already-fetched controller image; on its own page (new firmware) it is read from the ECU, so callers
     * must be on the communication thread.
     */
    public static String readScript(BinaryProtocol bp, StringIniField luaScript) {
        if (luaScript.getPageIndex() == 0) {
            ConfigurationImage image = bp.getControllerConfiguration();
            return image == null ? null : scriptFrom(image, luaScript);
        }
        // The main image only holds page 0, so fetch directly from the ECU.
        byte[] scriptArr = bp.readFromPage(luaScript.getPageIndex(), luaScript.getOffset(), luaScript.getSize());
        return scriptArr == null ? null : toScript(scriptArr);
    }

    /** The script held in a main-page tune image (old firmware, or an offline tune); no ECU I/O. */
    public static String scriptFrom(ConfigurationImage image, StringIniField luaScript) {
        ByteBuffer luaScriptBuffer = image.getByteBuffer(luaScript.getOffset(), luaScript.getSize());
        byte[] scriptArr = new byte[luaScript.getSize()];
        luaScriptBuffer.get(scriptArr);
        return toScript(scriptArr);
    }

    private static String toScript(byte[] scriptArr) {
        int i = findNullTerminator(scriptArr);
        return new String(scriptArr, 0, i, StandardCharsets.US_ASCII);
    }

    public static StringIniField getLuaScriptField(BinaryProtocol bp) {
        Objects.requireNonNull(bp, "BinaryProtocol");
        // todo: do we have "luaScript" as code-generated constant anywhere?
        IniFileModel iniFile = bp.getIniFileNullable();
        if (iniFile == null)
            return null;
        // findIniField searches both the main page (old firmware) and secondary
        // pages (new firmware places luaScript on its own dedicated page).
        return iniFile.findIniField("LUASCRIPT")
            .filter(f -> f instanceof StringIniField)
            .map(f -> (StringIniField) f)
            .orElse(null);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    private static int findNullTerminator(byte[] scriptArr) {
        int i;
        for (i = 0; i < scriptArr.length && scriptArr[i] != 0; i++) ;
        return i;
    }

    private void writeScriptToEcu() {
        String script = getScript();
        BinaryProtocol bp = context.getBinaryProtocol();
        if (bp == null || context.isOfflineMode() || context.isTuneReviewInProgress()) {
            return;
        }
        StringIniField field = getLuaScriptField(bp);
        if (field == null) {
            log.error("luaScript field not found in .ini — cannot write");
            return;
        }
        byte[] paddedScript = getScriptBytes(field, script);
        // Legacy firmware keeps Lua in the main tune: later tune uploads must retain this edit.
        if (field.getPageIndex() == 0 && !context.applyTuneEdit(image ->
                System.arraycopy(paddedScript, 0, image.getContent(), field.getOffset(), paddedScript.length))) {
            log.error("Cannot edit the active tune — Lua script was not sent");
            return;
        }

        submitCommunicationTask.accept(() -> {
            if (context.getBinaryProtocol() != bp || bp.isClosed()
                    || context.isOfflineMode() || context.isTuneReviewInProgress()) {
                return;
            }
            try {
                ConfigurationImage before = readSecondaryTunePage(bp, field);
                log.info("Sending " + field + " page=" + field.getDisplayPage());
                bp.writeInBlocks(paddedScript, 0, field.getOffset(), paddedScript.length, field.getPageIndex());
                ConfigurationImage after = before == null ? null : before.clone();
                if (after != null) {
                    System.arraycopy(paddedScript, 0, after.getContent(), field.getOffset(), paddedScript.length);
                }
                // Reset only after the direct write is acknowledged, not after the tune's debounce.
                resetLua();
                SwingUtilities.invokeLater(() -> {
                    if (context.getBinaryProtocol() == bp) {
                        if (after != null) {
                            context.publishSecondaryTuneEdit(field.getPageIndex(), before, after);
                        }
                        scriptText.markWrittenTextClean(script);
                    }
                });
            } catch (RuntimeException e) {
                log.error("Failed to write Lua script", e);
            }
        });
        // resume messages on 'write new script to ECU'
        mp.setPaused(false);
        // the new script may no longer send 'set_bg_color=', drop back to default
        MessagesPaneBackgroundColor.reset();
    }

    private static ConfigurationImage readSecondaryTunePage(BinaryProtocol bp, StringIniField field) {
        if (field.getPageIndex() == 0) {
            return null;
        }
        com.opensr5.ini.IniFileMetaInfo meta = bp.getIniFileNullable().getMetaInfo();
        for (int ordinal = 1; ordinal < meta.getnPages(); ordinal++) {
            if (meta.getPageIdentifier(ordinal) == field.getPageIndex()) {
                int size = meta.getPageSize(ordinal);
                byte[] bytes = bp.readFromPage(field.getPageIndex(), 0, size);
                if (bytes == null || bytes.length != size || field.getOffset() + field.getSize() > size) {
                    throw new IllegalStateException("Unable to read the Lua calibration page.");
                }
                return new ConfigurationImage(bytes, bp.getWireByteOrder());
            }
        }
        return null;
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
        commandWriter.accept(Integration.CMD_LUA_RESET);
    }
}
