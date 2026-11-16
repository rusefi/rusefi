package com.rusefi.ui.basic;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class StatusPanelWithProgressBarTest {
    @Test
    void activityStaysHiddenUntilWorkStartsAndRemainsVisibleForItsLog() throws Exception {
        AtomicReference<StatusPanelWithProgressBar> panel = new AtomicReference<>();
        AtomicReference<Boolean> visible = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            StatusPanelWithProgressBar value = new StatusPanelWithProgressBar();
            panel.set(value);
            value.setActivityVisibilityListener(visible::set);
            assertFalse(value.isActivityVisible());
            value.updateFirmwarePhase(UpdateOperationCallbacks.FirmwareUpdateStage.DOWNLOAD);
            assertTrue(value.isActivityVisible());
        });
        SwingUtilities.invokeAndWait(() -> {});
        assertEquals(Boolean.TRUE, visible.get());
        SwingUtilities.invokeAndWait(() -> {
            panel.get().done();
            assertTrue(panel.get().isActivityVisible(), "completed operation keeps its log available");
            panel.get().clear();
        });
        SwingUtilities.invokeAndWait(() -> {});
        assertEquals(Boolean.FALSE, visible.get(), "clearing for the next operation hides old activity");
        SwingUtilities.invokeAndWait(() -> panel.get().error());
        SwingUtilities.invokeAndWait(() -> {});
        assertTrue(panel.get().isActivityVisible(), "an error before progress is still surfaced");
    }

    @Test
    void rejectedJobDisplaysExactReasonOnEdtAndRetainsLog() throws Exception {
        AtomicReference<StatusPanelWithProgressBar> panel = new AtomicReference<>();
        AtomicReference<String> displayed = new AtomicReference<>();
        AtomicInteger displays = new AtomicInteger();
        SwingUtilities.invokeAndWait(() -> panel.set(spy(new StatusPanelWithProgressBar(reason -> {
            assertTrue(SwingUtilities.isEventDispatchThread());
            displayed.set(reason);
            displays.incrementAndGet();
        }))));
        String reason = "Install an intermediate version first.\nDetails: <current> & <destination>";
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(panel.get());
        CountDownLatch finished = new CountDownLatch(1);
        executor.addOnJobReleasedListener(finished::countDown);

        assertTrue(executor.startJob(new AsyncJob("blocked firmware") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
                callbacks.firmwareUpdateBlocked(reason);
                callbacks.error();
                onJobFinished.run();
            }
        }, null));
        assertTrue(finished.await(5, TimeUnit.SECONDS));
        SwingUtilities.invokeAndWait(() -> {
            assertTrue(panel.get().getContent().getComponent(1).isVisible(),
                "a failed update automatically reveals the details log");
            assertTrue(panel.get().isActivityVisible(),
                "blocked guidance reveals Activity before any progress callback");
            JToggleButton details = find(panel.get().getContent(), JToggleButton.class);
            assertTrue(details.isSelected());
            assertEquals("Hide details", details.getText());
        });

        assertEquals(reason, displayed.get());
        assertEquals(1, displays.get());
        verify(panel.get()).log(reason, true, true);
        assertEquals(UpdateFirmwareResult.FAILURE, executor.getLastResult());
        assertTrue(executor.isNotInProgress());
    }

    @Test
    void acceptedJobRevealsActivityBeforeItsFirstProgressCallback() throws Exception {
        AtomicReference<StatusPanelWithProgressBar> panel = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> panel.set(new StatusPanelWithProgressBar()));
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(panel.get());
        CountDownLatch started = new CountDownLatch(1);
        CountDownLatch finish = new CountDownLatch(1);
        CountDownLatch released = new CountDownLatch(1);
        executor.addOnJobReleasedListener(released::countDown);

        assertTrue(executor.startJob(new AsyncJob("accepted without early progress") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
                callbacks.logLine("Starting operation");
                started.countDown();
                try {
                    finish.await(5, TimeUnit.SECONDS);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
                callbacks.done();
                onJobFinished.run();
            }
        }, null));
        try {
            assertTrue(started.await(5, TimeUnit.SECONDS));
            SwingUtilities.invokeAndWait(() -> {
                assertTrue(panel.get().isActivityVisible());
                assertTrue(panel.get().getContent().isVisible());
                assertTrue(((JTextArea) panel.get().getLogTextArea()).getText().contains("Starting operation"));
            });
        } finally {
            finish.countDown();
        }
        assertTrue(released.await(5, TimeUnit.SECONDS));
        SwingUtilities.invokeAndWait(() -> {
            assertTrue(panel.get().isActivityVisible(), "completion retains Activity and its log");
            assertTrue(((JTextArea) panel.get().getLogTextArea()).getText().contains("Starting operation"));
        });
    }

    @Test
    void ordinaryErrorsDoNotOpenBlockedUpdateScreen() throws Exception {
        AtomicInteger displays = new AtomicInteger();
        SwingUtilities.invokeAndWait(() -> {
            StatusPanelWithProgressBar panel = new StatusPanelWithProgressBar(reason -> displays.incrementAndGet());
            panel.error();
            panel.done();
        });
        SwingUtilities.invokeAndWait(() -> {});
        assertEquals(0, displays.get());
    }

    @Test
    void nonUiCallbackDefaultsToLoggingTheReason() {
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class, CALLS_REAL_METHODS);
        callbacks.firmwareUpdateBlocked("Intermediate update required");
        verify(callbacks).log("Intermediate update required", true, true);
    }

    @Test
    void largeProgressAndSafeCompletionStatusStayVisibleAboveCollapsedDetails() throws Exception {
        AtomicReference<StatusPanelWithProgressBar> panel = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            StatusPanelWithProgressBar value = new StatusPanelWithProgressBar();
            panel.set(value);
            JPanel content = value.getContent();
            assertTrue(content.getLayout() instanceof BorderLayout);
            Component top = ((BorderLayout) content.getLayout()).getLayoutComponent(BorderLayout.NORTH);
            JProgressBar progress = find(top, JProgressBar.class);
            assertNotNull(progress);
            assertTrue(progress.getPreferredSize().height >= 40);
            assertTrue(progress.getFont().getSize2D() >= 16f);
            progress.setStringPainted(false);
            progress.setValue(50);
            Dimension progressSize = progress.getPreferredSize();
            progress.setSize(progressSize);
            java.awt.image.BufferedImage painted = new java.awt.image.BufferedImage(
                progressSize.width, progressSize.height, java.awt.image.BufferedImage.TYPE_INT_ARGB);
            progress.paint(painted.getGraphics());
            int background = progress.getBackground().getRGB();
            int paintedRows = 0;
            for (int y = 0; y < painted.getHeight(); y++) {
                if (painted.getRGB(progress.getWidth() / 4, y) != background) {
                    paintedRows++;
                }
            }
            assertTrue(paintedRows >= 28, "painted fill should be visibly tall, rows=" + paintedRows);
            JToggleButton details = find(top, JToggleButton.class);
            assertNotNull(details);
            assertEquals("Show details", details.getText());
            Container topContainer = (Container) top;
            topContainer.setSize(topContainer.getPreferredSize());
            layout(topContainer);
            JLabel status = find(top, JLabel.class);
            JPanel detailsRow = (JPanel) details.getParent();
            Rectangle statusBounds = SwingUtilities.convertRectangle(status.getParent(), status.getBounds(), top);
            Rectangle progressBounds = SwingUtilities.convertRectangle(progress.getParent(), progress.getBounds(), top);
            Rectangle detailsBounds = SwingUtilities.convertRectangle(details.getParent(), details.getBounds(), top);
            assertEquals(statusBounds.x, progressBounds.x, "progress track aligns with its status label");
            assertEquals(statusBounds.x, detailsBounds.x, "details control aligns with the status label");
            assertEquals(12, detailsBounds.y - (progressBounds.y + progressBounds.height),
                "details control has a 12px gap after the progress track");
            assertEquals(8, detailsRow.getHeight() - (details.getY() + details.getHeight()),
                "details row adds 8px bottom padding");
            assertFalse(content.getComponent(1).isVisible(), "operation log starts collapsed");
            details.doClick();
            assertTrue(content.getComponent(1).isVisible());
            assertEquals("Hide details", details.getText());
            details.doClick();
            value.updateProgress(100);
        });
        SwingUtilities.invokeAndWait(() -> {
            JLabel status = find(panel.get().getContent(), JLabel.class);
            assertTrue(status.getText().contains("waiting for operation result"), status.getText());
        });
    }

    @Test
    void terminalErrorAndWarningStayVisibleAfterLogsAndProgress() throws Exception {
        for (boolean warning : new boolean[]{false, true}) {
            AtomicReference<StatusPanelWithProgressBar> panel = new AtomicReference<>();
            SwingUtilities.invokeAndWait(() -> {
                StatusPanelWithProgressBar value = new StatusPanelWithProgressBar();
                panel.set(value);
                if (warning) {
                    value.warning();
                } else {
                    value.error();
                }
                value.log("Detailed cause: <html>device changed", true, false);
                value.updateFirmwarePhase(UpdateOperationCallbacks.FirmwareUpdateStage.ERASE);
                value.updateProgress(100);
            });
            SwingUtilities.invokeAndWait(() -> {
                assertEquals(warning ? "Operation warning — see details" : "Operation failed — see details",
                    find(panel.get().getContent(), JLabel.class).getText());
                assertEquals(100, find(panel.get().getContent(), JProgressBar.class).getValue());
                assertTrue(panel.get().getContent().getComponent(1).isVisible());
                assertTrue(((JTextArea) panel.get().getLogTextArea()).getText().contains("<html>device changed"));
            });
        }
    }

    @Test
    void successfulTerminalCompletesProgressAndNextJobResetsIt() throws Exception {
        AtomicReference<StatusPanelWithProgressBar> panel = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> panel.set(new StatusPanelWithProgressBar()));
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(panel.get());
        CountDownLatch firstFinished = new CountDownLatch(1);
        executor.addOnJobReleasedListener(firstFinished::countDown);

        assertTrue(executor.startJob(new AsyncJob("successful update") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
                callbacks.updateProgress(99);
                callbacks.done();
                onJobFinished.run();
            }
        }, null));
        assertTrue(firstFinished.await(5, TimeUnit.SECONDS));
        SwingUtilities.invokeAndWait(() -> assertEquals(100, find(panel.get().getContent(), JProgressBar.class).getValue()));

        CountDownLatch secondStarted = new CountDownLatch(1);
        CountDownLatch finishSecond = new CountDownLatch(1);
        CountDownLatch secondFinished = new CountDownLatch(1);
        executor.addOnJobReleasedListener(secondFinished::countDown);
        assertTrue(executor.startJob(new AsyncJob("next update") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
                secondStarted.countDown();
                try {
                    finishSecond.await(5, TimeUnit.SECONDS);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
                callbacks.error();
                onJobFinished.run();
            }
        }, null));
        try {
            assertTrue(secondStarted.await(5, TimeUnit.SECONDS));
            SwingUtilities.invokeAndWait(() -> assertEquals(0, find(panel.get().getContent(), JProgressBar.class).getValue()));
        } finally {
            finishSecond.countDown();
        }
        assertTrue(secondFinished.await(5, TimeUnit.SECONDS));
        SwingUtilities.invokeAndWait(() -> assertEquals(0, find(panel.get().getContent(), JProgressBar.class).getValue()));
    }

    @Test
    void acceptedDownloadContinuesIntoFirmwareJobWithoutReset() throws Exception {
        AtomicReference<StatusPanelWithProgressBar> panel = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            panel.set(new StatusPanelWithProgressBar());
            panel.get().updateFirmwareProgress(UpdateOperationCallbacks.FirmwareUpdateStage.DOWNLOAD, 100);
        });
        SingleAsyncJobExecutor executor = new SingleAsyncJobExecutor(panel.get());
        CountDownLatch jobStarted = new CountDownLatch(1);
        CountDownLatch continueJob = new CountDownLatch(1);
        CountDownLatch finished = new CountDownLatch(1);
        executor.addOnJobReleasedListener(finished::countDown);

        assertTrue(executor.startJobContinuingProgress(new AsyncJob("downloaded firmware") {
            @Override
            public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
                jobStarted.countDown();
                try {
                    continueJob.await(5, TimeUnit.SECONDS);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
                callbacks.updateFirmwareProgress(UpdateOperationCallbacks.FirmwareUpdateStage.PROGRAM, 100);
                callbacks.done();
                onJobFinished.run();
            }
        }, null, () -> { }));

        try {
            assertTrue(jobStarted.await(5, TimeUnit.SECONDS));
            SwingUtilities.invokeAndWait(() -> assertEquals(15, find(panel.get().getContent(), JProgressBar.class).getValue()));
        } finally {
            continueJob.countDown();
        }
        assertTrue(finished.await(5, TimeUnit.SECONDS));
        SwingUtilities.invokeAndWait(() -> assertEquals(100, find(panel.get().getContent(), JProgressBar.class).getValue()));
    }

    @Test
    void failedFirmwareStageRemainsBelowCompletion() throws Exception {
        AtomicReference<StatusPanelWithProgressBar> panel = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            panel.set(new StatusPanelWithProgressBar());
            panel.get().updateFirmwareProgress(UpdateOperationCallbacks.FirmwareUpdateStage.PROGRAM, 100);
            panel.get().error();
        });
        SwingUtilities.invokeAndWait(() -> {
            assertEquals(95, find(panel.get().getContent(), JProgressBar.class).getValue());
            assertEquals("Operation failed — see details", find(panel.get().getContent(), JLabel.class).getText());
        });
    }

    @Test
    void firmwareStageShowsItsPhaseInsteadOfGenericProgress() throws Exception {
        AtomicReference<StatusPanelWithProgressBar> panel = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            panel.set(new StatusPanelWithProgressBar());
            panel.get().updateFirmwareProgress(UpdateOperationCallbacks.FirmwareUpdateStage.ERASE, 0);
        });
        SwingUtilities.invokeAndWait(() -> assertEquals("Operation: erasing",
            find(panel.get().getContent(), JLabel.class).getText()));
    }

    @Test
    void arbitraryHtmlAndLongLogLinesNeverBecomeTheHeadline() throws Exception {
        AtomicReference<StatusPanelWithProgressBar> panel = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            StatusPanelWithProgressBar value = new StatusPanelWithProgressBar();
            panel.set(value);
            StringBuilder log = new StringBuilder("<html>");
            for (int i = 0; i < 2000; i++) {
                log.append('x');
            }
            value.log(log.toString(), true, false);
        });
        SwingUtilities.invokeAndWait(() -> {
            JLabel status = find(panel.get().getContent(), JLabel.class);
            assertEquals("Ready", status.getText());
            assertTrue(status.getPreferredSize().width < 500);
            assertFalse(panel.get().getContent().getComponent(1).isVisible());
        });
    }

    private static <T extends Component> T find(Component root, Class<T> type) {
        if (type.isInstance(root)) {
            return type.cast(root);
        }
        if (root instanceof Container) {
            for (Component child : ((Container) root).getComponents()) {
                T found = find(child, type);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static void layout(Container container) {
        container.doLayout();
        for (Component child : container.getComponents()) {
            if (child instanceof Container) layout((Container) child);
        }
    }
}
