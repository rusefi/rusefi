package com.rusefi.ui.lua;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.TuningPane;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.UiUtils;
import org.fife.ui.rsyntaxtextarea.RSyntaxTextArea;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Component;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

class LuaTuneEditTest {
    private static final String SCRIPT = "return 'new script'";

    @Test
    void legacyWritePreservesPendingTuneAndWritesBeforeReset() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = new UIContext();
            context.setOfflineMode(true);
            context.setIniFileModelForOffline(ini);
            BinaryProtocol protocol = mock(BinaryProtocol.class);
            byte[] ram = new byte[128];
            when(protocol.getControllerConfiguration()).thenAnswer(ignored -> ConfigurationImageTestHelper.newLittleEndian(ram.clone()));
            when(protocol.getIniFileNullable()).thenReturn(ini);
            context.getLinkManager().setBinaryProtocolForTests(protocol);
            TuningPane tuning = new TuningPane(context);
            try {
                tuning.seedOfflineImage(ConfigurationImageTestHelper.newLittleEndian(ram.clone()), null);
                context.setOfflineMode(false);
                context.setTuneEditor(tuning::applyEdit);
                assertTrue(tuning.applyEdit(image -> image.getContent()[0] = 42));
                List<String> events = new ArrayList<>();
                doAnswer(invocation -> {
                    byte[] bytes = invocation.getArgument(0);
                    int source = invocation.getArgument(1);
                    int destination = invocation.getArgument(2);
                    int length = invocation.getArgument(3);
                    System.arraycopy(bytes, source, ram, destination, length);
                    events.add("write");
                    return null;
                }).when(protocol).writeInBlocks(any(byte[].class), anyInt(), anyInt(), anyInt(), eq(0));
                ArrayDeque<Runnable> tasks = new ArrayDeque<>();
                LuaScriptPanel panel = new LuaScriptPanel(context, new Node(), tasks::add,
                    listener -> {}, command -> {
                        assertEquals(SCRIPT, script(ram));
                        events.add("reset");
                    });
                ((RSyntaxTextArea) panel.getZoomableText()[0]).setText(SCRIPT);
                writeButton(panel).doClick(0);
                while (!tasks.isEmpty()) {
                    tasks.remove().run();
                }

                assertEquals(Arrays.asList("write", "reset"), events);
                ConfigurationImage working = context.getCurrentConfigurationImage();
                assertEquals(42, working.getContent()[0], "Writing Lua preserves pending tuning edits");
                assertEquals(SCRIPT, script(working.getContent()), "The next tune upload must retain the script");
            } finally {
                tuning.destroy();
                context.getLinkManager().setNotifyGlobalStatusOnClose(false).close();
            }
        });
    }

    @Test
    void secondaryPageWriteDoesNotModifyTheMainTune() throws Throwable {
        IniFileModel ini = ini();
        StringIniField field = (StringIniField) ini.findIniField("luaScript").get();
        field.setPageIndex(0x400);
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = new UIContext();
            BinaryProtocol protocol = mock(BinaryProtocol.class);
            when(protocol.getIniFileNullable()).thenReturn(ini);
            context.getLinkManager().setBinaryProtocolForTests(protocol);
            context.setTuneEditor(edit -> {
                fail("An extra Lua page is not part of the main tune");
                return false;
            });
            try {
                LuaScriptPanel panel = new LuaScriptPanel(context, new Node(), Runnable::run,
                    listener -> {}, command -> {});
                ((RSyntaxTextArea) panel.getZoomableText()[0]).setText(SCRIPT);
                writeButton(panel).doClick(0);
                verify(protocol).writeInBlocks(any(byte[].class), eq(0), eq(4), eq(64), eq(0x400));
            } finally {
                context.getLinkManager().setNotifyGlobalStatusOnClose(false).close();
            }
        });
    }

    @Test
    void acknowledgedWriteAllowsCleanRefresh() throws Throwable {
        refreshAfterWrite(false, false);
    }

    @Test
    void failedWritePreservesEditsAndDoesNotResetLua() throws Throwable {
        refreshAfterWrite(true, false);
    }

    @Test
    void acknowledgingOlderWritePreservesNewerEdits() throws Throwable {
        refreshAfterWrite(false, true);
    }

    @Test
    void acknowledgingDifferentWritePreservesTextRevertedBeforeAcknowledgement() throws Throwable {
        IniFileModel ini = ini();
        StringIniField field = (StringIniField) ini.findIniField("luaScript").get();
        field.setPageIndex(0x400);
        String initial = "return 'accepted A'";
        String written = "return 'written B'";
        UIContext context = new UIContext();
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        when(protocol.getIniFileNullable()).thenReturn(ini);
        when(protocol.readFromPage(0x400, 4, 64)).thenReturn(
            Arrays.copyOf(initial.getBytes(StandardCharsets.US_ASCII), 64),
            Arrays.copyOf(written.getBytes(StandardCharsets.US_ASCII), 64));
        context.getLinkManager().setBinaryProtocolForTests(protocol);
        ArrayDeque<Runnable> tasks = new ArrayDeque<>();
        AtomicReference<LuaScriptPanel> panel = new AtomicReference<>();
        try {
            SwingUtilities.invokeAndWait(() -> panel.set(new LuaScriptPanel(context, new Node(),
                tasks::add, listener -> {}, command -> {})));
            panel.get().readFromECU();
            SwingUtilities.invokeAndWait(() -> {});
            SwingUtilities.invokeAndWait(() -> {
                RSyntaxTextArea editor = (RSyntaxTextArea) panel.get().getZoomableText()[0];
                assertEquals(initial, editor.getText(), "The initial ECU read establishes accepted text A");
                editor.setText(written);
                writeButton(panel.get()).doClick(0);
                editor.setText(initial);
            });

            while (!tasks.isEmpty()) {
                tasks.remove().run();
            }
            SwingUtilities.invokeAndWait(() -> {});
            panel.get().readFromECU();
            SwingUtilities.invokeAndWait(() -> assertEquals(initial,
                ((RSyntaxTextArea) panel.get().getZoomableText()[0]).getText(),
                "After ECU acknowledges B, visible A is still an edit relative to the acknowledged baseline"));

            org.mockito.ArgumentCaptor<byte[]> transmitted =
                org.mockito.ArgumentCaptor.forClass(byte[].class);
            verify(protocol).writeInBlocks(transmitted.capture(), eq(0), eq(4), eq(64), eq(0x400));
            assertEquals(written, new String(transmitted.getValue(), 0, written.length(), StandardCharsets.US_ASCII));
        } finally {
            context.getLinkManager().setNotifyGlobalStatusOnClose(false).close();
        }
    }

    private static void refreshAfterWrite(boolean failWrite, boolean editDuringWrite) throws Throwable {
        IniFileModel ini = ini();
        StringIniField field = (StringIniField) ini.findIniField("luaScript").get();
        field.setPageIndex(0x400);
        UIContext context = new UIContext();
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        when(protocol.getIniFileNullable()).thenReturn(ini);
        String refreshed = "return 'ECU refreshed'";
        when(protocol.readFromPage(0x400, 4, 64)).thenReturn(
            Arrays.copyOf(refreshed.getBytes(StandardCharsets.US_ASCII), 64));
        if (failWrite) {
            doThrow(new IllegalStateException("write failed")).when(protocol)
                .writeInBlocks(any(byte[].class), anyInt(), anyInt(), anyInt(), anyInt());
        }
        context.getLinkManager().setBinaryProtocolForTests(protocol);
        ArrayDeque<Runnable> tasks = new ArrayDeque<>();
        List<String> resets = new ArrayList<>();
        AtomicReference<LuaScriptPanel> panel = new AtomicReference<>();
        String newer = SCRIPT + "\n-- newer edit";
        try {
            SwingUtilities.invokeAndWait(() -> {
                panel.set(new LuaScriptPanel(context, new Node(), tasks::add, listener -> {}, resets::add));
                RSyntaxTextArea editor = (RSyntaxTextArea) panel.get().getZoomableText()[0];
                editor.setText(SCRIPT);
                writeButton(panel.get()).doClick(0);
                if (editDuringWrite) {
                    editor.setText(newer);
                }
            });
            while (!tasks.isEmpty()) {
                tasks.remove().run();
            }
            SwingUtilities.invokeAndWait(() -> {});
            panel.get().readFromECU();
            SwingUtilities.invokeAndWait(() -> {
                String expected = editDuringWrite ? newer : failWrite ? SCRIPT : refreshed;
                assertEquals(expected, ((RSyntaxTextArea) panel.get().getZoomableText()[0]).getText());
            });
            assertEquals(failWrite ? 0 : 1, resets.size());
        } finally {
            context.getLinkManager().setNotifyGlobalStatusOnClose(false).close();
        }
    }

    private static JButton writeButton(LuaScriptPanel panel) {
        for (Component component : UiUtils.getAllComponents(panel.getPanel())) {
            if (component instanceof JButton && LuaScriptPanel.WRITE.equals(((JButton) component).getText())) {
                return (JButton) component;
            }
        }
        throw new AssertionError("No Lua Write button");
    }

    private static String script(byte[] bytes) {
        int end = 4;
        while (end < 68 && bytes[end] != 0) {
            end++;
        }
        return new String(bytes, 4, end - 4, StandardCharsets.US_ASCII);
    }

    private static IniFileModel ini() throws Throwable {
        String text = "[Constants]\nsignature = \"lua-tune-test\"\nnPages = 1\npageSize = 128\n"
            + "pageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
            + "fuel = scalar, U08, 0, \"\", 1, 0, 0, 100, 0\n"
            + "luaScript = string, ASCII, 4, 64\n"
            + "[Menu]\nmenu = \"Setup\"\nsubMenu = fuelSettings, \"Fuel\"\n"
            + "[UserDefined]\ndialog = fuelSettings, \"Fuel\"\nfield = \"Fuel\", fuel\n";
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(raw, "lua-tune-test.ini", new IniFileMetaInfoImpl(raw));
    }
}
