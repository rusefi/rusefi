package com.rusefi.ui.logview;

import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.icons.FlatSearchIcon;
import com.opensr5.ini.IniFileModel;
import com.rusefi.sensor_logs.CustomBinaryLogEntry;
import com.rusefi.sensor_logs.LoggingProfile;
import com.rusefi.sensor_logs.LoggingProfiles;
import com.rusefi.sensor_logs.SensorLogger;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.DefaultListModel;
import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.ListCellRenderer;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.Window;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Collection;
import java.util.EnumMap;
import java.util.EnumSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.function.BiFunction;
import java.util.function.Consumer;
import java.util.function.Predicate;

import static com.rusefi.ui.logview.ThemedStrip.themed;

/**
 * Edit profiles: the logging profiles on the left; the chosen profile's signals in the middle (the locked ones,
 * then each signal with an x to remove it, then the recommended signals the user removed, with Restore); and
 * on the right every signal data logging can record, grouped like the Log Viewer's sidebar and collapsed
 * until opened or searched, to add. Full can't
 * be edited; Trigger and Idle can, keeping their changes apart so Reset to recommended undoes them; custom
 * profiles start empty and can be renamed and deleted. Changes are saved as they are made and reach the
 * profile menus at once. Signals are named by their log label from the ECU's .ini; without an .ini nothing can
 * be added. [tag:console_theme]
 */
public final class LoggingProfilesEditor extends JPanel {
    /** Width of the action (x, Restore, + Add) at the end of a row, which a click there triggers. */
    static final int ACTION_WIDTH = 96;
    private static final int ROW_HEIGHT = 30;
    private static LoggingProfilesEditor open;

    /** GROUP: a collapsible group heading of the add list; HEADING: a fixed section heading. */
    enum Kind {HEADING, GROUP, LOCKED, SIGNAL, REMOVED, EMPTY, AVAILABLE}

    /** A row of the signal list or of the add list. */
    static final class Row {
        final Kind kind;
        final String text;
        final String channel;
        /** "recommended", "added", "not in this .ini": shown before the action. */
        final String tag;
        /** For an available signal: already in the profile. */
        final boolean inProfile;
        /** For a group: its signals are shown. */
        final boolean open;

        Row(Kind kind, String text, String channel, String tag, boolean inProfile) {
            this(kind, text, channel, tag, inProfile, false);
        }

        private Row(Kind kind, String text, String channel, String tag, boolean inProfile, boolean open) {
            this.kind = kind;
            this.text = text;
            this.channel = channel;
            this.tag = tag;
            this.inProfile = inProfile;
            this.open = open;
        }

        static Row heading(String text, String count) {
            return new Row(Kind.HEADING, text, null, count, false);
        }

        static Row group(String title, String count, boolean open) {
            return new Row(Kind.GROUP, title, null, count, false, open);
        }

        @Override
        public String toString() {
            return text;
        }
    }

    /** A signal data logging can record: its .ini channel, its log label and its sidebar group. */
    private static final class Signal {
        final String channel;
        final String label;
        final SignalCategories.Group group;

        Signal(String channel, String label, SignalCategories.Group group) {
            this.channel = channel;
            this.label = label;
            this.group = group;
        }
    }

    private final LoggingProfiles profiles;
    private final IniFileModel ini;
    /** Recordable signals in recording order, one per channel. */
    private final List<Signal> catalog = new ArrayList<>();
    private final Map<String, Signal> byChannel = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final Runnable profilesListener = this::refresh;

    final DefaultListModel<LoggingProfile> profileModel = new DefaultListModel<>();
    final JList<LoggingProfile> profileList = new JList<>(profileModel);
    final JButton newButton = new JButton("New");
    final JButton duplicateButton = new JButton("Duplicate");
    final JButton renameButton = new JButton("Rename");
    final JButton deleteButton = new JButton("Delete");
    final JButton resetButton = new JButton("Reset to recommended");
    final JLabel title = new JLabel();
    final Badge badge = new Badge();
    private final CardLayout bodyCards = new CardLayout();
    private final JPanel body = new JPanel(bodyCards);
    private final JLabel fullText = new JLabel();
    final DefaultListModel<Row> signalModel = new DefaultListModel<>();
    final JList<Row> signalList = rowList(signalModel);
    final JTextField search = new JTextField();
    /** Under the search: how many signals there are, or match, and how many the profile records. */
    final JLabel total = new JLabel();
    /** Groups of the add list the user opened; a search opens every group with a match. */
    private final Set<SignalCategories.Group> openGroups = EnumSet.noneOf(SignalCategories.Group.class);
    final DefaultListModel<Row> addModel = new DefaultListModel<>();
    final JList<Row> addList = rowList(addModel);
    final JLabel footer = new JLabel();
    private boolean refreshing;

    /** Asks for a profile name (dialog title, initial text); null when cancelled. Replaced in tests. */
    BiFunction<String, String, String> namePrompt = (title, initial) ->
            (String) JOptionPane.showInputDialog(this, "Name:", title, JOptionPane.PLAIN_MESSAGE, null, null, initial);
    /** Asks to go ahead with the given question. Replaced in tests. */
    Predicate<String> confirm = question -> JOptionPane.showConfirmDialog(this, question, "Logging profiles",
            JOptionPane.OK_CANCEL_OPTION) == JOptionPane.OK_OPTION;
    Consumer<String> messages = message -> JOptionPane.showMessageDialog(this, message, "Logging profiles",
            JOptionPane.WARNING_MESSAGE);

    /** @param ini the ECU's .ini, for the signals' names and the ones that can be added; null without one */
    public LoggingProfilesEditor(LoggingProfiles profiles, IniFileModel ini) {
        super(new BorderLayout());
        this.profiles = profiles;
        this.ini = ini;
        Map<String, SignalCategories.Group> groups = SignalCategories.groupsByChannel(ini);
        if (ini != null) {
            for (CustomBinaryLogEntry entry : SensorLogger.getOutputChannels(ini)) {
                if (!byChannel.containsKey(entry.getChannel())) {
                    SignalCategories.Group group = groups.get(entry.getChannel());
                    Signal signal = new Signal(entry.getChannel(), entry.getName(),
                            group != null ? group : SignalCategories.ruleGroup(entry.getName()));
                    catalog.add(signal);
                    byChannel.put(signal.channel, signal);
                }
            }
        }
        setBackground(themed("Console.Card.background", Color.WHITE));

        JPanel columns = new JPanel(new BorderLayout());
        columns.setOpaque(false);
        columns.add(profileColumn(), BorderLayout.WEST);
        columns.add(signalColumn(), BorderLayout.CENTER);
        columns.add(addColumn(), BorderLayout.EAST);
        add(columns, BorderLayout.CENTER);

        footer.setForeground(muted());
        footer.setBorder(BorderFactory.createEmptyBorder(8, 16, 8, 16));
        add(footer, BorderLayout.SOUTH);

        LoggingProfile selected = profiles.selected();
        refresh();
        select(selected.getId());
    }

    /**
     * Shows the editor in its own window, or brings the open one forward, with a profile chosen.
     *
     * @param profileId the profile to show first, or null for the one recordings use
     */
    public static void showDialog(Component parent, LoggingProfiles profiles, IniFileModel ini, String profileId) {
        if (open != null && open.isShowing()) {
            open.select(profileId != null ? profileId : profiles.selected().getId());
            SwingUtilities.getWindowAncestor(open).toFront();
            return;
        }
        Window owner = parent == null ? null : SwingUtilities.getWindowAncestor(parent);
        JDialog dialog = new JDialog(owner, "Logging profiles");
        LoggingProfilesEditor editor = new LoggingProfilesEditor(profiles, ini);
        if (profileId != null) {
            editor.select(profileId);
        }
        JButton close = new JButton("Close");
        close.addActionListener(e -> dialog.dispose());
        JPanel buttons = new JPanel(new FlowLayout(FlowLayout.RIGHT, 12, 8));
        buttons.setOpaque(false);
        buttons.add(close);
        editor.remove(editor.footer);
        JPanel bottom = new JPanel(new BorderLayout());
        bottom.setBackground(editor.getBackground());
        bottom.setBorder(BorderFactory.createMatteBorder(1, 0, 0, 0, separator()));
        bottom.add(editor.footer, BorderLayout.CENTER);
        bottom.add(buttons, BorderLayout.EAST);
        editor.add(bottom, BorderLayout.SOUTH);
        dialog.setContentPane(editor);
        dialog.getRootPane().registerKeyboardAction(e -> dialog.dispose(),
                KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0), JComponent.WHEN_IN_FOCUSED_WINDOW);
        dialog.setSize(1120, 700);
        dialog.setLocationRelativeTo(owner);
        open = editor;
        dialog.setVisible(true);
    }

    /** Follows changes made elsewhere while shown, e.g. a profile saved from the Log Viewer. */
    @Override
    public void addNotify() {
        super.addNotify();
        profiles.addListener(profilesListener);
    }

    @Override
    public void removeNotify() {
        profiles.removeListener(profilesListener);
        if (open == this) {
            open = null;
        }
        super.removeNotify();
    }

    private JComponent profileColumn() {
        JPanel column = new JPanel(new BorderLayout());
        column.setBackground(themed("Console.Card.sectionBackground", new Color(0xf7f8fa)));
        column.setBorder(BorderFactory.createMatteBorder(0, 0, 0, 1, separator()));
        column.setPreferredSize(new Dimension(240, 0));
        column.add(sectionLabel("Profiles", 12), BorderLayout.NORTH);

        profileList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        profileList.setCellRenderer(new ProfileRenderer());
        profileList.setOpaque(false);
        profileList.addListSelectionListener(e -> {
            if (!e.getValueIsAdjusting() && !refreshing) {
                refreshSelected();
            }
        });
        JScrollPane scroll = new JScrollPane(profileList);
        scroll.setBorder(BorderFactory.createEmptyBorder(0, 8, 0, 8));
        scroll.setOpaque(false);
        scroll.getViewport().setOpaque(false);
        column.add(scroll, BorderLayout.CENTER);

        newButton.setToolTipText("A new profile, starting with only the signals every log has");
        newButton.addActionListener(e -> createNew());
        duplicateButton.setToolTipText("A new profile recording what this one records");
        duplicateButton.addActionListener(e -> duplicate());
        renameButton.addActionListener(e -> rename());
        deleteButton.addActionListener(e -> delete());
        JPanel buttons = new JPanel(new GridLayout(2, 2, 6, 6));
        buttons.setOpaque(false);
        buttons.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createMatteBorder(1, 0, 0, 0, separator()),
                BorderFactory.createEmptyBorder(10, 12, 10, 12)));
        buttons.add(newButton);
        buttons.add(duplicateButton);
        buttons.add(renameButton);
        buttons.add(deleteButton);
        column.add(buttons, BorderLayout.SOUTH);
        return column;
    }

    private JComponent signalColumn() {
        JPanel column = new JPanel(new BorderLayout());
        column.setOpaque(false);
        JPanel header = new JPanel(new BorderLayout(10, 0));
        header.setOpaque(false);
        header.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createMatteBorder(0, 0, 1, 0, separator()),
                BorderFactory.createEmptyBorder(10, 16, 10, 16)));
        title.setFont(title.getFont().deriveFont(Font.BOLD, title.getFont().getSize2D() + 2));
        JPanel titleRow = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
        titleRow.setOpaque(false);
        titleRow.add(title);
        titleRow.add(Box.createHorizontalStrut(10));
        titleRow.add(badge);
        header.add(titleRow, BorderLayout.CENTER);
        resetButton.setToolTipText("Undo your changes to this profile");
        resetButton.addActionListener(e -> reset());
        header.add(resetButton, BorderLayout.EAST);
        column.add(header, BorderLayout.NORTH);

        fullText.setVerticalAlignment(JLabel.TOP);
        JPanel banner = new JPanel(new BorderLayout());
        banner.setBackground(themed("Console.Banner.info.background", new Color(0xeef4ff)));
        banner.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(themed("Console.Banner.info.borderColor", new Color(0xc9dafb))),
                BorderFactory.createEmptyBorder(12, 14, 12, 14)));
        banner.add(fullText, BorderLayout.CENTER);
        JPanel fullCard = new JPanel(new BorderLayout());
        fullCard.setOpaque(false);
        fullCard.setBorder(BorderFactory.createEmptyBorder(16, 16, 16, 16));
        fullCard.add(banner, BorderLayout.NORTH);

        signalList.setToolTipText("");
        signalList.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseReleased(MouseEvent event) {
                Row row = rowAt(signalList, event);
                if (row != null && inActionZone(signalList, event)) {
                    act(row);
                }
            }
        });
        signalList.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_DELETE, 0), "removeSignal");
        signalList.getActionMap().put("removeSignal", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Row row = signalList.getSelectedValue();
                if (row != null && row.kind == Kind.SIGNAL) {
                    act(row);
                }
            }
        });
        JScrollPane signals = new JScrollPane(signalList);
        signals.setBorder(BorderFactory.createEmptyBorder(4, 8, 8, 8));
        body.setOpaque(false);
        body.add(signals, "edit");
        body.add(fullCard, "full");
        column.add(body, BorderLayout.CENTER);
        return column;
    }

    private JComponent addColumn() {
        JPanel column = new JPanel(new BorderLayout());
        column.setBackground(themed("Console.Card.sectionBackground", new Color(0xf7f8fa)));
        column.setBorder(BorderFactory.createMatteBorder(0, 1, 0, 0, separator()));
        column.setPreferredSize(new Dimension(340, 0));
        search.putClientProperty(FlatClientProperties.PLACEHOLDER_TEXT, "Search signals to add");
        search.putClientProperty(FlatClientProperties.TEXT_FIELD_LEADING_ICON, new FlatSearchIcon());
        search.putClientProperty(FlatClientProperties.TEXT_FIELD_SHOW_CLEAR_BUTTON, true);
        search.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                refreshAvailable();
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                refreshAvailable();
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                refreshAvailable();
            }
        });
        // Enter in the search adds the only match, or the selected one
        search.addActionListener(e -> {
            Row row = addList.getSelectedValue();
            if (row == null) {
                List<Row> matches = availableSignals();
                row = matches.size() == 1 ? matches.get(0) : null;
            }
            if (row != null) {
                act(row);
            }
        });
        JPanel top = new JPanel(new BorderLayout(0, 6));
        top.setOpaque(false);
        top.setBorder(BorderFactory.createEmptyBorder(10, 12, 6, 12));
        top.add(search, BorderLayout.CENTER);
        total.setForeground(muted());
        total.setFont(total.getFont().deriveFont(total.getFont().getSize2D() - 1));
        total.setBorder(BorderFactory.createEmptyBorder(0, 2, 0, 0));
        top.add(total, BorderLayout.SOUTH);
        column.add(top, BorderLayout.NORTH);

        addList.setOpaque(false);
        addList.setToolTipText("");
        addList.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseReleased(MouseEvent event) {
                Row row = rowAt(addList, event);
                if (row != null && row.kind == Kind.GROUP) {
                    toggleGroup(row.text);
                } else if (row != null && row.kind == Kind.AVAILABLE) {
                    act(row);
                }
            }
        });
        addList.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), "addSignal");
        addList.getActionMap().put("addSignal", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Row row = addList.getSelectedValue();
                if (row != null && row.kind == Kind.GROUP) {
                    toggleGroup(row.text);
                } else if (row != null) {
                    act(row);
                }
            }
        });
        JScrollPane scroll = new JScrollPane(addList);
        scroll.setBorder(BorderFactory.createEmptyBorder(0, 8, 8, 8));
        scroll.setOpaque(false);
        scroll.getViewport().setOpaque(false);
        column.add(scroll, BorderLayout.CENTER);
        return column;
    }

    private JList<Row> rowList(DefaultListModel<Row> model) {
        JList<Row> list = new JList<Row>(model) {
            @Override
            public String getToolTipText(MouseEvent event) {
                int index = locationToIndex(event.getPoint());
                if (index < 0 || !getCellBounds(index, index).contains(event.getPoint())) {
                    return null;
                }
                return tooltip(model.get(index));
            }
        };
        list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        list.setFixedCellHeight(ROW_HEIGHT);
        list.setCellRenderer(new RowRenderer());
        return list;
    }

    private String tooltip(Row row) {
        switch (row.kind) {
            case LOCKED:
                return row.text + " is in every log";
            case SIGNAL:
                return "Click × or press Delete to stop recording " + row.text;
            case REMOVED:
                return "Removed from the recommended signals · click Restore to record it again";
            case AVAILABLE:
                return row.inProfile ? row.text + " is already recorded" : "Click to record " + row.text;
            case GROUP:
                return row.open ? "Click to hide these signals" : "Click to show these signals";
            default:
                return null;
        }
    }

    private static Row rowAt(JList<Row> list, MouseEvent event) {
        int index = list.locationToIndex(event.getPoint());
        if (index < 0 || !list.getCellBounds(index, index).contains(event.getPoint())) {
            return null;
        }
        return list.getModel().getElementAt(index);
    }

    private static boolean inActionZone(JList<Row> list, MouseEvent event) {
        int index = list.locationToIndex(event.getPoint());
        Rectangle cell = list.getCellBounds(index, index);
        return event.getX() >= cell.x + cell.width - ACTION_WIDTH;
    }

    // ---- what the rows do

    /** Removes a signal, restores a removed one, or adds an available one. */
    void act(Row row) {
        LoggingProfile profile = selected();
        if (profile == null || profile.isFull()) {
            return;
        }
        switch (row.kind) {
            case SIGNAL:
                profile.remove(row.channel, ini);
                break;
            case REMOVED:
            case AVAILABLE:
                if (row.inProfile) {
                    return;
                }
                profile.add(row.channel);
                break;
            default:
                return;
        }
        changed();
    }

    void reset() {
        LoggingProfile profile = selected();
        if (profile != null && profile.isEdited()
                && confirm.test("Undo your changes to " + profile.getName() + " and record its recommended signals?")) {
            profile.reset();
            changed();
        }
    }

    void createNew() {
        String name = askName("New logging profile", uniqueName("New profile"));
        if (name != null) {
            String id = profiles.create(name).getId();
            refresh();
            select(id);
        }
    }

    void duplicate() {
        LoggingProfile profile = selected();
        if (profile == null) {
            return;
        }
        String name = askName("Duplicate " + profile.getName(), uniqueName(profile.getName() + " copy"));
        if (name != null) {
            String id = profiles.duplicate(profile, name, ini).getId();
            refresh();
            select(id);
        }
    }

    void rename() {
        LoggingProfile profile = selected();
        if (profile == null || profile.getKind() != LoggingProfile.Kind.CUSTOM) {
            return;
        }
        String name = namePrompt.apply("Rename " + profile.getName(), profile.getName());
        if (name == null || name.trim().equals(profile.getName())) {
            return;
        }
        String problem = name.trim().equalsIgnoreCase(profile.getName()) ? null : profiles.problem(name);
        if (problem != null) {
            messages.accept(problem);
            return;
        }
        profiles.rename(profile, name);
        refresh();
    }

    void delete() {
        LoggingProfile profile = selected();
        if (profile == null || profile.getKind() != LoggingProfile.Kind.CUSTOM) {
            return;
        }
        boolean chosen = profiles.selected() == profile;
        if (!confirm.test("Delete the logging profile \"" + profile.getName() + "\"?"
                + (chosen ? " Recordings will use Full." : ""))) {
            return;
        }
        int index = profileList.getSelectedIndex();
        profiles.delete(profile.getId());
        refresh();
        profileList.setSelectedIndex(Math.min(index, profileModel.size() - 1));
    }

    private String askName(String dialogTitle, String initial) {
        String name = namePrompt.apply(dialogTitle, initial);
        if (name == null) {
            return null;
        }
        String problem = profiles.problem(name);
        if (problem != null) {
            messages.accept(problem);
            return null;
        }
        return name.trim();
    }

    private String uniqueName(String base) {
        String name = base;
        for (int number = 2; profiles.problem(name) != null; number++) {
            name = base + " " + number;
        }
        return name;
    }

    private void changed() {
        // tells the profile menus; refresh() follows through the listener while shown, else here
        profiles.save();
        if (!isDisplayable()) {
            refresh();
        }
    }

    // ---- showing the state

    LoggingProfile selected() {
        return profileList.getSelectedValue();
    }

    void select(String id) {
        for (int index = 0; index < profileModel.size(); index++) {
            if (profileModel.get(index).getId().equals(id)) {
                profileList.setSelectedIndex(index);
                profileList.ensureIndexIsVisible(index);
                return;
            }
        }
    }

    /** Rebuilds everything from the profiles, keeping the chosen profile when it still exists. */
    void refresh() {
        LoggingProfile previous = selected();
        refreshing = true;
        try {
            profileModel.clear();
            for (LoggingProfile profile : profiles.list()) {
                profileModel.addElement(profile);
            }
        } finally {
            refreshing = false;
        }
        int index = previous == null ? -1 : profileModel.indexOf(previous);
        profileList.setSelectedIndex(index >= 0 ? index : 0);
        refreshSelected();
    }

    private void refreshSelected() {
        LoggingProfile profile = selected();
        if (profile == null) {
            return;
        }
        boolean custom = profile.getKind() == LoggingProfile.Kind.CUSTOM;
        title.setText(profile.getName());
        badge.show(profile);
        resetButton.setVisible(profile.isEdited());
        renameButton.setEnabled(custom);
        deleteButton.setEnabled(custom);
        renameButton.setToolTipText(custom ? "Rename this profile" : "Built-in profiles can't be renamed");
        deleteButton.setToolTipText(custom ? "Delete this profile" : "Built-in profiles can't be deleted");
        search.setEnabled(!profile.isFull() && ini != null);
        addList.setEnabled(search.isEnabled());
        if (profile.isFull()) {
            fullText.setText("<html><b>Full records every signal the ECU reports.</b><br>It can't be edited, so a "
                    + "full log is always one click away. To start from everything and remove signals, use "
                    + "Duplicate.</html>");
            bodyCards.show(body, "full");
        } else {
            bodyCards.show(body, "edit");
            refreshSignals(profile);
        }
        refreshAvailable();
        int recorded = recorded(profile).size();
        if (ini == null) {
            footer.setText("Connect to the ECU to see and add its signals · changes are saved as you make them");
        } else if (profile.isFull()) {
            footer.setText(String.format(Locale.ROOT, "Full: all %,d signals", catalog.size()));
        } else {
            footer.setText(String.format(Locale.ROOT, "%s: %,d of %,d signals · changes are saved as you make them",
                    profile.getName(), recorded, catalog.size()));
        }
        profileList.repaint();
    }

    private void refreshSignals(LoggingProfile profile) {
        int keep = signalList.getSelectedIndex();
        signalModel.clear();
        signalModel.addElement(Row.heading("Always recorded", ""));
        for (String channel : LoggingProfile.LOCKED) {
            signalModel.addElement(new Row(Kind.LOCKED, labelOf(channel), channel, "", false));
        }
        Set<String> recommended = profile.recommended(ini);
        List<String> channels = recorded(profile);
        channels.removeIf(LoggingProfile::isLocked);
        signalModel.addElement(Row.heading("Signals", String.valueOf(channels.size())));
        if (channels.isEmpty()) {
            signalModel.addElement(new Row(Kind.EMPTY, "No signals yet: add them from the list on the right.",
                    null, "", false));
        }
        for (String channel : channels) {
            String tag = !byChannel.containsKey(channel) && ini != null ? "not in this .ini"
                    : profile.getKind() != LoggingProfile.Kind.RECOMMENDED ? ""
                    : recommended.contains(channel) && !profile.getAdded().contains(channel) ? "recommended" : "added";
            signalModel.addElement(new Row(Kind.SIGNAL, labelOf(channel), channel, tag, false));
        }
        List<String> removed = new ArrayList<>();
        for (String channel : profile.getRemoved()) {
            if (recommended.contains(channel)) {
                removed.add(channel);
            }
        }
        if (!removed.isEmpty()) {
            signalModel.addElement(Row.heading("Removed from recommended", String.valueOf(removed.size())));
            for (String channel : ordered(removed)) {
                signalModel.addElement(new Row(Kind.REMOVED, labelOf(channel), channel, "", false));
            }
        }
        if (keep >= 0) {
            // after a removal, the next row takes the removed one's place
            int index = Math.min(keep, signalModel.size() - 1);
            signalList.setSelectedIndex(index);
        }
    }

    /** Opens or closes a group of the add list, keeping it selected. */
    void toggleGroup(String title) {
        for (SignalCategories.Group group : SignalCategories.Group.values()) {
            if (group.title.equals(title) && !openGroups.remove(group)) {
                openGroups.add(group);
            }
        }
        refreshAvailable();
        for (int index = 0; index < addModel.size(); index++) {
            if (addModel.get(index).kind == Kind.GROUP && addModel.get(index).text.equals(title)) {
                addList.setSelectedIndex(index);
                addList.ensureIndexIsVisible(index);
            }
        }
    }

    private void refreshAvailable() {
        addModel.clear();
        LoggingProfile profile = selected();
        total.setVisible(ini != null);
        if (profile == null || ini == null) {
            return;
        }
        Set<String> inProfile = profile.channels(ini);
        String needle = search.getText().trim().toLowerCase(Locale.ROOT);
        Map<SignalCategories.Group, List<Row>> byGroup = new EnumMap<>(SignalCategories.Group.class);
        for (Signal signal : catalog) {
            if (needle.isEmpty() || signal.label.toLowerCase(Locale.ROOT).contains(needle)
                    || signal.channel.toLowerCase(Locale.ROOT).contains(needle)) {
                byGroup.computeIfAbsent(signal.group, group -> new ArrayList<>()).add(new Row(Kind.AVAILABLE,
                        signal.label, signal.channel, "", profile.isFull() || inProfile.contains(signal.channel)));
            }
        }
        int matches = 0;
        for (Map.Entry<SignalCategories.Group, List<Row>> group : byGroup.entrySet()) {
            boolean open = !needle.isEmpty() || openGroups.contains(group.getKey());
            matches += group.getValue().size();
            addModel.addElement(Row.group(group.getKey().title,
                    String.format(Locale.ROOT, "%,d", group.getValue().size()), open));
            if (open) {
                for (Row row : group.getValue()) {
                    addModel.addElement(row);
                }
            }
        }
        int recorded = recorded(profile).size();
        if (!needle.isEmpty()) {
            total.setText(String.format(Locale.ROOT, "%,d of %,d signals match", matches, catalog.size()));
        } else if (profile.isFull()) {
            total.setText(String.format(Locale.ROOT, "%,d signals, all recorded by Full", catalog.size()));
        } else {
            total.setText(String.format(Locale.ROOT, "%,d signals · %,d in this profile", catalog.size(), recorded));
        }
    }

    /**
     * A profile's signals in recording order. With an .ini, recommended context signals it lacks are left out,
     * as they can't be recorded; signals the user added stay, marked, so they can be removed.
     */
    private List<String> recorded(LoggingProfile profile) {
        List<String> channels = ordered(profile.channels(ini));
        if (ini != null) {
            channels.removeIf(channel -> !byChannel.containsKey(channel) && !profile.getAdded().contains(channel));
        }
        return channels;
    }

    /** The search's matches that can still be added. */
    private List<Row> availableSignals() {
        List<Row> rows = new ArrayList<>();
        for (int index = 0; index < addModel.size(); index++) {
            Row row = addModel.get(index);
            if (row.kind == Kind.AVAILABLE && !row.inProfile) {
                rows.add(row);
            }
        }
        return rows;
    }

    /** Recordable channels in recording order, then any the .ini lacks. */
    private List<String> ordered(Collection<String> channels) {
        Set<String> wanted = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);
        wanted.addAll(channels);
        List<String> ordered = new ArrayList<>();
        for (Signal signal : catalog) {
            if (wanted.remove(signal.channel)) {
                ordered.add(signal.channel);
            }
        }
        ordered.addAll(wanted);
        return ordered;
    }

    private String labelOf(String channel) {
        Signal signal = byChannel.get(channel);
        return signal != null ? signal.label : channel;
    }

    /** What a profile records, for the list on the left. */
    String subtitle(LoggingProfile profile) {
        if (profile.isFull()) {
            return "Every signal · read-only";
        }
        int count = recorded(profile).size();
        String signals = String.format(Locale.ROOT, "%,d signal%s", count, count == 1 ? "" : "s");
        if (profile.getKind() == LoggingProfile.Kind.CUSTOM) {
            return signals + " · yours";
        }
        return signals + (profile.isEdited() ? " · recommended, edited" : " · recommended");
    }

    private static Color muted() {
        return themed("Console.LogViewer.mutedForeground", Color.GRAY);
    }

    private static Color accent() {
        return themed("Console.Banner.info.accent", new Color(0x1d4ed8));
    }

    private static Color separator() {
        return themed("Console.Card.borderColor", Color.LIGHT_GRAY);
    }

    private static JLabel sectionLabel(String text, int top) {
        JLabel label = new JLabel(text.toUpperCase(Locale.ROOT));
        label.setFont(label.getFont().deriveFont(Font.BOLD, label.getFont().getSize2D() - 1));
        label.setForeground(muted());
        label.setBorder(BorderFactory.createEmptyBorder(top, 14, 6, 12));
        return label;
    }

    /** Profile name over what it records. */
    private final class ProfileRenderer extends JPanel implements ListCellRenderer<LoggingProfile> {
        private final JLabel name = new JLabel();
        private final JLabel detail = new JLabel();

        ProfileRenderer() {
            super(new GridLayout(2, 1, 0, 1));
            setBorder(BorderFactory.createEmptyBorder(6, 10, 6, 10));
            add(name);
            add(detail);
        }

        @Override
        public Component getListCellRendererComponent(JList<? extends LoggingProfile> list, LoggingProfile profile,
                                                      int index, boolean selected, boolean focused) {
            setOpaque(selected);
            setBackground(themed("Console.Card.background", Color.WHITE));
            setBorder(BorderFactory.createCompoundBorder(
                    BorderFactory.createLineBorder(selected
                            ? themed("Console.Banner.info.borderColor", new Color(0xc9dafb)) : list.getBackground()),
                    BorderFactory.createEmptyBorder(5, 9, 5, 9)));
            name.setText(profile.getName() + (profile == profiles.selected() ? "  ·  recording with" : ""));
            name.setFont(list.getFont().deriveFont(Font.BOLD));
            name.setForeground(list.getForeground());
            detail.setText(subtitle(profile));
            detail.setFont(list.getFont().deriveFont(list.getFont().getSize2D() - 1));
            detail.setForeground(muted());
            return this;
        }
    }

    /** Headings, locked signals, signals with their tag and action. */
    private final class RowRenderer extends JPanel implements ListCellRenderer<Row> {
        private final JLabel name = new JLabel();
        private final JLabel tag = new JLabel();
        private final JLabel action = new JLabel();
        private final LockIcon lock = new LockIcon();
        private final JPanel end = new JPanel(new BorderLayout(10, 0));

        RowRenderer() {
            super(new BorderLayout(8, 0));
            end.setOpaque(false);
            end.add(tag, BorderLayout.CENTER);
            end.add(action, BorderLayout.EAST);
            // actions, "In profile" and section counts end at one right edge
            action.setPreferredSize(new Dimension(72, ROW_HEIGHT));
            action.setHorizontalAlignment(JLabel.RIGHT);
            action.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 6));
            // room for text that renders a little wider than measured, so "recommended" keeps its last letter
            tag.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 4));
            add(name, BorderLayout.CENTER);
            add(end, BorderLayout.EAST);
            name.setIconTextGap(8);
        }

        // Like the sidebar's renderer: lay out whenever the list sizes the row, since validate() is a no-op
        // without a native peer (e.g. when painting offscreen), and skip revalidation per row.
        @Override
        public void setBounds(int x, int y, int width, int height) {
            super.setBounds(x, y, width, height);
            doLayout();
            end.doLayout();
        }

        @Override
        public void invalidate() {
        }

        @Override
        public void validate() {
        }

        @Override
        public void revalidate() {
        }

        @Override
        public Component getListCellRendererComponent(JList<? extends Row> list, Row row, int index,
                                                      boolean selected, boolean focused) {
            Font font = list.getFont();
            boolean enabled = list.isEnabled();
            setOpaque(selected && row.kind != Kind.HEADING);
            setBackground(themed("Console.LogViewer.chipBackground", list.getSelectionBackground()));
            setBorder(BorderFactory.createEmptyBorder(row.kind == Kind.HEADING ? 8 : 0, row.kind == Kind.GROUP ? 2 : 8,
                    0, 4));
            name.setIcon(row.kind == Kind.LOCKED ? lock : null);
            name.setText(row.text);
            name.setFont(font);
            name.setForeground(enabled ? list.getForeground() : muted());
            tag.setText("");
            tag.setFont(font.deriveFont(font.getSize2D() - 1));
            tag.setForeground(muted());
            action.setText("");
            action.setFont(font);
            action.setForeground(accent());
            switch (row.kind) {
                case GROUP:
                case HEADING:
                    // like the Log Viewer sidebar's sections
                    name.setText((row.kind == Kind.GROUP ? (row.open ? "▾  " : "▸  ") : "")
                            + row.text.toUpperCase(Locale.ROOT));
                    name.setFont(font.deriveFont(Font.BOLD, font.getSize2D() - 1));
                    name.setForeground(muted());
                    action.setText(row.tag);
                    action.setFont(font.deriveFont(font.getSize2D() - 1));
                    action.setForeground(muted());
                    break;
                case LOCKED:
                    tag.setText("always recorded");
                    break;
                case SIGNAL:
                    tag.setText(row.tag);
                    action.setText("×");
                    action.setFont(font.deriveFont(font.getSize2D() + 3));
                    action.setForeground(muted());
                    break;
                case REMOVED:
                    name.setText("<html><s>" + escape(row.text) + "</s></html>");
                    name.setForeground(muted());
                    action.setText("Restore");
                    break;
                case EMPTY:
                    name.setFont(font.deriveFont(Font.ITALIC));
                    name.setForeground(muted());
                    break;
                case AVAILABLE:
                    if (row.inProfile) {
                        name.setForeground(muted());
                        action.setText("In profile");
                        action.setFont(font.deriveFont(font.getSize2D() - 1));
                        action.setForeground(muted());
                    } else if (enabled) {
                        action.setText("+ Add");
                    }
                    break;
            }
            return this;
        }
    }

    private static String escape(String text) {
        return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
    }

    /** Colours by kind of profile: grey built-in, green recommended, amber edited, blue the user's. */
    static final class Badge extends JLabel {
        private Color fill = Color.LIGHT_GRAY;

        Badge() {
            setBorder(BorderFactory.createEmptyBorder(2, 9, 2, 9));
            setFont(getFont().deriveFont(Font.BOLD, getFont().getSize2D() - 1));
        }

        void show(LoggingProfile profile) {
            String style;
            switch (profile.getKind()) {
                case FULL:
                    setText("Built-in · read-only");
                    fill = themed("Console.LogViewer.chipBackground", new Color(0xeef0f3));
                    setForeground(muted());
                    return;
                case RECOMMENDED:
                    if (profile.isEdited()) {
                        setText("Recommended · edited (+" + profile.getAdded().size() + " −"
                                + profile.getRemoved().size() + ")");
                        style = "pending";
                    } else {
                        setText("Recommended");
                        style = "connected";
                    }
                    fill = themed("Console.Status." + style + ".background", new Color(0xe7f6ec));
                    setForeground(themed("Console.Status." + style + ".foreground", new Color(0x15803d)));
                    return;
                default:
                    setText("Your profile");
                    fill = themed("Console.Banner.info.background", new Color(0xeef4ff));
                    setForeground(accent());
            }
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(fill);
                g2.fillRoundRect(0, 0, getWidth(), getHeight(), getHeight(), getHeight());
            } finally {
                g2.dispose();
            }
            super.paintComponent(g);
        }
    }

    /** A small padlock in the muted colour, before the always-recorded signals. */
    private static final class LockIcon implements Icon {
        private static final int SIZE = 12;

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(muted());
                g2.setStroke(new BasicStroke(1.4f));
                g2.drawArc(x + 3, y, 6, 8, 0, 180);
                g2.drawLine(x + 3, y + 4, x + 3, y + 6);
                g2.drawLine(x + 9, y + 4, x + 9, y + 6);
                g2.fillRoundRect(x + 1, y + 5, 10, 7, 2, 2);
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return SIZE;
        }

        @Override
        public int getIconHeight() {
            return SIZE;
        }
    }
}
