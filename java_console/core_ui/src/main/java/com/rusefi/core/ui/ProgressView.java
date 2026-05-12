package com.rusefi.core.ui;

import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.net.ConnectionAndMeta;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * @see FirmwareUpdateTabSandbox
 */
public class ProgressView {
    private final FrameHelper frameHelper;
    private final JProgressBar progressBar;
    private final JPanel panel;
    private final JLabel errorLabel;
    private final JButton retryButton;

    private volatile CountDownLatch retryLatch;
    private final AtomicBoolean retryRequested = new AtomicBoolean(false);

    ProgressView(FrameHelper frameHelper, JProgressBar progressBar, JPanel panel) {
        this.frameHelper = frameHelper;
        this.progressBar = progressBar;
        this.panel = panel;

        if (panel != null) {
            errorLabel = new JLabel();
            errorLabel.setForeground(Color.RED);
            errorLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
            errorLabel.setVisible(false);

            retryButton = new JButton("Retry");
            retryButton.setAlignmentX(Component.CENTER_ALIGNMENT);
            retryButton.setVisible(false);
            retryButton.addActionListener(e -> {
                retryRequested.set(true);
                if (retryLatch != null)
                    retryLatch.countDown();
            });

            panel.add(Box.createVerticalStrut(10));
            panel.add(errorLabel);
            panel.add(Box.createVerticalStrut(5));
            panel.add(retryButton);

            if (frameHelper != null) {
                frameHelper.getFrame().addWindowListener(new WindowAdapter() {
                    @Override
                    public void windowClosing(WindowEvent e) {
                        if (retryLatch != null)
                            retryLatch.countDown();
                    }
                });
            }
        } else {
            errorLabel = null;
            retryButton = null;
        }
    }

    static @NotNull ProgressView create(String title) {
        FrameHelper frameHelper = new FrameHelper();
        AutoupdateUtil.setAppIcon(frameHelper.getFrame());
        frameHelper.getFrame().setTitle(title);

        JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
        panel.setBorder(BorderFactory.createEmptyBorder(40, 40, 40, 40));

        ImageIcon logoIcon = AutoupdateUtil.loadIcon("/com/rusefi/logo.png");
        if (logoIcon != null) {
            JLabel logoLabel = new JLabel(logoIcon);
            logoLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
            panel.add(logoLabel);
            panel.add(Box.createVerticalStrut(20));
        }

        JLabel branchLabel = new JLabel(BundleUtil.readBundleFullNameNotNull().getUiLabel());
        branchLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        branchLabel.setFont(branchLabel.getFont().deriveFont(12f));
        panel.add(branchLabel);
        panel.add(Box.createVerticalStrut(20));

        JProgressBar jProgressBar = new JProgressBar();
        jProgressBar.setMaximum(ConnectionAndMeta.CENTUM);
        jProgressBar.setStringPainted(true);
        jProgressBar.setMaximumSize(new Dimension(400, 25));
        jProgressBar.setPreferredSize(new Dimension(400, 25));
        jProgressBar.setAlignmentX(Component.CENTER_ALIGNMENT);
        panel.add(jProgressBar);

        frameHelper.getFrame().setSize(480, 400);
        frameHelper.getFrame().setLocationRelativeTo(null);
        frameHelper.showFrame(panel, false);
        return new ProgressView(frameHelper, jProgressBar, panel);
    }

    public JProgressBar getProgressBar() {
        return progressBar;
    }

    public boolean showErrorAndWaitForRetry(String message) {
        if (progressBar == null || panel == null) return false;

        retryLatch = new CountDownLatch(1);
        retryRequested.set(false);

        SwingUtilities.invokeLater(() -> {
            errorLabel.setText(message);
            errorLabel.setVisible(true);
            retryButton.setVisible(true);
            panel.revalidate();
            panel.repaint();
        });

        try {
            retryLatch.await();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        return retryRequested.get();
    }

    /**
     * Resets the view to its initial state
     */
    public void resetForRetry() {
        if (progressBar == null || panel == null) return;
        SwingUtilities.invokeLater(() -> {
            progressBar.setValue(0);
            errorLabel.setVisible(false);
            retryButton.setVisible(false);
            panel.revalidate();
            panel.repaint();
        });
    }

    public void dispose() {
        if (frameHelper != null) {
            frameHelper.getFrame().dispose();
        }
    }
}
