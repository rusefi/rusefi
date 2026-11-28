package com.rusefi.ui.widgets.tune;

import com.devexperts.logging.Logging;
import com.formdev.flatlaf.FlatClientProperties;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.StringIniField;
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
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Optional;
import java.util.concurrent.ConcurrentHashMap;
import java.util.function.Predicate;
import java.util.function.Supplier;

/**
 * [tag:restore_points] File › Restore Points…: the open car's restore points, or those of another car on the same
 * board picked in Car, newest first, with when and why each was made and how many values differ from the tune open
 * now; for the selected one, the settings it would change. Restore as changes applies it like any edit (undoable,
 * unburned), asking first for another car's point; Save as .msq copies it out; Delete removes it.
 * <p>
 * Each point's note and differences are worked out in the background, as reading 50 tunes takes a moment. The list
 * follows the store, so a point saved meanwhile (e.g. by a Burn) shows up by itself.
 */
public final class RestorePointsWindow {
    private static final Logging log = Logging.getLogging(RestorePointsWindow.class);
    private static final DateTimeFormatter WHEN = DateTimeFormatter.ofPattern("d MMM HH:mm", Locale.ENGLISH);
    static final String EMPTY_TEXT = "<html><div style=\"text-align: center\">One is kept automatically before every "
        + "Burn, Load Tune and firmware update.<br>You can also make one now, before trying something.</div></html>";

    private final JDialog dialog;
    private final RestorePointStore store;
    private final Supplier<IniFileModel> ini;
    private final Supplier<ConfigurationImage> openTune;
    private final Predicate<RestorePointStore.RestorePoint> restore;

    private final PointsModel pointsModel = new PointsModel();
    final JTable pointsTable = new JTable(pointsModel);
    private final DiffModel diffModel = new DiffModel();
    final JTable diffTable = new JTable(diffModel);
    final JLabel titleLabel = new JLabel(" ");
    final JLabel descriptionLabel = new JLabel(" ");
    final JLabel emptyLabel = new JLabel(EMPTY_TEXT);
    /** Instead of the list and details while the car shown has no points. */
    final JPanel emptyPanel = new JPanel(new GridBagLayout());
    private final CardLayout bodyCards = new CardLayout();
    private final JPanel body = new JPanel(bodyCards);
    static final String UNNAMED_HINT = "Set a Vehicle Name (Setup › Vehicle Information) so each of your cars keeps "
        + "its own list of restore points.";
    final JLabel unnamedHint = new JLabel(UNNAMED_HINT);
    /** Opens the page with the Vehicle Name setting; shown once {@link #setOpenVehicleName} gives a way to. */
    final JButton vehicleNameButton = new JButton("Open Vehicle Information");
    /** Opens that page; false when it cannot (no tune, or the .ini has no such page). */
    private java.util.function.BooleanSupplier openVehicleName;
    /** {@link #unnamedHint} as a note under the header, shown while the open car has no Vehicle Name. */
    final JPanel unnamedBanner = new JPanel(new BorderLayout(10, 0));
    /** The car shown, e.g. "Orange Miata" or "This car". */
    final JLabel carTitle = new JLabel(" ");
    /** Its board, how many points it has and how many are kept. */
    final JLabel carSubtitle = new JLabel(" ");
    private final JLabel carLabel = new JLabel("Car");
    /** The board folder in {@link #carBox}: cars with no name or serial, and points from before cars had folders. */
    static final String NO_VEHICLE_NAME = "No vehicle name";
    final JLabel folderLabel = new JLabel(" ");
    private final CardLayout detailCards = new CardLayout();
    private final JPanel detailPanel = new JPanel(detailCards);
    final JButton changeFolderButton = new JButton("Change folder…");
    /** Picks the new folder; a file chooser unless a test sets it. */
    private Supplier<Path> folderChooser = this::chooseFolder;
    /** Asks whether to move the points kept so far: YES, NO or CANCEL_OPTION; a dialog unless a test sets it. */
    private java.util.function.IntUnaryOperator moveQuestion = this::askToMove;
    /** Says some points were not moved; true to try again. A dialog unless a test sets it. */
    private Predicate<String> retryQuestion = this::askToRetryMove;
    final JButton restoreButton = new JButton("Restore as changes");
    final JButton saveAsButton = new JButton("Save as .msq…");
    final JButton deleteButton = new JButton("Delete");
    /** The board's cars with restore points, and always the car open now. */
    final JComboBox<CarChoice> carBox = new JComboBox<>();
    /** The car picked in {@link #carBox} when not the one open now; null shows the open car. */
    private Path chosenCar;
    /** True while {@link #reload} refills {@link #carBox}: its selection events are not the user's. */
    private boolean fillingCars;
    /** Asks before restoring another car's point; a dialog unless a test sets it. */
    private Predicate<String> otherCarQuestion = this::askToRestoreOtherCar;

    /** A car folder in {@link #carBox}. */
    static final class CarChoice {
        final Path folder;
        final String label;

        CarChoice(Path folder, String label) {
            this.folder = folder;
            this.label = label;
        }

        @Override
        public String toString() {
            return label;
        }
    }

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
                               Supplier<ConfigurationImage> openTune, Predicate<RestorePointStore.RestorePoint> restore,
                               Action createAction) {
        this.store = store;
        this.ini = ini;
        this.openTune = openTune;
        this.restore = restore;
        dialog = new JDialog(owner, "Restore Points", Dialog.ModalityType.MODELESS);
        dialog.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

        // header: the car shown, the Car box (only with other cars to pick) and the main action
        JButton create = new JButton(createAction);
        create.setText("Create restore point…");
        create.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        carTitle.setFont(carTitle.getFont().deriveFont(Font.BOLD, carTitle.getFont().getSize2D() + 5));
        carSubtitle.setForeground(TuningStyle.muted());
        JPanel titles = new JPanel(new GridLayout(0, 1, 0, 2));
        titles.setOpaque(false);
        titles.add(carTitle);
        titles.add(carSubtitle);
        carBox.setToolTipText("Whose restore points to show: this car's, or another car's on the same board");
        carBox.setPrototypeDisplayValue(new CarChoice(null, "A long vehicle name (this car)"));
        carBox.addActionListener(e -> {
            CarChoice choice = (CarChoice) carBox.getSelectedItem();
            if (fillingCars || choice == null) {
                return;
            }
            Path thisCar = carFolder();
            chosenCar = choice.folder.equals(thisCar) ? null : choice.folder;
            reload();
        });
        carLabel.setLabelFor(carBox);
        carLabel.setDisplayedMnemonic('a');
        JPanel actions = new JPanel(new FlowLayout(FlowLayout.RIGHT, 8, 0));
        actions.setOpaque(false);
        actions.add(carLabel);
        actions.add(carBox);
        actions.add(Box.createHorizontalStrut(8));
        actions.add(create);
        JPanel header = new JPanel(new BorderLayout(16, 0));
        header.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createMatteBorder(0, 0, 1, 0, TuningStyle.separator()),
            BorderFactory.createEmptyBorder(14, 18, 14, 18)));
        header.setBackground(TuningStyle.barBackground());
        header.add(titles, BorderLayout.CENTER);
        // at their own height, not stretched to the two title lines
        header.add(centredVertically(actions), BorderLayout.EAST);

        // a note, not one more grey line: the open car has no Vehicle Name
        Color accent = TuningStyle.color("Component.accentColor", new Color(0x2675BF));
        Color panel = TuningStyle.color("Panel.background", Color.WHITE);
        unnamedBanner.setBackground(mix(accent, panel, 0.12));
        unnamedBanner.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(mix(accent, panel, 0.35)),
            BorderFactory.createEmptyBorder(8, 12, 8, 12)));
        unnamedBanner.add(new JLabel(UIManager.getIcon("OptionPane.informationIcon") == null ? null
            : scaled(UIManager.getIcon("OptionPane.informationIcon"), 18)), BorderLayout.WEST);
        unnamedHint.setToolTipText("Without a Vehicle Name, the points are kept by the ECU's serial number");
        unnamedBanner.add(unnamedHint, BorderLayout.CENTER);
        vehicleNameButton.setToolTipText("Open the page with the Vehicle Name setting; type a name, then Burn");
        vehicleNameButton.setVisible(false);
        vehicleNameButton.addActionListener(e -> {
            if (openVehicleName != null && !openVehicleName.getAsBoolean()) {
                JOptionPane.showMessageDialog(dialog, "The Vehicle Information page cannot be opened now: no tune "
                    + "is loaded, or this firmware has no such page.", "Vehicle Name", JOptionPane.WARNING_MESSAGE);
            }
        });
        unnamedBanner.add(centredVertically(vehicleNameButton), BorderLayout.EAST);
        JPanel bannerHolder = new JPanel(new BorderLayout());
        bannerHolder.setBorder(BorderFactory.createEmptyBorder(12, 18, 12, 18));
        bannerHolder.add(unnamedBanner, BorderLayout.CENTER);

        // footer: where the points are kept, an occasional setting
        JButton openFolder = new JButton("Open folder");
        openFolder.setToolTipText("The folder of the car shown: one folder per car");
        openFolder.addActionListener(e -> openFolder());
        folderLabel.setForeground(TuningStyle.muted());
        changeFolderButton.setToolTipText("Keep the restore points in another folder, e.g. one synced between PCs");
        changeFolderButton.addActionListener(e -> showFolderMenu());
        JPanel folderButtons = new JPanel(new FlowLayout(FlowLayout.RIGHT, 8, 0));
        folderButtons.setOpaque(false);
        folderButtons.add(changeFolderButton);
        folderButtons.add(openFolder);
        JPanel footer = new JPanel(new BorderLayout(10, 0));
        footer.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createMatteBorder(1, 0, 0, 0, TuningStyle.separator()),
            BorderFactory.createEmptyBorder(8, 18, 8, 18)));
        footer.setBackground(TuningStyle.barBackground());
        footer.add(folderLabel, BorderLayout.CENTER);
        footer.add(folderButtons, BorderLayout.EAST);

        // no points: the whole body says so and offers to make one
        JLabel emptyTitle = new JLabel("No restore points yet");
        emptyTitle.setFont(emptyTitle.getFont().deriveFont(Font.BOLD, emptyTitle.getFont().getSize2D() + 2));
        emptyLabel.setForeground(TuningStyle.muted());
        JButton createNow = new JButton(createAction);
        createNow.setText("Create restore point now…");
        GridBagConstraints c = new GridBagConstraints();
        c.gridx = 0;
        c.insets = new Insets(4, 0, 4, 0);
        emptyPanel.add(emptyTitle, c);
        emptyPanel.add(emptyLabel, c);
        c.insets = new Insets(12, 0, 0, 0);
        emptyPanel.add(createNow, c);

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
        JScrollPane listScroll = new JScrollPane(pointsTable);
        listScroll.setBorder(BorderFactory.createEmptyBorder());
        listScroll.setPreferredSize(new Dimension(430, 420));

        diffTable.setFillsViewportHeight(true);
        diffTable.setShowVerticalLines(false);
        diffTable.setRowSelectionAllowed(false);
        diffTable.getColumnModel().getColumn(0).setPreferredWidth(300);
        diffTable.getColumnModel().getColumn(1).setCellRenderer(valueRenderer());
        diffTable.getColumnModel().getColumn(2).setCellRenderer(valueRenderer());
        // a long text, e.g. the Lua script, does not fit a cell: a click shows all of it, Then beside Now
        diffTable.addMouseListener(new java.awt.event.MouseAdapter() {
            @Override
            public void mouseClicked(java.awt.event.MouseEvent e) {
                int row = diffTable.rowAtPoint(e.getPoint());
                if (row >= 0 && SwingUtilities.isLeftMouseButton(e)) {
                    showFullValue(row);
                }
            }
        });
        diffTable.addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
            @Override
            public void mouseMoved(java.awt.event.MouseEvent e) {
                int row = diffTable.rowAtPoint(e.getPoint());
                boolean opens = row >= 0 && hasLongValue(diffModel.rows.get(row));
                diffTable.setCursor(opens ? Cursor.getPredefinedCursor(Cursor.HAND_CURSOR) : Cursor.getDefaultCursor());
            }
        });
        titleLabel.setFont(titleLabel.getFont().deriveFont(Font.BOLD, titleLabel.getFont().getSize2D() + 2));
        descriptionLabel.setForeground(TuningStyle.muted());

        restoreButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        restoreButton.setToolTipText("Review and choose which changes to restore without burning");
        restoreButton.addActionListener(e -> restoreSelected());
        saveAsButton.setToolTipText("Save a copy of this restore point as a tune file");
        saveAsButton.addActionListener(e -> saveSelectedAs());
        deleteButton.setToolTipText("Delete this restore point");
        deleteButton.addActionListener(e -> deleteSelected());
        JPanel rightButtons = new JPanel(new FlowLayout(FlowLayout.RIGHT, 8, 0));
        rightButtons.add(deleteButton);
        rightButtons.add(saveAsButton);
        rightButtons.add(restoreButton);
        JLabel restoreHint = muted(new JLabel("Restoring works like an edit: review, Undo or Burn."));
        restoreHint.setToolTipText("The values become unburned changes; nothing is burned until you Burn");
        JPanel buttons = new JPanel(new BorderLayout(8, 0));
        buttons.add(restoreHint, BorderLayout.CENTER);
        buttons.add(rightButtons, BorderLayout.EAST);

        JPanel heading = new JPanel(new GridLayout(0, 1, 0, 2));
        heading.add(titleLabel);
        heading.add(descriptionLabel);
        JPanel detail = new JPanel(new BorderLayout(0, 12));
        detail.setBorder(BorderFactory.createEmptyBorder(16, 18, 14, 18));
        detail.add(heading, BorderLayout.NORTH);
        detail.add(new JScrollPane(diffTable), BorderLayout.CENTER);
        detail.add(buttons, BorderLayout.SOUTH);
        detail.setPreferredSize(new Dimension(560, 420));
        detailPanel.add(detail, "detail");
        detailPanel.add(new JPanel(), "none");

        JSplitPane split = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, listScroll, detailPanel);
        split.setResizeWeight(0.42);
        split.setBorder(BorderFactory.createEmptyBorder());
        body.add(split, "points");
        body.add(emptyPanel, "empty");
        JPanel north = new JPanel(new BorderLayout());
        north.add(header, BorderLayout.NORTH);
        north.add(bannerHolder, BorderLayout.SOUTH);
        JPanel content = new JPanel(new BorderLayout());
        content.add(north, BorderLayout.NORTH);
        content.add(body, BorderLayout.CENTER);
        content.add(footer, BorderLayout.SOUTH);
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

    /** The open tune's car folder; null with no ECU. */
    private Path carFolder() {
        String signature = signature();
        return signature == null ? null : store.folderFor(signature, vehicleNameOf(openTune.get(), ini.get()));
    }

    /** The Vehicle Name in a tune image, or empty. */
    static String vehicleNameOf(ConfigurationImage image, IniFileModel model) {
        if (image == null || model == null) {
            return "";
        }
        Optional<IniField> field = model.findIniField(RestorePointStore.VEHICLE_NAME);
        if (!field.isPresent() || !(field.get() instanceof StringIniField)) {
            return "";
        }
        StringIniField text = (StringIniField) field.get();
        if (text.getOffset() < 0 || text.getOffset() + text.getSize() > image.getSize()) {
            return "";
        }
        byte[] bytes = new byte[text.getSize()];
        image.getByteBuffer(text.getOffset(), text.getSize()).get(bytes);
        int end = 0;
        while (end < bytes.length && bytes[end] != 0) {
            end++;
        }
        return new String(bytes, 0, end, StandardCharsets.US_ASCII).trim();
    }

    /** Lists the points again and works out the details not yet known (all of them when the tune changed). */
    void reload() {
        RestorePointStore.RestorePoint selected = selected();
        ConfigurationImage tune = openTune.get();
        Path folder = carFolder();
        Path shown = fillCars(folder);
        List<RestorePointStore.RestorePoint> points;
        try {
            points = shown == null ? Collections.emptyList() : RestorePointStore.list(shown);
        } catch (IOException e) {
            log.error("Failed to list restore points", e);
            points = Collections.emptyList();
        }
        if (tune != comparedWith) {
            details.clear();
            comparedWith = tune;
        }
        String signature = signature();
        boolean unnamed = signature != null && RestorePointStore.isUnnamed(vehicleNameOf(tune, ini.get()));
        carTitle.setText(signature == null ? "No ECU" : carName(shown, folder, store.folderFor(signature)));
        carSubtitle.setText(signature == null ? "Connect to an ECU to see its restore points"
            : subtitle(store.folderFor(signature).getFileName().toString(), !shown.equals(folder),
            unnamed && shown.equals(folder), points.size()));
        dialog.setTitle("Restore Points — " + carTitle.getText());
        unnamedBanner.getParent().setVisible(unnamed && shown.equals(folder));
        folderLabel.setText("Kept in " + store.getRoot() + (store.isDefaultRoot() ? " (default)" : ""));
        folderLabel.setToolTipText(store.getRoot().toString());
        pointsModel.setPoints(points);
        bodyCards.show(body, points.isEmpty() ? "empty" : "points");
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

    /**
     * Refills {@link #carBox} with the board's cars and the open one, keeps the car picked earlier while it is still
     * there, and returns the folder to list; null with no ECU.
     */
    private Path fillCars(Path thisCar) {
        String signature = signature();
        List<CarChoice> choices = new ArrayList<>();
        if (signature != null) {
            Path board = store.folderFor(signature);
            List<Path> folders;
            try {
                folders = new ArrayList<>(store.carFolders(signature));
            } catch (IOException e) {
                log.error("Failed to list the cars' restore points", e);
                folders = new ArrayList<>();
            }
            if (!folders.contains(thisCar)) {
                folders.add(0, thisCar);
            }
            for (Path folder : folders) {
                // only this board's cars are listed: its name would add nothing
                String name = folder.equals(board) ? NO_VEHICLE_NAME
                    : folder.getFileName().toString();
                choices.add(new CarChoice(folder, folder.equals(thisCar) ? name + " (this car)" : name));
            }
        }
        if (chosenCar != null && choices.stream().noneMatch(choice -> choice.folder.equals(chosenCar))) {
            // e.g. its last point was deleted
            chosenCar = null;
        }
        Path shown = chosenCar != null ? chosenCar : thisCar;
        fillingCars = true;
        try {
            carBox.setModel(new DefaultComboBoxModel<>(choices.toArray(new CarChoice[0])));
            for (CarChoice choice : choices) {
                if (choice.folder.equals(shown)) {
                    carBox.setSelectedItem(choice);
                }
            }
        } finally {
            fillingCars = false;
        }
        // nothing to pick with one car: no greyed-out box
        carBox.setVisible(choices.size() > 1);
        carLabel.setVisible(choices.size() > 1);
        return shown;
    }

    /**
     * The header's name of the car shown: its Vehicle Name; "This car" for the open car without one (its folder is
     * named by serial, or is the board folder); for another car without one, its serial or "No vehicle name".
     */
    static String carName(Path shown, Path thisCar, Path board) {
        if (shown.equals(thisCar) && (shown.equals(board) || shown.getFileName().toString().startsWith("ecu-"))) {
            return "This car";
        }
        if (shown.equals(board)) {
            return NO_VEHICLE_NAME;
        }
        String name = shown.getFileName().toString();
        return name.startsWith("ecu-") ? "ECU " + name.substring(4) : name;
    }

    /** E.g. "uaefi · 5 restore points · the newest 50 are kept". */
    static String subtitle(String board, boolean otherCar, boolean unnamed, int count) {
        StringBuilder text = new StringBuilder(board);
        if (otherCar) {
            text.append(" · not the car open now");
        } else if (unnamed) {
            text.append(" · no Vehicle Name set");
        }
        if (count == 0) {
            text.append(" · no restore points yet");
        } else {
            text.append(" · ").append(count).append(count == 1 ? " restore point" : " restore points")
                .append(" · the newest ").append(RestorePointStore.DEFAULT_KEEP).append(" are kept");
        }
        return text.toString();
    }

    /** The car picked in {@link #carBox} when it is not the one open now, else null. */
    private CarChoice otherCar() {
        CarChoice choice = (CarChoice) carBox.getSelectedItem();
        return chosenCar == null || choice == null ? null : choice;
    }

    private boolean askToRestoreOtherCar(String car) {
        return JOptionPane.showConfirmDialog(dialog, "This restore point is from " + car + ", not the car open now.\n"
                + "Put its values in place as unburned changes anyway?", "Restore from another car",
            JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE) == JOptionPane.YES_OPTION;
    }

    /**
     * Gives the no-Vehicle-Name note a button that opens the page with that setting. Once the name is typed, the note
     * goes on the next reload: the window compares with the tune being edited, unburned changes included.
     */
    public void setOpenVehicleName(java.util.function.BooleanSupplier open) {
        openVehicleName = open;
        vehicleNameButton.setVisible(open != null);
    }

    void setOtherCarQuestionForTests(Predicate<String> question) {
        otherCarQuestion = question;
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
        detailCards.show(detailPanel, hasPoint ? "detail" : "none");
        deleteButton.setEnabled(hasPoint);
        saveAsButton.setEnabled(hasPoint);
        restoreButton.setEnabled(known != null && known.diff != null);
        if (!hasPoint) {
            titleLabel.setText(" ");
            descriptionLabel.setText(" ");
            diffModel.setRows(Collections.emptyList());
            return;
        }
        titleLabel.setText(point.reason.label + " · " + WHEN.format(point.when));
        diffModel.setRows(known == null || known.diff == null ? Collections.emptyList() : known.diff.rows);
        CarChoice other = otherCar();
        descriptionLabel.setText((other == null ? "" : "From " + other.label + ", not this car. ")
            + describe(point, known));
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
        return what + (changed == 0 ? " No differences on the main tune page."
            : String.format(Locale.ROOT, " %,d value%s differ%s on the main tune page.", changed,
                changed == 1 ? "" : "s", changed == 1 ? "s" : ""));
    }

    void restoreSelected() {
        RestorePointStore.RestorePoint point = selected();
        Details known = point == null ? null : details.get(point.file);
        if (known == null || known.diff == null) {
            return;
        }
        CarChoice other = otherCar();
        if (other != null && !otherCarQuestion.test(other.label)) {
            return;
        }
        if (restore.test(point)) {
            dialog.dispose();
        } else {
            JOptionPane.showMessageDialog(dialog, "Open a tune and wait for any ongoing tune operation to finish.", "Restore Points",
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
        CarChoice shown = (CarChoice) carBox.getSelectedItem();
        Path folder = shown == null ? null : shown.folder;
        if (folder == null) {
            return;
        }
        try {
            Files.createDirectories(folder);
            Desktop.getDesktop().open(folder.toFile());
        } catch (IOException | RuntimeException e) {
            log.error("Failed to open restore points folder", e);
        }
    }

    private void showFolderMenu() {
        JPopupMenu menu = new JPopupMenu();
        JMenuItem choose = new JMenuItem("Choose folder…");
        choose.addActionListener(e -> {
            Path folder = folderChooser.get();
            if (folder != null) {
                changeFolder(folder);
            }
        });
        JMenuItem useDefault = new JMenuItem("Use default folder (" + store.getDefaultRoot() + ")");
        useDefault.setEnabled(!store.isDefaultRoot());
        useDefault.addActionListener(e -> changeFolder(null));
        menu.add(choose);
        menu.add(useDefault);
        menu.show(changeFolderButton, 0, changeFolderButton.getHeight());
    }

    private Path chooseFolder() {
        JFileChooser chooser = new JFileChooser(store.getRoot().toFile());
        chooser.setDialogTitle("Folder for restore points");
        chooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
        chooser.setAcceptAllFileFilterUsed(false);
        return chooser.showDialog(dialog, "Use this folder") == JFileChooser.APPROVE_OPTION
            ? chooser.getSelectedFile().toPath() : null;
    }

    private int askToMove(int count) {
        return JOptionPane.showConfirmDialog(dialog, String.format(Locale.ROOT,
                "Move the %d restore point%s kept so far to the new folder?%n"
                    + "Otherwise they stay in %s and are not listed.", count, count == 1 ? "" : "s",
                store.getRoot()),
            "Change folder", JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE);
    }

    /**
     * Keeps the points in {@code folder} from now on (null: the default), first offering to move the ones kept so
     * far; points already in the new folder (synced from another PC) stay as they are.
     */
    void changeFolder(Path folder) {
        Path from = store.getRoot();
        Path to = folder == null ? store.getDefaultRoot() : folder.toAbsolutePath().normalize();
        if (to.equals(from)) {
            return;
        }
        int answer;
        try {
            int count = RestorePointStore.countMovable(from, to);
            answer = count == 0 ? JOptionPane.NO_OPTION : moveQuestion.applyAsInt(count);
            if (answer != JOptionPane.YES_OPTION && answer != JOptionPane.NO_OPTION) {
                return;
            }
            store.setRoot(folder);
        } catch (IOException | RuntimeException e) {
            log.error("Failed to change the restore points folder", e);
            JOptionPane.showMessageDialog(dialog, "Failed to use " + to + ": " + e.getMessage(), "Change folder",
                JOptionPane.ERROR_MESSAGE);
            return;
        }
        if (answer != JOptionPane.YES_OPTION) {
            return;
        }
        // stop reading the old points' details: on Windows a file open there cannot be moved
        if (worker != null) {
            worker.cancel(true);
            worker = null;
        }
        while (true) {
            RestorePointStore.MoveResult result;
            try {
                result = RestorePointStore.movePoints(from, to);
            } catch (IOException e) {
                // the old folder could not even be listed
                log.error("Failed to move restore points from " + from + " to " + to, e);
                result = null;
            }
            if (result != null) {
                log.info("Moved " + result.moved + " restore points from " + from + " to " + to);
                if (result.notMoved.isEmpty()) {
                    break;
                }
                log.error("Not moved: " + result.notMoved, result.firstFailure);
            }
            int left = result == null ? 0 : result.notMoved.size();
            if (!retryQuestion.test(notMovedMessage(left, from))) {
                break;
            }
        }
        // the move itself does not tell the store's listeners
        reload();
    }

    /** Why some points stayed behind and what to do, in plain words; the error itself goes to the log. */
    static String notMovedMessage(int left, Path from) {
        String what = left == 0 ? "The restore points could not be moved; another program may have one open."
            : left == 1 ? "1 restore point could not be moved; another program may have it open."
            : left + " restore points could not be moved; another program may have them open.";
        return what + "\n" + (left == 1 ? "It stays" : "They stay") + " in " + from
            + " and " + (left == 1 ? "is" : "are") + " not listed.\nClose that program and try again.";
    }

    private boolean askToRetryMove(String message) {
        Object[] options = {"Try again", "Leave it"};
        return JOptionPane.showOptionDialog(dialog, message, "Change folder", JOptionPane.YES_NO_OPTION,
            JOptionPane.WARNING_MESSAGE, null, options, options[0]) == 0;
    }

    void setRetryQuestionForTests(Predicate<String> question) {
        retryQuestion = question;
    }

    /** Waits for the details being worked out in the background, for tests. */
    void awaitDetailsForTests() throws Exception {
        if (worker != null) {
            worker.get();
        }
    }

    void setFolderDialogsForTests(Supplier<Path> chooser, java.util.function.IntUnaryOperator moveAnswer) {
        folderChooser = chooser;
        moveQuestion = moveAnswer;
    }

    /** {@code share} of {@code a} over {@code b}: a tint that follows the light or dark theme. */
    private static Color mix(Color a, Color b, double share) {
        // derived from the two theme colours, not written into the code
        return new Color(blend(a.getRed(), b.getRed(), share), blend(a.getGreen(), b.getGreen(), share),
            blend(a.getBlue(), b.getBlue(), share));
    }

    private static int blend(int a, int b, double share) {
        return (int) Math.round(a * share + b * (1 - share));
    }

    /** The icon drawn at {@code size} px, e.g. the option pane's 32 px info icon in a one-line note. */
    private static Icon scaled(Icon icon, int size) {
        return new Icon() {
            @Override
            public void paintIcon(Component c, Graphics g, int x, int y) {
                Graphics2D g2 = (Graphics2D) g.create();
                try {
                    g2.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BILINEAR);
                    g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                    g2.translate(x, y);
                    g2.scale(size / (double) icon.getIconWidth(), size / (double) icon.getIconHeight());
                    icon.paintIcon(c, g2, 0, 0);
                } finally {
                    g2.dispose();
                }
            }

            @Override
            public int getIconWidth() {
                return size;
            }

            @Override
            public int getIconHeight() {
                return size;
            }
        };
    }

    private static JPanel centredVertically(JComponent component) {
        JPanel holder = new JPanel(new GridBagLayout());
        holder.setOpaque(false);
        holder.add(component);
        return holder;
    }

    private static JLabel muted(JLabel label) {
        label.setForeground(TuningStyle.muted());
        return label;
    }

    /** Longer than a cell shows, or on several lines: opens in {@link #showFullValue}. */
    static boolean isLongValue(String value) {
        return value != null && (value.length() > LONG_VALUE || value.indexOf('\n') >= 0 || value.indexOf('\r') >= 0);
    }

    private static final int LONG_VALUE = 40;

    static boolean hasLongValue(RestorePointDiff.Row row) {
        return isLongValue(row.then) || isLongValue(row.now);
    }

    /** What a cell shows of a long value: a link to all of it, not a cut copy that two columns make unreadable. */
    static String cellText(String value) {
        if (!isLongValue(value)) {
            return value;
        }
        int lines = lines(value).size();
        return lines == 1 ? "Show text…" : "Show text (" + lines + " lines)";
    }

    private static List<String> lines(String text) {
        List<String> lines = new ArrayList<>(java.util.Arrays.asList((text == null ? "" : text).split("\\R", -1)));
        // a final line break makes no line of its own
        if (lines.size() > 1 && lines.get(lines.size() - 1).isEmpty()) {
            lines.remove(lines.size() - 1);
        }
        return lines;
    }

    /**
     * The lines of each text that the other lacks, by line number from 0: [0] for {@code then}, [1] for {@code now}.
     * A longest-common-subsequence match, so one inserted line marks just that line; past {@value #DIFF_LINES} lines
     * each it compares line by line instead.
     */
    static List<java.util.Set<Integer>> changedLines(String then, String now) {
        List<String> a = lines(then);
        List<String> b = lines(now);
        java.util.Set<Integer> onlyA = new java.util.TreeSet<>();
        java.util.Set<Integer> onlyB = new java.util.TreeSet<>();
        if (a.size() > DIFF_LINES || b.size() > DIFF_LINES) {
            for (int i = 0; i < Math.max(a.size(), b.size()); i++) {
                String x = i < a.size() ? a.get(i) : null;
                String y = i < b.size() ? b.get(i) : null;
                if (x == null || !x.equals(y)) {
                    if (x != null) {
                        onlyA.add(i);
                    }
                    if (y != null) {
                        onlyB.add(i);
                    }
                }
            }
            return java.util.Arrays.asList(onlyA, onlyB);
        }
        int[][] common = new int[a.size() + 1][b.size() + 1];
        for (int i = a.size() - 1; i >= 0; i--) {
            for (int j = b.size() - 1; j >= 0; j--) {
                common[i][j] = a.get(i).equals(b.get(j)) ? common[i + 1][j + 1] + 1
                    : Math.max(common[i + 1][j], common[i][j + 1]);
            }
        }
        int i = 0;
        int j = 0;
        while (i < a.size() && j < b.size()) {
            if (a.get(i).equals(b.get(j))) {
                i++;
                j++;
            } else if (common[i + 1][j] >= common[i][j + 1]) {
                onlyA.add(i++);
            } else {
                onlyB.add(j++);
            }
        }
        while (i < a.size()) {
            onlyA.add(i++);
        }
        while (j < b.size()) {
            onlyB.add(j++);
        }
        return java.util.Arrays.asList(onlyA, onlyB);
    }

    private static final int DIFF_LINES = 3000;

    /** Then and Now of a row with a long value, whole, side by side; nothing for short ones. */
    void showFullValue(int row) {
        if (row < 0 || row >= diffModel.rows.size() || !hasLongValue(diffModel.rows.get(row))) {
            return;
        }
        RestorePointDiff.Row r = diffModel.rows.get(row);
        JDialog viewer = new JDialog(dialog, r.label + " — Then and Now", Dialog.ModalityType.MODELESS);
        viewer.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
        List<java.util.Set<Integer>> changed = changedLines(r.then, r.now);
        JPanel sides = new JPanel(new GridLayout(1, 2, 12, 0));
        sides.add(textSide("Then: in the restore point", r.then, changed.get(0)));
        sides.add(textSide("Now: in the tune open now", r.now, changed.get(1)));
        JButton close = new JButton("Close");
        close.addActionListener(e -> viewer.dispose());
        int differing = Math.max(changed.get(0).size(), changed.get(1).size());
        JLabel summary = muted(new JLabel(differing == 0 ? "The lines are the same; only spacing or line ends differ."
            : (differing == 1 ? "1 line differs" : differing + " lines differ") + ", highlighted. Select text to copy it."));
        JPanel south = new JPanel(new BorderLayout(12, 0));
        south.add(summary, BorderLayout.CENTER);
        south.add(close, BorderLayout.EAST);
        lastViewerSummary = summary.getText();
        JPanel content = new JPanel(new BorderLayout(0, 12));
        content.setBorder(BorderFactory.createEmptyBorder(14, 16, 14, 16));
        content.add(sides, BorderLayout.CENTER);
        content.add(south, BorderLayout.SOUTH);
        viewer.setContentPane(content);
        viewer.getRootPane().registerKeyboardAction(e -> viewer.dispose(),
            KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_ESCAPE, 0), JComponent.WHEN_IN_FOCUSED_WINDOW);
        viewer.getRootPane().setDefaultButton(close);
        viewer.setSize(960, 560);
        viewer.setLocationRelativeTo(dialog);
        lastViewer = viewer;
        viewer.setVisible(true);
    }

    /** The last {@link #showFullValue} window and its line count, for tests. */
    JDialog lastViewer;
    String lastViewerSummary;

    /** A titled, read-only, monospaced text that can be selected and copied, its {@code changed} lines tinted. */
    private static JPanel textSide(String title, String text, java.util.Set<Integer> changed) {
        JTextArea area = new JTextArea(String.join("\n", lines(text)));
        area.setEditable(false);
        Color background = UIManager.getColor("TextArea.background");
        if (background != null) {
            // read-only, yet as readable as an editor
            area.setBackground(background);
        }
        area.setFont(new Font(Font.MONOSPACED, Font.PLAIN, area.getFont().getSize()));
        area.setTabSize(4);
        Color tint = mix(TuningStyle.color("Component.accentColor", new Color(0x2675BF)), area.getBackground(), 0.18);
        javax.swing.text.Highlighter.HighlightPainter painter = new LinePainter(tint);
        for (int line : changed) {
            try {
                // to the line's end with its break: never empty, so the highlighter paints it
                area.getHighlighter().addHighlight(area.getLineStartOffset(line),
                    Math.max(area.getLineEndOffset(line), area.getLineStartOffset(line) + 1), painter);
            } catch (javax.swing.text.BadLocationException e) {
                // past the end: nothing to mark
            }
        }
        area.setCaretPosition(0);
        JLabel heading = new JLabel(title);
        heading.setFont(heading.getFont().deriveFont(Font.BOLD));
        JPanel side = new JPanel(new BorderLayout(0, 6));
        side.add(heading, BorderLayout.NORTH);
        side.add(new JScrollPane(area), BorderLayout.CENTER);
        return side;
    }

    /** Tints the whole width of the line a highlight starts on, like a diff tool. */
    private static final class LinePainter implements javax.swing.text.Highlighter.HighlightPainter {
        private final Color color;

        LinePainter(Color color) {
            this.color = color;
        }

        @Override
        public void paint(Graphics g, int start, int end, Shape bounds, javax.swing.text.JTextComponent c) {
            try {
                java.awt.geom.Rectangle2D line = c.modelToView2D(start);
                if (line != null) {
                    g.setColor(color);
                    g.fillRect(0, (int) line.getY(), c.getWidth(), (int) Math.ceil(line.getHeight()));
                }
            } catch (javax.swing.text.BadLocationException e) {
                // the text changed meanwhile: nothing to paint
            }
        }
    }

    /** Right-aligned like a number; a long text left-aligned, as a link to the whole of it. */
    private static DefaultTableCellRenderer valueRenderer() {
        return new DefaultTableCellRenderer() {
            @Override
            public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected,
                                                           boolean hasFocus, int row, int column) {
                String text = value == null ? "" : value.toString();
                boolean isLong = isLongValue(text);
                super.getTableCellRendererComponent(table, cellText(text), isSelected, false, row, column);
                setHorizontalAlignment(isLong ? SwingConstants.LEFT : SwingConstants.RIGHT);
                setToolTipText(isLong ? "Click to see the whole text, Then beside Now" : null);
                if (isLong && !isSelected) {
                    Color link = UIManager.getColor("Component.linkColor");
                    setForeground(link != null ? link : new Color(0x1F5FA8));
                } else if (!isSelected) {
                    setForeground(table.getForeground());
                }
                return this;
            }
        };
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
