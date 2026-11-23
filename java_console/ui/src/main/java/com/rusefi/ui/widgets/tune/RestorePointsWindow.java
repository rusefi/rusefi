package com.rusefi.ui.widgets.tune;

import com.devexperts.logging.Logging;
import com.formdev.flatlaf.FlatClientProperties;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.tune.xml.Msq;
import com.rusefi.util.RestorePointStore;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.function.Predicate;
import java.util.function.Supplier;

/**
 * [tag:restore_points] File › Restore Points…: the board's restore points, newest first, with when and why each was
 * made and how many values differ from the tune open now; for the selected one, the settings it would change. Restore
 * as changes applies it like any edit (undoable, unburned); Save as .msq copies it out; Delete removes it.
 * <p>
 * Each point's note and differences are worked out in the background, as reading 50 tunes takes a moment. The list
 * follows the store, so a point saved meanwhile (e.g. by a Burn) shows up by itself.
 */
public final class RestorePointsWindow {
    private static final Logging log = Logging.getLogging(RestorePointsWindow.class);
    private static final DateTimeFormatter WHEN = DateTimeFormatter.ofPattern("d MMM HH:mm", Locale.ENGLISH);
    /** On several lines: the list is narrow. */
    static final String EMPTY_TEXT = "<html><div style=\"text-align: center\">No restore points yet.<br><br>"
        + "One is kept before every Burn,<br>Load Tune and firmware update.</div></html>";

    private final JDialog dialog;
    private final RestorePointStore store;
    private final Supplier<IniFileModel> ini;
    private final Supplier<ConfigurationImage> openTune;
    private final Predicate<ConfigurationImage> restore;

    private final PointsModel pointsModel = new PointsModel();
    final JTable pointsTable = new JTable(pointsModel);
    private final DiffModel diffModel = new DiffModel();
    final JTable diffTable = new JTable(diffModel);
    final JLabel titleLabel = new JLabel(" ");
    final JLabel descriptionLabel = new JLabel(" ");
    final JLabel emptyLabel = new JLabel(EMPTY_TEXT);
    final JButton restoreButton = new JButton("Restore as changes");
    final JButton saveAsButton = new JButton("Save as .msq…");
    final JButton deleteButton = new JButton("Delete");
    private final CardLayout listCards = new CardLayout();
    private final JPanel listPanel = new JPanel(listCards);

    /** Worked out in the background, by file; cleared when the open tune changes. */
    private final Map<Path, Details> details = new ConcurrentHashMap<>();
    private final Runnable storeListener = () -> SwingUtilities.invokeLater(this::reload);
    private SwingWorker<Void, Path> worker;
    /** The tune the details were worked out against. */
    private ConfigurationImage comparedWith;
    private java.util.concurrent.Executor detailWork;

    /** A point's note and what restoring it would change; null diff when the tune could not be read. */
    static final class Details {
        final String note;
        final RestorePointDiff diff;

        Details(String note, RestorePointDiff diff) {
            this.note = note;
            this.diff = diff;
        }
    }

    /**
     * @param restore applies the restored tune as an edit; false when there is no tune to edit
     * @param createAction File › Create Restore Point…, offered at the top
     */
    public RestorePointsWindow(Window owner, RestorePointStore store, Supplier<IniFileModel> ini,
                               Supplier<ConfigurationImage> openTune, Predicate<ConfigurationImage> restore,
                               Action createAction) {
        this.store = store;
        this.ini = ini;
        this.openTune = openTune;
        this.restore = restore;
        dialog = new JDialog(owner, "Restore Points", Dialog.ModalityType.MODELESS);
        dialog.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

        JButton create = new JButton(createAction);
        create.setText("Create restore point…");
        JLabel hint = muted(new JLabel("One is kept before every Burn, Load Tune and firmware update. "
            + "The newest " + RestorePointStore.DEFAULT_KEEP + " are kept."));
        JButton openFolder = new JButton("Open folder");
        openFolder.putClientProperty(FlatClientProperties.BUTTON_TYPE, FlatClientProperties.BUTTON_TYPE_BORDERLESS);
        openFolder.setToolTipText("Where the restore points are kept, one folder per board");
        openFolder.addActionListener(e -> openFolder());
        JPanel top = new JPanel(new BorderLayout(10, 0));
        top.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createMatteBorder(0, 0, 1, 0, TuningStyle.separator()),
            BorderFactory.createEmptyBorder(10, 14, 10, 10)));
        top.setBackground(TuningStyle.barBackground());
        top.add(create, BorderLayout.WEST);
        top.add(hint, BorderLayout.CENTER);
        top.add(openFolder, BorderLayout.EAST);

        pointsTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        pointsTable.setFillsViewportHeight(true);
        pointsTable.setShowVerticalLines(false);
        pointsTable.setRowHeight(28);
        pointsTable.getColumnModel().getColumn(0).setPreferredWidth(110);
        pointsTable.getColumnModel().getColumn(1).setPreferredWidth(250);
        pointsTable.getColumnModel().getColumn(2).setPreferredWidth(60);
        pointsTable.getColumnModel().getColumn(2).setCellRenderer(rightAligned());
        pointsTable.getSelectionModel().addListSelectionListener(e -> {
            if (!e.getValueIsAdjusting()) {
                showSelected();
            }
        });
        emptyLabel.setHorizontalAlignment(SwingConstants.CENTER);
        emptyLabel.setForeground(TuningStyle.muted());
        listPanel.add(new JScrollPane(pointsTable), "list");
        listPanel.add(emptyLabel, "empty");
        listPanel.setPreferredSize(new Dimension(440, 420));

        diffTable.setFillsViewportHeight(true);
        diffTable.setShowVerticalLines(false);
        diffTable.setRowSelectionAllowed(false);
        diffTable.getColumnModel().getColumn(0).setPreferredWidth(300);
        diffTable.getColumnModel().getColumn(1).setCellRenderer(rightAligned());
        diffTable.getColumnModel().getColumn(2).setCellRenderer(rightAligned());
        titleLabel.setFont(titleLabel.getFont().deriveFont(Font.BOLD, titleLabel.getFont().getSize2D() + 2));
        descriptionLabel.setForeground(TuningStyle.muted());

        restoreButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        restoreButton.setToolTipText("Put this tune's values in place as unburned changes: review, Undo or Burn them");
        restoreButton.addActionListener(e -> restoreSelected());
        saveAsButton.setToolTipText("Save a copy of this restore point as a tune file");
        saveAsButton.addActionListener(e -> saveSelectedAs());
        deleteButton.setToolTipText("Delete this restore point");
        deleteButton.addActionListener(e -> deleteSelected());
        JPanel buttons = new JPanel(new BorderLayout());
        JPanel rightButtons = new JPanel(new FlowLayout(FlowLayout.RIGHT, 8, 0));
        rightButtons.add(saveAsButton);
        rightButtons.add(restoreButton);
        buttons.add(deleteButton, BorderLayout.WEST);
        buttons.add(rightButtons, BorderLayout.EAST);
        JLabel restoreHint = muted(new JLabel("<html>Restoring works like an edit: the values become<br>"
            + "unburned changes you can review, undo or Burn.</html>"));

        JPanel heading = new JPanel(new GridLayout(0, 1, 0, 2));
        heading.add(titleLabel);
        heading.add(descriptionLabel);
        JPanel south = new JPanel(new BorderLayout(0, 8));
        south.add(buttons, BorderLayout.NORTH);
        south.add(restoreHint, BorderLayout.SOUTH);
        JPanel detail = new JPanel(new BorderLayout(0, 12));
        detail.setBorder(BorderFactory.createEmptyBorder(14, 18, 14, 18));
        detail.add(heading, BorderLayout.NORTH);
        detail.add(new JScrollPane(diffTable), BorderLayout.CENTER);
        detail.add(south, BorderLayout.SOUTH);
        detail.setPreferredSize(new Dimension(520, 420));

        JSplitPane split = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, listPanel, detail);
        split.setResizeWeight(0.45);
        split.setBorder(BorderFactory.createEmptyBorder());
        JPanel content = new JPanel(new BorderLayout());
        content.add(top, BorderLayout.NORTH);
        content.add(split, BorderLayout.CENTER);
        dialog.setContentPane(content);
        dialog.getRootPane().registerKeyboardAction(e -> dialog.dispose(),
            KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_ESCAPE, 0), JComponent.WHEN_IN_FOCUSED_WINDOW);

        store.addListener(storeListener);
        dialog.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosed(WindowEvent e) {
                store.removeListener(storeListener);
                if (worker != null) {
                    worker.cancel(true);
                }
            }

            @Override
            public void windowActivated(WindowEvent e) {
                // the tune may have been edited while another window had focus
                if (openTune.get() != comparedWith) {
                    reload();
                }
            }
        });
        dialog.pack();
        dialog.setLocationRelativeTo(owner);
        reload();
    }

    public void show() {
        dialog.setVisible(true);
        dialog.toFront();
    }

    public boolean isShowing() {
        return dialog.isDisplayable() && dialog.isVisible();
    }

    JDialog getDialogForTests() {
        return dialog;
    }

    /** Works the details out on the calling thread, for tests. */
    void setDetailWorkForTests(java.util.concurrent.Executor work) {
        detailWork = work;
    }

    private String signature() {
        IniFileModel model = ini.get();
        return model == null ? null : model.getSignature();
    }

    /** Lists the points again and works out the details not yet known (all of them when the tune changed). */
    void reload() {
        RestorePointStore.RestorePoint selected = selected();
        List<RestorePointStore.RestorePoint> points;
        try {
            points = signature() == null ? Collections.emptyList() : store.list(signature());
        } catch (IOException e) {
            log.error("Failed to list restore points", e);
            points = Collections.emptyList();
        }
        ConfigurationImage tune = openTune.get();
        if (tune != comparedWith) {
            details.clear();
            comparedWith = tune;
        }
        dialog.setTitle("Restore Points — " + (signature() == null ? "no ECU"
            : store.folderFor(signature()).getFileName()));
        pointsModel.setPoints(points);
        listCards.show(listPanel, points.isEmpty() ? "empty" : "list");
        int row = selected == null ? -1 : pointsModel.indexOf(selected.file);
        if (row < 0 && !points.isEmpty()) {
            row = 0;
        }
        if (row >= 0) {
            pointsTable.getSelectionModel().setSelectionInterval(row, row);
        }
        showSelected();
        workOutDetails(points, tune);
    }

    private void workOutDetails(List<RestorePointStore.RestorePoint> points, ConfigurationImage tune) {
        IniFileModel model = ini.get();
        List<RestorePointStore.RestorePoint> missing = new ArrayList<>();
        for (RestorePointStore.RestorePoint point : points) {
            if (!details.containsKey(point.file)) {
                missing.add(point);
            }
        }
        if (missing.isEmpty() || model == null) {
            return;
        }
        if (worker != null) {
            worker.cancel(true);
            worker = null;
        }
        if (detailWork != null) {
            detailWork.execute(() -> {
                for (RestorePointStore.RestorePoint point : missing) {
                    details.put(point.file, detailsOf(point, tune, model));
                }
            });
            // rows updated, not data changed: that would drop the selection
            pointsModel.fireTableRowsUpdated(0, Math.max(0, pointsModel.getRowCount() - 1));
            showSelected();
            return;
        }
        worker = new SwingWorker<Void, Path>() {
            @Override
            protected Void doInBackground() {
                for (RestorePointStore.RestorePoint point : missing) {
                    if (isCancelled()) {
                        return null;
                    }
                    details.put(point.file, detailsOf(point, tune, model));
                    publish(point.file);
                }
                return null;
            }

            @Override
            protected void process(List<Path> done) {
                pointsModel.fireTableRowsUpdated(0, Math.max(0, pointsModel.getRowCount() - 1));
                RestorePointStore.RestorePoint selected = selected();
                if (selected != null && done.contains(selected.file)) {
                    showSelected();
                }
            }
        };
        worker.execute();
    }

    static Details detailsOf(RestorePointStore.RestorePoint point, ConfigurationImage tune, IniFileModel model) {
        try {
            Msq msq = point.readTune();
            String note = msq.bibliography.getTuneComment();
            RestorePointDiff diff = tune == null ? null : RestorePointDiff.of(msq, tune, model);
            return new Details(note == null ? "" : note.trim(), diff);
        } catch (Exception e) {
            log.error("Failed to read restore point " + point.file, e);
            return new Details("", null);
        }
    }

    private RestorePointStore.RestorePoint selected() {
        int row = pointsTable.getSelectedRow();
        return row < 0 ? null : pointsModel.points.get(row);
    }

    private void showSelected() {
        RestorePointStore.RestorePoint point = selected();
        Details known = point == null ? null : details.get(point.file);
        boolean hasPoint = point != null;
        deleteButton.setEnabled(hasPoint);
        saveAsButton.setEnabled(hasPoint);
        restoreButton.setEnabled(known != null && known.diff != null && known.diff.changedValues > 0);
        if (!hasPoint) {
            titleLabel.setText(" ");
            descriptionLabel.setText(" ");
            diffModel.setRows(Collections.emptyList());
            return;
        }
        titleLabel.setText(point.reason.label + " · " + WHEN.format(point.when));
        diffModel.setRows(known == null || known.diff == null ? Collections.emptyList() : known.diff.rows);
        descriptionLabel.setText(describe(point, known));
    }

    /** What the point is and how far it is from the tune open now. */
    static String describe(RestorePointStore.RestorePoint point, Details known) {
        String what;
        switch (point.reason) {
            case BEFORE_BURN:
                what = "The ECU's tune just before that Burn.";
                break;
            case BEFORE_LOAD:
                what = "The tune open before a tune file was loaded.";
                break;
            case BEFORE_FIRMWARE_UPDATE:
                what = "The ECU's tune before a firmware update.";
                break;
            default:
                what = known != null && !known.note.isEmpty() ? "Made by hand: " + known.note + "." : "Made by hand.";
                break;
        }
        if (known == null) {
            return what + " Comparing with the tune open now…";
        }
        if (known.diff == null) {
            return what + " It cannot be compared: no tune is open, or the file cannot be read.";
        }
        int changed = known.diff.changedValues;
        return what + (changed == 0 ? " It is the same as the tune open now."
            : String.format(Locale.ROOT, " %,d value%s differ%s from the tune open now.", changed,
            changed == 1 ? "" : "s", changed == 1 ? "s" : ""));
    }

    void restoreSelected() {
        RestorePointStore.RestorePoint point = selected();
        Details known = point == null ? null : details.get(point.file);
        if (known == null || known.diff == null) {
            return;
        }
        if (restore.test(known.diff.restored)) {
            dialog.dispose();
        } else {
            JOptionPane.showMessageDialog(dialog, "No tune is open to restore into.", "Restore Points",
                JOptionPane.WARNING_MESSAGE);
        }
    }

    private void saveSelectedAs() {
        RestorePointStore.RestorePoint point = selected();
        if (point == null) {
            return;
        }
        JFileChooser chooser = new JFileChooser();
        chooser.setFileFilter(new FileNameExtensionFilter("Tune files (.msq)", "msq"));
        chooser.setSelectedFile(new File(point.file.getFileName().toString()));
        if (chooser.showSaveDialog(dialog) != JFileChooser.APPROVE_OPTION) {
            return;
        }
        File target = chooser.getSelectedFile();
        if (!target.getName().toLowerCase(Locale.ROOT).endsWith(".msq")) {
            target = new File(target.getParentFile(), target.getName() + ".msq");
        }
        if (target.exists() && JOptionPane.showConfirmDialog(dialog, target.getName() + " already exists. Replace it?",
            "Save as .msq", JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE) != JOptionPane.YES_OPTION) {
            return;
        }
        try {
            Files.copy(point.file, target.toPath(), StandardCopyOption.REPLACE_EXISTING);
        } catch (IOException e) {
            JOptionPane.showMessageDialog(dialog, "Failed to save: " + e.getMessage(), "Save as .msq",
                JOptionPane.ERROR_MESSAGE);
        }
    }

    private void deleteSelected() {
        RestorePointStore.RestorePoint point = selected();
        if (point == null || JOptionPane.showConfirmDialog(dialog,
            "Delete the restore point " + point.reason.label.toLowerCase(Locale.ROOT) + ", " + WHEN.format(point.when)
                + "?", "Delete restore point", JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE)
            != JOptionPane.YES_OPTION) {
            return;
        }
        deletePoint(point);
    }

    void deletePoint(RestorePointStore.RestorePoint point) {
        try {
            // the store tells the list
            store.delete(point);
        } catch (IOException e) {
            JOptionPane.showMessageDialog(dialog, "Failed to delete: " + e.getMessage(), "Delete restore point",
                JOptionPane.ERROR_MESSAGE);
        }
    }

    private void openFolder() {
        String signature = signature();
        if (signature == null) {
            return;
        }
        try {
            Path folder = store.folderFor(signature);
            Files.createDirectories(folder);
            Desktop.getDesktop().open(folder.toFile());
        } catch (IOException | RuntimeException e) {
            log.error("Failed to open restore points folder", e);
        }
    }

    private static JLabel muted(JLabel label) {
        label.setForeground(TuningStyle.muted());
        return label;
    }

    private static DefaultTableCellRenderer rightAligned() {
        DefaultTableCellRenderer renderer = new DefaultTableCellRenderer();
        renderer.setHorizontalAlignment(SwingConstants.RIGHT);
        return renderer;
    }

    /** When, Why (reason and note), Differs. */
    private final class PointsModel extends AbstractTableModel {
        private List<RestorePointStore.RestorePoint> points = Collections.emptyList();

        void setPoints(List<RestorePointStore.RestorePoint> points) {
            this.points = points;
            fireTableDataChanged();
        }

        int indexOf(Path file) {
            for (int i = 0; i < points.size(); i++) {
                if (points.get(i).file.equals(file)) {
                    return i;
                }
            }
            return -1;
        }

        @Override
        public int getRowCount() {
            return points.size();
        }

        @Override
        public int getColumnCount() {
            return 3;
        }

        @Override
        public String getColumnName(int column) {
            return column == 0 ? "When" : column == 1 ? "Why" : "Differs";
        }

        @Override
        public Object getValueAt(int row, int column) {
            RestorePointStore.RestorePoint point = points.get(row);
            Details known = details.get(point.file);
            switch (column) {
                case 0:
                    return WHEN.format(point.when);
                case 1:
                    return known == null || known.note.isEmpty() ? point.reason.label
                        : point.reason.label + ": " + known.note;
                default:
                    return known == null ? "…" : known.diff == null ? "?"
                        : String.format(Locale.ROOT, "%,d", known.diff.changedValues);
            }
        }
    }

    /** Setting, Then (the restore point), Now (the open tune). */
    private static final class DiffModel extends AbstractTableModel {
        private List<RestorePointDiff.Row> rows = Collections.emptyList();

        void setRows(List<RestorePointDiff.Row> rows) {
            this.rows = rows;
            fireTableDataChanged();
        }

        @Override
        public int getRowCount() {
            return rows.size();
        }

        @Override
        public int getColumnCount() {
            return 3;
        }

        @Override
        public String getColumnName(int column) {
            return column == 0 ? "Setting" : column == 1 ? "Then" : "Now";
        }

        @Override
        public Object getValueAt(int row, int column) {
            RestorePointDiff.Row r = rows.get(row);
            return column == 0 ? r.label : column == 1 ? r.then : r.now;
        }
    }
}
