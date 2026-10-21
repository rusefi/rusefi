package com.rusefi.ui.logview;

import com.devexperts.logging.FileLogger;
import com.rusefi.logview.CsvLogReader;
import com.rusefi.logview.LogFiles;
import com.rusefi.ui.widgets.JoinedButtons;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.KeyStroke;
import javax.swing.SwingWorker;
import javax.swing.TransferHandler;
import javax.swing.UIManager;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.text.JTextComponent;
import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.datatransfer.DataFlavor;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.function.Consumer;

/**
 * Hosts the log viewer and opens logs: the Open button, Ctrl+O, or dropping a file anywhere on the
 * viewer. Files load in the background; on failure the previous log stays. Chart settings (plot colours,
 * grid, cursor) carry over from one log to the next.
 */
public final class LogViewerHost extends JPanel {
    /** Title of the console's main tab. */
    public static final String TITLE = "Log Viewer";
    static final String SUPPORTED = ".mlg or .msl";

    private final ChartSettings settings = new ChartSettings();
    private final TransferHandler fileDrop = new FileDropHandler();
    private Consumer<String> titleListener = name -> {
    };
    private Consumer<String> errorReporter = message ->
            JOptionPane.showMessageDialog(this, message, "Open log", JOptionPane.ERROR_MESSAGE);
    private File directory = new File(FileLogger.DIR);
    private LogViewerPanel current;
    private SwingWorker<CsvLogReader, Void> loading;

    public LogViewerHost() {
        super(new BorderLayout());
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

    public void showLog(CsvLogReader log, String fileName) {
        current = new LogViewerPanel(log, fileName, settings, this::chooseAndOpen);
        setContent(current);
        titleListener.accept(fileName);
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
                    showLog(get(), file.getName());
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
        return centered(title, hint, new JoinedButtons(openButton));
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
