package com.rusefi.ui.llm;

import com.rusefi.io.LinkManager;
import com.rusefi.mcp.ConsoleEcuSession;
import com.rusefi.core.net.FirmwareSourceCodeDownloader;
import com.rusefi.core.net.ConnectionAndMeta.DownloadProgressListener;
import com.rusefi.ui.widgets.tune.CalibrationFieldFactory;

import com.rusefi.ui.llm.ChatGptClient.Account;
import com.rusefi.ui.llm.ChatGptClient.Cancellation;
import com.rusefi.ui.llm.ChatGptClient.Model;
import org.json.simple.JSONArray;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.net.URI;
import java.nio.file.Path;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.function.Consumer;

/** ChatGPT troubleshooting through the existing Console connection. Construct and close on the EDT. */
public final class LLMTab implements AutoCloseable {
    // Never log prompts, reply text or tokens; operation names, model slugs and exceptions only.
    private static final com.devexperts.logging.Logging log = com.devexperts.logging.Logging.getLogging(LLMTab.class);
    private final LinkManager linkManager;
    private final Path storage;
    private final SourcePreparation sources;
    private final ClientFactory clients;
    private final BrowserOpener browser;
    private final FallbackDialogFactory fallbackDialogs;
    private final AuthDialogLifecycle authDialogs = new AuthDialogLifecycle();
    private Path knowledgeDirectory;
    private ConsoleEcuSession session;
    private DiagnosticCaseStore caseEvidence;
    private final CardLayout cards = new CardLayout();
    private final JPanel content = new JPanel(cards);
    private final JPanel llmControls = new JPanel(new BorderLayout(8, 8));
    private final JButton startDownload = new JButton("Start Download");
    private final JProgressBar sourceProgress = new JProgressBar(0, 100);
    private final JLabel sourceStatus = new JLabel("Checking cached source code...");
    private Future<?> sourceTask;
    private boolean preparingSources;
    private final JComboBox<Account> accounts = new JComboBox<>();
    private final JComboBox<Model> models = new JComboBox<>();
    private final JButton login = new JButton("Continue with ChatGPT");
    private final JButton addAccount = new JButton("Add account");
    private final JButton logout = new JButton("Sign out");
    private final JButton cancelSignIn = new JButton("Cancel sign-in");
    private final JButton send = new JButton("Send");
    private final JButton stop = new JButton("Stop");
    private final JButton reset = new JButton("New conversation");
    private final JTextArea terminal = new JTextArea(24, 90);
    private final JTextArea prompt = new JTextArea(3, 70);
    private final JLabel status = new JLabel("Opening account store...");
    private final JSONArray history = new JSONArray();
    private final ExecutorService worker = Executors.newSingleThreadExecutor(r -> {
        Thread thread = new Thread(r, "chatgpt-console");
        thread.setDaemon(true);
        return thread;
    });
    private ChatGptClient client; // Only accessed on the worker.
    private Cancellation running;
    private String selectedId;
    private boolean updatingAccounts;
    private boolean closed;
    private boolean ready;
    private boolean signingIn;
    private String initialLastAccount = "";

    public LLMTab(Path storage, LinkManager linkManager) {
        this(storage, linkManager, new FirmwareSourceCodeDownloader());
    }

    public LLMTab(Path storage, LinkManager linkManager, FirmwareSourceCodeDownloader downloader) {
        this(storage, linkManager, new SourcePreparation() {
            @Override public Path prepareCached(DownloadProgressListener progress) throws Exception {
                return downloader.prepareCached(progress);
            }
            @Override public Path download(DownloadProgressListener progress) throws Exception {
                return downloader.downloadFresh(progress);
            }
        });
    }

    interface SourcePreparation {
        Path prepareCached(DownloadProgressListener progress) throws Exception;
        Path download(DownloadProgressListener progress) throws Exception;
    }

    /** Test seam: source preparation can be held or failed without real network/cache access. */
    LLMTab(Path storage, LinkManager linkManager, SourcePreparation sources) {
        this(storage, linkManager, sources, ChatGptClient::new, null, null);
    }

    interface ClientFactory { ChatGptClient create(Path storage) throws Exception; }
    interface BrowserOpener { boolean open(URI uri) throws Exception; }
    interface AuthDialogHandle { void dispose(); }
    interface FallbackDialogFactory { AuthDialogHandle show(URI uri, Runnable onCancel); }

    static final class AuthDialogLifecycle {
        private AuthDialogHandle handle;
        private Cancellation owner;

        void opened(Cancellation cancellation, AuthDialogHandle dialog) {
            close();
            owner = cancellation;
            handle = dialog;
        }

        void finish(Cancellation cancellation) {
            if (owner == cancellation) { close(); }
        }

        void close() {
            AuthDialogHandle active = handle;
            handle = null;
            owner = null;
            if (active != null) { active.dispose(); }
        }

        static boolean mayBrowse(Cancellation attempt, Cancellation current, boolean tabClosed) {
            return !tabClosed && attempt != null && attempt == current && !attempt.isCancelled();
        }
    }

    /** Test seam: supplies isolated auth transport, browser and modeless fallback dialog. */
    LLMTab(Path storage, LinkManager linkManager, SourcePreparation sources,
           ClientFactory clients, BrowserOpener browser, FallbackDialogFactory fallbackDialogs) {
        this.linkManager = linkManager;
        this.storage = storage;
        this.sources = sources;
        this.clients = clients;
        this.browser = browser;
        this.fallbackDialogs = fallbackDialogs == null ? this::showFallbackDialog : fallbackDialogs;
        CalibrationFieldFactory.applyStyle(startDownload);
        content.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        JPanel sourceCard = new JPanel(new GridBagLayout());
        JPanel downloadControls = new JPanel(new GridLayout(0, 1, 0, 8));
        downloadControls.add(new JLabel("Hit the button below to download the rusEFI source code gist."));
        downloadControls.add(startDownload);
        sourceProgress.setStringPainted(true);
        downloadControls.add(sourceProgress);
        downloadControls.add(sourceStatus);
        sourceCard.add(downloadControls);
        content.add(sourceCard, "sources");
        content.add(llmControls, "chat");
        cards.show(content, "sources");
        startDownload.addActionListener(e -> prepareSources(true));
        JPanel accountBar = new JPanel(new FlowLayout(FlowLayout.LEFT));
        CalibrationFieldFactory.applyStyle(accounts);
        CalibrationFieldFactory.applyStyle(login);
        CalibrationFieldFactory.applyStyle(addAccount);
        CalibrationFieldFactory.applyStyle(logout);
        CalibrationFieldFactory.applyStyle(cancelSignIn);
        accounts.setPreferredSize(new Dimension(500, accounts.getPreferredSize().height));
        accounts.setToolTipText("Saved ChatGPT account and workspace registrations");
        accountBar.add(accounts);
        accountBar.add(login);
        accountBar.add(addAccount);
        addAccount.setVisible(false);
        accountBar.add(logout);
        accountBar.add(cancelSignIn);
        JPanel header = new JPanel(new BorderLayout());
        header.add(accountBar, BorderLayout.NORTH);
        JPanel modelBar = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JLabel modelLabel = new JLabel("Model:");
        CalibrationFieldFactory.applyStyle(modelLabel);
        modelBar.add(modelLabel);
        CalibrationFieldFactory.applyStyle(models);
        CalibrationFieldFactory.applyStyle(reset);
        models.setPreferredSize(new Dimension(250, models.getPreferredSize().height));
        modelBar.add(models);
        modelBar.add(reset);
        modelBar.add(status);
        header.add(modelBar, BorderLayout.SOUTH);
        llmControls.add(header, BorderLayout.NORTH);

        Font mono = new Font(Font.MONOSPACED, Font.PLAIN, 14);
        terminal.setFont(mono);
        terminal.setBackground(new Color(24, 27, 32));
        terminal.setForeground(new Color(220, 230, 220));
        terminal.setCaretColor(Color.WHITE);
        terminal.setEditable(false);
        terminal.setLineWrap(true);
        terminal.setWrapStyleWord(true);
        terminal.setMargin(new Insets(10, 10, 10, 10));
        terminal.getAccessibleContext().setAccessibleName("ChatGPT terminal output");
        llmControls.add(new JScrollPane(terminal), BorderLayout.CENTER);
        prompt.setFont(mono);
        CalibrationFieldFactory.applyStyle(prompt);
        prompt.setLineWrap(true);
        prompt.setWrapStyleWord(true);
        prompt.getAccessibleContext().setAccessibleName("Message to ChatGPT");
        prompt.setToolTipText("Type a message, then press Ctrl+Enter or click Send. Enter inserts a new line.");
        prompt.getInputMap(JComponent.WHEN_FOCUSED).put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, InputEvent.CTRL_DOWN_MASK), "send-prompt");
        prompt.getActionMap().put("send-prompt", new AbstractAction() {
            @Override public void actionPerformed(ActionEvent e) {
                if (send.isEnabled()) { send(); }
            }
        });
        JPanel input = new JPanel(new BorderLayout(8, 8));
        JLabel promptLabel = new JLabel("Describe your problem below and hit 'Send'");
        CalibrationFieldFactory.applyStyle(promptLabel);
        input.add(promptLabel, BorderLayout.NORTH);
        input.add(new JScrollPane(prompt), BorderLayout.CENTER);
        send.setToolTipText("Send the message (Ctrl+Enter from the message field)");
        CalibrationFieldFactory.applyStyle(send);
        CalibrationFieldFactory.applyStyle(stop);
        JPanel buttons = new JPanel(new GridLayout(2, 1, 4, 4));
        buttons.add(send);
        buttons.add(stop);
        input.add(buttons, BorderLayout.EAST);
        llmControls.add(input, BorderLayout.SOUTH);
        append("rusEFI Troubleshooting\nSign in to use your ChatGPT plan.\n"
                + "Your messages, requested ECU readings and source/wiki excerpts are sent to OpenAI. ECU access is read-only.\n"
                + "Ask to export a diagnostic case to save findings and selected evidence locally.\n"
                + "Connect to an ECU in Console, then describe the problem.\n\n");

        login.addActionListener(e -> signIn(selectedId));
        addAccount.addActionListener(e -> signIn(null));
        cancelSignIn.addActionListener(e -> {
            if (signingIn && running != null) {
                running.cancel();
                status.setText("Cancelling sign-in...");
                updateControls();
            }
        });
        logout.addActionListener(e -> {
            String id = selectedId;
            perform("Signing out...", cancellation -> {
                boolean revoked = client.signOut(id, cancellation);
                return () -> {
                    clearConversation();
                    models.removeAllItems();
                    append(revoked ? "Signed out.\n" : "Signed out locally. Remote revocation was not confirmed; disconnect this app in ChatGPT Settings.\n");
                };
            });
        });
        accounts.addActionListener(e -> {
            if (updatingAccounts) { return; }
            Account account = (Account) accounts.getSelectedItem();
            selectedId = account == null ? null : account.id;
            updateLoginLabel(account);
            updateAccountStatus(account);
            updateControls();
            clearConversation();
            models.removeAllItems();
            loadModels();
        });
        models.addActionListener(e -> { if (running == null) { clearConversation(); } });
        reset.addActionListener(e -> { clearConversation(); append("New conversation.\n"); });
        stop.addActionListener(e -> {
            if (running != null) {
                running.cancel();
                stop.setEnabled(false);
                status.setText("Stopping...");
            }
        });
        send.addActionListener(e -> send());
        updateControls();
        prepareSources(false);
    }

    private void prepareSources(boolean download) {
        if (closed || preparingSources) { return; }
        preparingSources = true;
        startDownload.setEnabled(false);
        sourceProgress.setValue(0);
        sourceProgress.setIndeterminate(true);
        sourceStatus.setText(download ? "Downloading and extracting source code..." : "Checking cached source code...");
        sourceTask = worker.submit(() -> {
            try {
                DownloadProgressListener progress = percent -> SwingUtilities.invokeLater(() -> {
                    if (closed) { return; }
                    sourceProgress.setIndeterminate(false);
                    sourceProgress.setValue(percent);
                });
                Path prepared = download ? sources.download(progress) : sources.prepareCached(progress);
                SwingUtilities.invokeLater(() -> {
                    if (closed) { return; }
                    preparingSources = false;
                    sourceProgress.setIndeterminate(false);
                    if (prepared == null) {
                        sourceStatus.setText("Source code download required.");
                        startDownload.setEnabled(true);
                        return;
                    }
                    sourceProgress.setValue(100);
                    knowledgeDirectory = prepared;
                    cards.show(content, "chat");
                    openAccountStore();
                });
            } catch (Exception e) {
                log.error("source preparation failed", e);
                SwingUtilities.invokeLater(() -> {
                    if (closed) { return; }
                    preparingSources = false;
                    sourceProgress.setIndeterminate(false);
                    sourceStatus.setText("Source preparation failed: " + safeMessage(e) + " Click Start Download to retry.");
                    startDownload.setEnabled(true);
                });
            }
        });
    }

    private void openAccountStore() {
        perform("Opening account store...", cancellation -> {
            client = clients.create(storage);
            initialLastAccount = client.lastAccount();
            return () -> append("Select a saved connection or click Continue with ChatGPT.\n");
        });
    }

    private void signIn(String id) {
        signingIn = true;
        perform("Complete sign-in in your browser...", cancellation -> {
            String signedIn = client.signIn(id, uri -> SwingUtilities.invokeLater(() -> browse(uri, cancellation)), cancellation);
            List<Model> available = loadModels(signedIn, cancellation);
            return () -> {
                selectedId = signedIn;
                clearConversation();
                setModels(available);
                append("Connected to ChatGPT.\n");
            };
        });
    }

    private void loadModels() {
        String id = selectedId;
        if (id == null) { updateControls(); return; }
        perform("Loading models...", cancellation -> {
            List<Model> available = loadModels(id, cancellation);
            return () -> setModels(available);
        });
    }

    private List<Model> loadModels(String id, Cancellation cancellation) throws Exception {
        Account account = client.accounts().stream().filter(a -> a.id.equals(id)).findFirst().orElse(null);
        if (account == null || account.reLoginRequired) { return Collections.emptyList(); }
        client.rememberLastAccount(id);
        if (!account.planEnabled) { return Collections.emptyList(); }
        try {
            List<Model> available = client.models(id, cancellation);
            if (available.isEmpty()) {
                SwingUtilities.invokeLater(() -> append("No models available for this connection. Check ChatGPT Settings > Usage.\n"));
            }
            return available;
        } catch (Exception e) {
            if (cancellation.isCancelled()) { throw e; }
            SwingUtilities.invokeLater(() -> append("Could not load models: " + safeMessage(e) + "\nReselect the account to retry.\n"));
            return Collections.emptyList();
        }
    }

    @SuppressWarnings("unchecked")
    private void send() {
        String text = prompt.getText().trim();
        Model model = (Model) models.getSelectedItem();
        if (text.isEmpty() || model == null || running != null) { return; }
        String id = selectedId;
        if (session != null && !session.isCurrent()) {
            clearConversation();
            append("Console connection changed; starting a new conversation.\n");
        }
        if (session == null) {
            try {
                session = new ConsoleEcuSession(linkManager);
                caseEvidence = new DiagnosticCaseStore(storage.resolve("diagnostic-cases"));
            } catch (java.io.IOException e) {
                append("[" + e.getMessage() + "]\n");
                return;
            }
        }
        ConsoleEcuSession current = session;
        DiagnosticCaseStore turnEvidence = new DiagnosticCaseStore(caseEvidence);
        JSONArray previous = new JSONArray();
        previous.addAll(history);
        append("\n> " + text + "\n\n");
        prompt.setText("");
        log.info("troubleshooting turn: model=" + model.slug + ", prompt=" + text.length() + " chars, "
                + history.size() + " history items");
        perform("Troubleshooting...", cancellation -> {
            ChatGptAgent.Tools tools = new TroubleshootingTools(current, knowledgeDirectory, turnEvidence);
            JSONArray completed = ChatGptAgent.run(
                    (input, definitions, delta, c) -> client.respondWithTools(id, model.slug, input, definitions, delta, c),
                    tools, previous, text, delta -> SwingUtilities.invokeLater(() -> append(delta)),
                    progress -> SwingUtilities.invokeLater(() -> append(progress)), cancellation);
            return () -> {
                if (cancellation.isCancelled() || !current.isCurrent()) {
                    append("\n[Turn discarded: stopped or Console connection changed.]\n");
                    return;
                }
                history.clear();
                history.addAll(completed);
                caseEvidence = turnEvidence;
                append("\n");
            };
        });
    }

    private interface Work { Runnable run(Cancellation cancellation) throws Exception; }

    private void perform(String message, Work work) {
        if (running != null || closed) { return; }
        Cancellation cancellation = new Cancellation();
        running = cancellation;
        status.setText(message);
        updateControls();
        log.info("operation started: " + message);
        worker.execute(() -> {
            Runnable success;
            try {
                success = work.run(cancellation);
            } catch (Exception e) {
                // The UI shows a sanitized message; keep the full diagnosis in the console log.
                log.error("operation failed (" + message + ", cancelled=" + cancellation.isCancelled() + ")", e);
                success = () -> append(cancellation.isCancelled() ? "\n[Stopped or time limit reached; partial turn was not saved.]\n"
                        : "\n[" + safeMessage(e) + "]\n");
            }
            List<Account> saved = client == null ? Collections.emptyList() : client.accounts();
            boolean initialized = client != null;
            Runnable finish = success;
            SwingUtilities.invokeLater(() -> {
                authDialogs.finish(cancellation);
                if (closed) { return; }
                finish.run();
                updatingAccounts = true;
                accounts.removeAllItems();
                for (Account account : saved) { accounts.addItem(account); }
                accounts.setSelectedIndex(-1);
                for (Account account : saved) {
                    if (account.id.equals(selectedId)) { accounts.setSelectedItem(account); }
                }
                boolean loadInitialModels = false;
                if ("Opening account store...".equals(message) && selectedId == null && !saved.isEmpty()) {
                    Account initial = saved.stream().filter(account -> account.id.equals(initialLastAccount) && isReadyAccount(account))
                            .findFirst().orElseGet(() -> saved.stream().filter(LLMTab::isReadyAccount).findFirst().orElse(saved.get(0)));
                    selectedId = initial.id;
                    accounts.setSelectedItem(initial);
                    loadInitialModels = isReadyAccount(initial);
                }
                addAccount.setVisible(!saved.isEmpty());
                updatingAccounts = false;
                running = null;
                if (signingIn) { signingIn = false; }
                ready = initialized;
                Account selected = (Account) accounts.getSelectedItem();
                status.setText(!ready ? "Account store unavailable; reopen the Console" : selected == null ? "Not connected"
                        : selected.reLoginRequired ? "Re-login required" : !selected.connected ? "Not connected"
                        : selected.planEnabled ? "ChatGPT plan usage enabled" : "Connected; plan access required");
                updateLoginLabel(selected);
                updateControls();
                if (loadInitialModels) { SwingUtilities.invokeLater(this::loadModels); }
            });
        });
    }

    private void setModels(List<Model> available) {
        models.removeAllItems();
        for (Model model : available) { models.addItem(model); }
    }

    private void updateControls() {
        boolean idle = running == null && !closed && ready;
        accounts.setEnabled(idle);
        addAccount.setEnabled(idle);
        addAccount.setVisible(accounts.getItemCount() > 0);
        Account selected = (Account) accounts.getSelectedItem();
        login.setEnabled(idle && (selected == null || !selected.connected || selected.reLoginRequired || !selected.planEnabled));
        logout.setEnabled(idle && selected != null && selected.connected);
        cancelSignIn.setEnabled(signingIn && running != null && !running.isCancelled() && !closed);
        models.setEnabled(idle);
        send.setEnabled(canSend(idle, selected, models.getItemCount()));
        prompt.setEnabled(idle);
        stop.setEnabled(running != null && !closed);
        reset.setEnabled(idle);
    }

    static boolean canSend(boolean idle, Account selected, int modelCount) {
        return idle && selected != null && !selected.reLoginRequired && selected.planEnabled && modelCount > 0;
    }

    private static boolean isReadyAccount(Account account) {
        return account.connected && account.planEnabled && !account.reLoginRequired;
    }

    private void updateLoginLabel(Account selected) {
        boolean reLoginRequired = selected != null && selected.reLoginRequired;
        login.setText(reLoginRequired ? "Continue with ChatGPT (re-login required)" : "Continue with ChatGPT");
        login.setToolTipText(reLoginRequired ? "Re-login required for this saved connection." : "Sign in to ChatGPT.");
    }

    private void updateAccountStatus(Account selected) {
        status.setText(selected == null ? "Not connected" : selected.reLoginRequired ? "Re-login required"
                : !selected.connected ? "Not connected" : selected.planEnabled ? "ChatGPT plan usage enabled"
                : "Connected; plan access required");
    }

    private void clearConversation() {
        history.clear();
        caseEvidence = null;
        terminal.setText("");
        if (session != null) {
            session.close();
            session = null;
        }
    }

    public JPanel getContent() { return content; }

    private void append(String text) {
        if (closed) { return; }
        terminal.append(text);
        int excess = terminal.getDocument().getLength() - 200000;
        if (excess > 0) { terminal.replaceRange("", 0, excess); }
        terminal.setCaretPosition(terminal.getDocument().getLength());
    }

    private void browse(URI uri, Cancellation cancellation) {
        if (!AuthDialogLifecycle.mayBrowse(cancellation, running, closed)) { return; }
        try {
            if (browser != null) {
                if (browser.open(uri)) { return; }
            } else if (Desktop.isDesktopSupported() && Desktop.getDesktop().isSupported(Desktop.Action.BROWSE)) {
                Desktop.getDesktop().browse(uri);
                return;
            }
        } catch (Exception ignored) { }
        authDialogs.opened(cancellation, fallbackDialogs.show(uri, cancellation::cancel));
    }

    private AuthDialogHandle showFallbackDialog(URI uri, Runnable onCancel) {
        JTextArea link = new JTextArea(uri.toString(), 5, 70);
        link.setEditable(false);
        link.setLineWrap(true);
        link.setWrapStyleWord(true);
        JDialog dialog = new JDialog(SwingUtilities.getWindowAncestor(content), "Open this link in your browser",
                Dialog.ModalityType.MODELESS);
        dialog.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
        dialog.addWindowListener(new java.awt.event.WindowAdapter() {
            @Override public void windowClosing(java.awt.event.WindowEvent e) { onCancel.run(); }
        });
        JButton cancel = new JButton("Cancel sign-in");
        cancel.addActionListener(e -> { onCancel.run(); dialog.dispose(); });
        JPanel panel = new JPanel(new BorderLayout(8, 8));
        panel.add(new JLabel("Open this sign-in link in your browser, or cancel sign-in:"), BorderLayout.NORTH);
        panel.add(new JScrollPane(link), BorderLayout.CENTER);
        panel.add(cancel, BorderLayout.SOUTH);
        dialog.setContentPane(panel);
        dialog.pack();
        dialog.setLocationRelativeTo(content);
        dialog.setVisible(true);
        return dialog::dispose;
    }

    private static String safeMessage(Exception error) {
        // Parser/JWT error messages can contain token data; only our IO messages are displayed.
        return error instanceof java.io.IOException ? error.getMessage() : "The operation failed. Please try again.";
    }

    @Override public void close() {
        if (closed) { return; }
        closed = true;
        if (sourceTask != null) { sourceTask.cancel(true); }
        if (session != null) { session.close(); }
        if (running != null) { running.cancel(); }
        authDialogs.close();
        worker.execute(() -> {
            if (client != null) {
                try { client.close(); } catch (Exception ignored) { }
            }
        });
        worker.shutdown();
    }

}
