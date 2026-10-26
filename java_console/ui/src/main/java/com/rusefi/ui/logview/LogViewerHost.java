package com.rusefi.ui.logview;

import com.devexperts.logging.FileLogger;
import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.logview.CsvLogReader;
import com.rusefi.logview.LogFiles;
import com.rusefi.logview.MslWriter;
import com.rusefi.sensor_logs.LoggingProfiles;
import com.rusefi.ui.widgets.JoinedButtons;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JProgressBar;
import javax.swing.KeyStroke;
import javax.swing.ListCellRenderer;
import javax.swing.SwingConstants;
import javax.swing.SwingWorker;
import javax.swing.TransferHandler;
import javax.swing.UIManager;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.text.JTextComponent;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Cursor;
import java.awt.Desktop;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.datatransfer.DataFlavor;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.IOException;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.concurrent.ExecutionException;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Supplier;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Hosts the log viewer and opens logs: the recent logs on the empty card, the toolbar's Open menu, Ctrl+O,
 * or dropping a file anywhere on the viewer. Files load in the background; on failure the previous log
 * stays. Chart settings (plot colours, grid, cursor) carry over from one log to the next.
 * Right-clicking the plots exports the visible data or drops data before or after the clicked time; dropped data
 * stays in memory for Undo until the edited log is saved, and opening another log first offers to save it.
 */
public final class LogViewerHost extends JPanel {
    private static final Logging log = getLogging(LogViewerHost.class);
    /** Title of the console's main tab. */
    public static final String TITLE = "Log Viewer";
    static final String SUPPORTED = ".mlg or .msl";

    private final ChartSettings settings = new ChartSettings();
    private final SignalLists lists;
    private final SidebarState sidebar;
    private final SignalColors colors;
    private final SignalScales scales;
    private final HistogramSettings histogramSettings;
    private final HistogramSettings scatterSettings;
    private Supplier<IniFileModel> iniSupplier = () -> null;
    private LoggingProfiles loggingProfiles;
    private Supplier<ConfigurationImage> tuneSupplier = () -> null;
    private final TransferHandler fileDrop = new FileDropHandler();
    private Consumer<String> titleListener = name -> {
    };
    private Consumer<String> errorReporter = message ->
            JOptionPane.showMessageDialog(this, message, TITLE, JOptionPane.ERROR_MESSAGE);
    private final Node config;
    private final File logsFolder;
    private File directory;
    private LogViewerPanel current;
    private File currentFile;
    /** The log the viewer shows, after any edits, and the name of the file it came from. */
    private CsvLogReader shownLog;
    private String shownName;
    /** Unsaved edits of the shown log, oldest first, for Undo. */
    private final List<Edit> edits = new ArrayList<>();
    private final EditBar editBar = new EditBar(this::saveEdited, this::undo);
    /** Picks the file to save to, starting from the suggested one; null when cancelled. Replaced in tests. */
    Function<File, File> saveChooser = this::chooseSaveFile;
    /**
     * Asks whether to save unsaved edits before they are lost: YES_OPTION saves them, NO_OPTION drops them, anything
     * else cancels. Replaced in tests.
     */
    Function<String, Integer> unsavedPrompt = question -> JOptionPane.showConfirmDialog(this, question,
            "Edited log", JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.WARNING_MESSAGE);
    private SwingWorker<CsvLogReader, Void> loading;
    private JList<RecentLogs.Entry> recentList;
    private Runnable configSaver = () -> {
    };

    /** A viewer that remembers nothing between runs, e.g. the sandbox. */
    public LogViewerHost() {
        this(new Node(), new File(FileLogger.DIR));
    }

    /** @param config where logs opened from other folders are remembered; saved with the console config */
    public LogViewerHost(Node config) {
        this(config, new File(FileLogger.DIR));
    }

    LogViewerHost(Node config, File logsFolder) {
        super(new BorderLayout());
        this.config = config;
        this.logsFolder = logsFolder;
        lists = new SignalLists(config.getChild("lists"));
        // lists are saved as they change, like the recent logs, in case the console is killed
        lists.addListener(() -> configSaver.run());
        sidebar = new SidebarState(config.getChild("sidebar"));
        colors = new SignalColors(config.getChild("colors"));
        // saved as they change, like the lists
        colors.addListener(() -> configSaver.run());
        scales = new SignalScales(config.getChild("scales"));
        scales.addListener(() -> configSaver.run());
        histogramSettings = new HistogramSettings(config.getChild("histogram"));
        scatterSettings = new HistogramSettings(config.getChild("scatter"));
        directory = logsFolder;
        setPreferredSize(new Dimension(1400, 840));
        KeyStroke open = KeyStroke.getKeyStroke(KeyEvent.VK_O, InputEvent.CTRL_DOWN_MASK);
        getInputMap(WHEN_ANCESTOR_OF_FOCUSED_COMPONENT).put(open, "openLog");
        getActionMap().put("openLog", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent event) {
                chooseAndOpen();
            }
        });
        setContent(emptyCard());
    }

    /**
     * Writes the config to disk. Called right after a log opens or a recording stops, so remembered logs and
     * durations survive the console being killed; normally the console saves only on exit.
     */
    public void setConfigSaver(Runnable configSaver) {
        this.configSaver = configSaver;
    }

    /** A recording that just stopped: its duration is known without reading the file, so it is listed at once. */
    public void recordingStopped(File file, double seconds) {
        RecentLogs.rememberDuration(durations(), file, seconds);
        RecentLogs.pruneDurations(durations(), recentEntries());
        configSaver.run();
        if (current == null && loading == null) {
            // the empty card is showing: list the new recording
            setContent(emptyCard());
        }
    }

    /**
     * The ECU's .ini (or null) at the time a log opens, which groups its signals by the .ini's gauge
     * categories; without one they are grouped by name.
     */
    public void setIniSupplier(Supplier<IniFileModel> iniSupplier) {
        this.iniSupplier = iniSupplier;
    }

    /**
     * The logging profiles, listed in the sidebar below the signal lists; a list or the plotted signals can be
     * saved as a new one. The sidebar follows changes made elsewhere, e.g. beside the record button.
     */
    public void setLoggingProfiles(LoggingProfiles profiles) {
        loggingProfiles = profiles;
        profiles.addListener(() -> {
            if (current != null) {
                current.setLoggingProfiles(profiles);
            }
        });
        if (current != null) {
            current.setLoggingProfiles(profiles);
        }
    }

    /**
     * The working tune (or null), read when the histogram bins by a tune table's axes; it is read each time, so
     * the bins follow tune edits and connecting.
     */
    public void setTuneSupplier(Supplier<ConfigurationImage> tuneSupplier) {
        this.tuneSupplier = tuneSupplier;
        if (current != null) {
            current.setTuneSupplier(tuneSupplier);
        }
    }

    /** Receives the file name of each log shown, e.g. for the window title. */
    public void setTitleListener(Consumer<String> titleListener) {
        this.titleListener = titleListener;
    }

    void setErrorReporter(Consumer<String> errorReporter) {
        this.errorReporter = errorReporter;
    }

    LogViewerPanel currentPanel() {
        return current;
    }

    /** True from {@link #open} until the loaded log (or the error) has been shown. */
    boolean isLoading() {
        return loading != null;
    }

    /** The recent-logs list of the empty card, or null while a log is shown. */
    JList<RecentLogs.Entry> recentList() {
        return recentList;
    }

    public void showLog(CsvLogReader log, String fileName) {
        edits.clear();
        shownName = fileName;
        display(log, null);
    }

    /** Shows {@code log} under {@link #shownName}, marked edited while it has unsaved edits. */
    private void display(CsvLogReader log, PlotLayout plots) {
        shownLog = log;
        String fileName = edits.isEmpty() ? shownName : shownName + " (edited)";
        current = new LogViewerPanel(log, fileName, settings, lists, sidebar, colors, currentIni(),
                this::showOpenMenu, plots);
        current.chart().setEditActions(new LogChart.EditActions() {
            @Override
            public void exportVisible() {
                LogViewerHost.this.exportVisible();
            }

            @Override
            public void dropBefore(int sample) {
                drop(sample, log.sampleCount() - 1, "Dropped " + range(log, 0, sample - 1));
            }

            @Override
            public void dropAfter(int sample) {
                drop(0, sample, "Dropped " + range(log, sample + 1, log.sampleCount() - 1));
            }
        });
        current.chart().setSignalScales(scales);
        current.setHistogramSettings(histogramSettings);
        current.setScatterSettings(scatterSettings);
        current.setTuneSupplier(tuneSupplier);
        current.setLoggingProfiles(loggingProfiles);
        // the next log opens in the view the last one was left in
        String view = histogramSettings.view();
        if (HistogramSettings.HISTOGRAM.equals(view) || HistogramSettings.SCATTER.equals(view)) {
            current.setView(view);
        }
        setContent(current);
        if (!edits.isEmpty()) {
            editBar.show(edits.get(edits.size() - 1).description, edits.size());
        }
        titleListener.accept(fileName);
    }

    /** The shown log before an edit, and what the edit did. */
    private static final class Edit {
        final CsvLogReader before;
        final String description;

        Edit(CsvLogReader before, String description) {
            this.before = before;
            this.description = description;
        }
    }

    /** "0.00–12.34 s (1235 samples)": the time of samples {@code from} to {@code to}, both included. */
    private static String range(CsvLogReader log, int from, int to) {
        return String.format(Locale.ROOT, "%.2f–%.2f s (%d samples)", log.value(from, 0), log.value(to, 0),
                to - from + 1);
    }

    EditBar editBar() {
        return editBar;
    }

    /** Keeps samples {@code from} to {@code to} of the shown log, in the same plots. */
    private void drop(int from, int to, String description) {
        CsvLogReader trimmed;
        try {
            trimmed = shownLog.slice(from, to);
        } catch (IOException e) {
            errorReporter.accept("Could not drop the data:\n" + e.getMessage());
            return;
        }
        edits.add(new Edit(shownLog, description));
        display(trimmed, current.chart().plotLayout().copy());
    }

    /** Brings back the log as it was before the last edit. */
    void undo() {
        if (edits.isEmpty()) {
            return;
        }
        CsvLogReader before = edits.remove(edits.size() - 1).before;
        display(before, current.chart().plotLayout().copy());
    }

    /** Saves the samples of the visible time to a new file. */
    private void exportVisible() {
        TimeWindow window = current.chart().timeWindow();
        int from = shownLog.firstSampleAtOrAfter(window.start());
        int to = shownLog.firstSampleAtOrAfter(Math.nextUp(window.end())) - 1;
        if (to < from) {
            errorReporter.accept("There are no samples in the visible time.");
            return;
        }
        File file = saveChooser.apply(suggestedFile(String.format(Locale.ROOT, "_%.1f-%.1fs",
                shownLog.value(from, 0), shownLog.value(to, 0))));
        if (file != null) {
            write(from, to, "Exported from " + shownName + ": " + range(shownLog, from, to), file);
        }
    }

    /**
     * Saves the edited log to a new file, which is then the shown log; false when cancelled or failed, so the
     * edits stay.
     */
    boolean saveEdited() {
        File file = saveChooser.apply(suggestedFile("_edited"));
        if (file == null || !write(0, shownLog.sampleCount() - 1, "Edited from " + shownName, file)) {
            return false;
        }
        edits.clear();
        shownName = file.getName();
        currentFile = file.getAbsoluteFile();
        directory = currentFile.getParentFile();
        RecentLogs.remember(config, file);
        RecentLogs.rememberDuration(durations(), file, shownLog.maximum(0) - shownLog.minimum(0));
        configSaver.run();
        display(shownLog, current.chart().plotLayout().copy());
        return true;
    }

    /** True when the shown log has edits that are not saved. */
    boolean hasUnsavedEdits() {
        return !edits.isEmpty();
    }

    /** Before the edited log is replaced: offers to save it. False when the user cancels or saving fails. */
    private boolean confirmDiscardEdits() {
        if (edits.isEmpty()) {
            return true;
        }
        int answer = unsavedPrompt.apply("The shown log has edits that are not saved.\n"
                + "Save them before opening another log?");
        if (answer == JOptionPane.YES_OPTION) {
            return saveEdited();
        }
        return answer == JOptionPane.NO_OPTION;
    }

    /** A .msl next to the shown log, named after it plus {@code suffix}. */
    private File suggestedFile(String suffix) {
        String base = shownName.replaceFirst("\\.[^.]*$", "");
        File folder = currentFile != null ? currentFile.getParentFile() : directory;
        return new File(folder, base + suffix + ".msl");
    }

    private boolean write(int from, int to, String title, File file) {
        Cursor previous = getCursor();
        setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        try {
            MslWriter.write(shownLog, from, to, title, file);
            return true;
        } catch (IOException e) {
            errorReporter.accept("Could not save " + file.getName() + ":\n" + e.getMessage());
            return false;
        } finally {
            setCursor(previous);
        }
    }

    private File chooseSaveFile(File suggested) {
        JFileChooser chooser = new JFileChooser(suggested.getParentFile());
        chooser.setDialogTitle("Save data log");
        chooser.setFileFilter(new FileNameExtensionFilter("Text data logs (.msl)", "msl"));
        chooser.setSelectedFile(suggested);
        if (chooser.showSaveDialog(this) != JFileChooser.APPROVE_OPTION) {
            return null;
        }
        File file = chooser.getSelectedFile();
        if (!file.getName().toLowerCase(Locale.ROOT).endsWith(".msl")) {
            file = new File(file.getPath() + ".msl");
        }
        if (file.exists() && JOptionPane.showConfirmDialog(this, file.getName() + " already exists. Replace it?",
                "Save data log", JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE) != JOptionPane.YES_OPTION) {
            return null;
        }
        return file;
    }

    private IniFileModel currentIni() {
        try {
            return iniSupplier.get();
        } catch (RuntimeException e) {
            // the .ini only refines the grouping: open the log anyway
            log.warn("No .ini for the log viewer: " + e);
            return null;
        }
    }

    public void chooseAndOpen() {
        JFileChooser chooser = new JFileChooser(directory.isDirectory() ? directory : null);
        chooser.setDialogTitle("Open data log");
        chooser.setFileFilter(new FileNameExtensionFilter("Data logs (" + SUPPORTED + ")", LogFiles.EXTENSIONS));
        if (chooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            open(chooser.getSelectedFile());
        }
    }

    public void open(File file) {
        if (!confirmDiscardEdits()) {
            return;
        }
        directory = file.getAbsoluteFile().getParentFile();
        if (loading != null) {
            loading.cancel(true);
        }
        LogViewerPanel previous = current;
        setContent(loadingCard(file.getName()));
        loading = new SwingWorker<CsvLogReader, Void>() {
            @Override
            protected CsvLogReader doInBackground() throws IOException {
                return LogFiles.read(file);
            }

            @Override
            protected void done() {
                if (loading != this || isCancelled()) {
                    return;
                }
                loading = null;
                try {
                    CsvLogReader log = get();
                    showLog(log, file.getName());
                    currentFile = file.getAbsoluteFile();
                    RecentLogs.remember(config, file);
                    // the list shows the duration of every log opened once, without reading the file again
                    RecentLogs.rememberDuration(durations(), file, log.maximum(0) - log.minimum(0));
                    RecentLogs.pruneDurations(durations(), recentEntries());
                    configSaver.run();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                } catch (ExecutionException e) {
                    restore(previous);
                    Throwable cause = e.getCause() != null ? e.getCause() : e;
                    errorReporter.accept("Could not open " + file.getName() + ":\n" + cause.getMessage());
                }
            }
        };
        loading.execute();
    }

    private void restore(LogViewerPanel previous) {
        current = previous;
        setContent(previous != null ? previous : emptyCard());
    }

    private void setContent(JComponent content) {
        removeAll();
        add(content, BorderLayout.CENTER);
        add(editBar, BorderLayout.NORTH);
        editBar.setVisible(!edits.isEmpty() && content == current);
        acceptDrops(content);
        revalidate();
        repaint();
    }

    /**
     * Drops land on the deepest component, so every non-text component accepts log files. Components that
     * already drag signals (chart, signal list) keep their handler, which passes log files on to this one.
     */
    private void acceptDrops(Component component) {
        if (component instanceof JComponent && !(component instanceof JTextComponent)) {
            JComponent target = (JComponent) component;
            if (target.getTransferHandler() instanceof SignalTransfer.Handler) {
                ((SignalTransfer.Handler) target.getTransferHandler()).setFileDrop(fileDrop);
            } else {
                target.setTransferHandler(fileDrop);
            }
        }
        if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                acceptDrops(child);
            }
        }
    }

    TransferHandler fileDropHandler() {
        return fileDrop;
    }

    private JComponent emptyCard() {
        JButton openButton = openLogButton("Open log…");
        openButton.addActionListener(e -> chooseAndOpen());
        JLabel title = new JLabel("No data log open");
        title.setFont(title.getFont().deriveFont(Font.BOLD, title.getFont().getSize2D() + 3));
        JLabel hint = ChartToolbar.muted(new JLabel("Drop an " + SUPPORTED + " file here, or"));
        return centered(title, hint, new JoinedButtons(openButton), recentLogs());
    }

    List<RecentLogs.Entry> recentEntries() {
        return RecentLogs.find(logsFolder, RecentLogs.remembered(config), durations());
    }

    private Node durations() {
        return config.getChild("durations");
    }

    /** The newest logs, newest first; a click opens one. */
    private JComponent recentLogs() {
        List<RecentLogs.Entry> entries = recentEntries();
        JLabel heading = new JLabel("RECENT LOGS");
        heading.setFont(heading.getFont().deriveFont(Font.BOLD, heading.getFont().getSize2D() - 1));
        heading.setForeground(ThemedStrip.themed("Console.LogViewer.mutedForeground", Color.GRAY));
        JPanel section = new JPanel(new BorderLayout(0, 6));
        section.setOpaque(false);
        section.setBorder(BorderFactory.createEmptyBorder(14, 0, 0, 0));
        section.add(heading, BorderLayout.NORTH);
        if (entries.isEmpty()) {
            recentList = null;
            section.add(ChartToolbar.muted(new JLabel("Logs you record with Binary Logging > Start appear here")),
                    BorderLayout.CENTER);
            return section;
        }
        DefaultListModel<RecentLogs.Entry> model = new DefaultListModel<>();
        for (RecentLogs.Entry entry : entries) {
            model.addElement(entry);
        }
        JList<RecentLogs.Entry> list = new JList<>(model);
        list.setCellRenderer(new RecentRenderer());
        list.setFixedCellHeight(32);
        list.setFixedCellWidth(RECENT_WIDTH);
        list.setVisibleRowCount(entries.size());
        list.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
        list.setBorder(BorderFactory.createLineBorder(ThemedStrip.themed("Separator.foreground", Color.LIGHT_GRAY)));
        list.setToolTipText("");
        // the row under the pointer is highlighted and opens on click
        MouseAdapter mouse = new MouseAdapter() {
            @Override
            public void mouseMoved(MouseEvent event) {
                int index = list.locationToIndex(event.getPoint());
                if (index >= 0 && list.getCellBounds(index, index).contains(event.getPoint())) {
                    list.setSelectedIndex(index);
                } else {
                    list.clearSelection();
                }
            }

            @Override
            public void mouseExited(MouseEvent event) {
                list.clearSelection();
            }

            @Override
            public void mouseClicked(MouseEvent event) {
                int index = list.locationToIndex(event.getPoint());
                if (index >= 0 && list.getCellBounds(index, index).contains(event.getPoint())) {
                    open(model.get(index).file);
                }
            }
        };
        list.addMouseListener(mouse);
        list.addMouseMotionListener(mouse);
        list.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), "openRecent");
        list.getActionMap().put("openRecent", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent event) {
                if (list.getSelectedValue() != null) {
                    open(list.getSelectedValue().file);
                }
            }
        });
        recentList = list;
        section.add(list, BorderLayout.CENTER);
        return section;
    }

    private static final int RECENT_WIDTH = 680;

    /** Date, file name, duration (once the log has been opened) and size of a recent log. */
    static final class RecentRenderer extends JPanel implements ListCellRenderer<RecentLogs.Entry> {
        private final JLabel when = new JLabel();
        private final JLabel name = new JLabel();
        final JLabel duration = new JLabel("", SwingConstants.RIGHT);
        final JLabel size = new JLabel("", SwingConstants.RIGHT);
        private final JPanel details = new JPanel(new BorderLayout(12, 0));

        RecentRenderer() {
            super(new BorderLayout(12, 0));
            setBorder(BorderFactory.createEmptyBorder(0, 12, 0, 12));
            when.setPreferredSize(new Dimension(120, 20));
            duration.setPreferredSize(new Dimension(90, 20));
            size.setPreferredSize(new Dimension(70, 20));
            details.setOpaque(false);
            details.add(duration, BorderLayout.WEST);
            details.add(size, BorderLayout.EAST);
            add(when, BorderLayout.WEST);
            add(name, BorderLayout.CENTER);
            add(details, BorderLayout.EAST);
        }

        @Override
        public Component getListCellRendererComponent(JList<? extends RecentLogs.Entry> list, RecentLogs.Entry entry,
                                                      int index, boolean selected, boolean focused) {
            Color muted = ThemedStrip.themed("Console.LogViewer.mutedForeground", Color.GRAY);
            when.setText(RecentLogs.when(entry.recorded, LocalDate.now()));
            when.setForeground(muted);
            name.setText(entry.file.getName());
            name.setForeground(list.getForeground());
            duration.setText(RecentLogs.duration(entry.duration));
            duration.setForeground(muted);
            size.setText(RecentLogs.size(entry.size));
            size.setForeground(muted);
            setToolTipText(entry.file.getAbsolutePath());
            setBackground(selected ? ThemedStrip.themed("Console.LogViewer.chipBackground", list.getSelectionBackground())
                    : list.getBackground());
            return this;
        }

        // Like DefaultListCellRenderer: lay out when sized (validate() needs a native peer and is
        // disabled below), including the nested duration/size panel
        @Override
        public void setBounds(int x, int y, int width, int height) {
            super.setBounds(x, y, width, height);
            doLayout();
            details.doLayout();
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
    }

    /** The toolbar's Open menu: recent logs (the open one ticked), Browse… and the logs folder. */
    JPopupMenu openMenu() {
        JPopupMenu menu = new JPopupMenu();
        List<RecentLogs.Entry> entries = recentEntries();
        LocalDate today = LocalDate.now();
        for (RecentLogs.Entry entry : entries) {
            String duration = RecentLogs.duration(entry.duration);
            String details = (duration.isEmpty() ? "" : duration + ", ") + RecentLogs.size(entry.size);
            JCheckBoxMenuItem item = new JCheckBoxMenuItem(RecentLogs.when(entry.recorded, today) + "     "
                    + entry.file.getName() + "   (" + details + ")", entry.file.equals(currentFile));
            item.setToolTipText(entry.file.getAbsolutePath());
            item.addActionListener(e -> open(entry.file));
            menu.add(item);
        }
        if (entries.isEmpty()) {
            JMenuItem none = new JMenuItem("No recent logs");
            none.setEnabled(false);
            menu.add(none);
        }
        menu.addSeparator();
        JMenuItem browse = new JMenuItem("Browse…");
        browse.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, InputEvent.CTRL_DOWN_MASK));
        browse.addActionListener(e -> chooseAndOpen());
        menu.add(browse);
        JMenuItem folder = new JMenuItem("Open logs folder");
        folder.setEnabled(logsFolder.isDirectory() && Desktop.isDesktopSupported()
                && Desktop.getDesktop().isSupported(Desktop.Action.OPEN));
        folder.addActionListener(e -> {
            try {
                Desktop.getDesktop().open(logsFolder);
            } catch (IOException | RuntimeException failure) {
                errorReporter.accept("Could not open the logs folder: " + failure.getMessage());
            }
        });
        menu.add(folder);
        return menu;
    }

    private void showOpenMenu(Component button) {
        openMenu().show(button, 0, button.getHeight() + 4);
    }

    /**
     * The open-log button of the empty card and the chart toolbar, wrapped in {@link JoinedButtons} like the
     * console's other toolbar buttons (Edit dashboard, Launch Wizard): outlined, icon, toolbar height.
     */
    static JButton openLogButton(String text) {
        JButton button = new JButton(text, UIManager.getIcon("Tree.openIcon"));
        button.setIconTextGap(8);
        button.setToolTipText("Open a data log (" + SUPPORTED + ") - Ctrl+O, or drop a file on the viewer");
        return button;
    }

    private JComponent loadingCard(String fileName) {
        JProgressBar progress = new JProgressBar();
        progress.setIndeterminate(true);
        progress.setMaximumSize(new Dimension(240, progress.getPreferredSize().height));
        return centered(new JLabel("Loading " + fileName + "…"), progress);
    }

    private static JComponent centered(JComponent... rows) {
        JPanel column = new JPanel();
        column.setLayout(new BoxLayout(column, BoxLayout.Y_AXIS));
        column.setOpaque(false);
        for (JComponent row : rows) {
            row.setAlignmentX(Component.CENTER_ALIGNMENT);
            if (row instanceof JoinedButtons) {
                // BoxLayout would otherwise stretch the button group across the column
                row.setMaximumSize(row.getPreferredSize());
            }
            column.add(row);
            column.add(Box.createVerticalStrut(10));
        }
        JPanel card = new JPanel(new GridBagLayout());
        card.setBorder(BorderFactory.createEmptyBorder(24, 24, 24, 24));
        card.add(column);
        return card;
    }

    private final class FileDropHandler extends TransferHandler {
        @Override
        public boolean canImport(TransferSupport support) {
            return support.isDataFlavorSupported(DataFlavor.javaFileListFlavor);
        }

        @Override
        public boolean importData(TransferSupport support) {
            if (!canImport(support)) {
                return false;
            }
            try {
                @SuppressWarnings("unchecked")
                List<File> files = (List<File>) support.getTransferable().getTransferData(DataFlavor.javaFileListFlavor);
                for (File file : files) {
                    if (LogFiles.isSupported(file.getName())) {
                        open(file);
                        return true;
                    }
                }
                errorReporter.accept("Only " + SUPPORTED + " data logs can be opened.");
            } catch (Exception e) {
                errorReporter.accept("Could not read the dropped file: " + e.getMessage());
            }
            return false;
        }
    }
}
