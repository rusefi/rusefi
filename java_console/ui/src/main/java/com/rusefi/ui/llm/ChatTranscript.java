package com.rusefi.ui.llm;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * The conversation as chat bubbles, all on the left: the user's messages tinted, the assistant's replies with
 * a chip for each tool it used (ECU readings, wiki/source look-ups, a saved diagnostic case), and notices in
 * between. Only the display: the saved transcript stays plain text in the format {@link ChatGptAgent} and
 * {@link LLMTab} write, and {@link #load} splits such a text into bubbles again.
 */
final class ChatTranscript extends JPanel implements Scrollable {
    private static final Pattern TOOL = Pattern.compile("\\[Tool: (.+)]");
    /** A file or picture sent with the user's message, one per line after it in the saved transcript. */
    static final Pattern ATTACHED = Pattern.compile("\\[Attached: (.+)]");
    private static final Pattern CASE_SAVED = Pattern.compile("\\[Diagnostic case saved: (.+)]");
    private static final Pattern NOTICE = Pattern.compile("\\[(.+)]");
    /** The page around the chat already pads it; this only keeps the bubble borders off the edge. */
    private static final int PAD_X = 1;
    private static final int PAD_Y = 4;
    private static final int GAP = 12;
    private final String placeholder;
    /** The assistant's reply being written, or null when the next text starts a new one. */
    private Bubble reply;
    private int length;

    ChatTranscript(String placeholder) {
        super(null);
        this.placeholder = placeholder;
        setLayout(new Column());
        setOpaque(true);
        getAccessibleContext().setAccessibleName("ChatGPT conversation");
        // a wrapped bubble's height depends on the width, so a new width needs a new layout
        addComponentListener(new ComponentAdapter() {
            @Override public void componentResized(ComponentEvent e) {
                revalidate();
            }
        });
    }

    void clear() {
        removeAll();
        reply = null;
        length = 0;
        changed(false);
    }

    void addUser(String text) {
        addUser(text, java.util.Collections.emptyList());
    }

    /** A message of the user's, with a chip per attached file or picture. */
    void addUser(String text, List<String> attachments) {
        endReply();
        Bubble bubble = new Bubble(Kind.USER);
        for (String name : attachments) {
            bubble.addChip(ToolChip.forAttachment(name));
        }
        bubble.setText(text.trim());
        addBubble(bubble, length(text));
    }

    /** Streams reply text into the current reply, starting one if needed. */
    void appendAssistant(String delta) {
        Bubble bubble = reply();
        bubble.append(delta);
        length(delta);
        changed(true);
    }

    /** A progress line from the agent: a tool chip, a saved case or a notice. */
    void addProgress(String progress) {
        for (String line : progress.split("\n")) {
            line = line.trim();
            if (line.isEmpty()) {
                continue;
            }
            Matcher tool = TOOL.matcher(line);
            Matcher saved = CASE_SAVED.matcher(line);
            if (tool.matches()) {
                reply().addChip(ToolChip.forTool(tool.group(1)));
            } else if (saved.matches()) {
                reply().addChip(ToolChip.forSavedCase(saved.group(1)));
            } else {
                addNotice(line);
            }
            length(line);
        }
        changed(true);
    }

    /** A line from the tab itself, e.g. that a turn was stopped; surrounding brackets are dropped. */
    void addNotice(String text) {
        String line = text.trim();
        if (line.isEmpty()) {
            return;
        }
        Matcher bracketed = NOTICE.matcher(line);
        if (bracketed.matches()) {
            line = bracketed.group(1);
        }
        endReply();
        Bubble notice = new Bubble(Kind.NOTICE);
        notice.setText(line);
        addBubble(notice, length(line));
    }

    /** Shows a saved transcript: "> " starts a message of the user's, which runs to the next blank line. */
    void load(String transcript) {
        removeAll();
        reply = null;
        length = 0;
        StringBuilder user = null;
        List<String> attached = new ArrayList<>();
        for (String line : transcript.split("\n", -1)) {
            if (line.startsWith("> ")) {
                if (user != null) {
                    addUser(user.toString(), attached);
                    attached = new ArrayList<>();
                }
                user = new StringBuilder(line.substring(2));
            } else if (user != null) {
                Matcher file = ATTACHED.matcher(line.trim());
                if (line.isEmpty()) {
                    addUser(user.toString(), attached);
                    attached = new ArrayList<>();
                    user = null;
                } else if (file.matches()) {
                    attached.add(file.group(1));
                } else {
                    user.append('\n').append(line);
                }
            } else if (TOOL.matcher(line.trim()).matches() || CASE_SAVED.matcher(line.trim()).matches()) {
                addProgress(line);
            } else if (NOTICE.matcher(line.trim()).matches()) {
                addNotice(line);
            } else if (reply != null || !line.trim().isEmpty()) {
                // blank lines between blocks do not start a reply
                reply().append(line + "\n");
            }
        }
        if (user != null) {
            addUser(user.toString(), attached);
        }
        endReply();
        changed(false);
        SwingUtilities.invokeLater(this::scrollToEnd);
    }

    /** Each bubble as "kind[chips]: text", for tests. */
    List<String> describeForTest() {
        List<String> blocks = new ArrayList<>();
        for (Component component : getComponents()) {
            Bubble bubble = (Bubble) component;
            List<String> chips = new ArrayList<>();
            for (ToolChip chip : bubble.chips) {
                chips.add(chip.getText());
            }
            blocks.add(bubble.kind.name().toLowerCase() + (chips.isEmpty() ? "" : chips.toString()) + ": "
                    + bubble.text.getText().trim());
        }
        return blocks;
    }

    /** The conversation as plain text, e.g. for tests. */
    String getText() {
        StringBuilder text = new StringBuilder();
        for (Component component : getComponents()) {
            text.append(((Bubble) component).text.getText()).append('\n');
        }
        return text.toString();
    }

    @Override public void setFont(Font font) {
        super.setFont(font);
        for (Component component : getComponents()) {
            ((Bubble) component).applyFont(font);
        }
        revalidate();
    }

    /** Ends the current reply; its trailing blank lines would show as empty space in the bubble. */
    private void endReply() {
        if (reply != null) {
            reply.trimEnd();
            reply = null;
        }
    }

    private Bubble reply() {
        if (reply == null) {
            reply = new Bubble(Kind.ASSISTANT);
            addBubble(reply, 0);
        }
        return reply;
    }

    private int length(String text) {
        return text == null ? 0 : text.length();
    }

    private void addBubble(Bubble bubble, int added) {
        bubble.applyFont(getFont());
        super.add(bubble);
        length += added;
        changed(true);
    }

    /** Lays out again, drops the oldest bubbles beyond the transcript limit and follows the end if it was shown. */
    private void changed(boolean follow) {
        while (length > TroubleshootingSessionStore.MAX_TRANSCRIPT && getComponentCount() > 1) {
            Bubble oldest = (Bubble) getComponent(0);
            length -= oldest.text.getText().length();
            remove(0);
        }
        boolean atEnd = follow && isAtEnd();
        revalidate();
        repaint();
        if (atEnd) {
            SwingUtilities.invokeLater(this::scrollToEnd);
        }
    }

    private boolean isAtEnd() {
        Container parent = getParent();
        if (!(parent instanceof JViewport)) {
            return true;
        }
        Rectangle view = ((JViewport) parent).getViewRect();
        return view.y + view.height >= getHeight() - 48;
    }

    private void scrollToEnd() {
        scrollRectToVisible(new Rectangle(0, Math.max(0, getPreferredSize().height - 1), 1, 1));
    }

    @Override protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        if (getComponentCount() > 0) {
            return;
        }
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
            g2.setFont(getFont());
            g2.setColor(themed("TextField.placeholderForeground", Color.GRAY));
            g2.drawString(placeholder, PAD_X + 2, PAD_Y + 12 + g2.getFontMetrics().getAscent());
        } finally {
            g2.dispose();
        }
    }

    @Override public Dimension getPreferredScrollableViewportSize() {
        return getPreferredSize();
    }

    @Override public int getScrollableUnitIncrement(Rectangle visible, int orientation, int direction) {
        return 24;
    }

    @Override public int getScrollableBlockIncrement(Rectangle visible, int orientation, int direction) {
        return Math.max(24, visible.height - 48);
    }

    @Override public boolean getScrollableTracksViewportWidth() {
        return true;
    }

    @Override public boolean getScrollableTracksViewportHeight() {
        Container parent = getParent();
        return parent instanceof JViewport && getPreferredSize().height < parent.getHeight();
    }

    private enum Kind {USER, ASSISTANT, NOTICE}

    /** Stacks the bubbles on the left: the user's as wide as their text, at most 3/4; replies and notices across. */
    private static final class Column implements LayoutManager {
        @Override public void addLayoutComponent(String name, Component comp) {
        }

        @Override public void removeLayoutComponent(Component comp) {
        }

        @Override public Dimension preferredLayoutSize(Container parent) {
            int width = Math.max(parent.getWidth(), 300);
            int y = PAD_Y;
            for (Component component : parent.getComponents()) {
                Bubble bubble = (Bubble) component;
                y += bubble.heightFor(widthOf(bubble, width)) + GAP;
            }
            return new Dimension(width, y - GAP + PAD_Y);
        }

        @Override public Dimension minimumLayoutSize(Container parent) {
            return new Dimension(200, 0);
        }

        @Override public void layoutContainer(Container parent) {
            int width = parent.getWidth();
            int y = PAD_Y;
            for (Component component : parent.getComponents()) {
                Bubble bubble = (Bubble) component;
                int w = widthOf(bubble, width);
                int h = bubble.heightFor(w);
                // every bubble starts at the left; the user's keep their colour and their own width
                int x = PAD_X;
                bubble.setBounds(x, y, w, h);
                y += h + GAP;
            }
        }

        private static int widthOf(Bubble bubble, int width) {
            int available = Math.max(120, width - 2 * PAD_X);
            if (bubble.kind != Kind.USER) {
                return available;
            }
            return Math.min(bubble.naturalWidth(), available * 3 / 4);
        }
    }

    /** One message: rounded, with the tool chips above the text. */
    private static final class Bubble extends JComponent {
        private static final Insets PADDING = new Insets(10, 14, 10, 14);
        private static final int CHIP_GAP = 6;
        final Kind kind;
        final JTextArea text = new JTextArea();
        private final List<ToolChip> chips = new ArrayList<>();

        Bubble(Kind kind) {
            this.kind = kind;
            text.setEditable(false);
            text.setLineWrap(true);
            text.setWrapStyleWord(true);
            text.setOpaque(false);
            text.setBorder(BorderFactory.createEmptyBorder());
            text.setForeground(kind == Kind.NOTICE ? themed("Label.disabledForeground", Color.GRAY)
                    : themed("Label.foreground", Color.BLACK));
            add(text);
        }

        void setText(String value) {
            text.setText(value);
        }

        void append(String delta) {
            // a reply starts at its first visible character
            String added = text.getDocument().getLength() == 0 ? stripLeading(delta) : delta;
            if (!added.isEmpty()) {
                text.append(added);
            }
        }

        void trimEnd() {
            String value = text.getText();
            int end = value.length();
            while (end > 0 && Character.isWhitespace(value.charAt(end - 1))) {
                end--;
            }
            if (end < value.length()) {
                text.setText(value.substring(0, end));
            }
        }

        void addChip(ToolChip chip) {
            chips.add(chip);
            add(chip);
        }

        void applyFont(Font font) {
            if (font == null) {
                return;
            }
            text.setFont(kind == Kind.NOTICE ? font.deriveFont(font.getSize2D() - 1) : font);
            for (ToolChip chip : chips) {
                chip.setFont(font.deriveFont(font.getSize2D() - 1));
            }
        }

        private boolean hasText() {
            return !text.getText().trim().isEmpty();
        }

        int naturalWidth() {
            FontMetrics metrics = text.getFontMetrics(text.getFont());
            int widest = 0;
            for (String line : text.getText().split("\n")) {
                widest = Math.max(widest, metrics.stringWidth(line));
            }
            // a few pixels spare: fractional scaling paints text wider than it measures
            return widest + PADDING.left + PADDING.right + 8;
        }

        int heightFor(int width) {
            return place(width, false);
        }

        @Override public void doLayout() {
            place(getWidth(), true);
        }

        /** Positions the chips (wrapping) and the text for this width; returns the height they need. */
        private int place(int width, boolean apply) {
            int inner = Math.max(40, width - PADDING.left - PADDING.right);
            int x = 0;
            int y = 0;
            int rowHeight = 0;
            for (ToolChip chip : chips) {
                Dimension size = chip.getPreferredSize();
                if (x > 0 && x + size.width > inner) {
                    x = 0;
                    y += rowHeight + CHIP_GAP;
                    rowHeight = 0;
                }
                if (apply) {
                    chip.setBounds(PADDING.left + x, PADDING.top + y, Math.min(size.width, inner), size.height);
                }
                x += size.width + CHIP_GAP;
                rowHeight = Math.max(rowHeight, size.height);
            }
            int chipsHeight = chips.isEmpty() ? 0 : y + rowHeight;
            boolean showText = hasText();
            text.setVisible(showText);
            int textHeight = 0;
            if (showText) {
                text.setSize(inner, Short.MAX_VALUE);
                textHeight = text.getPreferredSize().height;
                int textY = PADDING.top + chipsHeight + (chips.isEmpty() ? 0 : 8);
                if (apply) {
                    text.setBounds(PADDING.left, textY, inner, textHeight);
                }
                textHeight += chips.isEmpty() ? 0 : 8;
            }
            if (kind == Kind.NOTICE) {
                return chipsHeight + textHeight + 4;
            }
            return PADDING.top + chipsHeight + textHeight + PADDING.bottom;
        }

        @Override protected void paintComponent(Graphics g) {
            if (kind == Kind.NOTICE) {
                return;
            }
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                boolean user = kind == Kind.USER;
                g2.setColor(user ? themed("Console.Status.accent.background", Color.LIGHT_GRAY)
                        : new Color(themed("Console.Card.background", Color.WHITE).getRGB()));
                g2.fillRoundRect(0, 0, getWidth() - 1, getHeight() - 1, 12, 12);
                g2.setColor(themed("Console.Card.borderColor", Color.LIGHT_GRAY));
                g2.drawRoundRect(0, 0, getWidth() - 1, getHeight() - 1, 12, 12);
            } finally {
                g2.dispose();
            }
        }

        private static String stripLeading(String value) {
            int start = 0;
            while (start < value.length() && Character.isWhitespace(value.charAt(start))) {
                start++;
            }
            return value.substring(start);
        }
    }

    /** A small rounded label naming what the assistant looked at. */
    static final class ToolChip extends JLabel {
        private final String tone;

        private ToolChip(String text, TroubleshootingIcons.ToolKind kind, String tone, String tooltip) {
            super(text, new TroubleshootingIcons.ToolIcon(kind), LEADING);
            this.tone = tone;
            setIconTextGap(6);
            setBorder(BorderFactory.createEmptyBorder(3, 8, 3, 10));
            setForeground(themed(tone + ".foreground", Color.DARK_GRAY));
            setToolTipText(tooltip);
        }

        static ToolChip forTool(String name) {
            TroubleshootingIcons.ToolKind kind = "export_diagnostic_case".equals(name) ? TroubleshootingIcons.ToolKind.CASE
                    : LocalKnowledgeTools.handles(name) ? TroubleshootingIcons.ToolKind.KNOWLEDGE
                    : TroubleshootingIcons.ToolKind.ECU;
            return new ToolChip(describe(name), kind, "Console.Status.neutral", "Tool: " + name);
        }

        static ToolChip forAttachment(String name) {
            return new ToolChip(name, Attachment.isImageName(name) || name.startsWith("Pasted picture")
                    ? TroubleshootingIcons.ToolKind.IMAGE : TroubleshootingIcons.ToolKind.FILE,
                    "Console.Status.neutral", "Sent with this message");
        }

        static ToolChip forSavedCase(String path) {
            ToolChip chip = new ToolChip("Diagnostic case saved", TroubleshootingIcons.ToolKind.CASE,
                    "Console.Status.connected", "<html>" + escape(path) + "<br>Click to show the file</html>");
            chip.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
            chip.addMouseListener(new MouseAdapter() {
                @Override public void mouseClicked(MouseEvent e) {
                    File folder = new File(path).getParentFile();
                    try {
                        if (folder != null && Desktop.isDesktopSupported()) { Desktop.getDesktop().open(folder); }
                    } catch (Exception ignored) { }
                }
            });
            return chip;
        }

        /** What a tool does, in words; unknown tools by their name. */
        static String describe(String name) {
            switch (name) {
                case "list_output_channels": return "ECU: list live values";
                case "read_live_values": return "ECU: read live values";
                case "diagnostic_snapshot": return "ECU: diagnostic snapshot";
                case "read_tune_fields": return "ECU: read tune settings";
                case "get_lua": return "ECU: read Lua script";
                case "capture_live_log": return "ECU: record live log";
                case "capture_engine_sniffer": return "ECU: engine sniffer capture";
                case "ecu_info": return "ECU: identify";
                case "search_knowledge": return "Search wiki and source";
                case "read_knowledge": return "Read wiki/source page";
                case "export_diagnostic_case": return "Export diagnostic case";
                default: return name.replace('_', ' ');
            }
        }

        private static String escape(String text) {
            return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
        }

        @Override protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(themed(tone + ".background", Color.LIGHT_GRAY));
                g2.fillRoundRect(0, 0, getWidth(), getHeight(), getHeight(), getHeight());
            } finally {
                g2.dispose();
            }
            super.paintComponent(g);
        }
    }

    private static Color themed(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }
}
