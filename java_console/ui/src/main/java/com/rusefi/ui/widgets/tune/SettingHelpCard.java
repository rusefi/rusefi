package com.rusefi.ui.widgets.tune;

import javax.swing.*;
import java.awt.*;
import java.awt.event.AWTEventListener;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.util.ArrayDeque;
import java.util.regex.Pattern;

/**
 * Help card beside a setting row: the setting's name, units and the first sentence of its INI help,
 * with "More" expanding the full text in the card. It opens after the mouse rests on any part of a
 * row that has help, when Tab moves focus into the row, or at once (expanded) from the row's help
 * icon. Help appears where the user is looking: tooltips that only repeat what the row shows, and a
 * help line in the status strip, which users rarely notice, were both dropped in its favour.
 * <p>
 * The mouse must be able to reach the card, often diagonally across the rows below, while moving up
 * and down the rows should follow at once. So a card switches rows immediately unless the mouse is
 * heading for it ({@link #headingTo}, the "menu-aim" triangle); then it switches only if the mouse
 * stops on another row. Hiding waits until the mouse rests off the rows and the card. "More" and the
 * help icon pin the card: it then ignores the mouse and closes on its close button, Esc or a click
 * elsewhere. One card at a time. Users who find the cards distracting turn off "Help cards" on the
 * Tuning toolbar ({@link #setAutoShow}); the help icon still opens them.
 * <p>
 * A dialog's, table's or curve's "Help" button (INI topic help) opens the same card, pinned, below
 * the button, so all help in Tuning looks alike.
 */
public final class SettingHelpCard {
    static final String KEY = "rusefi.settingHelp";
    /** The topic help a dialog's Help button opens; not a hover target like {@link #KEY}. */
    static final String TOPIC_KEY = "rusefi.topicHelp";
    static final int SHOW_DELAY_MS = 500;
    private static final int SWITCH_DELAY_MS = 300;
    private static final int TRAIL_LENGTH = 4;
    private static final int AIM_SLACK = 30;
    private static final int HIDE_DELAY_MS = 400;
    private static final int GAP = 8;
    private static final int TEXT_WIDTH = 320;
    private static final int MAX_TEXT_HEIGHT = 320;
    private static final Pattern SENTENCE_END = Pattern.compile("(?<=[.!?])\\s+(?=[A-Z])");

    /** What a card shows; stored on a setting row as its {@link #KEY} client property. */
    static final class Help {
        final String name;
        final String units;
        final String text;
        /** Web help ("Read more"), or null. */
        final String link;

        Help(String name, String units, String text) {
            this(name, units, text, null);
        }

        Help(String name, String units, String text, String link) {
            this.name = name;
            this.units = units;
            this.text = text;
            this.link = link;
        }

        String summary() {
            return firstSentence(text);
        }

        boolean hasMore() {
            return !summary().equals(text.replace("\\n", "\n").trim());
        }
    }

    private static Tracker tracker;
    private static boolean autoShow = true;
    private static final Icon HELP_ICON = loadHelpIcon();

    private SettingHelpCard() {
    }

    /**
     * Whether cards open by themselves on hover and Tab (the Tuning toolbar's "Help cards" choice).
     * When off, only the row's help icon opens one.
     */
    public static void setAutoShow(boolean on) {
        autoShow = on;
        if (!on && tracker != null) {
            tracker.hideUnpinned();
        }
    }

    public static boolean isAutoShow() {
        return autoShow;
    }

    /** Gives a setting row a help card; rows without help text get none. */
    static void attach(JComponent row, String name, String units, String helpText) {
        if (helpText == null || helpText.trim().isEmpty()) {
            return;
        }
        String plainName = name == null ? "" : name.replaceAll("<[^>]*>", "").trim();
        row.putClientProperty(KEY, new Help(plainName, units == null ? "" : units.trim(), helpText.trim()));
        tracker();
    }

    /** The card tracker, started with the first help; none without a screen. */
    private static Tracker tracker() {
        if (tracker == null && !GraphicsEnvironment.isHeadless()) {
            tracker = new Tracker();
        }
        return tracker;
    }

    /** Opens a topic help card below its Help button, pinned. */
    static void showTopic(JComponent button, Help help) {
        Tracker t = tracker();
        if (t != null) {
            t.show(button, help, true, true, true);
        }
    }

    /** Closes the open card, e.g. when the dialog changes. */
    static void hideCard() {
        if (tracker != null) {
            tracker.hide();
        }
    }

    /** Opens the card of the row holding {@code component}, already expanded (the row's help icon). */
    static void showExpanded(Component component) {
        JComponent row = findRow(component);
        if (row != null && tracker() != null) {
            tracker.show(row, (Help) row.getClientProperty(KEY), true, true, false);
        }
    }

    /** The nearest ancestor row with help, or null. */
    static JComponent findRow(Component component) {
        for (Component c = component; c != null && !(c instanceof Window); c = c.getParent()) {
            if (c instanceof JComponent && ((JComponent) c).getClientProperty(KEY) instanceof Help) {
                return (JComponent) c;
            }
        }
        return null;
    }

    /** The first line or sentence of INI help text, whose line breaks are written as "\n". */
    static String firstSentence(String text) {
        String first = text.replace("\\n", "\n").trim().split("\n", 2)[0];
        return SENTENCE_END.split(first, 2)[0].trim();
    }

    /**
     * Where the card goes: right of the row's controls when it fits in {@code bounds}, else below the
     * row, right-aligned with them; always kept inside {@code bounds}.
     */
    static Point place(Rectangle controls, Dimension card, Rectangle bounds) {
        return place(controls, card, bounds, true);
    }

    /** @param besideFirst false: start at {@code controls}' top left (the topic card under its button) */
    static Point place(Rectangle controls, Dimension card, Rectangle bounds, boolean besideFirst) {
        int x;
        int y;
        if (!besideFirst) {
            x = controls.x;
            y = controls.y;
        } else if (controls.x + controls.width + GAP + card.width <= bounds.x + bounds.width) {
            x = controls.x + controls.width + GAP;
            y = controls.y;
        } else {
            x = controls.x + controls.width - card.width;
            y = controls.y + controls.height + GAP / 2;
        }
        x = Math.max(bounds.x, Math.min(x, bounds.x + bounds.width - card.width));
        y = Math.max(bounds.y, Math.min(y, bounds.y + bounds.height - card.height));
        return new Point(x, y);
    }

    /**
     * Whether a mouse moving from {@code from} to {@code to} is heading for the card: {@code to} lies in
     * the triangle between {@code from} and the card's near edge (its left edge when the card is to the
     * right, else its top edge). Moving up or down a dialog's rows is not, so the card follows at once.
     */
    static boolean headingTo(Point from, Point to, Rectangle card) {
        if (from == null || from.equals(to)) {
            return false;
        }
        Point a;
        Point b;
        // the edge is stretched by AIM_SLACK at both ends: hand movements curve a little
        if (card.x >= from.x) {
            a = new Point(card.x, card.y - AIM_SLACK);
            b = new Point(card.x, card.y + card.height + AIM_SLACK);
        } else if (card.y >= from.y) {
            a = new Point(card.x - AIM_SLACK, card.y);
            b = new Point(card.x + card.width + AIM_SLACK, card.y);
        } else {
            return false;
        }
        long d1 = cross(from, a, to);
        long d2 = cross(a, b, to);
        long d3 = cross(b, from, to);
        boolean negative = d1 < 0 || d2 < 0 || d3 < 0;
        boolean positive = d1 > 0 || d2 > 0 || d3 > 0;
        return !(negative && positive);
    }

    private static long cross(Point p, Point q, Point r) {
        return (long) (q.x - p.x) * (r.y - p.y) - (long) (q.y - p.y) * (r.x - p.x);
    }

    static JComponent createCard(Help help, boolean expanded, Runnable onToggle) {
        return createCard(help, expanded, onToggle, null);
    }

    /**
     * The card: a neutral surface with a thin accent bar on the left, the help icon and name, the
     * text, then "More" and "Read more" links. Colours from the theme files [tag:console_theme].
     *
     * @param onClose shows a close button when not null (a pinned card)
     */
    static JComponent createCard(Help help, boolean expanded, Runnable onToggle, Runnable onClose) {
        JPanel card = new JPanel(new BorderLayout(0, 6));
        card.setName("settingHelpCard");
        card.setBackground(CalibrationFieldFactory.helpBackground());
        card.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(TuningStyle.color("Console.Tuning.help.outline", Color.LIGHT_GRAY)),
            BorderFactory.createCompoundBorder(
                BorderFactory.createMatteBorder(0, 3, 0, 0,
                    TuningStyle.color("Console.Tuning.help.borderColor", Color.BLUE)),
                BorderFactory.createEmptyBorder(10, 12, 10, 10))));

        JLabel title = new JLabel(help.name, HELP_ICON, SwingConstants.LEADING);
        title.setIconTextGap(6);
        title.setFont(title.getFont().deriveFont(Font.BOLD, title.getFont().getSize2D() + 1));
        JPanel north = new JPanel(new BorderLayout(8, 0));
        north.setOpaque(false);
        north.add(title, BorderLayout.CENTER);
        JPanel titleEnd = new JPanel(new FlowLayout(FlowLayout.RIGHT, 6, 0));
        titleEnd.setOpaque(false);
        if (!help.units.isEmpty()) {
            JLabel units = new JLabel(help.units);
            units.setName("settingHelpUnits");
            units.setForeground(TuningStyle.color("Console.Tuning.mutedForeground", Color.GRAY));
            titleEnd.add(units);
        }
        if (onClose != null) {
            JButton close = linkButton("\u00D7", "settingHelpClose", onClose);
            close.setForeground(TuningStyle.color("Console.Tuning.mutedForeground", Color.GRAY));
            close.setFont(close.getFont().deriveFont(close.getFont().getSize2D() + 4));
            close.getAccessibleContext().setAccessibleName("Close help");
            titleEnd.add(close);
        }
        north.add(titleEnd, BorderLayout.EAST);
        card.add(north, BorderLayout.NORTH);

        String shown = expanded ? help.text : help.summary();
        if (!shown.isEmpty()) {
            JEditorPane text = new JEditorPane("text/html", CalibrationFieldFactory.formatHelpHtml(shown));
            text.setName("settingHelpText");
            text.setEditable(false);
            text.setFocusable(false);
            text.setOpaque(false);
            text.putClientProperty(JEditorPane.HONOR_DISPLAY_PROPERTIES, Boolean.TRUE);
            text.setFont(title.getFont().deriveFont(Font.PLAIN, title.getFont().getSize2D() - 1));
            text.setForeground(UIManager.getColor("Label.foreground"));
            text.setBorder(BorderFactory.createEmptyBorder());
            text.addHyperlinkListener(CalibrationFieldFactory::openHelpLink);
            // measure the wrapped height at the card's text width
            text.setSize(new Dimension(TEXT_WIDTH, Short.MAX_VALUE));
            Dimension size = text.getPreferredSize();
            text.setPreferredSize(new Dimension(TEXT_WIDTH, size.height));
            if (size.height > MAX_TEXT_HEIGHT) {
                JScrollPane scroll = new JScrollPane(text);
                scroll.setBorder(BorderFactory.createEmptyBorder());
                scroll.setOpaque(false);
                scroll.getViewport().setOpaque(false);
                scroll.setPreferredSize(new Dimension(
                    TEXT_WIDTH + scroll.getVerticalScrollBar().getPreferredSize().width, MAX_TEXT_HEIGHT));
                card.add(scroll, BorderLayout.CENTER);
            } else {
                card.add(text, BorderLayout.CENTER);
            }
        } else {
            // a link-only card keeps its width
            card.add(Box.createHorizontalStrut(TEXT_WIDTH), BorderLayout.CENTER);
        }

        JPanel links = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
        links.setOpaque(false);
        if (help.hasMore()) {
            links.add(linkButton(expanded ? "Show less \u25B4" : "More \u25B8", "settingHelpMore", onToggle));
        }
        if (help.link != null && !help.link.trim().isEmpty()) {
            if (links.getComponentCount() > 0) {
                links.add(Box.createHorizontalStrut(16));
            }
            links.add(linkButton("Read more online \u2197", "settingHelpLink", () -> CalibrationFieldFactory.openUrl(help.link)));
        }
        if (links.getComponentCount() > 0) {
            card.add(links, BorderLayout.SOUTH);
        }
        return card;
    }

    private static JButton linkButton(String text, String name, Runnable action) {
        JButton button = new JButton(text);
        button.setName(name);
        button.setFocusable(false);
        button.setBorderPainted(false);
        button.setContentAreaFilled(false);
        button.setBorder(BorderFactory.createEmptyBorder());
        button.setForeground(TuningStyle.color("Console.Link.foreground", Color.BLUE));
        button.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
        button.addActionListener(e -> action.run());
        return button;
    }

    private static Icon loadHelpIcon() {
        ImageIcon source = com.rusefi.ui.widgets.TuningIcons.load("help");
        return source == null ? null
            : new ImageIcon(source.getImage().getScaledInstance(16, 16, Image.SCALE_SMOOTH));
    }

    /** Follows the mouse, keyboard focus and Esc for all setting rows; one card at a time. */
    private static final class Tracker implements AWTEventListener {
        private final Timer showTimer = new Timer(SHOW_DELAY_MS, e -> showPending());
        private final Timer switchTimer = new Timer(SWITCH_DELAY_MS, e -> showPending());
        private final Timer hideTimer = new Timer(HIDE_DELAY_MS, e -> hide());
        // closes the card once its row scrolls, moves or disappears
        private final Timer watch = new Timer(200, e -> checkRow());
        private JComponent pendingRow;
        private JComponent row;
        private Point rowLocation;
        private Popup popup;
        private JComponent card;
        private boolean expanded;
        private boolean pinned;
        private boolean lastInputWasKey;
        // recent mouse positions on screen; the oldest is where a movement toward the card starts
        private final ArrayDeque<Point> trail = new ArrayDeque<>();

        Tracker() {
            showTimer.setRepeats(false);
            switchTimer.setRepeats(false);
            hideTimer.setRepeats(false);
            Toolkit.getDefaultToolkit().addAWTEventListener(this,
                AWTEvent.MOUSE_EVENT_MASK | AWTEvent.MOUSE_MOTION_EVENT_MASK
                    | AWTEvent.MOUSE_WHEEL_EVENT_MASK | AWTEvent.KEY_EVENT_MASK);
            KeyboardFocusManager.getCurrentKeyboardFocusManager()
                .addPropertyChangeListener("permanentFocusOwner", e -> onFocus((Component) e.getNewValue()));
        }

        @Override
        public void eventDispatched(AWTEvent event) {
            if (event instanceof KeyEvent) {
                KeyEvent key = (KeyEvent) event;
                if (key.getID() == KeyEvent.KEY_PRESSED) {
                    lastInputWasKey = true;
                    if (key.getKeyCode() == KeyEvent.VK_ESCAPE) {
                        hide();
                    }
                }
                return;
            }
            MouseEvent mouse = (MouseEvent) event;
            int id = mouse.getID();
            Component under = componentUnder(mouse);
            boolean inCard = card != null && under != null && SwingUtilities.isDescendingFrom(under, card);
            if (id == MouseEvent.MOUSE_PRESSED) {
                lastInputWasKey = false;
                // a click elsewhere closes a pinned card; a help icon click then opens its own
                if (pinned && !inCard) {
                    hide();
                }
                return;
            }
            if (id == MouseEvent.MOUSE_WHEEL) {
                if (!inCard) {
                    hide();
                }
                return;
            }
            if (id != MouseEvent.MOUSE_MOVED && id != MouseEvent.MOUSE_ENTERED && id != MouseEvent.MOUSE_EXITED) {
                return;
            }
            Point from = trail.size() < TRAIL_LENGTH ? trail.peekFirst() : trail.pollFirst();
            trail.addLast(mouse.getLocationOnScreen());
            if (inCard) {
                stopMouseTimers();
                return;
            }
            if (!autoShow) {
                return;
            }
            JComponent hovered = findRow(under);
            if (row != null) {
                if (pinned) {
                    return;
                }
                if (hovered == row) {
                    stopMouseTimers();
                } else if (hovered != null) {
                    hideTimer.stop();
                    if (card.isShowing() && headingTo(from, mouse.getLocationOnScreen(),
                        new Rectangle(card.getLocationOnScreen(), card.getSize()))) {
                        // on the way to the card: switch only if the mouse stops on this row
                        pendingRow = hovered;
                        switchTimer.restart();
                    } else {
                        show(hovered, (Help) hovered.getClientProperty(KEY), false, false, false);
                    }
                } else {
                    switchTimer.stop();
                    pendingRow = null;
                    hideTimer.restart();
                }
            } else if (hovered == null) {
                showTimer.stop();
                pendingRow = null;
            } else if (hovered != pendingRow || !showTimer.isRunning()) {
                pendingRow = hovered;
                showTimer.restart();
            }
        }

        private void stopMouseTimers() {
            showTimer.stop();
            switchTimer.stop();
            hideTimer.stop();
            pendingRow = null;
        }

        private static Component componentUnder(MouseEvent mouse) {
            Component source = mouse.getComponent();
            Window window = source instanceof Window ? (Window) source : SwingUtilities.getWindowAncestor(source);
            if (window == null) {
                return null;
            }
            Point p = SwingUtilities.convertPoint(source, mouse.getPoint(), window);
            // leaving the window reports a point outside it
            return window.contains(p) ? SwingUtilities.getDeepestComponentAt(window, p.x, p.y) : null;
        }

        private void onFocus(Component owner) {
            JComponent focused = findRow(owner);
            if (autoShow && focused != null && lastInputWasKey && focused != row) {
                show(focused, (Help) focused.getClientProperty(KEY), false, false, false);
            }
        }

        private void showPending() {
            if (pendingRow != null && pendingRow.isShowing()) {
                show(pendingRow, (Help) pendingRow.getClientProperty(KEY), false, false, false);
            }
        }

        /** @param below under {@code target} (a Help button) instead of right of a row's controls */
        void show(JComponent target, Help help, boolean expand, boolean pin, boolean below) {
            if (help == null || !target.isShowing()) {
                return;
            }
            closePopup();
            row = target;
            expanded = expand;
            pinned = pin;
            stopMouseTimers();
            // asking for more pins the card, so it stays while the text is read
            card = createCard(help, expanded, () -> show(target, help, !expanded, true, below),
                pinned ? this::hide : null);
            Window window = SwingUtilities.getWindowAncestor(target);
            Rectangle bounds = window == null ? target.getGraphicsConfiguration().getBounds() : window.getBounds();
            Point at;
            if (below) {
                Point origin = target.getLocationOnScreen();
                at = place(new Rectangle(origin.x, origin.y + target.getHeight() + GAP / 2, 0, 0),
                    card.getPreferredSize(), bounds, false);
            } else {
                at = place(controlsOnScreen(target), card.getPreferredSize(), bounds);
            }
            popup = PopupFactory.getSharedInstance().getPopup(target, card, at.x, at.y);
            popup.show();
            rowLocation = target.getLocationOnScreen();
            watch.start();
        }

        /** The row's controls on screen: from its left edge to the right edge of its last visible child. */
        private static Rectangle controlsOnScreen(JComponent target) {
            int right = 0;
            for (Component child : target.getComponents()) {
                if (child.isVisible() && child.getWidth() > 0) {
                    right = Math.max(right, child.getX() + child.getWidth());
                }
            }
            Point origin = target.getLocationOnScreen();
            return new Rectangle(origin.x, origin.y, right > 0 ? right : target.getWidth(), target.getHeight());
        }

        private void checkRow() {
            if (row == null || !row.isShowing() || !row.getLocationOnScreen().equals(rowLocation)) {
                hide();
            }
        }

        void hideUnpinned() {
            if (!pinned) {
                hide();
            }
        }

        void hide() {
            closePopup();
            row = null;
            expanded = false;
            pinned = false;
            stopMouseTimers();
            watch.stop();
        }

        private void closePopup() {
            if (popup != null) {
                popup.hide();
                popup = null;
                card = null;
            }
        }
    }
}
