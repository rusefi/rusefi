package com.rusefi.ui.llm;

import com.rusefi.io.LinkManager;
import com.rusefi.mcp.ConsoleEcuSession;
import com.rusefi.core.net.FirmwareSourceCodeDownloader;
import com.rusefi.core.net.ConnectionAndMeta.DownloadProgressListener;
import com.rusefi.ui.widgets.ConsoleFonts;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.ToolbarStrip;
import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.ui.FlatLineBorder;

import com.rusefi.ui.llm.ChatGptClient.Account;
import com.rusefi.ui.llm.ChatGptClient.Cancellation;
import com.rusefi.ui.llm.ChatGptClient.Model;
import org.json.simple.JSONArray;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.io.IOException;
import java.net.URI;
import java.nio.file.Path;
import java.util.ArrayList;
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
    static final String PROMPT_PLACEHOLDER = "Describe your problem here, then click Send or press Ctrl+Enter";
    private final LinkManager linkManager;
    private final Path storage;
    private final TroubleshootingSessionStore savedSessions;
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
    private final JPanel llmControls = new JPanel(new BorderLayout());
    private final JButton startDownload = new JButton("Start Download");
    private final JProgressBar sourceProgress = new JProgressBar(0, 100);
    private final JLabel sourceStatus = new JLabel("Checking cached source code...");
    private Future<?> sourceTask;
    private boolean preparingSources;
    private final JComboBox<Account> accounts = new JComboBox<>();
    private final JComboBox<Model> models = new JComboBox<>();
    private final JButton login = roomyButton("Continue with ChatGPT");
    private final JButton addAccount = roomyButton("Add account");
    private final JButton logout = roomyButton("Sign out");
    private final JButton cancelSignIn = roomyButton("Cancel sign-in");
    private final JButton send = new JButton("Send");
    private final JButton stop = new JButton("Stop");
    private final JButton reset = roomyButton("New conversation");
    private final JComboBox<TroubleshootingSessionStore.Summary> recentSessions = new JComboBox<>();
    private final JButton resume = roomyButton("Resume");
    private final JButton delete = roomyButton("Delete session");
    private final JTextArea terminal = new JTextArea(24, 90);
    private final JTextArea prompt = new PlaceholderTextArea(4, 70, PROMPT_PLACEHOLDER);
    private final JLabel status = new JLabel("Opening account store...");
    private final JSONArray history = new JSONArray();
    private String committedTranscript = "";
    private TroubleshootingSessionStore.Session activeSession;
    private final ExecutorService worker = Executors.newSingleThreadExecutor(r -> {
        Thread thread = new Thread(r, "chatgpt-console");
        thread.setDaemon(true);
        return thread;
    });
    private ChatGptClient client; // Only accessed on the worker.
    private Cancellation running;
    private String selectedId;
    private boolean updatingAccounts;
    private boolean updatingModels;
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
        savedSessions = new TroubleshootingSessionStore(storage.resolve("sessions"));
        this.sources = sources;
        this.clients = clients;
        this.browser = browser;
        this.fallbackDialogs = fallbackDialogs == null ? this::showFallbackDialog : fallbackDialogs;
        content.add(createSourceCard(), "sources");
        content.add(llmControls, "chat");
        cards.show(content, "sources");
        startDownload.addActionListener(e -> prepareSources(true));
        llmControls.add(createToolbar(), BorderLayout.NORTH);

        terminal.setFont(ConsoleFonts.mono(Font.PLAIN, ConsoleFonts.baseSize()));
        // a plain Color, not the theme's UIResource: FlatLaf greys out read-only text areas otherwise
        terminal.setBackground(new Color(themed("Console.Card.background", Color.WHITE).getRGB()));
        terminal.setEditable(false);
        terminal.setLineWrap(true);
        terminal.setWrapStyleWord(true);
        terminal.setMargin(new Insets(10, 14, 10, 14));
        terminal.getAccessibleContext().setAccessibleName("ChatGPT terminal output");
        JScrollPane terminalScroll = new JScrollPane(terminal);
        terminalScroll.setBorder(BorderFactory.createEmptyBorder());
        // the transcript on a white card under a titled header, like the Knock Analyzer plots
        JPanel conversation = new JPanel(new BorderLayout());
        conversation.setBackground(terminal.getBackground());
        conversation.setBorder(BorderFactory.createLineBorder(themed("Console.Card.borderColor", Color.LIGHT_GRAY)));
        conversation.add(ToolbarStrip.header("Conversation", null), BorderLayout.NORTH);
        conversation.add(terminalScroll, BorderLayout.CENTER);

        prompt.setLineWrap(true);
        prompt.setWrapStyleWord(true);
        prompt.setMargin(new Insets(6, 8, 6, 8));
        prompt.getAccessibleContext().setAccessibleName("Message to ChatGPT");
        prompt.setToolTipText("Type a message, then press Ctrl+Enter or click Send. Enter inserts a new line.");
        prompt.getInputMap(JComponent.WHEN_FOCUSED).put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, InputEvent.CTRL_DOWN_MASK), "send-prompt");
        prompt.getActionMap().put("send-prompt", new AbstractAction() {
            @Override public void actionPerformed(ActionEvent e) {
                if (send.isEnabled()) { send(); }
            }
        });
        send.setToolTipText("Send the message (Ctrl+Enter from the message field)");
        send.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        send.setPreferredSize(new Dimension(send.getPreferredSize().width + 32, JoinedButtons.HEIGHT));
        stop.setToolTipText("Stop the running request");
        // a plain outlined button: Send and Stop share the column width and keep their text centred
        stop.setPreferredSize(new Dimension(stop.getPreferredSize().width, JoinedButtons.HEIGHT));
        JPanel buttons = new JPanel(new GridLayout(2, 1, 0, JoinedButtons.ROW_GAP));
        buttons.setOpaque(false);
        buttons.add(send);
        buttons.add(stop);
        JPanel buttonColumn = new JPanel(new BorderLayout());
        buttonColumn.setOpaque(false);
        buttonColumn.add(buttons, BorderLayout.NORTH);
        JPanel input = new JPanel(new BorderLayout(JoinedButtons.ROW_GAP, 0));
        input.setOpaque(false);
        input.add(new JScrollPane(prompt), BorderLayout.CENTER);
        input.add(buttonColumn, BorderLayout.EAST);
        input.setMinimumSize(new Dimension(0, buttons.getPreferredSize().height));
        conversation.setMinimumSize(new Dimension(0, 120));

        // drag the divider up to give the message box more room
        JSplitPane split = new JSplitPane(JSplitPane.VERTICAL_SPLIT, conversation, input);
        split.setResizeWeight(1);
        split.setContinuousLayout(true);
        split.setDividerSize(12);
        split.setBorder(BorderFactory.createEmptyBorder());
        split.setOpaque(false);

        JPanel page = new JPanel(new BorderLayout());
        page.setBorder(BorderFactory.createEmptyBorder(12, 12, 12, 12));
        page.add(split, BorderLayout.CENTER);
        llmControls.add(page, BorderLayout.CENTER);
        append("rusEFI Troubleshooting\nSign in to use your ChatGPT plan.\n"
                + "Your messages, requested ECU readings and source/wiki excerpts are sent to OpenAI. ECU access is read-only.\n"
                + "Ask to export a diagnostic case to save findings and selected evidence locally.\n"
                + "Completed conversations and their collected context are saved locally and can be resumed here.\n"
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
            setRecentSessions(Collections.emptyList());
            loadModels();
        });
        models.addActionListener(e -> {
            if (!updatingModels && running == null && activeSession != null) {
                clearConversation();
                append("Model changed; start a new conversation or resume a saved one.\n");
            }
        });
        reset.addActionListener(e -> { clearConversation(); append("New conversation.\n"); });
        recentSessions.addActionListener(e -> updateControls());
        resume.addActionListener(e -> resumeSession());
        delete.addActionListener(e -> deleteSession());
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
            List<TroubleshootingSessionStore.Summary> recent = savedSessions.list(signedIn);
            return () -> {
                selectedId = signedIn;
                clearConversation();
                setModels(available);
                setRecentSessions(recent);
                append("Connected to ChatGPT.\n");
            };
        });
    }

    private void loadModels() {
        String id = selectedId;
        if (id == null) { updateControls(); return; }
        perform("Loading models...", cancellation -> {
            List<TroubleshootingSessionStore.Summary> recent = savedSessions.list(id);
            List<Model> available = loadModels(id, cancellation);
            return () -> {
                setModels(available);
                setRecentSessions(recent);
            };
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
            session.close();
            session = null;
            append("[Console connection changed; collecting fresh ECU evidence for this conversation.]\n");
        }
        if (session == null) {
            try {
                session = new ConsoleEcuSession(linkManager);
                if (caseEvidence == null) {
                    caseEvidence = new DiagnosticCaseStore(storage.resolve("diagnostic-cases"));
                }
            } catch (java.io.IOException e) {
                append("[" + e.getMessage() + "]\n");
                return;
            }
        }
        ConsoleEcuSession current = session;
        DiagnosticCaseStore turnEvidence = new DiagnosticCaseStore(caseEvidence);
        JSONArray previous = new JSONArray();
        previous.addAll(history);
        TroubleshootingSessionStore.Session base = activeSession == null
                ? TroubleshootingSessionStore.Session.start(id, model.slug, text) : activeSession;
        String prefix = "\n> " + text + "\n\n";
        StringBuilder turnTranscript = new StringBuilder(prefix);
        String precedingTranscript = committedTranscript;
        append(prefix);
        prompt.setText("");
        log.info("troubleshooting turn: model=" + model.slug + ", prompt=" + text.length() + " chars, "
                + history.size() + " history items");
        perform("Troubleshooting...", cancellation -> {
            ChatGptAgent.Tools tools = new TroubleshootingTools(current, knowledgeDirectory, turnEvidence);
            JSONArray completed = ChatGptAgent.run(
                    (input, definitions, delta, c) -> client.respondWithTools(id, model.slug, input, definitions, delta, c),
                    tools, previous, text, delta -> {
                        turnTranscript.append(delta);
                        SwingUtilities.invokeLater(() -> append(delta));
                    }, progress -> {
                        turnTranscript.append(progress);
                        SwingUtilities.invokeLater(() -> append(progress));
                    }, cancellation);
            String transcript = precedingTranscript + turnTranscript + "\n";
            if (transcript.length() > TroubleshootingSessionStore.MAX_TRANSCRIPT) {
                transcript = transcript.substring(transcript.length() - TroubleshootingSessionStore.MAX_TRANSCRIPT);
            }
            TroubleshootingSessionStore.Session finished = base.completed(model.slug, completed, transcript,
                    turnEvidence.snapshot());
            IOException saveFailure = null;
            if (!cancellation.isCancelled() && current.isCurrent()) {
                try {
                    savedSessions.save(finished);
                } catch (IOException e) {
                    log.error("troubleshooting session save failed", e);
                    saveFailure = e;
                }
            }
            IOException finalSaveFailure = saveFailure;
            return () -> {
                if (cancellation.isCancelled() || !current.isCurrent()) {
                    append("\n[Turn discarded: stopped or Console connection changed.]\n");
                    return;
                }
                history.clear();
                history.addAll(completed);
                caseEvidence = turnEvidence;
                activeSession = finished;
                committedTranscript = finished.transcript;
                append("\n");
                if (finalSaveFailure == null) {
                    rememberSession(finished.summary());
                } else {
                    append("[Could not save this conversation locally: " + safeMessage(finalSaveFailure) + "]\n");
                }
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
        updatingModels = true;
        try {
            models.removeAllItems();
            for (Model model : available) { models.addItem(model); }
        } finally {
            updatingModels = false;
        }
    }

    private void setRecentSessions(List<TroubleshootingSessionStore.Summary> recent) {
        recentSessions.removeAllItems();
        for (TroubleshootingSessionStore.Summary item : recent) { recentSessions.addItem(item); }
        recentSessions.setSelectedIndex(-1);
        updateControls();
    }

    private void rememberSession(TroubleshootingSessionStore.Summary updated) {
        List<TroubleshootingSessionStore.Summary> recent = new ArrayList<>();
        recent.add(updated);
        for (int index = 0; index < recentSessions.getItemCount(); index++) {
            TroubleshootingSessionStore.Summary existing = recentSessions.getItemAt(index);
            if (!existing.id.equals(updated.id)) { recent.add(existing); }
        }
        recent.sort((left, right) -> Long.compare(right.updatedAt, left.updatedAt));
        setRecentSessions(recent);
    }

    @SuppressWarnings("unchecked")
    private void resumeSession() {
        TroubleshootingSessionStore.Summary selected = (TroubleshootingSessionStore.Summary) recentSessions.getSelectedItem();
        String accountId = selectedId;
        if (selected == null || accountId == null) { return; }
        perform("Opening session...", cancellation -> {
            TroubleshootingSessionStore.Session restored = savedSessions.load(accountId, selected.id);
            DiagnosticCaseStore evidence = DiagnosticCaseStore.restore(storage.resolve("diagnostic-cases"), restored.evidence);
            return () -> {
                clearConversation();
                activeSession = restored;
                history.addAll(restored.history);
                caseEvidence = evidence;
                committedTranscript = restored.transcript;
                terminal.setText(restored.transcript);
                terminal.setCaretPosition(terminal.getDocument().getLength());
                updatingModels = true;
                boolean matchingModel = false;
                try {
                    for (int index = 0; index < models.getItemCount(); index++) {
                        Model model = models.getItemAt(index);
                        if (model.slug.equals(restored.model)) {
                            models.setSelectedIndex(index);
                            matchingModel = true;
                            break;
                        }
                    }
                } finally {
                    updatingModels = false;
                }
                if (!matchingModel) { append("\n[Previous model unavailable. Choose an available model to continue.]\n"); }
            };
        });
    }

    private void deleteSession() {
        TroubleshootingSessionStore.Summary selected = (TroubleshootingSessionStore.Summary) recentSessions.getSelectedItem();
        String accountId = selectedId;
        if (selected == null || accountId == null) { return; }
        int choice = JOptionPane.showConfirmDialog(content, "Delete this saved troubleshooting session?\n" + selected.title,
                "Delete session", JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE);
        if (choice != JOptionPane.YES_OPTION) { return; }
        perform("Deleting session...", cancellation -> {
            savedSessions.delete(accountId, selected.id);
            List<TroubleshootingSessionStore.Summary> remaining = savedSessions.list(accountId);
            return () -> {
                if (activeSession != null && activeSession.id.equals(selected.id)) { clearConversation(); }
                setRecentSessions(remaining);
            };
        });
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
        recentSessions.setEnabled(idle);
        boolean hasRecent = recentSessions.getSelectedItem() != null;
        resume.setEnabled(idle && hasRecent);
        delete.setEnabled(idle && hasRecent);
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
        activeSession = null;
        committedTranscript = "";
        caseEvidence = null;
        terminal.setText("");
        if (session != null) {
            session.close();
            session = null;
        }
    }

    /** Centred card for the source download step, like the Knock Analyzer's start card. */
    private JComponent createSourceCard() {
        JLabel title = new JLabel("Prepare troubleshooting knowledge");
        Font base = UIManager.getFont("Label.font");
        if (base != null) {
            title.setFont(base.deriveFont(Font.BOLD, base.getSize2D() + 2));
        }
        JLabel description = muted(new JLabel("Hit the button below to download the rusEFI source code gist."));
        startDownload.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        startDownload.setPreferredSize(new Dimension(startDownload.getPreferredSize().width + 32, JoinedButtons.HEIGHT));
        sourceProgress.setStringPainted(true);
        sourceProgress.setPreferredSize(new Dimension(360, sourceProgress.getPreferredSize().height));
        muted(sourceStatus);

        JPanel card = new JPanel(new GridBagLayout());
        // FlatPanelUI paints a rounded background when the border is a FlatLineBorder with an arc
        card.setBorder(new FlatLineBorder(new Insets(24, 28, 24, 28), themed("Console.Card.borderColor", Color.LIGHT_GRAY), 1, 10));
        card.setBackground(new Color(themed("Console.Card.background", Color.WHITE).getRGB()));
        GridBagConstraints c = new GridBagConstraints();
        c.gridx = 0;
        c.insets = new Insets(0, 0, 8, 0);
        card.add(title, c);
        c.insets = new Insets(0, 0, 16, 0);
        card.add(description, c);
        card.add(startDownload, c);
        c.insets = new Insets(0, 0, 8, 0);
        c.fill = GridBagConstraints.HORIZONTAL;
        card.add(sourceProgress, c);
        c.fill = GridBagConstraints.NONE;
        c.insets = new Insets(0, 0, 0, 0);
        card.add(sourceStatus, c);

        JPanel page = new JPanel(new GridBagLayout());
        page.setBorder(BorderFactory.createEmptyBorder(16, 16, 16, 16));
        page.add(card);
        return page;
    }

    /** Account controls with the status on the right, then model and saved sessions, in the Gauges toolbar style. */
    private JComponent createToolbar() {
        accounts.setPreferredSize(new Dimension(320, JoinedButtons.HEIGHT));
        accounts.setToolTipText("Saved ChatGPT account and workspace registrations");
        models.setPreferredSize(new Dimension(220, JoinedButtons.HEIGHT));
        recentSessions.setPreferredSize(new Dimension(360, JoinedButtons.HEIGHT));
        recentSessions.setToolTipText("Saved conversations for the selected ChatGPT account");
        addAccount.setVisible(false);

        JPanel accountRow = toolbarRow();
        accountRow.add(muted(new JLabel("Account")));
        accountRow.add(accounts);
        accountRow.add(new JoinedButtons(login, addAccount));
        accountRow.add(new JoinedButtons(logout, cancelSignIn));
        // GridBagLayout centres the status vertically in the row
        JPanel statusBox = new JPanel(new GridBagLayout());
        statusBox.setOpaque(false);
        statusBox.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, JoinedButtons.ROW_GAP));
        statusBox.add(muted(status));
        JPanel first = new JPanel(new BorderLayout());
        first.setOpaque(false);
        first.add(accountRow, BorderLayout.CENTER);
        first.add(statusBox, BorderLayout.EAST);

        JPanel sessionRow = toolbarRow();
        sessionRow.add(muted(new JLabel("Model")));
        sessionRow.add(models);
        sessionRow.add(new JoinedButtons(reset));
        sessionRow.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        sessionRow.add(muted(new JLabel("Recent sessions")));
        sessionRow.add(recentSessions);
        sessionRow.add(new JoinedButtons(resume, delete));

        JPanel rows = new JPanel(new GridLayout(0, 1, 0, JoinedButtons.ROW_GAP));
        rows.setOpaque(false);
        rows.add(first);
        rows.add(sessionRow);
        JPanel toolbar = new ToolbarStrip(new BorderLayout());
        toolbar.setBorder(BorderFactory.createEmptyBorder(8, 0, 9, JoinedButtons.ROW_END_INSET - JoinedButtons.ROW_GAP));
        toolbar.add(rows, BorderLayout.CENTER);
        return toolbar;
    }

    /**
     * A toolbar button a few pixels wider than its text needs. At fractional display scaling (125%, 150%)
     * Java paints text wider than it measures it, so an exactly fitted label ends in "...".
     */
    private static JButton roomyButton(String text) {
        return new JButton(text) {
            @Override public Dimension getPreferredSize() {
                Dimension size = super.getPreferredSize();
                if (!isPreferredSizeSet()) {
                    size.width += Math.max(8, getFontMetrics(getFont()).stringWidth(getText()) / 8);
                }
                return size;
            }
        };
    }

    /** Text area that shows a muted hint while it is empty, so the user knows where to type. */
    private static final class PlaceholderTextArea extends JTextArea {
        private final String placeholder;

        PlaceholderTextArea(int rows, int columns, String placeholder) {
            super(rows, columns);
            this.placeholder = placeholder;
            getDocument().addDocumentListener(new javax.swing.event.DocumentListener() {
                @Override public void insertUpdate(javax.swing.event.DocumentEvent e) { repaint(); }
                @Override public void removeUpdate(javax.swing.event.DocumentEvent e) { repaint(); }
                @Override public void changedUpdate(javax.swing.event.DocumentEvent e) { repaint(); }
            });
        }

        @Override protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            if (getDocument().getLength() > 0) { return; }
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
                g2.setFont(getFont());
                g2.setColor(themed("TextField.placeholderForeground", Color.GRAY));
                Insets insets = getInsets();
                g2.drawString(placeholder, insets.left, insets.top + g2.getFontMetrics().getAscent());
            } finally {
                g2.dispose();
            }
        }
    }

    private static JPanel toolbarRow() {
        JPanel row = new JPanel(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, 0));
        row.setOpaque(false);
        return row;
    }

    private static JLabel muted(JLabel label) {
        label.setForeground(UIManager.getColor("Label.disabledForeground"));
        return label;
    }

    private static Color themed(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }

    public JPanel getContent() { return content; }

    private void append(String text) {
        if (closed) { return; }
        terminal.append(text);
        int excess = terminal.getDocument().getLength() - TroubleshootingSessionStore.MAX_TRANSCRIPT;
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
        link.setFont(ConsoleFonts.mono(Font.PLAIN, ConsoleFonts.baseSize()));
        link.setMargin(new Insets(6, 8, 6, 8));
        JButton cancel = new JButton("Cancel sign-in");
        cancel.addActionListener(e -> { onCancel.run(); dialog.dispose(); });
        JPanel footer = new JPanel(new FlowLayout(FlowLayout.RIGHT, 0, 0));
        footer.add(new JoinedButtons(cancel));
        JPanel panel = new JPanel(new BorderLayout(0, 12));
        panel.setBorder(BorderFactory.createEmptyBorder(16, 16, 16, 16));
        panel.add(new JLabel("Open this sign-in link in your browser, or cancel sign-in:"), BorderLayout.NORTH);
        panel.add(new JScrollPane(link), BorderLayout.CENTER);
        panel.add(footer, BorderLayout.SOUTH);
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
