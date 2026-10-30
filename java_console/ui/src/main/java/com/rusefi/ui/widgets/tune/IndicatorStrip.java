package com.rusefi.ui.widgets.tune;

import com.opensr5.ini.FrontPageModel;
import com.opensr5.ini.IndicatorModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TsStringFunction;
import com.rusefi.io.ConnectionStatusLogic;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;
import java.awt.dnd.DragSource;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.*;
import java.util.List;
import java.util.function.Supplier;

/**
 * The INI [FrontPage] indicators as one strip of chips, shown by both the Gauges and the Tuning tab.
 * <p>
 * The strip is built from the INI the console has now and rebuilt when that changes, for example
 * when an ECU with different firmware connects, so the chips always match the connected ECU.
 * The strip is hidden while the INI has no indicators.
 * <p>
 * Chips are dragged into another order like the main tabs, and hidden from their right-click menu;
 * both are kept in the shared {@link IndicatorStripSettings}, so every strip follows.
 * <p>
 * The user's {@link CustomIndicator custom indicators} join the INI ones. One whose channel this
 * ECU's firmware does not have stays grey and says so in its tooltip, instead of disappearing.
 */
public class IndicatorStrip {
    public static final String RESET_ORDER = "Reset indicator order";
    public static final String SHOW_ALL = "Show all indicators";
    public static final String HIDDEN_MENU = "Hidden indicators";
    public static final String NEW_INDICATOR = "New indicator\u2026";
    public static final String EDIT_INDICATOR = "Edit indicator\u2026";
    public static final String RENAME_INDICATOR = "Rename indicator…";
    public static final String DELETE_INDICATOR = "Delete indicator";
    static final String CHIP_HINT = "Drag to move · right-click to hide, edit or add indicators";
    public static final String ALL_HIDDEN = "All indicators are hidden. Right-click to show them";
    /** Larger than a twitch during a click, so a click never moves a chip (as for the main tabs). */
    static final int START_DISTANCE = Math.max(5, DragSource.getDragThreshold());
    private static final int LINE_WIDTH = 3;

    private final Supplier<IniFileModel> iniSupplier;
    private final IndicatorStripSettings settings;
    private final JPanel content = new JPanel(new BorderLayout()) {
        @Override
        public void paint(Graphics g) {
            super.paint(g);
            paintInsertionLine(g);
        }
    };
    // status changes arrive off the EDT; the INI may only be known from the second one (LOADING, then CONNECTED)
    private final ConnectionStatusLogic.Listener connectionListener =
        isConnected -> SwingUtilities.invokeLater(this::refresh);
    private final Runnable settingsListener = this::rebuildChips;
    private final JLabel allHiddenLabel = new JLabel(ALL_HIDDEN);
    private IniFileModel ini;
    private Prompts prompts = new Prompts();
    /** Every indicator of the INI, in INI order. */
    private List<Entry> iniEntries = Collections.emptyList();
    /** The INI indicators, then the custom ones. */
    private List<Entry> entries = Collections.emptyList();
    /** The chips on show, in strip order. */
    private List<Entry> shown = Collections.emptyList();
    private IndicatorPanel indicators;
    private boolean built;
    private boolean active;
    private boolean destroyed;

    private int dragged = -1;
    private Point pressedAt;
    private boolean dragging;
    /** Where the dragged chip would go, counted before it is removed; -1 when no line is shown. */
    private int insertBefore = -1;
    private Rectangle line;

    public IndicatorStrip(Supplier<IniFileModel> iniSupplier, IndicatorStripSettings settings) {
        this.iniSupplier = iniSupplier;
        this.settings = settings;
        content.setOpaque(false);
        allHiddenLabel.setForeground(UIManager.getColor("Label.disabledForeground"));
        allHiddenLabel.setBorder(BorderFactory.createEmptyBorder(4, 6, 4, 6));
        content.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                maybeShowMenu(e, null);
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                maybeShowMenu(e, null);
            }
        });
        refresh();
        settings.addListener(settingsListener);
        ConnectionStatusLogic.INSTANCE.addListener(connectionListener);
    }

    public JComponent getContent() {
        return content;
    }

    /** Rebuilds the chips if the INI changed since they were built. */
    public void refresh() {
        if (destroyed) {
            return;
        }
        IniFileModel current = iniSupplier.get();
        if (built && current == ini) {
            return;
        }
        built = true;
        ini = current;
        iniEntries = entriesOf(current);
        rebuildChips();
    }

    private static List<Entry> entriesOf(IniFileModel ini) {
        FrontPageModel frontPage = ini == null ? null : ini.getFrontPage();
        if (frontPage == null) {
            return Collections.emptyList();
        }
        Set<String> taken = new HashSet<>();
        List<Entry> result = new ArrayList<>();
        for (IndicatorModel model : frontPage.getIndicators()) {
            result.add(new Entry(IndicatorStripSettings.keyOf(model.getExpression(), taken), model, null, null));
        }
        return result;
    }

    private Entry customEntry(CustomIndicator indicator) {
        List<String> missing = indicator.missingChannels(ini);
        if (ini == null || missing.isEmpty()) {
            // without an INI the chip waits for data rather than calling channels unknown
            return new Entry(indicator.key(), indicator.toModel(), null, indicator.getName());
        }
        // never on, in the neutral colour, and polling nothing
        IndicatorModel unavailable = new IndicatorModel("{ 0 }", indicator.getOffLabel(), indicator.getOnLabel(),
            CustomIndicator.Tone.NEUTRAL.iniBackground, CustomIndicator.Tone.NEUTRAL.iniForeground,
            CustomIndicator.Tone.NEUTRAL.iniBackground, CustomIndicator.Tone.NEUTRAL.iniForeground);
        String note = (missing.size() == 1 ? "channel " : "channels ") + String.join(", ", missing)
            + (missing.size() == 1 ? " is" : " are") + " not available on this ECU's firmware";
        return new Entry(indicator.key(), unavailable, note, indicator.getName());
    }

    /** Lays the chips out again in the shared order, without the hidden ones. */
    private void rebuildChips() {
        if (destroyed) {
            return;
        }
        cancelDrag();
        if (indicators != null) {
            indicators.destroy();
            indicators = null;
        }
        content.removeAll();
        entries = new ArrayList<>(iniEntries);
        for (CustomIndicator indicator : settings.getCustomIndicators()) {
            entries.add(customEntry(indicator));
        }
        shown = new ArrayList<>();
        for (Entry entry : settings.arrange(entries, e -> e.key)) {
            if (!settings.isHidden(entry.key)) {
                shown.add(entry);
            }
        }
        if (!shown.isEmpty()) {
            List<IndicatorModel> models = new ArrayList<>();
            for (Entry entry : shown) {
                models.add(entry.model);
            }
            indicators = new IndicatorPanel(models, ini, 0);
            JPanel panel = indicators.getPanel();
            panel.setOpaque(false);
            Component[] chips = panel.getComponents();
            for (int i = 0; i < chips.length; i++) {
                installDrag(chips[i], i);
                if (shown.get(i).note != null) {
                    indicators.setNote(i, shown.get(i).note);
                }
                if (!shown.get(i).title.isEmpty()) {
                    indicators.setTitle(i, shown.get(i).title);
                }
            }
            indicators.setHint(CHIP_HINT);
            indicators.setActive(active);
            content.add(panel, BorderLayout.CENTER);
        } else if (!entries.isEmpty()) {
            // keep something to right-click, or hidden chips could never come back
            content.add(allHiddenLabel, BorderLayout.CENTER);
        }
        content.setVisible(!entries.isEmpty());
        content.revalidate();
        content.repaint();
    }

    private void installDrag(Component chip, int index) {
        chip.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                if (!maybeShowMenu(e, shown.get(index))) {
                    pressed(index, toPanel(e), SwingUtilities.isLeftMouseButton(e));
                }
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                // Windows shows the menu on release
                maybeShowMenu(e, shown.get(index));
                released();
            }
        });
        chip.addMouseMotionListener(new MouseAdapter() {
            @Override
            public void mouseDragged(MouseEvent e) {
                dragged(toPanel(e));
            }
        });
    }

    private Point toPanel(MouseEvent e) {
        return SwingUtilities.convertPoint(e.getComponent(), e.getPoint(), indicators.getPanel());
    }

    void pressed(int index, Point point, boolean leftButton) {
        cancelDrag();
        if (leftButton) {
            dragged = index;
            pressedAt = point;
        }
    }

    void dragged(Point point) {
        if (dragged < 0) {
            return;
        }
        if (!dragging && point.distance(pressedAt) < START_DISTANCE) {
            return;
        }
        dragging = true;
        content.setCursor(Cursor.getPredefinedCursor(Cursor.MOVE_CURSOR));
        updateInsertionPoint(point);
        content.repaint();
    }

    void released() {
        if (dragging && insertBefore >= 0 && insertBefore != dragged && insertBefore != dragged + 1) {
            move(dragged, insertBefore);
        }
        cancelDrag();
    }

    private void cancelDrag() {
        boolean wasShowingLine = line != null;
        dragged = -1;
        pressedAt = null;
        dragging = false;
        insertBefore = -1;
        line = null;
        content.setCursor(null);
        if (wasShowingLine) {
            content.repaint();
        }
    }

    /**
     * The chip the dragged one would go before, in the row under the pointer (or the nearest row, when
     * the pointer is between or beyond the rows): the first whose middle is right of the pointer, or
     * just after the row's last chip.
     */
    private void updateInsertionPoint(Point point) {
        Component[] chips = indicators.getPanel().getComponents();
        Component nearest = null;
        int nearestDistance = Integer.MAX_VALUE;
        for (Component chip : chips) {
            int distance = Math.abs(point.y - (chip.getY() + chip.getHeight() / 2));
            if (distance < nearestDistance) {
                nearest = chip;
                nearestDistance = distance;
            }
        }
        if (nearest == null) {
            insertBefore = -1;
            line = null;
            return;
        }
        int lastInRow = -1;
        for (int i = 0; i < chips.length; i++) {
            if (chips[i].getY() != nearest.getY()) {
                continue;
            }
            Rectangle bounds = chips[i].getBounds();
            if (point.x < bounds.x + bounds.width / 2) {
                insertBefore = i;
                line = new Rectangle(bounds.x - 3 - LINE_WIDTH / 2, bounds.y, LINE_WIDTH, bounds.height);
                return;
            }
            lastInRow = i;
        }
        Rectangle bounds = chips[lastInRow].getBounds();
        insertBefore = lastInRow + 1;
        line = new Rectangle(bounds.x + bounds.width + 3 - LINE_WIDTH / 2, bounds.y, LINE_WIDTH, bounds.height);
    }

    /** Draws the insertion line, unless dropping there would leave the chip where it is. */
    private void paintInsertionLine(Graphics g) {
        if (!dragging || line == null || indicators == null || insertBefore == dragged || insertBefore == dragged + 1) {
            return;
        }
        Rectangle r = SwingUtilities.convertRectangle(indicators.getPanel(), line, content);
        Color color = UIManager.getColor("Component.accentColor");
        g.setColor(color != null ? color : content.getForeground());
        g.fillRect(r.x, r.y + 2, r.width, r.height - 4);
    }

    /** Moves shown chip {@code from} before shown chip {@code before} (or to the end), keeping hidden ones in place. */
    void move(int from, int before) {
        List<String> order = new ArrayList<>();
        for (Entry entry : settings.arrange(entries, e -> e.key)) {
            order.add(entry.key);
        }
        String movedKey = shown.get(from).key;
        order.remove(movedKey);
        if (before < shown.size()) {
            order.add(order.indexOf(shown.get(before).key), movedKey);
        } else {
            order.add(order.indexOf(shown.get(shown.size() - 1).key) + 1, movedKey);
        }
        // keep the place of indicators only another ECU's firmware has
        for (String stored : settings.getOrder()) {
            if (!order.contains(stored)) {
                order.add(stored);
            }
        }
        settings.setOrder(order);
    }

    private boolean maybeShowMenu(MouseEvent e, Entry chip) {
        if (!e.isPopupTrigger() || !content.isShowing()) {
            return false;
        }
        cancelDrag();
        JPopupMenu menu = new JPopupMenu();
        fillMenu(menu, chip);
        menu.show(e.getComponent(), e.getX(), e.getY());
        return true;
    }

    /**
     * The strip's menu: edit or delete {@code chip} when it is a custom one, hide it (when a chip was
     * clicked), add a custom indicator, bring hidden ones back, reset the order.
     */
    public void fillMenu(JComponent menu, Entry chip) {
        CustomIndicator custom = chip == null ? null : customIndicator(chip.key);
        if (custom != null) {
            JMenuItem edit = new JMenuItem(EDIT_INDICATOR);
            edit.setToolTipText("Change the condition, labels or colours of this indicator");
            edit.addActionListener(e -> {
                CustomIndicator changed = prompts.edit(dialogParent(), ini, custom);
                if (changed != null) {
                    settings.putCustomIndicator(changed);
                }
            });
            menu.add(edit);
            JMenuItem rename = new JMenuItem(RENAME_INDICATOR);
            rename.setToolTipText("Change the name menus and tooltips use for this indicator");
            rename.addActionListener(e -> {
                String name = prompts.rename(dialogParent(), custom.getName());
                if (name != null) {
                    settings.putCustomIndicator(custom.renamed(name));
                }
            });
            menu.add(rename);
            JMenuItem delete = new JMenuItem(DELETE_INDICATOR);
            delete.setToolTipText("Remove this indicator from the Gauges and Tuning tabs");
            delete.addActionListener(e -> {
                if (prompts.confirmDelete(dialogParent(), chip.getName())) {
                    settings.removeCustomIndicator(custom.getId());
                }
            });
            menu.add(delete);
        }
        if (chip != null) {
            JMenuItem hide = new JMenuItem("Hide “" + chip.getName() + "”");
            hide.setToolTipText("Hide this indicator on the Gauges and Tuning tabs");
            hide.addActionListener(e -> settings.setHidden(chip.key, true));
            menu.add(hide);
            menu.add(new JPopupMenu.Separator());
        }
        JMenuItem create = new JMenuItem(NEW_INDICATOR);
        boolean full = settings.getCustomIndicators().size() >= IndicatorStripSettings.MAX_CUSTOM;
        create.setEnabled(!full);
        create.setToolTipText(full ? "There can be at most " + IndicatorStripSettings.MAX_CUSTOM + " custom indicators"
            : "Add an indicator that turns on when a channel meets a condition");
        create.addActionListener(e -> {
            CustomIndicator created = prompts.edit(dialogParent(), ini, null);
            if (created != null) {
                settings.putCustomIndicator(created);
            }
        });
        menu.add(create);
        JMenu hiddenMenu = new JMenu(HIDDEN_MENU);
        hiddenMenu.setToolTipText("Choose a hidden indicator to show it again");
        for (Entry entry : entries) {
            if (settings.isHidden(entry.key)) {
                JMenuItem show = new JMenuItem(entry.getName());
                show.addActionListener(e -> settings.setHidden(entry.key, false));
                hiddenMenu.add(show);
            }
        }
        hiddenMenu.setEnabled(hiddenMenu.getItemCount() > 0);
        menu.add(hiddenMenu);
        JMenuItem showAll = new JMenuItem(SHOW_ALL);
        showAll.setEnabled(hiddenMenu.getItemCount() > 0);
        showAll.addActionListener(e -> settings.showAll());
        menu.add(showAll);
        JMenuItem reset = new JMenuItem(RESET_ORDER);
        reset.setToolTipText("Put the indicators back in the firmware's order");
        reset.setEnabled(settings.hasCustomOrder());
        reset.addActionListener(e -> settings.resetOrder());
        menu.add(reset);
    }

    /** Polls the indicator channels only while the tab showing this strip is visible. */
    public void setActive(boolean active) {
        this.active = active;
        if (active) {
            refresh();
        }
        if (indicators != null) {
            indicators.setActive(active);
        }
    }

    public void destroy() {
        destroyed = true;
        settings.removeListener(settingsListener);
        ConnectionStatusLogic.INSTANCE.removeListener(connectionListener);
        if (indicators != null) {
            indicators.destroy();
            indicators = null;
        }
    }

    @Nullable
    private CustomIndicator customIndicator(String key) {
        for (CustomIndicator indicator : settings.getCustomIndicators()) {
            if (indicator.key().equals(key)) {
                return indicator;
            }
        }
        return null;
    }

    /**
     * The window the strip is in: an option pane centres on its parent component, and the strip is a thin
     * band at the top of the window, so dialogs opened from it would otherwise appear at the top edge.
     */
    Component dialogParent() {
        Window window = SwingUtilities.getWindowAncestor(content);
        return window != null ? window : content;
    }

    /** The dialogs the menu opens; tests replace them. */
    static class Prompts {
        /** The new or changed indicator, or null when cancelled. */
        @Nullable
        CustomIndicator edit(Component parent, @Nullable IniFileModel ini, @Nullable CustomIndicator existing) {
            return new CustomIndicatorDialog(ini, existing).show(parent);
        }

        /** The new name, empty to name the indicator by its labels again, or null when cancelled. */
        @Nullable
        String rename(Component parent, String current) {
            String name = (String) JOptionPane.showInputDialog(parent,
                "Name (leave empty to use the labels)", RENAME_INDICATOR.replace("…", ""),
                JOptionPane.PLAIN_MESSAGE, null, null, current);
            if (name != null && name.trim().length() > CustomIndicatorDialog.MAX_LABEL) {
                JOptionPane.showMessageDialog(parent, "A name can be at most " + CustomIndicatorDialog.MAX_LABEL
                    + " characters.", RENAME_INDICATOR.replace("…", ""), JOptionPane.ERROR_MESSAGE);
                return null;
            }
            return name;
        }

        boolean confirmDelete(Component parent, String name) {
            return JOptionPane.showConfirmDialog(parent, "Delete the indicator \u201c" + name + "\u201d?",
                DELETE_INDICATOR, JOptionPane.OK_CANCEL_OPTION) == JOptionPane.OK_OPTION;
        }
    }

    void setPromptsForTests(Prompts prompts) {
        this.prompts = prompts;
    }

    // Package-private so tests see which indicators the strip is showing.
    IndicatorPanel getIndicatorPanel() {
        return indicators;
    }

    List<Entry> getShown() {
        return shown;
    }

    boolean isDragging() {
        return dragging;
    }

    /** One indicator on the strip and the key its place and visibility are stored under. */
    public static final class Entry {
        final String key;
        final IndicatorModel model;
        /** Why the chip cannot show its state, or null. */
        final String note;
        /** The name the user gave a custom indicator; empty when it has none. */
        final String title;

        Entry(String key, IndicatorModel model, String note, @Nullable String title) {
            this.key = key;
            this.model = model;
            this.note = note;
            this.title = title == null ? "" : title;
        }

        /** The name the user gave it, else "Fan off / Fan on": both labels, or the one that is plain text. */
        String getName() {
            if (!title.isEmpty()) {
                return title;
            }
            String off = plainLabel(model.getOffLabel());
            String on = plainLabel(model.getOnLabel());
            if (!off.isEmpty() && !on.isEmpty() && !off.equals(on)) {
                return off + " / " + on;
            }
            if (!on.isEmpty()) {
                return on;
            }
            return off.isEmpty() ? key : off;
        }

        private static String plainLabel(String label) {
            if (label == null || TsStringFunction.containsStringFunction(label)) {
                return "";
            }
            return IndicatorPanel.resolveLabel(label, null, Collections.emptyMap());
        }
    }
}
