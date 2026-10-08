package com.rusefi.ui.llm;

import com.rusefi.core.net.ConnectionAndMeta.DownloadProgressListener;
import com.rusefi.io.LinkManager;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.*;
import java.awt.Component;
import java.awt.Container;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.FutureTask;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

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
