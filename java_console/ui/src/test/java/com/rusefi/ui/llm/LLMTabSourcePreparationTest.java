package com.rusefi.ui.llm;

import com.rusefi.core.net.ConnectionAndMeta.DownloadProgressListener;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.widgets.tune.CalibrationFieldFactory;
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
            assertTrue(visible(JTextArea.class).isEmpty());
            assertTrue(visible(JComboBox.class).isEmpty());
            assertTrue(visible(JLabel.class).stream().anyMatch(label -> label.getText().contains("source code gist")));
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
            assertFalse(visible(JTextArea.class).isEmpty());
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

    @Test void troubleshootingInputsAndButtonsUseTuningFieldStyle() throws Exception {
        sources.cached = true;
        open();
        await(() -> button("Continue with ChatGPT") != null && button("Continue with ChatGPT").isEnabled());
        edt(() -> {
            int styledButtons = 0;
            for (String label : new String[]{"Continue with ChatGPT", "Sign out", "Cancel sign-in", "Send", "Stop", "New conversation"}) {
                JButton actual = button(label);
                assertNotNull(actual, label);
                JButton expected = new JButton();
                CalibrationFieldFactory.applyStyle(expected);
                assertEquals(expected.getFont(), actual.getFont(), label + " font");
                styledButtons++;
            }
            assertEquals(6, styledButtons);
            List<?> combos = visible(JComboBox.class);
            assertEquals(2, combos.size());
            int[] intendedWidths = {500, 250};
            for (int i = 0; i < combos.size(); i++) {
                JComboBox<?> combo = (JComboBox<?>) combos.get(i);
                JComboBox<?> expected = new JComboBox<>();
                CalibrationFieldFactory.applyStyle(expected);
                assertEquals(expected.getFont(), combo.getFont());
                assertEquals(intendedWidths[i], combo.getPreferredSize().width);
                assertEquals(expected.getPreferredSize().height, combo.getPreferredSize().height);
            }
            JTextArea prompt = visible(JTextArea.class).stream().filter(area -> "Message to ChatGPT".equals(
                    area.getAccessibleContext().getAccessibleName())).findFirst().orElseThrow();
            JTextArea expectedPrompt = new JTextArea();
            expectedPrompt.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 14));
            CalibrationFieldFactory.applyStyle(expectedPrompt);
            assertEquals(expectedPrompt.getFont(), prompt.getFont());
            assertEquals(Font.MONOSPACED, prompt.getFont().getFamily());
            return null;
        });
    }

    @Test void firstRegistrationOffersContinueInsteadOfDuplicateAddAccount() throws Exception {
        sources.cached = true;
        open();
        await(() -> button("Continue with ChatGPT") != null && button("Continue with ChatGPT").isEnabled());
        edt(() -> {
            assertNull(button("Add account"));
            return null;
        });
        assertTrue(Files.exists(directory.resolve("accounts/session.lock")));
    }

    @Test void startupPrefersPersistedLastReadyAccountAndLoadsModelsWithoutBrowser() throws Exception {
        sources.cached = true;
        seedExpiredAndReadyAccounts("ready-id");
        UiTransport transport = new UiTransport();
        open(transport, uri -> { throw new AssertionError("Startup must not launch a browser"); });
        await(() -> transport.models.get() == 1 && button("Add account") != null && button("Add account").isEnabled());
        edt(() -> {
            @SuppressWarnings("unchecked") JComboBox<ChatGptClient.Account> picker = (JComboBox<ChatGptClient.Account>) visible(JComboBox.class).get(0);
            assertEquals("ready-id", ((ChatGptClient.Account) picker.getSelectedItem()).id);
            assertFalse(button("Continue with ChatGPT").isEnabled());
            return null;
        });
    }

    @Test void startupFallsBackFromExpiredLastAccountToFirstReadyAccount() throws Exception {
        sources.cached = true;
        seedExpiredAndReadyAccounts("expired-id");
        UiTransport transport = new UiTransport();
        open(transport, uri -> { throw new AssertionError("Startup must not launch a browser"); });
        await(() -> transport.models.get() == 1 && button("Add account") != null && button("Add account").isEnabled());
        edt(() -> {
            @SuppressWarnings("unchecked") JComboBox<ChatGptClient.Account> picker = (JComboBox<ChatGptClient.Account>) visible(JComboBox.class).get(0);
            assertEquals("ready-first", ((ChatGptClient.Account) picker.getSelectedItem()).id);
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
        await(() -> transport.models.get() == 1 && button("Add account") != null && button("Add account").isEnabled());
        edt(() -> { button("Add account").doClick(); return null; });
        assertTrue(browserRequested.await(5, TimeUnit.SECONDS));
        await(() -> button("Cancel sign-in") != null && button("Cancel sign-in").isEnabled());
        edt(() -> { button("Cancel sign-in").doClick(); return null; });
        await(() -> button("Add account") != null && button("Add account").isEnabled()
                && button("Cancel sign-in") != null && !button("Cancel sign-in").isEnabled());
        edt(() -> {
            @SuppressWarnings("unchecked") JComboBox<ChatGptClient.Account> picker = (JComboBox<ChatGptClient.Account>) visible(JComboBox.class).get(0);
            assertEquals("ready-id", ((ChatGptClient.Account) picker.getSelectedItem()).id);
            assertEquals(3, picker.getItemCount());
            assertFalse(button("Continue with ChatGPT").isEnabled());
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
                && visible(JLabel.class).stream().anyMatch(label -> label.getText().contains("Network unavailable")));
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
    private <T> List<T> visible(Class<T> type) {
        List<T> result = new ArrayList<>();
        collect(tab.getContent(), type, result);
        return result;
    }
    private static <T> void collect(Component component, Class<T> type, List<T> result) {
        if (!component.isVisible()) { return; }
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
        final FakeDialogHandle handle = new FakeDialogHandle();
        @Override public LLMTab.AuthDialogHandle show(URI uri, Runnable onCancel) {
            this.onCancel.set(onCancel);
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
