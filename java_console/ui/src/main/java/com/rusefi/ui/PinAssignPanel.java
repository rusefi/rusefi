package com.rusefi.ui;

import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.icons.FlatSearchIcon;
import com.rusefi.ui.widgets.JoinedButtons;

import javax.swing.BorderFactory;
import javax.swing.DefaultListModel;
import javax.swing.DefaultListSelectionModel;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.ListCellRenderer;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Window;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.function.Consumer;
import java.util.function.Function;

/**
 * Picks the setting that uses a pin, from the Pinout tab: the settings whose pin choices include the pin,
 * grouped by their Tuning dialog, with where each is now. Choosing one moves that function to the pin and sets
 * whatever used the pin before to NONE ({@link PinAssignments}); Unassign frees the pin. A plain panel so tests
 * can drive it without a window; {@link #showDialog} puts it in a dialog.
 */
final class PinAssignPanel extends JPanel {
    final JTextField search = new JTextField();
    final DefaultListModel<Object> model = new DefaultListModel<>();
    final JList<Object> list = new JList<>(model);
    final JTextArea effect = new JTextArea(3, 20);
    /** Shown when the Lua script reads pins directly ({@link LuaReadPins}); arrives after the panel opens. */
    final JTextArea luaNote = noteArea();
    final JButton assignButton = new JButton("Assign");
    final JButton unassignButton = new JButton("Unassign");
    /** For a setting Tuning hides: assigns, then opens Tuning where its feature is turned on. */
    final JButton assignAndOpenButton = new JButton("Assign and open in Tuning");
    final JButton cancelButton = new JButton("Cancel");

    private final String pinName;
    private final String tsName;
    private final List<PinAssignments.Candidate> candidates;
    private final List<PinAssignments.Candidate> users;
    /** A pin choice ({@code ts_name}) as the user knows it, e.g. "C12 on Connector C". */
    private final Function<String, String> describePin;
    private Consumer<PinAssignments.Candidate> onAssign = c -> { };
    private Runnable onUnassign = () -> { };
    private Runnable onClose = () -> { };
    /** Opens a setting in Tuning: {fieldKey, dialogKey}. */
    private Consumer<String[]> onOpenInTuning = target -> { };

    PinAssignPanel(PinoutPin pin, String connectorTitle, List<PinAssignments.Candidate> candidates,
                   Function<String, String> describePin) {
        super(new BorderLayout(0, 12));
        this.pinName = pin.name;
        this.tsName = pin.tsName;
        this.candidates = candidates;
        this.users = PinAssignments.users(candidates, pin.tsName);
        this.describePin = describePin;
        setBorder(BorderFactory.createEmptyBorder(16, 18, 14, 18));
        // a fixed size: a dialog keeps its first size, and the list scrolls
        setPreferredSize(new Dimension(560, 580));

        add(createHeader(pin, connectorTitle), BorderLayout.NORTH);
        add(createList(), BorderLayout.CENTER);
        add(createFooter(), BorderLayout.SOUTH);
        fill();
        if (!users.isEmpty()) {
            select(users.get(0).fieldKey);
        }
        updateEffect();
    }

    void setOnAssign(Consumer<PinAssignments.Candidate> onAssign) {
        this.onAssign = onAssign;
    }

    void setOnUnassign(Runnable onUnassign) {
        this.onUnassign = onUnassign;
    }

    /** Where Assign and open in Tuning goes, as {fieldKey, dialogKey}. */
    void setOnOpenInTuning(Consumer<String[]> onOpenInTuning) {
        this.onOpenInTuning = onOpenInTuning;
    }

    void setOnClose(Runnable onClose) {
        this.onClose = onClose;
    }

    /** The tune's Lua script, once read; null when unknown. Warns when it reads pins directly. */
    void setLuaScript(String script) {
        showLuaNote(luaNote, LuaReadPins.note(script, pinName));
    }

    /** A muted, wrapping note, hidden while empty. */
    static JTextArea noteArea() {
        JTextArea area = new JTextArea();
        area.setLineWrap(true);
        area.setWrapStyleWord(true);
        area.setEditable(false);
        area.setFocusable(false);
        area.setOpaque(false);
        area.setBorder(BorderFactory.createEmptyBorder());
        area.setFont(UIManager.getFont("Label.font"));
        area.setVisible(false);
        return muted(area);
    }

    static void showLuaNote(JTextArea area, String text) {
        area.setText(text);
        area.setVisible(!text.isEmpty());
        area.revalidate();
    }

    // ---- building

    private JComponent createHeader(PinoutPin pin, String connectorTitle) {
        JLabel title = new JLabel("Assign pin " + pin.name);
        title.setFont(PinoutConnectorPage.resized(title.getFont(), Font.BOLD, 3));
        JLabel where = muted(new JLabel(connectorTitle + (pin.function.isEmpty() ? "" : "  ·  " + pin.function)));
        JLabel now = new JLabel(users.isEmpty() ? "Now free" : "Now used by " + labels(users));
        JPanel text = new JPanel(new BorderLayout(0, 4));
        text.setOpaque(false);
        text.add(title, BorderLayout.NORTH);
        text.add(where, BorderLayout.CENTER);
        text.add(now, BorderLayout.SOUTH);

        search.putClientProperty(FlatClientProperties.PLACEHOLDER_TEXT, "Find a setting");
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
        // arrows move through the list while typing
        search.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_DOWN, 0), "listDown");
        search.getActionMap().put("listDown", new SwingAction(() -> move(1)));
        search.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_UP, 0), "listUp");
        search.getActionMap().put("listUp", new SwingAction(() -> move(-1)));
        search.addActionListener(e -> assign());

        JPanel header = new JPanel(new BorderLayout(0, 12));
        header.setOpaque(false);
        header.add(text, BorderLayout.NORTH);
        header.add(search, BorderLayout.SOUTH);
        return header;
    }

    private JComponent createList() {
        list.setSelectionModel(new SkipHeaders(model, item -> true));
        list.setCellRenderer(new Renderer());
        list.addListSelectionListener(e -> {
            if (!e.getValueIsAdjusting()) {
                updateEffect();
            }
        });
        list.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2 && list.getSelectedValue() instanceof PinAssignments.Candidate) {
                    assign();
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
        JLabel note = muted(new JLabel("Applied like an edit in Tuning: Undo there, and Burn to keep it."));

        assignButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        assignButton.addActionListener(e -> assign());
        unassignButton.setToolTipText("Set every setting using " + pinName + " to None."
            + restartNote(PinAssignments.restartLabels(users)));
        unassignButton.setVisible(!users.isEmpty());
        unassignButton.addActionListener(e -> {
            onUnassign.run();
            onClose.run();
        });
        cancelButton.addActionListener(e -> onClose.run());
        assignAndOpenButton.setToolTipText("Assign, then open Tuning where this setting can be turned on");
        assignAndOpenButton.setVisible(false);
        assignAndOpenButton.addActionListener(e -> assignAndOpen());

        JPanel left = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
        left.setOpaque(false);
        left.add(unassignButton);
        JPanel right = new JPanel(new FlowLayout(FlowLayout.RIGHT, JoinedButtons.ROW_GAP, 0));
        right.setOpaque(false);
        right.add(cancelButton);
        right.add(assignAndOpenButton);
        right.add(assignButton);
        JPanel buttons = new JPanel(new BorderLayout());
        buttons.setOpaque(false);
        buttons.add(left, BorderLayout.WEST);
        buttons.add(right, BorderLayout.EAST);

        JPanel footer = new JPanel(new BorderLayout(0, 10));
        footer.setOpaque(false);
        JPanel text = new JPanel(new BorderLayout(0, 6));
        text.setOpaque(false);
        text.add(effect, BorderLayout.NORTH);
        text.add(luaNote, BorderLayout.CENTER);
        text.add(note, BorderLayout.SOUTH);
        footer.add(text, BorderLayout.NORTH);
        footer.add(buttons, BorderLayout.SOUTH);
        return footer;
    }

    // ---- content

    /** Lists the candidates matching the search under their dialog titles, keeping the selection when shown. */
    private void fill() {
        Object selected = list.getSelectedValue();
        String needle = search.getText().trim().toLowerCase(Locale.ROOT);
        Map<String, List<PinAssignments.Candidate>> bySection = new LinkedHashMap<>();
        for (PinAssignments.Candidate candidate : candidates) {
            if (needle.isEmpty() || candidate.label.toLowerCase(Locale.ROOT).contains(needle)
                || candidate.section.toLowerCase(Locale.ROOT).contains(needle)
                || candidate.fieldKey.toLowerCase(Locale.ROOT).contains(needle)) {
                bySection.computeIfAbsent(candidate.section, s -> new ArrayList<>()).add(candidate);
            }
        }
        model.clear();
        for (Map.Entry<String, List<PinAssignments.Candidate>> section : bySection.entrySet()) {
            model.addElement(new Header(section.getKey()));
            for (PinAssignments.Candidate candidate : section.getValue()) {
                model.addElement(candidate);
            }
        }
        if (model.isEmpty()) {
            model.addElement(new Header(candidates.isEmpty() ? "No setting in this tune can use " + pinName
                : "No setting matches"));
        }
        int index = selected == null ? -1 : model.indexOf(selected);
        if (index >= 0) {
            list.setSelectedIndex(index);
        } else if (!needle.isEmpty()) {
            move(1);
        } else {
            list.clearSelection();
        }
        updateEffect();
    }

    /** Selects the next (or previous) setting, skipping section titles. */
    private void move(int step) {
        int index = list.getSelectedIndex();
        for (int i = index + step; i >= 0 && i < model.size(); i += step) {
            if (model.get(i) instanceof PinAssignments.Candidate) {
                list.setSelectedIndex(i);
                list.ensureIndexIsVisible(i);
                return;
            }
        }
    }

    /** Selects a setting by its key; false when it is not listed. */
    boolean select(String fieldKey) {
        for (int i = 0; i < model.size(); i++) {
            Object item = model.get(i);
            if (item instanceof PinAssignments.Candidate && ((PinAssignments.Candidate) item).fieldKey.equals(fieldKey)) {
                list.setSelectedIndex(i);
                list.ensureIndexIsVisible(i);
                return true;
            }
        }
        return false;
    }

    PinAssignments.Candidate selected() {
        Object value = list.getSelectedValue();
        return value instanceof PinAssignments.Candidate ? (PinAssignments.Candidate) value : null;
    }

    /** What Assign would do, and whether it can. */
    private void updateEffect() {
        PinAssignments.Candidate candidate = selected();
        boolean canAssign = candidate != null && !candidate.uses(tsName);
        assignButton.setEnabled(canAssign);
        // a hidden setting needs its feature turned on: offer to go there
        assignAndOpenButton.setVisible(canAssign && candidate.hidden);
        effect.setText(effectText(candidate));
    }

    String effectText(PinAssignments.Candidate candidate) {
        if (candidate == null) {
            return candidates.isEmpty() ? "" : "Choose the setting that should use " + pinName + ".";
        }
        if (candidate.uses(tsName)) {
            return candidate.label + " already uses " + pinName + ".";
        }
        StringBuilder text = new StringBuilder(candidate.label)
            .append(candidate.current == null ? " will use " + pinName + "."
                : " moves from " + describePin.apply(candidate.current) + " to " + pinName + ".");
        if (!users.isEmpty()) {
            text.append(' ').append(labels(users)).append(users.size() == 1 ? " is" : " are")
                .append(" set to None.");
        }
        text.append(hiddenNote(candidate));
        // clearing a setting changes it as much as setting it
        List<PinAssignments.Candidate> changed = new ArrayList<>(users);
        changed.add(0, candidate);
        text.append(restartNote(PinAssignments.restartLabels(changed)));
        return text.toString();
    }

    /**
     * Why Tuning hides or disables a setting, e.g. " Tuning hides it because of Enable injection, Cylinders count:
     * assigning a pin does not turn it on."; empty while it is shown.
     */
    static String hiddenNote(PinAssignments.Candidate candidate) {
        return candidate.hidden ? " " + hiddenReason(candidate) + ": assigning a pin does not turn it on." : "";
    }

    /** "Tuning hides or disables it because of Enable injection, Cylinders count", for a hidden setting. */
    static String hiddenReason(PinAssignments.Candidate candidate) {
        return "Tuning hides or disables it"
            + (candidate.hiddenBy.isEmpty() ? "" : " because of " + String.join(", ", candidate.hiddenBy));
    }

    /**
     * " Takes effect after you burn and restart the ECU: Fan output." for settings the firmware applies only at
     * start-up ({@code requiresPowerCycle}); empty when none.
     */
    static String restartNote(List<String> labels) {
        return labels.isEmpty() ? "" : " Takes effect after you burn and restart the ECU: " + String.join(", ", labels)
            + ".";
    }

    private void assign() {
        PinAssignments.Candidate candidate = selected();
        if (candidate == null || candidate.uses(tsName)) {
            return;
        }
        onAssign.accept(candidate);
        onClose.run();
    }

    /** Assigns, closes, then opens Tuning where the hidden setting's feature is turned on. */
    private void assignAndOpen() {
        PinAssignments.Candidate candidate = selected();
        if (candidate == null || candidate.uses(tsName)) {
            return;
        }
        onAssign.accept(candidate);
        onClose.run();
        onOpenInTuning.accept(candidate.openTarget());
    }

    private static String labels(List<PinAssignments.Candidate> candidates) {
        List<String> labels = new ArrayList<>();
        for (PinAssignments.Candidate candidate : candidates) {
            labels.add(candidate.label);
        }
        return String.join(", ", labels);
    }

    static <T extends JComponent> T muted(T component) {
        component.setForeground(UIManager.getColor("Label.disabledForeground"));
        return component;
    }

    /** Shows the panel in a modal dialog over {@code owner}; Escape cancels and Assign is the default. */
    static void showDialog(Component owner, PinAssignPanel panel) {
        showModal(owner, "Assign pin", panel, panel.assignButton, panel.search, panel::setOnClose);
    }

    /**
     * Shows a picker panel in a modal dialog over {@code owner}: {@code defaultButton} answers Enter, Escape
     * closes, {@code focus} starts with the focus, and {@code closeHook} receives what closes the dialog.
     */
    static void showModal(Component owner, String title, JComponent panel, JButton defaultButton,
                          JComponent focus, Consumer<Runnable> closeHook) {
        Window window = owner == null ? null : SwingUtilities.getWindowAncestor(owner);
        JDialog dialog = new JDialog(window, title, JDialog.ModalityType.APPLICATION_MODAL);
        closeHook.accept(dialog::dispose);
        dialog.setContentPane(panel);
        dialog.getRootPane().setDefaultButton(defaultButton);
        dialog.getRootPane().registerKeyboardAction(e -> dialog.dispose(),
            KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0), JComponent.WHEN_IN_FOCUSED_WINDOW);
        dialog.pack();
        dialog.setLocationRelativeTo(window);
        SwingUtilities.invokeLater(focus::requestFocusInWindow);
        dialog.setVisible(true);
    }

    // ---- small parts

    /** A dialog title between the settings; not selectable. */
    static final class Header {
        final String text;

        Header(String text) {
            this.text = text;
        }

        @Override
        public String toString() {
            return text;
        }
    }

    /**
     * Clicks and arrows land on choosable items, never on section titles or items {@code selectable} refuses:
     * a step onto one moves on in the same direction.
     */
    static final class SkipHeaders extends DefaultListSelectionModel {
        private final DefaultListModel<Object> model;
        private final java.util.function.Predicate<Object> selectable;

        SkipHeaders(DefaultListModel<Object> model, java.util.function.Predicate<Object> selectable) {
            this.model = model;
            this.selectable = selectable;
            setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        }

        private boolean skipped(int index) {
            Object item = model.get(index);
            return item instanceof Header || !selectable.test(item);
        }

        @Override
        public void setSelectionInterval(int index0, int index1) {
            int target = index1;
            if (target >= 0 && target < model.size() && skipped(target)) {
                int step = target >= getLeadSelectionIndex() ? 1 : -1;
                while (target >= 0 && target < model.size() && skipped(target)) {
                    target += step;
                }
                if (target < 0 || target >= model.size()) {
                    return;
                }
            }
            super.setSelectionInterval(target, target);
        }
    }

    /** A section title in small bold capitals; a setting with where it is now on the right. */
    private final class Renderer implements ListCellRenderer<Object> {
        private final JLabel headerLabel = new JLabel();
        private final JPanel row = new JPanel(new BorderLayout(12, 0));
        private final JLabel name = new JLabel();
        private final JLabel status = new JLabel();

        Renderer() {
            headerLabel.setBorder(BorderFactory.createEmptyBorder(10, 10, 4, 10));
            row.setBorder(BorderFactory.createEmptyBorder(5, 18, 5, 12));
            row.add(name, BorderLayout.CENTER);
            row.add(status, BorderLayout.EAST);
        }

        @Override
        public Component getListCellRendererComponent(JList<?> l, Object value, int index, boolean isSelected,
                                                      boolean cellHasFocus) {
            if (value instanceof Header) {
                headerLabel.setText(value.toString().toUpperCase(Locale.ROOT));
                headerLabel.setFont(PinoutConnectorPage.resized(l.getFont(), Font.BOLD, -1));
                headerLabel.setForeground(UIManager.getColor("Label.disabledForeground"));
                return headerLabel;
            }
            PinAssignments.Candidate candidate = (PinAssignments.Candidate) value;
            Color background = isSelected ? l.getSelectionBackground() : l.getBackground();
            Color foreground = isSelected ? l.getSelectionForeground() : l.getForeground();
            Color muted = isSelected ? foreground : UIManager.getColor("Label.disabledForeground");
            row.setBackground(background);
            name.setFont(l.getFont());
            name.setForeground(foreground);
            name.setText(PinoutFunctionsPage.settingText(candidate));
            row.setToolTipText(candidate.hidden ? hiddenNote(candidate).trim() : null);
            status.setFont(l.getFont());
            if (candidate.uses(tsName)) {
                status.setText("uses " + pinName);
                status.setForeground(isSelected ? foreground
                    : PinoutDrawing.themed("Console.Pinout.used", foreground));
            } else {
                status.setText(candidate.current == null ? "not set" : "on " + describePin.apply(candidate.current));
                status.setForeground(muted);
            }
            return row;
        }
    }

    /** An action from a lambda. */
    static final class SwingAction extends javax.swing.AbstractAction {
        private final Runnable action;

        SwingAction(Runnable action) {
            this.action = action;
        }

        @Override
        public void actionPerformed(java.awt.event.ActionEvent e) {
            action.run();
        }
    }
}
