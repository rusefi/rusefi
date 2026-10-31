package com.rusefi.ui.llm;

import javax.swing.*;
import java.awt.*;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.event.InputEvent;
import java.io.File;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.function.Consumer;

/**
 * The files and pictures waiting to go with the next message, shown as chips above the message box with a
 * button to remove each, plus a line for why something could not be attached. Hidden while empty.
 */
final class AttachmentBar extends JPanel {
    private final List<Attachment> attachments = new ArrayList<>();
    private final JPanel chips = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
    private final JLabel note = new JLabel();
    private Runnable onChange = () -> { };

    AttachmentBar() {
        super(new BorderLayout(0, 4));
        setOpaque(false);
        chips.setOpaque(false);
        note.setForeground(themed("Console.Status.pending.foreground", Color.DARK_GRAY));
        add(chips, BorderLayout.CENTER);
        add(note, BorderLayout.SOUTH);
        refresh();
    }

    void setOnChange(Runnable onChange) {
        this.onChange = onChange;
    }

    List<Attachment> pending() {
        return Collections.unmodifiableList(attachments);
    }

    boolean isEmpty() {
        return attachments.isEmpty();
    }

    /** Adds an attachment, or says why not: too many, or too large together. */
    void attach(Attachment attachment) {
        if (attachments.size() >= Attachment.MAX_COUNT) {
            setNote("At most " + Attachment.MAX_COUNT + " attachments per message.");
            return;
        }
        int total = attachment.size();
        for (Attachment existing : attachments) {
            total += existing.size();
        }
        if (total > Attachment.MAX_TOTAL_CHARS) {
            setNote(attachment.name + " does not fit: this message's attachments would be too large together.");
            return;
        }
        // the note may explain why another file of the same batch was refused, so it stays
        attachments.add(attachment);
        refresh();
    }

    void clear() {
        attachments.clear();
        setNote(null);
        refresh();
    }

    void setNote(String text) {
        note.setText(text == null ? "" : text);
        refresh();
    }

    String noteForTest() {
        return note.getText();
    }

    private void detach(Attachment attachment) {
        attachments.remove(attachment);
        setNote(null);
        refresh();
    }

    private void refresh() {
        chips.removeAll();
        for (Attachment attachment : attachments) {
            if (chips.getComponentCount() > 0) {
                chips.add(Box.createHorizontalStrut(6));
            }
            chips.add(chip(attachment));
        }
        chips.setVisible(!attachments.isEmpty());
        note.setVisible(!note.getText().isEmpty());
        setVisible(!attachments.isEmpty() || !note.getText().isEmpty());
        revalidate();
        repaint();
        onChange.run();
    }

    private JComponent chip(Attachment attachment) {
        JLabel label = new JLabel(attachment.name);
        label.setIcon(attachment.thumbnail != null ? new ImageIcon(attachment.thumbnail.getScaledInstance(-1, 20, Image.SCALE_SMOOTH))
                : new TroubleshootingIcons.ToolIcon(TroubleshootingIcons.ToolKind.FILE));
        label.setIconTextGap(6);
        label.setToolTipText(attachment.kind == Attachment.Kind.IMAGE ? "Picture, sent with the next message"
                : "Text file, " + attachment.size() / 1000 + "k characters, sent with the next message");
        JButton remove = new JButton("×");
        remove.setToolTipText("Remove " + attachment.name);
        remove.getAccessibleContext().setAccessibleName("Remove " + attachment.name);
        remove.putClientProperty("JButton.buttonType", "toolBarButton");
        remove.setMargin(new Insets(0, 4, 0, 4));
        remove.setFocusable(false);
        remove.addActionListener(e -> detach(attachment));
        JPanel chip = new JPanel(new BorderLayout(4, 0)) {
            @Override protected void paintComponent(Graphics g) {
                Graphics2D g2 = (Graphics2D) g.create();
                try {
                    g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                    g2.setColor(themed("Console.Status.neutral.background", Color.LIGHT_GRAY));
                    g2.fillRoundRect(0, 0, getWidth(), getHeight(), 10, 10);
                } finally {
                    g2.dispose();
                }
            }
        };
        chip.setOpaque(false);
        chip.setBorder(BorderFactory.createEmptyBorder(3, 8, 3, 2));
        chip.add(label, BorderLayout.CENTER);
        chip.add(remove, BorderLayout.EAST);
        return chip;
    }

    /**
     * Wraps a text component's transfer handler so that pasting or dropping files or a picture attaches them,
     * while text is still copied, cut, pasted and dragged as before.
     *
     * @param original the component's own handler, for text; may be null for a component without one
     */
    static TransferHandler handler(TransferHandler original, Consumer<List<Path>> files, Consumer<Image> picture) {
        return new TransferHandler() {
            @Override public boolean canImport(TransferSupport support) {
                if (support.isDataFlavorSupported(DataFlavor.javaFileListFlavor)
                        || support.isDataFlavorSupported(DataFlavor.imageFlavor)) {
                    return true;
                }
                return original != null && original.canImport(support);
            }

            @Override public boolean importData(TransferSupport support) {
                try {
                    Transferable data = support.getTransferable();
                    if (support.isDataFlavorSupported(DataFlavor.javaFileListFlavor)) {
                        List<Path> paths = new ArrayList<>();
                        for (Object file : (List<?>) data.getTransferData(DataFlavor.javaFileListFlavor)) {
                            paths.add(((File) file).toPath());
                        }
                        files.accept(paths);
                        return true;
                    }
                    if (support.isDataFlavorSupported(DataFlavor.imageFlavor)) {
                        picture.accept((Image) data.getTransferData(DataFlavor.imageFlavor));
                        return true;
                    }
                } catch (Exception e) {
                    return false;
                }
                return original != null && original.importData(support);
            }

            @Override public int getSourceActions(JComponent c) {
                return original == null ? NONE : original.getSourceActions(c);
            }

            @Override public void exportAsDrag(JComponent comp, InputEvent e, int action) {
                if (original != null) {
                    original.exportAsDrag(comp, e, action);
                }
            }

            @Override public void exportToClipboard(JComponent comp, Clipboard clip, int action) {
                if (original != null) {
                    original.exportToClipboard(comp, clip, action);
                }
            }
        };
    }

    private static Color themed(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }
}
