package com.rusefi.ui.lua;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.ui.UIContext;
import org.fife.ui.rsyntaxtextarea.RSyntaxTextArea;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

class LuaScriptPanelCharacterizationTest {
    private static final String ECU_SCRIPT = "-- scriptname script.lua\nreturn 'from ECU'\n";
    private static final String DISK_SCRIPT = "-- scriptname script.lua\nreturn 'from disk'\n";
    private static final String REFRESHED_SCRIPT = "-- scriptname script.lua\nreturn 'refreshed ECU'\n";

    @TempDir
    Path tempDir;
    private final List<Harness> harnesses = new ArrayList<>();

    @AfterEach
    void closeContexts() {
        for (Harness harness : harnesses) {
            harness.context.getLinkManager().setNotifyGlobalStatusOnClose(false).close();
        }
    }

    private Harness harness(BinaryProtocol protocol, Node config, boolean connected) throws Exception {
        Harness harness = new Harness(protocol, config, connected);
        harnesses.add(harness);
        return harness;
    }

    @Test
    void reloadFromDisk() throws Exception {
        Node config = new Node();
        config.setProperty("SCRIPT_FOLDER", tempDir.toString());
        Files.write(tempDir.resolve("script.lua"), DISK_SCRIPT.getBytes(StandardCharsets.US_ASCII));
        TestProtocol testProtocol = new TestProtocol();
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(2);
        when(meta.getPageIdentifier(1)).thenReturn(testProtocol.field.getPageIndex());
        when(meta.getPageSize(1)).thenReturn(testProtocol.field.getSize());
        when(testProtocol.ini.getMetaInfo()).thenReturn(meta);
        when(testProtocol.protocol.getWireByteOrder()).thenReturn(java.nio.ByteOrder.LITTLE_ENDIAN);
        when(testProtocol.protocol.readFromPage(testProtocol.field.getPageIndex(), 0, testProtocol.field.getSize()))
            .thenReturn(scriptBytes(ECU_SCRIPT, testProtocol.field.getSize()));
        Harness harness = harness(testProtocol.protocol, config, false);
        SwingUtilities.invokeAndWait(() -> editor(harness.panel).setText(ECU_SCRIPT));

        SwingUtilities.invokeAndWait(() ->
            ((JMenuItem) harness.panel.createMoreMenu().getComponent(3)).doClick());
        harness.runPendingCommunicationTasks();

        AtomicReference<String> visibleText = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> visibleText.set(editor(harness.panel).getText()));
        assertEquals(DISK_SCRIPT, visibleText.get(), "The reload action updates the editor from disk");

        org.mockito.ArgumentCaptor<byte[]> transmitted =
            org.mockito.ArgumentCaptor.forClass(byte[].class);
        verify(testProtocol.protocol).writeInBlocks(transmitted.capture(), eq(0),
            eq(testProtocol.field.getOffset()), eq(testProtocol.field.getSize()),
            eq(testProtocol.field.getPageIndex()));
        String sent = new String(transmitted.getValue(), StandardCharsets.US_ASCII);
        assertTrue(sent.startsWith(DISK_SCRIPT), "Reload transmits the same script displayed in the editor");
        assertFalse(sent.startsWith(ECU_SCRIPT));
    }

    @Test
    void burningLuaOnASecondaryPageUsesItsWirePage() throws Exception {
        TestProtocol testProtocol = new TestProtocol();
        when(testProtocol.protocol.burnPage(testProtocol.field.getPageIndex())).thenReturn(true);
        Harness harness = harness(testProtocol.protocol, new Node(), false);

        SwingUtilities.invokeAndWait(() -> button(harness.panel, LuaScriptPanel.BURN).doClick(0));
        harness.runPendingCommunicationTasks();

        verify(testProtocol.protocol).burnPage(testProtocol.field.getPageIndex());
        org.mockito.Mockito.verify(testProtocol.protocol, org.mockito.Mockito.never()).burn();
    }

    @Test
    void burningLuaOnPageZeroKeepsTheMainPageBurnPath() throws Exception {
        TestProtocol testProtocol = new TestProtocol();
        testProtocol.field.setPageIndex(0);
        Harness harness = harness(testProtocol.protocol, new Node(), false);

        SwingUtilities.invokeAndWait(() -> button(harness.panel, LuaScriptPanel.BURN).doClick(0));
        harness.runPendingCommunicationTasks();

        verify(testProtocol.protocol).burn();
        org.mockito.Mockito.verify(testProtocol.protocol, org.mockito.Mockito.never()).burnPage(0);
    }

    @Test
    void burningWithoutALuaFieldDoesNotBurnTheMainPage() throws Exception {
        TestProtocol testProtocol = new TestProtocol();
        when(testProtocol.protocol.getIniFileNullable()).thenReturn(null);
        Harness harness = harness(testProtocol.protocol, new Node(), false);

        SwingUtilities.invokeAndWait(() -> button(harness.panel, LuaScriptPanel.BURN).doClick(0));
        harness.runPendingCommunicationTasks();

        verify(testProtocol.protocol, org.mockito.Mockito.never()).burn();
        verify(testProtocol.protocol, org.mockito.Mockito.never()).burnPage(anyInt());
    }

    @Test
    void rejectedLuaPageBurnIsReported() throws Exception {
        TestProtocol testProtocol = new TestProtocol();
        Harness harness = harness(testProtocol.protocol, new Node(), false);
        AtomicReference<String> error = new AtomicReference<>();
        MessagesCentral.MessageListener listener = (source, message) -> {
            if (source == LuaScriptPanel.class && message.startsWith("Failed to burn Lua:")) {
                error.set(message);
            }
        };
        MessagesCentral central = MessagesCentral.getInstance();
        central.addListener(listener);
        try {
            SwingUtilities.invokeAndWait(() -> button(harness.panel, LuaScriptPanel.BURN).doClick(0));
            harness.runPendingCommunicationTasks();
            flushEdt();

            assertEquals("Failed to burn Lua: The ECU rejected the Lua page burn.", error.get());
            verify(testProtocol.protocol, org.mockito.Mockito.never()).burn();
        } finally {
            central.removeListener(listener);
        }
    }

    @Test
    void reconnectRefreshWithUnwrittenEdits() throws Exception {
        refreshWithUnwrittenEdits(true);
    }

    @Test
    void cleanReconnectRefreshUpdatesEditor() throws Exception {
        TestProtocol testProtocol = new TestProtocol();
        when(testProtocol.protocol.readFromPage(eq(testProtocol.field.getPageIndex()),
            eq(testProtocol.field.getOffset()), eq(testProtocol.field.getSize())))
            .thenReturn(scriptBytes(ECU_SCRIPT, testProtocol.field.getSize()),
                scriptBytes(REFRESHED_SCRIPT, testProtocol.field.getSize()));

        Harness harness = harness(testProtocol.protocol, new Node(), true);
        harness.runPendingCommunicationTasks();
        flushEdt();

        harness.statusListener.onConnectionStatus(false);
        harness.runPendingCommunicationTasks();
        harness.statusListener.onConnectionStatus(true);
        harness.runPendingCommunicationTasks();
        flushEdt();

        AtomicReference<String> visibleText = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> visibleText.set(editor(harness.panel).getText()));
        assertEquals(REFRESHED_SCRIPT, visibleText.get(),
            "A clean editor should still accept the refreshed ECU script");
        verify(testProtocol.protocol, org.mockito.Mockito.times(2)).readFromPage(
            testProtocol.field.getPageIndex(), testProtocol.field.getOffset(), testProtocol.field.getSize());
    }

    @Test
    void initialFailedReadDoesNotCauseLaterEditToBeReplaced() throws Exception {
        TestProtocol testProtocol = new TestProtocol();
        when(testProtocol.protocol.readFromPage(eq(testProtocol.field.getPageIndex()),
            eq(testProtocol.field.getOffset()), eq(testProtocol.field.getSize())))
            .thenReturn(null, scriptBytes(ECU_SCRIPT, testProtocol.field.getSize()));

        Harness harness = harness(testProtocol.protocol, new Node(), true);
        harness.runPendingCommunicationTasks();
        flushEdt();

        String unsavedEdit = "-- typed before a successful ECU read\nreturn 'keep me'\n";
        SwingUtilities.invokeAndWait(() -> editor(harness.panel).setText(unsavedEdit));
        harness.statusListener.onConnectionStatus(true);
        harness.runPendingCommunicationTasks();
        flushEdt();

        AtomicReference<String> visibleText = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> visibleText.set(editor(harness.panel).getText()));
        assertEquals(unsavedEdit, visibleText.get(),
            "A failed initial read must not make later user edits eligible for replacement");
    }

    @Test
    void lateConnectedReadWithNewEdits() throws Exception {
        TestProtocol testProtocol = new TestProtocol();
        CountDownLatch readStarted = new CountDownLatch(1);
        CountDownLatch allowReadToComplete = new CountDownLatch(1);
        AtomicBoolean blockRead = new AtomicBoolean();
        when(testProtocol.protocol.readFromPage(eq(testProtocol.field.getPageIndex()),
            eq(testProtocol.field.getOffset()), eq(testProtocol.field.getSize())))
            .thenAnswer(invocation -> {
                if (blockRead.get()) {
                    readStarted.countDown();
                    if (!allowReadToComplete.await(5, TimeUnit.SECONDS)) {
                        throw new AssertionError("Timed out waiting to complete scripted ECU read");
                    }
                }
                return scriptBytes(ECU_SCRIPT, testProtocol.field.getSize());
            });

        Harness harness = harness(testProtocol.protocol, new Node(), true);
        harness.runPendingCommunicationTasks();
        flushEdt();

        String unsavedEdit = "-- unsaved user edit\nreturn 'keep me'\n";
        blockRead.set(true);
        harness.statusListener.onConnectionStatus(true);
        Thread refresh = new Thread(harness::runPendingCommunicationTasks, "Lua test ECU read");
        refresh.start();
        try {
            assertTrue(readStarted.await(5, TimeUnit.SECONDS), "The ECU read should enter the fake protocol");
            SwingUtilities.invokeAndWait(() -> editor(harness.panel).setText(unsavedEdit + "-- more typing\n"));
        } finally {
            allowReadToComplete.countDown();
            refresh.join(5000);
        }
        assertFalse(refresh.isAlive(), "The scripted ECU read should finish");
        flushEdt();

        AtomicReference<String> textAfterRefresh = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> textAfterRefresh.set(editor(harness.panel).getText()));

        assertEquals(unsavedEdit + "-- more typing\n", textAfterRefresh.get(),
            "A late successful connected refresh must preserve edits made while the read was in flight");
        verify(testProtocol.protocol, org.mockito.Mockito.times(2)).readFromPage(
            testProtocol.field.getPageIndex(), testProtocol.field.getOffset(), testProtocol.field.getSize());
    }

    @Test
    void disconnectRefreshWithUnwrittenEdits() throws Exception {
        refreshWithUnwrittenEdits(false);
    }

    private void refreshWithUnwrittenEdits(boolean connected) throws Exception {
        TestProtocol testProtocol = new TestProtocol();
        when(testProtocol.protocol.readFromPage(eq(testProtocol.field.getPageIndex()),
            eq(testProtocol.field.getOffset()), eq(testProtocol.field.getSize())))
            .thenReturn(scriptBytes(ECU_SCRIPT, testProtocol.field.getSize()));

        Harness harness = harness(testProtocol.protocol, new Node(), true);
        harness.runPendingCommunicationTasks();
        flushEdt();

        String unsavedEdit = "-- unsaved user edit\nreturn 'keep me'\n";
        SwingUtilities.invokeAndWait(() -> editor(harness.panel).setText(unsavedEdit));
        harness.statusListener.onConnectionStatus(connected);
        harness.runPendingCommunicationTasks();
        flushEdt();

        AtomicReference<String> visibleText = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> visibleText.set(editor(harness.panel).getText()));
        assertEquals(unsavedEdit, visibleText.get(),
            "Connection refresh must preserve edits made since the last accepted ECU read/write");
        verify(testProtocol.protocol, org.mockito.Mockito.times(connected ? 2 : 1)).readFromPage(
            testProtocol.field.getPageIndex(), testProtocol.field.getOffset(), testProtocol.field.getSize());
    }

    private static UIContext contextWithProtocol(BinaryProtocol protocol) {
        UIContext context = new UIContext();
        context.getLinkManager().setBinaryProtocolForTests(protocol);
        return context;
    }

    private static RSyntaxTextArea editor(LuaScriptPanel panel) {
        return (RSyntaxTextArea) panel.getZoomableText()[0];
    }

    private static JButton button(LuaScriptPanel panel, String text) {
        return findButton(panel.getPanel(), text);
    }

    private static JButton findButton(java.awt.Container parent, String text) {
        for (java.awt.Component child : parent.getComponents()) {
            if (child instanceof JButton && text.equals(((JButton) child).getText())) {
                return (JButton) child;
            }
            if (child instanceof java.awt.Container) {
                JButton found = findButton((java.awt.Container) child, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static byte[] scriptBytes(String text, int size) {
        byte[] bytes = new byte[size];
        byte[] script = text.getBytes(StandardCharsets.US_ASCII);
        System.arraycopy(script, 0, bytes, 0, script.length);
        return bytes;
    }

    private static void flushEdt() throws Exception {
        SwingUtilities.invokeAndWait(() -> {});
    }

    private static final class Harness {
        private final ArrayDeque<Runnable> communicationTasks = new ArrayDeque<>();
        private final UIContext context;
        private final ConnectionStatusLogic.Listener statusListener;
        private final LuaScriptPanel panel;

        private Harness(BinaryProtocol protocol, Node config, boolean fireInitialConnected) throws Exception {
            context = contextWithProtocol(protocol);
            AtomicReference<ConnectionStatusLogic.Listener> listener = new AtomicReference<>();
            AtomicReference<LuaScriptPanel> panelRef = new AtomicReference<>();
            SwingUtilities.invokeAndWait(() -> panelRef.set(new LuaScriptPanel(context, config,
                communicationTasks::add, connectionListener -> {
                    listener.set(connectionListener);
                    if (fireInitialConnected) {
                        connectionListener.onConnectionStatus(true);
                    }
                }, ignored -> {})));
            panel = panelRef.get();
            statusListener = listener.get();
        }

        private void runPendingCommunicationTasks() {
            Runnable task;
            while ((task = communicationTasks.poll()) != null) {
                task.run();
            }
        }
    }

    private static final class TestProtocol {
        private final BinaryProtocol protocol = mock(BinaryProtocol.class);
        private final StringIniField field = new StringIniField("luaScript", 0, 4096);
        private final IniFileModel ini = mock(IniFileModel.class);

        private TestProtocol() {
            field.setPageIndex(0x0400);
            when(ini.findIniField("LUASCRIPT")).thenReturn(Optional.of(field));
            when(protocol.getIniFileNullable()).thenReturn(ini);
        }
    }
}
