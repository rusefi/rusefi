package com.rusefi.maintenance;

import com.rusefi.core.io.BoardCompatibility;
import com.rusefi.ui.wizard.DfuBoardSelectionPanel;

import javax.swing.*;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Locale;
import java.util.TreeSet;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.function.Consumer;

/** Selects from the exact board targets shipped in this bundle's board_compatibility property. */
final class DfuBoardPicker {
    private DfuBoardPicker() {
    }

    /** Keep the accepted DFU job busy while the full-size panel owns selection. */
    static String pick(String suggestedTarget, Consumer<JComponent> showPanel, Runnable closePanel) throws IOException {
        return pick(suggestedTarget, showPanel, closePanel,
            compatibleTargets(BoardCompatibility.getBoardCompatibility()));
    }

    static String pick(String suggestedTarget, Consumer<JComponent> showPanel, Runnable closePanel,
                       List<String> targets) throws IOException {
        if (SwingUtilities.isEventDispatchThread()) {
            throw new IllegalStateException("Board selection must wait on the DFU worker, not the EDT");
        }
        CompletableFuture<String> result = new CompletableFuture<>();
        SwingUtilities.invokeLater(() -> {
            if (result.isDone()) {
                return;
            }
            try {
                DfuBoardSelectionPanel panel = new DfuBoardSelectionPanel(selected -> {
                    try {
                        closePanel.run();
                        result.complete(selected);
                    } catch (RuntimeException e) {
                        result.completeExceptionally(e);
                    }
                });
                // Local bundle metadata is already loaded: populate before displaying the panel.
                panel.setBoards(targets, suggestedTarget);
                showPanel.accept(panel);
            } catch (RuntimeException e) {
                result.completeExceptionally(e);
            }
        });
        try {
            return result.get();
        } catch (InterruptedException e) {
            result.cancel(false);
            SwingUtilities.invokeLater(closePanel);
            Thread.currentThread().interrupt();
            throw new IOException("Board selection interrupted", e);
        } catch (ExecutionException e) {
            throw new IOException("Could not display board selection", e.getCause());
        }
    }

    static List<String> compatibleTargets(String compatibility) throws IOException {
        if (compatibility == null || compatibility.trim().isEmpty()
            || BoardCompatibility.WILDCARD.equals(compatibility.trim())) {
            throw new IOException("board_compatibility does not contain a concrete board list");
        }
        TreeSet<String> targets = new TreeSet<>(Comparator
            .comparingInt(DfuBoardPicker::boardGroup)
            .thenComparing(String.CASE_INSENSITIVE_ORDER)
            .thenComparing(Comparator.naturalOrder()));
        for (String token : compatibility.split(",")) {
            String target = token.trim();
            if (target.isEmpty()) {
                continue;
            }
            if (!target.matches("[A-Za-z0-9][A-Za-z0-9_-]*")) {
                throw new IOException("Invalid board target in board_compatibility: " + target);
            }
            if (!ManualDfuRecovery.isUniversalBundle(target)) {
                targets.add(target);
            }
        }
        if (targets.isEmpty()) {
            throw new IOException("board_compatibility contains no concrete board targets");
        }
        return new ArrayList<>(targets);
    }

    private static int boardGroup(String target) {
        String name = target.toLowerCase(Locale.ROOT).replace('-', '_');
        if (name.startsWith("uaefi") || name.startsWith("super_uaefi")) {
            return 0;
        }
        return name.startsWith("hellen") ? 1 : 2;
    }
}
