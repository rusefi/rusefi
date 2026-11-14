package com.rusefi.ui;

import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.icons.FlatSearchIcon;
import com.rusefi.ui.widgets.ConsoleFonts;
import com.rusefi.ui.widgets.JoinedButtons;

import javax.swing.BorderFactory;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.ListCellRenderer;
import javax.swing.UIManager;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.function.Consumer;

/**
 * Swaps what two pins do, from the Pinout tab: e.g. after wiring a connector the other way round. Lists the
 * board's other pins by connector with what uses them; a pin that cannot take this pin's settings (or give its
 * own) is greyed with the reason and cannot be chosen. The swap itself is {@link PinAssignments#swap}. A plain
 * panel so tests can drive it; {@link #showDialog} puts it in a dialog.
 */
final class PinSwapPanel extends JPanel {
    /** Another pin the selected one could swap with. */
    static final class Target {
        final PinoutPin pin;
        final String connector;
        /** Labels of the settings using it now; empty when free. */
        final List<String> users;
        /** Why it cannot swap, or null when it can. */
        final String problem;
        /** Labels of the settings this swap changes that take effect only after the ECU restarts. */
        final List<String> needRestart;

        Target(PinoutPin pin, String connector, List<String> users, String problem) {
            this(pin, connector, users, problem, java.util.Collections.emptyList());
        }

        Target(PinoutPin pin, String connector, List<String> users, String problem, List<String> needRestart) {
            this.pin = pin;
            this.connector = connector;
            this.users = users;
            this.problem = problem;
            this.needRestart = needRestart;
        }

        boolean canSwap() {
            return problem == null;
        }

        @Override
        public String toString() {
            return pin.name;
        }
    }

    final JTextField search = new JTextField();
    final DefaultListModel<Object> model = new DefaultListModel<>();
    final JList<Object> list = new JList<>(model);
    final JTextArea effect = new JTextArea(3, 20);
    /** Shown when the Lua script reads pins directly ({@link LuaReadPins}); arrives after the panel opens. */
    final JTextArea luaNote = PinAssignPanel.noteArea();
    final JButton swapButton = new JButton("Swap");
    final JButton cancelButton = new JButton("Cancel");

    private final PinoutPin source;
    private final List<String> sourceUsers;
    private final List<Target> targets;
    private Consumer<Target> onSwap = t -> { };
    private Runnable onClose = () -> { };

    PinSwapPanel(PinoutPin source, String connectorTitle, List<String> sourceUsers, List<Target> targets) {
        super(new BorderLayout(0, 12));
        this.source = source;
        this.sourceUsers = sourceUsers;
        this.targets = targets;
        setBorder(BorderFactory.createEmptyBorder(16, 18, 14, 18));
        // a fixed size: a dialog keeps its first size, and the list scrolls
        setPreferredSize(new Dimension(560, 580));

        add(createHeader(connectorTitle), BorderLayout.NORTH);
        add(createList(), BorderLayout.CENTER);
        add(createFooter(), BorderLayout.SOUTH);
        fill();
    }

    void setOnSwap(Consumer<Target> onSwap) {
        this.onSwap = onSwap;
    }

    void setOnClose(Runnable onClose) {
        this.onClose = onClose;
    }

    /** The tune's Lua script, once read; null when unknown. Warns when it reads pins directly. */
    void setLuaScript(String script) {
        PinAssignPanel.showLuaNote(luaNote, LuaReadPins.note(script, source.name + " or the pin you swap it with"));
    }

    // ---- building

    private JComponent createHeader(String connectorTitle) {
        JLabel title = new JLabel("Swap pin " + source.name + " with…");
        title.setFont(PinoutConnectorPage.resized(title.getFont(), Font.BOLD, 3));
        JLabel where = PinAssignPanel.muted(new JLabel(connectorTitle
            + (source.function.isEmpty() ? "" : "  ·  " + source.function)));
        JLabel now = new JLabel(sourceUsers.isEmpty() ? "Now free" : "Now used by " + String.join(", ", sourceUsers));
        JPanel text = new JPanel(new BorderLayout(0, 4));
        text.setOpaque(false);
        text.add(title, BorderLayout.NORTH);
        text.add(where, BorderLayout.CENTER);
        text.add(now, BorderLayout.SOUTH);

        search.putClientProperty(FlatClientProperties.PLACEHOLDER_TEXT, "Find a pin, function or tune use");
        search.putClientProperty(FlatClientProperties.TEXT_FIELD_LEADING_ICON, new FlatSearchIcon());
        search.putClientProperty(FlatClientProperties.TEXT_FIELD_SHOW_CLEAR_BUTTON, true);
        search.setPreferredSize(new Dimension(0, JoinedButtons.HEIGHT));
        search.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                fill();
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                fill();
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                fill();
            }
        });
        search.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_DOWN, 0), "listDown");
        search.getActionMap().put("listDown", new PinAssignPanel.SwingAction(() -> move(1)));
        search.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_UP, 0), "listUp");
        search.getActionMap().put("listUp", new PinAssignPanel.SwingAction(() -> move(-1)));
        search.addActionListener(e -> swap());

        JPanel header = new JPanel(new BorderLayout(0, 12));
        header.setOpaque(false);
        header.add(text, BorderLayout.NORTH);
        header.add(search, BorderLayout.SOUTH);
        return header;
    }

    private JComponent createList() {
        list.setSelectionModel(new PinAssignPanel.SkipHeaders(model,
            item -> item instanceof Target && ((Target) item).canSwap()));
        list.setCellRenderer(new Renderer());
        list.addListSelectionListener(e -> {
            if (!e.getValueIsAdjusting()) {
                updateEffect();
            }
        });
        list.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2 && selected() != null) {
                    swap();
                }
            }
        });
        JScrollPane scroll = new JScrollPane(list);
        scroll.setBorder(BorderFactory.createLineBorder(UIManager.getColor("Component.borderColor")));
        return scroll;
    }

    private JComponent createFooter() {
        effect.setLineWrap(true);
        effect.setWrapStyleWord(true);
        effect.setEditable(false);
        effect.setFocusable(false);
        effect.setOpaque(false);
        effect.setBorder(BorderFactory.createEmptyBorder());
        effect.setFont(UIManager.getFont("Label.font"));
        JLabel note = PinAssignPanel.muted(new JLabel("Applied like an edit in Tuning: Undo there, and Burn to keep it."));

        swapButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        swapButton.addActionListener(e -> swap());
        cancelButton.addActionListener(e -> onClose.run());
        JPanel buttons = new JPanel(new FlowLayout(FlowLayout.RIGHT, JoinedButtons.ROW_GAP, 0));
        buttons.setOpaque(false);
        buttons.add(cancelButton);
        buttons.add(swapButton);

        JPanel text = new JPanel(new BorderLayout(0, 6));
        text.setOpaque(false);
        text.add(effect, BorderLayout.NORTH);
        text.add(luaNote, BorderLayout.CENTER);
        text.add(note, BorderLayout.SOUTH);
        JPanel footer = new JPanel(new BorderLayout(0, 10));
        footer.setOpaque(false);
        footer.add(text, BorderLayout.NORTH);
        footer.add(buttons, BorderLayout.SOUTH);
        return footer;
    }

    // ---- content

    /** Lists the pins matching the search under their connector titles, keeping the selection when shown. */
    private void fill() {
        Object selected = list.getSelectedValue();
        String needle = search.getText().trim().toLowerCase(Locale.ROOT);
        model.clear();
        String connector = null;
        for (Target target : targets) {
            if (!matches(target, needle)) {
                continue;
            }
            if (!target.connector.equals(connector)) {
                connector = target.connector;
                model.addElement(new PinAssignPanel.Header(connector));
            }
            model.addElement(target);
        }
        if (model.isEmpty()) {
            model.addElement(new PinAssignPanel.Header(targets.isEmpty() ? "No other pin to swap with"
                : "No pin matches"));
        }
        int index = selected == null ? -1 : model.indexOf(selected);
        if (index >= 0) {
            list.setSelectedIndex(index);
        } else {
            list.clearSelection();
            if (!needle.isEmpty()) {
                move(1);
            }
        }
        updateEffect();
    }

    private static boolean matches(Target target, String needle) {
        if (needle.isEmpty()) {
            return true;
        }
        List<String> texts = new ArrayList<>(target.users);
        texts.add(target.pin.name);
        texts.add(target.pin.function);
        texts.add(target.connector);
        for (String text : texts) {
            if (text.toLowerCase(Locale.ROOT).contains(needle)) {
                return true;
            }
        }
        return false;
    }

    /** Selects the next (or previous) pin it can swap with. */
    private void move(int step) {
        int index = list.getSelectedIndex();
        for (int i = index + step; i >= 0 && i < model.size(); i += step) {
            Object item = model.get(i);
            if (item instanceof Target && ((Target) item).canSwap()) {
                list.setSelectedIndex(i);
                list.ensureIndexIsVisible(i);
                return;
            }
        }
    }

    /** Selects a pin by name; false when it is not listed or cannot swap. */
    boolean select(String pinName) {
        for (int i = 0; i < model.size(); i++) {
            Object item = model.get(i);
            if (item instanceof Target && ((Target) item).pin.name.equals(pinName)) {
                list.setSelectedIndex(i);
                list.ensureIndexIsVisible(i);
                return list.getSelectedIndex() == i;
            }
        }
        return false;
    }

    Target selected() {
        Object value = list.getSelectedValue();
        return value instanceof Target && ((Target) value).canSwap() ? (Target) value : null;
    }

    private void updateEffect() {
        Target target = selected();
        swapButton.setEnabled(target != null);
        effect.setText(effectText(target));
    }

    String effectText(Target target) {
        if (target == null) {
            return "Choose the pin to swap " + source.name + " with.";
        }
        List<String> moves = new ArrayList<>();
        if (!sourceUsers.isEmpty()) {
            moves.add(String.join(", ", sourceUsers) + " moves to " + target.pin.name);
        }
        if (!target.users.isEmpty()) {
            moves.add(String.join(", ", target.users) + " moves to " + source.name);
        }
        String text = String.join("; ", moves) + ".";
        if (sourceUsers.isEmpty() || target.users.isEmpty()) {
            // the pin whose settings moved away is left with none
            text += " " + (sourceUsers.isEmpty() ? target.pin.name : source.name) + " becomes free.";
        }
        return text + PinAssignPanel.restartNote(target.needRestart);
    }

    private void swap() {
        Target target = selected();
        if (target == null) {
            return;
        }
        onSwap.accept(target);
        onClose.run();
    }

    /** Shows the panel in a modal dialog over {@code owner}; Escape cancels and Swap is the default. */
    static void showDialog(Component owner, PinSwapPanel panel) {
        PinAssignPanel.showModal(owner, "Swap pins", panel, panel.swapButton, panel.search, panel::setOnClose);
    }

    /** A connector title in small bold capitals; a pin with its function, and its use or why it cannot swap. */
    private final class Renderer implements ListCellRenderer<Object> {
        private final JLabel headerLabel = new JLabel();
        private final JPanel row = new JPanel(new BorderLayout(12, 0));
        private final JLabel name = new JLabel();
        /** Shortened with "…" when the row is narrow. */
        private final JLabel function = new JLabel();
        private final JLabel status = new JLabel();

        Renderer() {
            headerLabel.setBorder(BorderFactory.createEmptyBorder(10, 10, 4, 10));
            row.setBorder(BorderFactory.createEmptyBorder(5, 18, 5, 12));
            name.setPreferredSize(new Dimension(56, name.getPreferredSize().height));
            row.add(name, BorderLayout.WEST);
            row.add(function, BorderLayout.CENTER);
            row.add(status, BorderLayout.EAST);
        }

        @Override
        public Component getListCellRendererComponent(JList<?> l, Object value, int index, boolean isSelected,
                                                      boolean cellHasFocus) {
            if (value instanceof PinAssignPanel.Header) {
                headerLabel.setText(value.toString().toUpperCase(Locale.ROOT));
                headerLabel.setFont(PinoutConnectorPage.resized(l.getFont(), Font.BOLD, -1));
                headerLabel.setForeground(UIManager.getColor("Label.disabledForeground"));
                return headerLabel;
            }
            Target target = (Target) value;
            boolean selected = isSelected && target.canSwap();
            Color foreground = selected ? l.getSelectionForeground()
                : target.canSwap() ? l.getForeground() : UIManager.getColor("Label.disabledForeground");
            Color muted = selected ? foreground : UIManager.getColor("Label.disabledForeground");
            row.setBackground(selected ? l.getSelectionBackground() : l.getBackground());
            name.setFont(ConsoleFonts.mono(Font.BOLD, l.getFont().getSize()));
            name.setForeground(foreground);
            name.setText(target.pin.name);
            function.setFont(l.getFont());
            function.setForeground(muted);
            function.setText(target.pin.function);
            status.setFont(l.getFont());
            if (!target.canSwap()) {
                status.setText(target.problem);
                status.setForeground(muted);
            } else if (target.users.isEmpty()) {
                status.setText("Free");
                status.setForeground(muted);
            } else {
                status.setText(String.join(", ", target.users));
                status.setForeground(selected ? foreground
                    : PinoutDrawing.themed("Console.Pinout.used", foreground));
            }
            return row;
        }
    }
}
