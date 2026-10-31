package com.rusefi.ui.llm;

import com.rusefi.core.net.ConnectionAndMeta.DownloadProgressListener;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.widgets.JoinedButtons;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.*;
import java.awt.Component;
import java.awt.Container;
import java.awt.Font;
import java.io.IOException;
import java.io.StringReader;
import java.net.URI;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.FutureTask;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class LLMTabSourcePreparationTest {
    @TempDir Path directory;
    private final LinkManager link = mock(LinkManager.class);
    private LLMTab tab;
    private final Sources sources = new Sources();

    @AfterEach void close() throws Exception {
        sources.release.countDown();
        if (tab != null) {
            edt(() -> { tab.close(); return null; });
        }
        verifyNoInteractions(link);
        if (Files.exists(directory.resolve("accounts"))) {
            // Also wait for asynchronous credential-store cleanup before JUnit removes the temp directory.
            await(() -> {
                try (ChatGptClient ignored = new ChatGptClient(directory.resolve("accounts"))) {
                    return true;
                } catch (IOException busy) {
                    return false;
                }
            });
        }
    }

    @Test void missingCacheShowsOnlyDownloadGateAndDoesNotOpenAccountStore() throws Exception {
        open();
        await(() -> button("Start Download") != null && button("Start Download").isEnabled());
        edt(() -> {
            assertEquals(1, visible(JButton.class).size());
            assertEquals(1, visible(JProgressBar.class).size());
            assertTrue(visible(JTextArea.class).stream().noneMatch(area -> area.getAccessibleContext().getAccessibleName() != null),
                    "neither the transcript nor the message box is shown");
            assertTrue(visible(JComboBox.class).isEmpty());
            assertTrue(visible(JLabel.class).stream().anyMatch(label -> label.getText() != null && label.getText().contains("source code gist")));
            return null;
        });
        assertEquals(0, sources.downloads.get());
        assertFalse(Files.exists(directory.resolve("accounts")));
    }

    @Test void keepsChatHiddenThroughProgressAndRevealsItOnlyAfterExtractionReturns() throws Exception {
        open();
        start();
        assertTrue(sources.downloading.await(5, TimeUnit.SECONDS));
        await(() -> visible(JProgressBar.class).get(0).getValue() == 100);
        edt(() -> {
            assertFalse(button("Start Download").isEnabled());
            button("Start Download").doClick(); // Disabled controls must not start a concurrent download.
            assertNull(button("Continue with ChatGPT"));
            return null;
        });
        assertEquals(1, sources.downloads.get());
        assertFalse(Files.exists(directory.resolve("accounts")));
        sources.release.countDown();
        await(() -> button("Continue with ChatGPT") != null && button("Continue with ChatGPT").isEnabled());
        edt(() -> {
            assertNull(button("Start Download"));
            assertTrue(visible(JLabel.class).stream().anyMatch(label -> "Ready".equals(label.getText())),
                    "the knowledge step shows it is done");
            assertNotNull(button("New conversation"), "the sidebar appears once the knowledge is there");
            return null;
        });
        assertTrue(Files.exists(directory.resolve("accounts/session.lock")));
    }

    @Test void validCachedSourcesOpenChatAutomaticallyWithoutDownload() throws Exception {
        sources.cached = true;
        open();
        await(() -> button("Continue with ChatGPT") != null && button("Continue with ChatGPT").isEnabled());
        assertEquals(0, sources.downloads.get());
    }

    @Test void conversationPageUsesConsoleControlStyle() throws Exception {
        sources.cached = true;
        seedExpiredAndReadyAccounts("ready-id");
        UiTransport transport = new UiTransport();
        open(transport, uri -> { throw new AssertionError("Startup must not launch a browser"); });
        await(() -> transport.models.get() == 1 && button("Send") != null && button("Send").isEnabled());
        edt(() -> {
            assertNull(button("Continue with ChatGPT"), "setup controls are hidden while chatting");
            assertNull(button("Stop"), "Stop takes Send's place only while a request runs");
            List<?> combos = visible(JComboBox.class);
            assertEquals(1, combos.size(), "only the model picker; accounts live in the account menu");
            JComboBox<?> model = (JComboBox<?>) combos.get(0);
            assertEquals(new JComboBox<>().getFont(), model.getFont());
            assertEquals(220, model.getPreferredSize().width);
            assertEquals(JoinedButtons.HEIGHT, model.getPreferredSize().height);
            JTextArea prompt = visible(JTextArea.class).stream().filter(area -> "Message to ChatGPT".equals(
                    area.getAccessibleContext().getAccessibleName())).findFirst().orElseThrow();
            assertEquals(new JTextArea().getFont(), prompt.getFont());
            InputMap keys = prompt.getInputMap(JComponent.WHEN_FOCUSED);
            assertEquals("send-prompt", keys.get(KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_ENTER, 0)));
            assertEquals(javax.swing.text.DefaultEditorKit.insertBreakAction, keys.get(KeyStroke.getKeyStroke(
                    java.awt.event.KeyEvent.VK_ENTER, java.awt.event.InputEvent.SHIFT_DOWN_MASK)));
            assertTrue(visible(JLabel.class).stream().anyMatch(label -> LLMTab.PRIVACY_NOTE.equals(label.getText())));
            assertTrue(visible(JLabel.class).stream().anyMatch(label -> "ready@example.com".equals(label.getText())),
                    "the account card shows the e-mail, not the generated connection name");
            assertNotNull(menuItem("Add account..."));
            assertTrue(menuItem("Sign out").isEnabled());
            assertTrue(visible(JLabel.class).stream().anyMatch(label -> label.getText() != null
                    && label.getText().startsWith("No ECU connected")));
            return null;
        });
        Path log = directory.resolve("cruise.msl");
        Files.write(log, "Time\tRPM\n0\t850\n".getBytes(java.nio.charset.StandardCharsets.UTF_8));
        Path binary = directory.resolve("log.mlg");
        Files.write(binary, new byte[]{'M', 'L', 0, 1});
        edt(() -> {
            assertNotNull(visible(JButton.class).stream().filter(b -> "Attach files".equals(
                    b.getAccessibleContext().getAccessibleName())).findFirst().orElse(null));
            assertTrue(visible(AttachmentBar.class).isEmpty(), "no attachment strip until something is attached");
            tab.attachFiles(java.util.Arrays.asList(log, binary));
            return null;
        });
        await(() -> tab.attachmentsForTest().pending().size() == 1
                && tab.attachmentsForTest().noteForTest().contains(".msl or .csv export"));
        edt(() -> {
            assertTrue(visible(JLabel.class).stream().anyMatch(label -> "cruise.msl".equals(label.getText())),
                    "the attached file shows as a chip");
            assertNotNull(visible(JButton.class).stream().filter(b -> "Remove cruise.msl".equals(
                    b.getAccessibleContext().getAccessibleName())).findFirst().orElse(null));
            return null;
        });
        try {
            com.rusefi.io.ConnectionStatusLogic.INSTANCE.setValue(com.rusefi.io.ConnectionStatusValue.CONNECTED);
            await(() -> visible(JLabel.class).stream().anyMatch(label -> label.getText() != null
                    && label.getText().startsWith("ECU connected")));
        } finally {
            com.rusefi.io.ConnectionStatusLogic.INSTANCE.setValue(com.rusefi.io.ConnectionStatusValue.NOT_CONNECTED);
        }
    }

    @Test void relativeTimesNameTodayAndYesterday() {
        long now = System.currentTimeMillis();
        assertTrue(LLMTab.relativeTime(now).startsWith("Today, "));
        assertTrue(LLMTab.relativeTime(now - TimeUnit.DAYS.toMillis(1)).startsWith("Yesterday, "));
        assertFalse(LLMTab.relativeTime(now - TimeUnit.DAYS.toMillis(400)).contains(","));
    }

    @Test void firstRegistrationOffersContinueInsteadOfDuplicateAddAccount() throws Exception {
        sources.cached = true;
        open();
        await(() -> button("Continue with ChatGPT") != null && button("Continue with ChatGPT").isEnabled());
        edt(() -> {
            assertNull(button("Use another account"));
            return null;
        });
        assertTrue(Files.exists(directory.resolve("accounts/session.lock")));
    }

    @Test void startupPrefersPersistedLastReadyAccountAndLoadsModelsWithoutBrowser() throws Exception {
        sources.cached = true;
        seedExpiredAndReadyAccounts("ready-id");
        UiTransport transport = new UiTransport();
        open(transport, uri -> { throw new AssertionError("Startup must not launch a browser"); });
        await(() -> transport.models.get() == 1 && button("Send") != null);
        edt(() -> {
            assertEquals("ready-id", tab.selectedAccountForTest().id);
            assertNull(button("Continue with ChatGPT"), "a ready account goes straight to the conversation");
            return null;
        });
    }

    @SuppressWarnings("unchecked")
    @Test void recentSessionResumesHistoryAndNewConversationKeepsItAvailable() throws Exception {
        sources.cached = true;
        seedExpiredAndReadyAccounts("ready-id");
        TroubleshootingSessionStore saved = new TroubleshootingSessionStore(directory.resolve("accounts/sessions"));
        org.json.simple.JSONArray history = new org.json.simple.JSONArray();
        history.add(ChatGptClient.object("role", "user", "content", "Why did RPM drop?"));
        TroubleshootingSessionStore.Session session = TroubleshootingSessionStore.Session
                .start("ready-id", "available", "Why did RPM drop?")
                .completed("available", history, "> Why did RPM drop?\nCheck voltage.\n", null);
        saved.save(session);
        UiTransport transport = new UiTransport();
        open(transport, uri -> { throw new AssertionError("Resume must not launch a browser"); });
        await(() -> transport.models.get() == 1 && tab.conversationsForTest().getModel().getSize() == 1
                && tab.conversationsForTest().isEnabled());
        edt(() -> {
            LLMTab.ConversationList list = tab.conversationsForTest();
            list.setSelectedIndex(0);
            list.getActionMap().get("open-conversation").actionPerformed(null);
            return null;
        });
        await(() -> visible(JTextArea.class).stream().anyMatch(area -> area.getText().contains("Check voltage.")));
        edt(() -> {
            assertTrue(visible(JLabel.class).stream().anyMatch(label -> "Why did RPM drop?".equals(label.getText())),
                    "the header names the open conversation");
            assertEquals(0, tab.conversationsForTest().getSelectedIndex(), "the open conversation is highlighted");
            assertTrue(button("Export case").isEnabled(), "a conversation with a finished turn can be exported");
            button("New conversation").doClick();
            assertFalse(button("Export case").isEnabled(), "a new conversation has nothing to export yet");
            assertEquals(1, tab.conversationsForTest().getModel().getSize());
            assertEquals(-1, tab.conversationsForTest().getSelectedIndex());
            assertTrue(visible(JLabel.class).stream().anyMatch(label -> "New conversation".equals(label.getText())));
            return null;
        });
        assertEquals(history, saved.load("ready-id", session.id).history);
    }

    @Test void startupFallsBackFromExpiredLastAccountToFirstReadyAccount() throws Exception {
        sources.cached = true;
        seedExpiredAndReadyAccounts("expired-id");
        UiTransport transport = new UiTransport();
        open(transport, uri -> { throw new AssertionError("Startup must not launch a browser"); });
        await(() -> transport.models.get() == 1 && button("Send") != null);
        edt(() -> {
            assertEquals("ready-first", tab.selectedAccountForTest().id);
            return null;
        });
    }

    @Test void missingPlanPermissionOffersSignInButDoesNotLabelTheAccountExpired() throws Exception {
        sources.cached = true;
        seedPlanOnlyAccount();
        open(new UiTransport(), uri -> { throw new AssertionError("Startup must not launch a browser"); });
        await(() -> button("Continue with ChatGPT") != null && button("Continue with ChatGPT").isEnabled());
        edt(() -> {
            assertTrue(visible(JLabel.class).stream().anyMatch(label -> "Connected; plan access required".equals(label.getText())));
            assertFalse(visible(JLabel.class).stream().anyMatch(label -> "Re-login required".equals(label.getText())));
            return null;
        });
    }

    @Test void cancellingAddAccountPreservesCurrentSelectionAndRegistration() throws Exception {
        sources.cached = true;
        seedExpiredAndReadyAccounts("ready-id");
        CountDownLatch browserRequested = new CountDownLatch(1);
        UiTransport transport = new UiTransport();
        open(transport, uri -> { browserRequested.countDown(); return true; });
        // Account-store startup briefly enables controls before queuing the initial model load.
        await(() -> transport.models.get() == 1 && menuItem("Add account...").isEnabled());
        edt(() -> { menuItem("Add account...").doClick(); return null; });
        assertTrue(browserRequested.await(5, TimeUnit.SECONDS));
        await(() -> button("Cancel sign-in") != null && button("Cancel sign-in").isEnabled());
        edt(() -> {
            assertNull(button("Send"), "the sign-in card replaces the conversation while signing in");
            button("Cancel sign-in").doClick();
            return null;
        });
        await(() -> button("Send") != null && button("Cancel sign-in") == null && menuItem("Add account...").isEnabled());
        edt(() -> {
            assertEquals("ready-id", tab.selectedAccountForTest().id);
            assertEquals(3, accountItems());
            assertNull(button("Continue with ChatGPT"));
            return null;
        });
    }

    @Test void modelessFallbackCancelAbortsAttemptAndTerminalCompletionDisposesIt() throws Exception {
        sources.cached = true;
        FakeFallbackDialog fallback = new FakeFallbackDialog();
        UiTransport transport = new UiTransport();
        open(transport, uri -> false, fallback);
        await(() -> button("Continue with ChatGPT") != null && button("Continue with ChatGPT").isEnabled());
        edt(() -> { button("Continue with ChatGPT").doClick(); return null; });
        assertTrue(fallback.shown.await(5, TimeUnit.SECONDS));
        await(() -> visible(JTextField.class).stream().anyMatch(field -> field.getText().equals(fallback.uri.get().toString())));
        edt(() -> {
            assertNotNull(button("Cancel sign-in"), "the sign-in card offers the link and a way out");
            assertTrue(button("Copy").isEnabled());
            return null;
        });
        fallback.onCancel.get().run();
        await(() -> button("Continue with ChatGPT") != null && button("Continue with ChatGPT").isEnabled());
        assertTrue(fallback.handle.disposed);
        assertEquals(0, transport.requests.get());
    }

    @Test void fallbackLifecycleDisposesOnTerminalPathsWithoutSyntheticCancelAndRejectsStaleCallbacks() {
        for (String terminal : new String[]{"success", "failure", "cancel"}) {
            LLMTab.AuthDialogLifecycle lifecycle = new LLMTab.AuthDialogLifecycle();
            ChatGptClient.Cancellation attempt = new ChatGptClient.Cancellation();
            FakeDialogHandle handle = new FakeDialogHandle();
            lifecycle.opened(attempt, handle);
            lifecycle.finish(attempt);
            assertTrue(handle.disposed, terminal);
            assertFalse(attempt.isCancelled(), terminal);
        }
        LLMTab.AuthDialogLifecycle lifecycle = new LLMTab.AuthDialogLifecycle();
        ChatGptClient.Cancellation active = new ChatGptClient.Cancellation();
        FakeDialogHandle handle = new FakeDialogHandle();
        lifecycle.opened(active, handle);
        lifecycle.close();
        assertTrue(handle.disposed);
        assertFalse(active.isCancelled(), "Programmatic disposal must not cancel a completed auth attempt");
        assertTrue(LLMTab.AuthDialogLifecycle.mayBrowse(active, active, false));
        assertFalse(LLMTab.AuthDialogLifecycle.mayBrowse(new ChatGptClient.Cancellation(), active, false));
        assertFalse(LLMTab.AuthDialogLifecycle.mayBrowse(active, active, true));
    }

    @Test void downloadFailureKeepsGateVisibleAndOffersRetry() throws Exception {
        sources.failDownload = true;
        sources.release.countDown();
        open();
        start();
        await(() -> button("Start Download") != null && button("Start Download").isEnabled()
                && visible(JLabel.class).stream().anyMatch(label -> label.getText() != null && label.getText().contains("Network unavailable")));
        assertFalse(Files.exists(directory.resolve("accounts")));
        sources.failDownload = false;
        start();
        await(() -> button("Continue with ChatGPT") != null && button("Continue with ChatGPT").isEnabled());
        assertEquals(2, sources.downloads.get());
    }

    @Test void corruptCacheRequiresExplicitDownload() throws Exception {
        sources.failCached = true;
        open();
        await(() -> button("Start Download") != null && button("Start Download").isEnabled());
        assertEquals(0, sources.downloads.get());
        assertFalse(Files.exists(directory.resolve("accounts")));
        sources.release.countDown();
        start();
        await(() -> button("Continue with ChatGPT") != null && button("Continue with ChatGPT").isEnabled());
        assertEquals(1, sources.downloads.get());
    }

    @Test void closingDuringPreparationPreventsLateChatAndAuthInitialization() throws Exception {
        open();
        start();
        assertTrue(sources.downloading.await(5, TimeUnit.SECONDS));
        edt(() -> { tab.close(); return null; });
        assertTrue(sources.interrupted.await(5, TimeUnit.SECONDS));
        edt(() -> {
            assertNull(button("Continue with ChatGPT"));
            return null;
        });
        assertFalse(Files.exists(directory.resolve("accounts")));
    }

    private void open() throws Exception {
        edt(() -> { tab = new LLMTab(directory.resolve("accounts"), link, sources); return null; });
    }
    private void open(UiTransport transport, LLMTab.BrowserOpener browser) throws Exception {
        open(transport, browser, null);
    }
    private void open(UiTransport transport, LLMTab.BrowserOpener browser, FakeFallbackDialog fallback) throws Exception {
        edt(() -> {
            tab = new LLMTab(directory.resolve("accounts"), link, sources,
                    path -> new ChatGptClient(path, transport), browser, fallback);
            return null;
        });
    }
    private void start() throws Exception {
        await(() -> button("Start Download") != null && button("Start Download").isEnabled());
        edt(() -> { button("Start Download").doClick(); return null; });
    }
    private JButton button(String text) {
        return visible(JButton.class).stream().filter(button -> text.equals(button.getText())).findFirst().orElse(null);
    }
    private JMenuItem menuItem(String text) {
        for (Component item : tab.buildAccountMenu().getComponents()) {
            if (item instanceof JMenuItem && text.equals(((JMenuItem) item).getText())) { return (JMenuItem) item; }
        }
        return null;
    }
    private int accountItems() {
        int count = 0;
        for (Component item : tab.buildAccountMenu().getComponents()) {
            if (item instanceof JRadioButtonMenuItem) { count++; }
        }
        return count;
    }
    private <T> List<T> visible(Class<T> type) {
        List<T> result = new ArrayList<>();
        collect(tab.getContent(), type, result);
        return result;
    }
    private static <T> void collect(Component component, Class<T> type, List<T> result) {
        // scroll bars bring arrow buttons of their own
        if (!component.isVisible() || component instanceof JScrollBar) { return; }
        if (type.isInstance(component)) { result.add(type.cast(component)); }
        if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) { collect(child, type, result); }
        }
    }
    private static <T> T edt(Callable<T> action) throws Exception {
        FutureTask<T> task = new FutureTask<>(action);
        SwingUtilities.invokeAndWait(task);
        return task.get();
    }
    private static void await(Callable<Boolean> condition) throws Exception {
        long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(5);
        while (!edt(condition)) {
            if (System.nanoTime() >= deadline) { fail("Timed out waiting for source preparation UI"); }
            Thread.sleep(10);
        }
    }

    @SuppressWarnings("unchecked")
    private void seedExpiredAndReadyAccounts(String lastAccount) throws Exception {
        try (ChatGptStore store = new ChatGptStore(directory.resolve("accounts"))) {
            org.json.simple.JSONObject profiles = (org.json.simple.JSONObject) store.data.get("profiles");
            profiles.put("expired-id", ChatGptClient.object("client_id", "client", "label", "A expired",
                    "email", "expired@example.com", "scope", "openid " + ChatGptClient.PLAN_SCOPE,
                    "access_token", "old-access", "expires_at", 0L));
            profiles.put("ready-first", ChatGptClient.object("client_id", "client", "label", "B ready first",
                    "email", "first@example.com", "scope", "openid " + ChatGptClient.PLAN_SCOPE,
                    "access_token", "old-access", "refresh_token", "refresh", "expires_at",
                    System.currentTimeMillis() + TimeUnit.HOURS.toMillis(1)));
            profiles.put("ready-id", ChatGptClient.object("client_id", "client", "label", "C ready last",
                    "email", "ready@example.com", "scope", "openid " + ChatGptClient.PLAN_SCOPE,
                    "access_token", "old-access", "refresh_token", "refresh", "expires_at",
                    System.currentTimeMillis() + TimeUnit.HOURS.toMillis(1)));
            store.data.put("last_account", lastAccount);
            store.save();
        }
    }

    @SuppressWarnings("unchecked")
    private void seedPlanOnlyAccount() throws Exception {
        try (ChatGptStore store = new ChatGptStore(directory.resolve("accounts"))) {
            org.json.simple.JSONObject profiles = (org.json.simple.JSONObject) store.data.get("profiles");
            profiles.put("plan-id", ChatGptClient.object("client_id", "client", "label", "Plan account",
                    "email", "plan@example.com", "scope", "openid profile", "access_token", "access",
                    "expires_at", System.currentTimeMillis() + TimeUnit.HOURS.toMillis(1)));
            store.save();
        }
    }

    private static final class UiTransport implements ChatGptClient.Transport {
        final AtomicInteger requests = new AtomicInteger();
        final AtomicInteger models = new AtomicInteger();
        @Override public java.io.Reader request(String method, String url, String token, String contentType,
                                                String body, ChatGptClient.Cancellation cancellation) throws Exception {
            cancellation.check();
            requests.incrementAndGet();
            if (url.endsWith("/models")) {
                models.incrementAndGet();
                return new StringReader("{\"models\":[{\"slug\":\"available\",\"visibility\":\"list\",\"display_name\":\"Available\"}]}");
            }
            throw new AssertionError("Unexpected API request " + url);
        }
    }

    private static final class FakeFallbackDialog implements LLMTab.FallbackDialogFactory {
        final CountDownLatch shown = new CountDownLatch(1);
        final AtomicReference<Runnable> onCancel = new AtomicReference<>();
        final AtomicReference<URI> uri = new AtomicReference<>();
        final FakeDialogHandle handle = new FakeDialogHandle();
        @Override public LLMTab.AuthDialogHandle show(URI uri, Runnable onCancel) {
            this.onCancel.set(onCancel);
            this.uri.set(uri);
            shown.countDown();
            return handle;
        }
    }

    private static final class FakeDialogHandle implements LLMTab.AuthDialogHandle {
        volatile boolean disposed;
        @Override public void dispose() { disposed = true; }
    }

    private final class Sources implements LLMTab.SourcePreparation {
        volatile boolean cached;
        volatile boolean failCached;
        volatile boolean failDownload;
        final AtomicInteger downloads = new AtomicInteger();
        final CountDownLatch downloading = new CountDownLatch(1);
        final CountDownLatch release = new CountDownLatch(1);
        final CountDownLatch interrupted = new CountDownLatch(1);

        @Override public Path prepareCached(DownloadProgressListener progress) throws Exception {
            assertFalse(SwingUtilities.isEventDispatchThread());
            if (failCached) { throw new IOException("Corrupt ZIP"); }
            return cached ? directory.resolve("sources") : null;
        }
        @Override public Path download(DownloadProgressListener progress) throws Exception {
            assertFalse(SwingUtilities.isEventDispatchThread());
            downloads.incrementAndGet();
            progress.onPercentage(100); // Completion of a progress event alone cannot reveal controls.
            downloading.countDown();
            try {
                if (!release.await(5, TimeUnit.SECONDS)) { throw new IOException("Test preparation timed out"); }
            } catch (InterruptedException e) {
                interrupted.countDown();
                throw e;
            }
            if (failDownload) { throw new IOException("Network unavailable"); }
            return directory.resolve("sources");
        }
    }
}
