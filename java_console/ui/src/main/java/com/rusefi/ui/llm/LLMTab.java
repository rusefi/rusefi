package com.rusefi.ui.llm;

import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.io.LinkManager;
import com.rusefi.mcp.ConsoleEcuSession;
import com.rusefi.core.net.FirmwareSourceCodeDownloader;
import com.rusefi.core.net.ConnectionAndMeta.DownloadProgressListener;
import com.rusefi.ui.widgets.ConsoleFonts;
import com.rusefi.ui.widgets.JoinedButtons;
import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.ui.FlatLineBorder;

import com.rusefi.ui.llm.ChatGptClient.Account;
import com.rusefi.ui.llm.ChatGptClient.Cancellation;
import com.rusefi.ui.llm.ChatGptClient.Model;
import com.rusefi.ui.llm.TroubleshootingIcons.StepState;
import org.json.simple.JSONArray;

import javax.swing.*;
import java.awt.*;
import java.awt.datatransfer.StringSelection;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.IOException;
import java.net.URI;
import java.nio.file.Path;
import java.time.Instant;
import java.time.LocalDate;
import java.time.ZoneId;
import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.function.Consumer;

/**
 * ChatGPT troubleshooting through the existing Console connection. Construct and close on the EDT.
 * Saved conversations and the account sit in a sidebar; the main area shows the setup checklist until the
 * knowledge is prepared and a ChatGPT account with plan access is chosen, the sign-in card while the browser
 * sign-in runs, and the conversation once an account is ready.
 */
public final class LLMTab implements AutoCloseable {
    // Never log prompts, reply text or tokens; operation names, model slugs and exceptions only.
    private static final com.devexperts.logging.Logging log = com.devexperts.logging.Logging.getLogging(LLMTab.class);
    static final String PROMPT_PLACEHOLDER = "Describe the problem: what happens, when, and what you already tried";
    /** Sent by Export case: the assistant writes the findings, so it exports them with its tool. */
    static final String EXPORT_REQUEST = "Export a diagnostic case with your findings, hypotheses and next "
            + "measurements so far, citing the evidence collected in this conversation.";
    /** Sent when the user attaches files without typing anything. */
    static final String ATTACHMENTS_ONLY_PROMPT = "Please look at the attached files.";
    static final String PRIVACY_NOTE = "Messages, requested ECU readings and source/wiki excerpts are sent to OpenAI. "
            + "ECU access is read-only.";
    private static final String SETUP = "setup";
    private static final String SIGN_IN = "signin";
    private static final String CHAT = "chat";
    private static final int CARD_WIDTH = 560;
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
    private final JPanel content = new JPanel(new BorderLayout());
    private JComponent sidebar;
    private JSplitPane sidebarSplit;
    private JComponent accountCard;
    private static final int SIDEBAR_DIVIDER = 10;
    private static final int SIDEBAR_WIDTH = 250;
    /** Height of the sidebar's title band and the conversation header, so their bottom lines meet. */
    private static final int HEADER_HEIGHT = JoinedButtons.HEIGHT + 18;
    private final CardLayout cards = new CardLayout();
    private final JPanel main = new JPanel(cards);
    private String shownCard = "";
    private final JButton startDownload = new JButton("Start Download");
    private final JProgressBar sourceProgress = new JProgressBar(0, 100);
    private final JLabel sourceStatus = new JLabel("Checking cached source code...");
    private Future<?> sourceTask;
    private boolean preparingSources;
    private final JComboBox<Account> accounts = new JComboBox<>();
    private final JComboBox<Model> models = new JComboBox<>();
    private final JButton login = roomyButton("Continue with ChatGPT");
    private final JButton addAccount = roomyButton("Use another account");
    private final JButton cancelSignIn = roomyButton("Cancel sign-in");
    private final JButton send = new JButton("Send");
    private final JButton stop = new JButton("Stop");
    private final CardLayout sendOrStop = new CardLayout();
    private final JPanel sendSlot = new JPanel(sendOrStop);
    private final JButton reset = new JButton("New conversation");
    private final DefaultListModel<TroubleshootingSessionStore.Summary> recentSessions = new DefaultListModel<>();
    private final ConversationList conversations = new ConversationList(recentSessions);
    private final ChatTranscript chat = new ChatTranscript(
            "Describe the problem below. Ask to export a diagnostic case to save findings and evidence locally.");
    /** Whether the ECU is connected, under the conversation title. */
    private final JLabel ecuStatus = new JLabel();
    private final TroubleshootingIcons.Dot ecuDot = new TroubleshootingIcons.Dot();
    private final JLabel ecuStep = new JLabel("Connect to your ECU in the Console, then describe the problem");
    private final JLabel ecuStepState = new JLabel();
    private final JButton exportCase = roomyButton("Export case");
    /** Files and pictures waiting to go with the next message. */
    private final AttachmentBar attachments = new AttachmentBar();
    private final JButton attach = new JButton(new TroubleshootingIcons.Paperclip());
    private File attachFolder;
    private int pastedPictures;
    private final ConnectionStatusLogic.Listener ecuListener = connected -> SwingUtilities.invokeLater(this::updateControls);
    private final JTextArea prompt = new PlaceholderTextArea(3, 70, PROMPT_PLACEHOLDER);
    /** The account's state, or what the tab is busy with. */
    private final JLabel status = new JLabel("Opening account store...");
    private final TroubleshootingIcons.Dot statusDot = new TroubleshootingIcons.Dot();
    private final TroubleshootingIcons.Avatar avatar = new TroubleshootingIcons.Avatar();
    private final JLabel accountName = new JLabel("Not signed in");
    private final JButton accountMenu = new JButton(new TroubleshootingIcons.Chevron());
    private final JLabel conversationTitle = new JLabel("New conversation");
    // setup checklist
    private final TroubleshootingIcons.StepBadge knowledgeBadge = new TroubleshootingIcons.StepBadge(1);
    private final TroubleshootingIcons.StepBadge signInBadge = new TroubleshootingIcons.StepBadge(2);
    private final TroubleshootingIcons.StepBadge ecuBadge = new TroubleshootingIcons.StepBadge(3);
    private final JPanel knowledgeBody = verticalBox();
    private final JLabel knowledgeReady = new JLabel("Ready");
    private final JPanel signInBody = verticalBox();
    private final JPanel savedSignIn = verticalBox();
    private final JTextArea setupNotice = paragraph("");
    private final JLabel signInStep = new JLabel("Sign in with your ChatGPT plan");
    // browser sign-in card
    private final JLabel signInHint = new JLabel("Browser didn't open? Copy this link:");
    private final JTextField signInLink = new JTextField();
    private final JButton copyLink = roomyButton("Copy");
    private final JButton openLink = roomyButton("Open");
    private URI signInUri;
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
    /** Shown when the browser could not be opened for sign-in; the default points at the sign-in card's link. */
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
        this.fallbackDialogs = fallbackDialogs == null ? this::showFallbackNotice : fallbackDialogs;

        main.add(createSetupPage(), SETUP);
        main.add(createSignInPage(), SIGN_IN);
        main.add(createChatPage(), CHAT);
        sidebar = createSidebar();
        // drag the divider to widen the sidebar, like the one above the message box
        sidebarSplit = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, sidebar, main);
        sidebarSplit.setResizeWeight(0);
        sidebarSplit.setContinuousLayout(true);
        sidebarSplit.setDividerSize(SIDEBAR_DIVIDER);
        sidebarSplit.setBorder(BorderFactory.createEmptyBorder());
        // the divider continues the sidebar's white and draws the line between sidebar and main area, and the
        // sidebar's title and account card lines across to the main area
        sidebarSplit.setBackground(cardBackground());
        ((javax.swing.plaf.basic.BasicSplitPaneUI) sidebarSplit.getUI()).getDivider().setBorder(new javax.swing.border.AbstractBorder() {
            @Override public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
                g.setColor(cardBorder());
                g.fillRect(x + width - 1, y, 1, height);
                g.fillRect(x, y + HEADER_HEIGHT - 1, width, 1);
                // sidebar and divider share the split's top, so the card's y is the divider's too
                g.fillRect(x, y + accountCard.getY(), width, 1);
            }
        });
        main.setMinimumSize(new Dimension(420, 0));
        content.add(sidebarSplit, BorderLayout.CENTER);
        startDownload.addActionListener(e -> prepareSources(true));

        login.addActionListener(e -> signIn(selectedId));
        addAccount.addActionListener(e -> signIn(null));
        cancelSignIn.addActionListener(e -> {
            if (signingIn && running != null) {
                running.cancel();
                status.setText("Cancelling sign-in...");
                updateControls();
            }
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
                notice("Model changed; start a new conversation or resume a saved one.");
            }
        });
        reset.addActionListener(e -> {
            clearConversation();
            updateControls();
            prompt.requestFocusInWindow();
        });
        stop.addActionListener(e -> {
            if (running != null) {
                running.cancel();
                stop.setEnabled(false);
                status.setText("Stopping...");
            }
        });
        send.addActionListener(e -> send());
        exportCase.addActionListener(e -> send(EXPORT_REQUEST, Collections.emptyList(), false));
        ConnectionStatusLogic.INSTANCE.addListener(ecuListener);
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
                    updateControls();
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
            return () -> { };
        });
    }

    private void signIn(String id) {
        signingIn = true;
        signInUri = null;
        perform("Complete sign-in in your browser...", cancellation -> {
            String signedIn = client.signIn(id, uri -> SwingUtilities.invokeLater(() -> browse(uri, cancellation)), cancellation);
            List<Model> available = loadModels(signedIn, cancellation);
            List<TroubleshootingSessionStore.Summary> recent = savedSessions.list(signedIn);
            return () -> {
                selectedId = signedIn;
                clearConversation();
                setModels(available);
                setRecentSessions(recent);
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
                SwingUtilities.invokeLater(() -> notice("No models available for this connection. Check ChatGPT Settings > Usage."));
            }
            return available;
        } catch (Exception e) {
            if (cancellation.isCancelled()) { throw e; }
            SwingUtilities.invokeLater(() -> notice("Could not load models: " + safeMessage(e) + " Reselect the account to retry."));
            return Collections.emptyList();
        }
    }

    private void signOut() {
        String id = selectedId;
        perform("Signing out...", cancellation -> {
            boolean revoked = client.signOut(id, cancellation);
            return () -> {
                clearConversation();
                models.removeAllItems();
                setupNotice.setText(revoked ? "Signed out."
                        : "Signed out locally. Remote revocation was not confirmed; disconnect this app in ChatGPT Settings.");
            };
        });
    }

    @SuppressWarnings("unchecked")
    private void send() {
        send(prompt.getText().trim(), attachments.pending(), true);
    }

    /**
     * @param typed       the message; may be empty when files are attached
     * @param files       files and pictures to send with it
     * @param fromMessageBox whether {@code typed} and {@code files} come from the message box, which is then cleared
     */
    @SuppressWarnings("unchecked")
    private void send(String typed, List<Attachment> files, boolean fromMessageBox) {
        Model model = (Model) models.getSelectedItem();
        String text = typed.isEmpty() && !files.isEmpty() ? ATTACHMENTS_ONLY_PROMPT : typed;
        if (text.isEmpty() || model == null || running != null) { return; }
        List<Attachment> sent = new ArrayList<>(files);
        String id = selectedId;
        if (session != null && !session.isCurrent()) {
            session.close();
            session = null;
            notice("Console connection changed; collecting fresh ECU evidence for this conversation.");
        }
        if (session == null) {
            try {
                session = new ConsoleEcuSession(linkManager);
                if (caseEvidence == null) {
                    caseEvidence = new DiagnosticCaseStore(storage.resolve("diagnostic-cases"));
                }
            } catch (java.io.IOException e) {
                notice(e.getMessage());
                return;
            }
        }
        ConsoleEcuSession current = session;
        DiagnosticCaseStore turnEvidence = new DiagnosticCaseStore(caseEvidence);
        JSONArray previous = new JSONArray();
        previous.addAll(history);
        TroubleshootingSessionStore.Session base = activeSession == null
                ? TroubleshootingSessionStore.Session.start(id, model.slug, text) : activeSession;
        // attachments are listed under the message, one "[Attached: name]" line each (see ChatTranscript.load)
        StringBuilder prefix = new StringBuilder(committedTranscript.isEmpty() ? "" : "\n").append("> ").append(text).append('\n');
        List<String> names = new ArrayList<>();
        for (Attachment attachment : sent) {
            names.add(attachment.name);
            prefix.append("[Attached: ").append(attachment.name).append("]\n");
        }
        prefix.append('\n');
        StringBuilder turnTranscript = new StringBuilder(prefix);
        String precedingTranscript = committedTranscript;
        chat.addUser(text, names);
        if (fromMessageBox) {
            prompt.setText("");
            attachments.clear();
        }
        if (activeSession == null) {
            conversationTitle.setText(base.title);
        }
        log.info("troubleshooting turn: model=" + model.slug + ", prompt=" + text.length() + " chars, "
                + sent.size() + " attachments, " + history.size() + " history items");
        perform("Troubleshooting...", cancellation -> {
            ChatGptAgent.Tools tools = new TroubleshootingTools(current, knowledgeDirectory, turnEvidence);
            JSONArray completed = ChatGptAgent.run(
                    (input, definitions, delta, c) -> client.respondWithTools(id, model.slug, input, definitions, delta, c),
                    tools, previous, text, sent, delta -> {
                        turnTranscript.append(delta);
                        SwingUtilities.invokeLater(() -> { if (!closed) { chat.appendAssistant(delta); } });
                    }, progress -> {
                        turnTranscript.append(progress);
                        SwingUtilities.invokeLater(() -> { if (!closed) { chat.addProgress(progress); } });
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
                    notice("Turn discarded: stopped or Console connection changed.");
                    return;
                }
                history.clear();
                history.addAll(completed);
                caseEvidence = turnEvidence;
                activeSession = finished;
                committedTranscript = finished.transcript;
                if (finalSaveFailure == null) {
                    rememberSession(finished.summary());
                } else {
                    notice("Could not save this conversation locally: " + safeMessage(finalSaveFailure));
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
        statusDot.colorKey = "Console.Status.pending.dot";
        setupNotice.setText("");
        updateControls();
        log.info("operation started: " + message);
        worker.execute(() -> {
            Runnable success;
            try {
                success = work.run(cancellation);
            } catch (Exception e) {
                // The UI shows a sanitized message; keep the full diagnosis in the console log.
                log.error("operation failed (" + message + ", cancelled=" + cancellation.isCancelled() + ")", e);
                String failure = cancellation.isCancelled() ? "Stopped or time limit reached; partial turn was not saved."
                        : safeMessage(e);
                success = () -> {
                    notice(failure);
                    // the transcript is hidden until an account is ready, e.g. when sign-in fails
                    setupNotice.setText(failure);
                };
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
                updatingAccounts = false;
                running = null;
                if (signingIn) { signingIn = false; }
                ready = initialized;
                Account selected = (Account) accounts.getSelectedItem();
                if (ready) {
                    updateAccountStatus(selected);
                } else {
                    status.setText("Account store unavailable; reopen the Console");
                    statusDot.colorKey = "Console.Status.disconnected.dot";
                }
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
        recentSessions.clear();
        for (TroubleshootingSessionStore.Summary item : recent) { recentSessions.addElement(item); }
        updateControls();
    }

    private void rememberSession(TroubleshootingSessionStore.Summary updated) {
        List<TroubleshootingSessionStore.Summary> recent = new ArrayList<>();
        recent.add(updated);
        for (int index = 0; index < recentSessions.getSize(); index++) {
            TroubleshootingSessionStore.Summary existing = recentSessions.getElementAt(index);
            if (!existing.id.equals(updated.id)) { recent.add(existing); }
        }
        recent.sort((left, right) -> Long.compare(right.updatedAt, left.updatedAt));
        setRecentSessions(recent);
    }

    @SuppressWarnings("unchecked")
    private void resumeSession(TroubleshootingSessionStore.Summary selected) {
        String accountId = selectedId;
        if (selected == null || accountId == null) { return; }
        if (activeSession != null && activeSession.id.equals(selected.id)) { return; }
        perform("Opening conversation...", cancellation -> {
            TroubleshootingSessionStore.Session restored = savedSessions.load(accountId, selected.id);
            DiagnosticCaseStore evidence = DiagnosticCaseStore.restore(storage.resolve("diagnostic-cases"), restored.evidence);
            return () -> {
                clearConversation();
                activeSession = restored;
                history.addAll(restored.history);
                caseEvidence = evidence;
                committedTranscript = restored.transcript;
                chat.load(restored.transcript);
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
                if (!matchingModel) { notice("Previous model unavailable. Choose an available model to continue."); }
            };
        });
    }

    private void deleteSession(TroubleshootingSessionStore.Summary selected) {
        String accountId = selectedId;
        if (selected == null || accountId == null || running != null) { return; }
        int choice = JOptionPane.showConfirmDialog(content, "Delete this saved conversation?\n" + selected.title,
                "Delete conversation", JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE);
        if (choice != JOptionPane.YES_OPTION) { return; }
        perform("Deleting conversation...", cancellation -> {
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
        Account selected = (Account) accounts.getSelectedItem();
        boolean accountReady = selected != null && isReadyAccount(selected);
        accounts.setEnabled(idle);
        addAccount.setEnabled(idle);
        addAccount.setVisible(accounts.getItemCount() > 0);
        savedSignIn.setVisible(accounts.getItemCount() > 0);
        login.setEnabled(idle && (selected == null || !selected.connected || selected.reLoginRequired || !selected.planEnabled));
        cancelSignIn.setEnabled(signingIn && running != null && !running.isCancelled() && !closed);
        accountMenu.setEnabled(idle);
        models.setEnabled(idle);
        conversations.setEnabled(idle && accountReady);
        send.setEnabled(canSend(idle, selected, models.getItemCount()));
        // a case needs a finished turn whose findings and evidence it can save
        exportCase.setEnabled(send.isEnabled() && activeSession != null);
        prompt.setEnabled(idle);
        attach.setEnabled(idle && accountReady);
        stop.setEnabled(running != null && !closed);
        reset.setEnabled(idle && accountReady);
        sendOrStop.show(sendSlot, running != null && !signingIn ? "stop" : "send");
        copyLink.setEnabled(signInUri != null);
        openLink.setEnabled(signInUri != null);
        updateView(selected, accountReady);
    }

    /** Shows the page and checklist state that fit the knowledge, account and sign-in state. */
    private void updateView(Account selected, boolean accountReady) {
        boolean sourcesReady = knowledgeDirectory != null;
        knowledgeBadge.state = sourcesReady ? StepState.DONE : StepState.CURRENT;
        knowledgeBody.setVisible(!sourcesReady);
        knowledgeReady.setVisible(sourcesReady);
        signInBadge.state = !sourcesReady ? StepState.LATER : accountReady ? StepState.DONE : StepState.CURRENT;
        signInBody.setVisible(sourcesReady);
        setupNotice.setVisible(!setupNotice.getText().isEmpty());
        signInStep.setForeground(sourcesReady ? UIManager.getColor("Label.foreground") : UIManager.getColor("Label.disabledForeground"));
        ConnectionStatusValue ecu = ConnectionStatusLogic.INSTANCE.getValue();
        ecuBadge.state = ecu == ConnectionStatusValue.CONNECTED ? StepState.DONE : StepState.LATER;
        ecuStepState.setText(ecu == ConnectionStatusValue.CONNECTED ? "Connected"
                : ecu == ConnectionStatusValue.LOADING ? "Connecting..." : "Not connected");
        String ecuTone = ecu == ConnectionStatusValue.CONNECTED ? "Console.Status.connected" : "Console.Status.neutral";
        ecuStepState.setForeground(themed(ecuTone + ".foreground", Color.DARK_GRAY));
        ecuDot.colorKey = ecu == ConnectionStatusValue.CONNECTED ? "Console.Status.connected.dot"
                : ecu == ConnectionStatusValue.LOADING ? "Console.Status.pending.dot" : "Console.Status.neutral.dot";
        ecuStatus.setText(ecu == ConnectionStatusValue.CONNECTED ? "ECU connected: the assistant can read live values"
                : ecu == ConnectionStatusValue.LOADING ? "ECU connecting..."
                : "No ECU connected: connect in the Console so the assistant can read live values");
        // the sidebar only matters once the knowledge is there: before, the checklist is the whole tab
        if (sidebar.isVisible() != sourcesReady) {
            sidebar.setVisible(sourcesReady);
            sidebarSplit.setDividerSize(sourcesReady ? SIDEBAR_DIVIDER : 0);
            // explicit: from preferred sizes a narrow window would squeeze the sidebar to its minimum
            if (sourcesReady) { sidebarSplit.setDividerLocation(SIDEBAR_WIDTH); }
        }
        conversationTitle.setText(activeSession == null ? "New conversation" : activeSession.title);
        conversations.showActive(activeSession == null ? null : activeSession.id);
        if (selected == null) {
            accountName.setText("Not signed in");
            accountName.setToolTipText(null);
            avatar.signedIn = false;
        } else {
            // the e-mail says who is signed in; the generated "Connection N" name only tells sign-ins apart in the menu
            accountName.setText(selected.email.isEmpty() ? selected.label : selected.email);
            accountName.setToolTipText(selected.label);
            avatar.signedIn = selected.connected;
        }
        String card = !sourcesReady ? SETUP : signingIn ? SIGN_IN : accountReady ? CHAT : SETUP;
        if (!card.equals(shownCard)) {
            shownCard = card;
            cards.show(main, card);
        }
        content.repaint();
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
        statusDot.colorKey = selected == null || !selected.connected ? "Console.Status.neutral.dot"
                : isReadyAccount(selected) ? "Console.Status.connected.dot" : "Console.Status.pending.dot";
    }

    private void clearConversation() {
        history.clear();
        activeSession = null;
        committedTranscript = "";
        caseEvidence = null;
        chat.clear();
        conversationTitle.setText("New conversation");
        if (session != null) {
            session.close();
            session = null;
        }
    }

    /** Conversations and the account, left of the main area. */
    private JComponent createSidebar() {
        JLabel title = new JLabel("Troubleshooting");
        title.setFont(boldFont(1));
        // an outlined button as tall as the console's other controls; primary blue stays with Send
        reset.setIcon(new TroubleshootingIcons.Plus());
        reset.setIconTextGap(8);
        reset.setToolTipText("Start a new troubleshooting conversation");
        reset.setAlignmentX(Component.LEFT_ALIGNMENT);
        reset.setPreferredSize(new Dimension(reset.getPreferredSize().width, JoinedButtons.HEIGHT));
        reset.setMaximumSize(new Dimension(Integer.MAX_VALUE, JoinedButtons.HEIGHT));
        JLabel recent = muted(new JLabel("Recent"));
        recent.setFont(boldFont(-1));
        JPanel titleBand = new JPanel(new BorderLayout());
        titleBand.setOpaque(false);
        titleBand.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createMatteBorder(0, 0, 1, 0, cardBorder()),
                BorderFactory.createEmptyBorder(0, 12, 0, 12)));
        titleBand.setPreferredSize(new Dimension(0, HEADER_HEIGHT));
        titleBand.add(title, BorderLayout.CENTER);
        JPanel top = verticalBox();
        top.setBorder(BorderFactory.createEmptyBorder(12, 12, 8, 12));
        recent.setAlignmentX(Component.LEFT_ALIGNMENT);
        top.add(reset);
        top.add(Box.createVerticalStrut(14));
        top.add(recent);

        conversations.onOpen = this::resumeSession;
        conversations.onDelete = this::deleteSession;
        JScrollPane list = new JScrollPane(conversations, ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED,
                ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        list.setBorder(BorderFactory.createEmptyBorder());
        list.setOpaque(false);
        list.getViewport().setOpaque(false);

        JPanel north = new JPanel(new BorderLayout());
        north.setOpaque(false);
        north.add(titleBand, BorderLayout.NORTH);
        north.add(top, BorderLayout.CENTER);
        JPanel sidebar = new JPanel(new BorderLayout());
        sidebar.setBackground(cardBackground());
        sidebar.setPreferredSize(new Dimension(SIDEBAR_WIDTH, 0));
        sidebar.setMinimumSize(new Dimension(180, 0));
        sidebar.add(north, BorderLayout.NORTH);
        sidebar.add(list, BorderLayout.CENTER);
        accountCard = createAccountCard();
        sidebar.add(accountCard, BorderLayout.SOUTH);
        return sidebar;
    }

    /** The chosen account with its state, and a menu to switch, add or sign out. */
    private JComponent createAccountCard() {
        JLabel picture = new JLabel(avatar);
        accountName.setFont(boldFont(0));
        status.setIcon(statusDot);
        status.setIconTextGap(5);
        status.setFont(smallFont());
        muted(status);
        JPanel text = new JPanel(new GridLayout(2, 1));
        text.setOpaque(false);
        text.add(accountName);
        text.add(status);
        accountMenu.setToolTipText("Switch account, add an account or sign out");
        accountMenu.getAccessibleContext().setAccessibleName("Account menu");
        accountMenu.addActionListener(e -> {
            JPopupMenu menu = buildAccountMenu();
            menu.show(accountMenu, 0, -menu.getPreferredSize().height);
        });
        JPanel card = new JPanel(new BorderLayout(10, 0));
        card.setOpaque(false);
        card.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createMatteBorder(1, 0, 0, 0, cardBorder()),
                BorderFactory.createEmptyBorder(10, 12, 12, 10)));
        card.add(picture, BorderLayout.WEST);
        card.add(text, BorderLayout.CENTER);
        JPanel menuBox = new JPanel(new GridBagLayout());
        menuBox.setOpaque(false);
        menuBox.add(accountMenu);
        card.add(menuBox, BorderLayout.EAST);
        return card;
    }

    /** The account menu as it opens now; package-private so tests can use its items. */
    JPopupMenu buildAccountMenu() {
        JPopupMenu menu = new JPopupMenu();
        Account selected = (Account) accounts.getSelectedItem();
        ButtonGroup group = new ButtonGroup();
        for (int index = 0; index < accounts.getItemCount(); index++) {
            Account account = accounts.getItemAt(index);
            JRadioButtonMenuItem item = new JRadioButtonMenuItem(account.label, account == selected);
            item.addActionListener(e -> {
                if (account != accounts.getSelectedItem()) { accounts.setSelectedItem(account); }
            });
            group.add(item);
            menu.add(item);
        }
        if (accounts.getItemCount() > 0) { menu.addSeparator(); }
        boolean idle = running == null && !closed && ready;
        JMenuItem add = new JMenuItem(accounts.getItemCount() > 0 ? "Add account..." : "Sign in...");
        add.setEnabled(idle);
        add.addActionListener(e -> signIn(null));
        menu.add(add);
        JMenuItem signOut = new JMenuItem("Sign out");
        signOut.setEnabled(idle && selected != null && selected.connected);
        signOut.addActionListener(e -> signOut());
        menu.add(signOut);
        return menu;
    }

    /** The checklist shown until an account is ready: knowledge, sign-in, ECU, and what is shared. */
    private JComponent createSetupPage() {
        JLabel title = new JLabel("rusEFI Troubleshooting");
        title.setFont(boldFont(4));
        JTextArea intro = muted(paragraph("Describe a problem in plain words. The assistant reads live values from "
                + "your ECU, checks the rusEFI wiki and source, and suggests what to try next."));

        JLabel description = muted(new JLabel("Download the rusEFI source code gist the assistant searches."));
        startDownload.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        startDownload.setPreferredSize(new Dimension(startDownload.getPreferredSize().width + 32, JoinedButtons.HEIGHT));
        sourceProgress.setStringPainted(true);
        sourceProgress.setPreferredSize(new Dimension(360, sourceProgress.getPreferredSize().height));
        muted(sourceStatus);
        knowledgeBody.add(left(description));
        knowledgeBody.add(Box.createVerticalStrut(10));
        knowledgeBody.add(left(row(startDownload)));
        knowledgeBody.add(Box.createVerticalStrut(10));
        knowledgeBody.add(left(sourceProgress));
        knowledgeBody.add(Box.createVerticalStrut(6));
        knowledgeBody.add(left(sourceStatus));
        knowledgeReady.setFont(smallFont());
        knowledgeReady.setForeground(themed("Console.Status.connected.foreground", Color.DARK_GRAY));

        accounts.setPreferredSize(new Dimension(320, JoinedButtons.HEIGHT));
        accounts.setToolTipText("Saved ChatGPT account and workspace registrations");
        JLabel savedLabel = muted(new JLabel("Saved sign-in"));
        savedLabel.setFont(smallFont());
        savedLabel.setLabelFor(accounts);
        savedSignIn.add(left(savedLabel));
        savedSignIn.add(Box.createVerticalStrut(4));
        savedSignIn.add(left(row(accounts)));
        savedSignIn.add(Box.createVerticalStrut(10));
        login.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        addAccount.setToolTipText("Sign in with a different ChatGPT account or workspace");
        setupNotice.setForeground(themed("Console.Status.pending.foreground", Color.DARK_GRAY));
        signInBody.add(left(savedSignIn));
        signInBody.add(left(row(login, addAccount)));
        signInBody.add(Box.createVerticalStrut(6));
        signInBody.add(left(setupNotice));

        JPanel steps = verticalBox();
        steps.setOpaque(true);
        steps.setBackground(cardBackground());
        steps.setBorder(new FlatLineBorder(new Insets(0, 0, 0, 0), cardBorder(), 1, 8));
        steps.add(step(knowledgeBadge, new JLabel("Troubleshooting knowledge"), knowledgeReady, knowledgeBody, true));
        steps.add(step(signInBadge, signInStep, null, signInBody, true));
        ecuStepState.setFont(smallFont());
        steps.add(step(ecuBadge, ecuStep, ecuStepState, null, false));

        JLabel goodToKnow = muted(new JLabel("Good to know"));
        goodToKnow.setFont(boldFont(-1));
        JPanel card = verticalBox();
        card.add(left(title));
        card.add(Box.createVerticalStrut(6));
        card.add(left(intro));
        card.add(Box.createVerticalStrut(20));
        card.add(left(steps));
        card.add(Box.createVerticalStrut(20));
        card.add(left(goodToKnow));
        card.add(Box.createVerticalStrut(6));
        card.add(left(bullet("Your messages, the ECU readings the assistant asks for and source/wiki excerpts are sent to OpenAI.")));
        card.add(left(bullet("ECU access is read-only: nothing in your tune is changed.")));
        card.add(left(bullet("Conversations are saved on this computer and can be resumed later. "
                + "Ask the assistant to export a diagnostic case to save findings and evidence to a file.")));
        return centredCard(card);
    }

    /** The card shown while the ChatGPT sign-in runs in the browser. */
    private JComponent createSignInPage() {
        JLabel title = new JLabel("Finish signing in in your browser");
        title.setFont(boldFont(3));
        JTextArea description = muted(paragraph("A ChatGPT sign-in page has opened. Come back here when it says "
                + "you're done: this page updates by itself."));
        signInHint.setFont(smallFont());
        muted(signInHint);
        signInLink.setEditable(false);
        signInLink.setFont(ConsoleFonts.mono(Font.PLAIN, ConsoleFonts.baseSize() - 1));
        signInLink.getAccessibleContext().setAccessibleName("Sign-in link");
        signInLink.setPreferredSize(new Dimension(CARD_WIDTH - 200, JoinedButtons.HEIGHT));
        copyLink.setToolTipText("Copy the sign-in link to paste it into a browser");
        copyLink.addActionListener(e -> {
            if (signInUri != null) {
                Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(signInUri.toString()), null);
            }
        });
        openLink.setToolTipText("Open the sign-in link in your browser again");
        openLink.addActionListener(e -> {
            try {
                if (signInUri != null && Desktop.isDesktopSupported()) { Desktop.getDesktop().browse(signInUri); }
            } catch (Exception ignored) { }
        });
        JPanel linkRow = new JPanel(new BorderLayout(6, 0));
        linkRow.setOpaque(false);
        linkRow.add(signInLink, BorderLayout.CENTER);
        linkRow.add(row(copyLink, openLink), BorderLayout.EAST);

        JPanel card = verticalBox();
        card.add(left(title));
        card.add(Box.createVerticalStrut(6));
        card.add(left(description));
        card.add(Box.createVerticalStrut(18));
        card.add(left(signInHint));
        card.add(Box.createVerticalStrut(4));
        card.add(left(linkRow));
        card.add(Box.createVerticalStrut(18));
        card.add(left(row(cancelSignIn)));
        return centredCard(card);
    }

    /** The conversation: title and model on top, transcript, then the message box. */
    private JComponent createChatPage() {
        conversationTitle.setFont(boldFont(2));
        models.setPreferredSize(new Dimension(220, JoinedButtons.HEIGHT));
        models.setToolTipText("ChatGPT model for this conversation");
        JLabel modelLabel = muted(new JLabel("Model"));
        modelLabel.setLabelFor(models);
        JPanel header = new JPanel(new BorderLayout(12, 0));
        header.setBackground(cardBackground());
        header.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createMatteBorder(0, 0, 1, 0, cardBorder()),
                BorderFactory.createEmptyBorder(0, 16, 0, 12)));
        header.setPreferredSize(new Dimension(0, HEADER_HEIGHT));
        ecuStatus.setIcon(ecuDot);
        ecuStatus.setIconTextGap(6);
        ecuStatus.setFont(smallFont());
        muted(ecuStatus);
        JPanel titles = new JPanel(new GridBagLayout());
        titles.setOpaque(false);
        GridBagConstraints c = new GridBagConstraints();
        c.gridx = 0;
        c.anchor = GridBagConstraints.WEST;
        c.weightx = 1;
        c.fill = GridBagConstraints.HORIZONTAL;
        titles.add(conversationTitle, c);
        titles.add(ecuStatus, c);
        header.add(titles, BorderLayout.CENTER);
        // GridBagLayout centres the picker vertically in the band
        JPanel picker = new JPanel(new GridBagLayout());
        picker.setOpaque(false);
        exportCase.setToolTipText("Ask the assistant to save its findings and the evidence it collected "
                + "as a diagnostic case file");
        picker.add(row(exportCase, modelLabel, models));
        header.add(picker, BorderLayout.EAST);

        chat.setFont(UIManager.getFont("Label.font"));
        JScrollPane chatScroll = new JScrollPane(chat, ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED,
                ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        chatScroll.setBorder(BorderFactory.createEmptyBorder());
        chatScroll.setMinimumSize(new Dimension(0, 120));

        prompt.setLineWrap(true);
        prompt.setWrapStyleWord(true);
        prompt.setMargin(new Insets(4, 4, 4, 4));
        prompt.setBackground(cardBackground());
        prompt.getAccessibleContext().setAccessibleName("Message to ChatGPT");
        prompt.setToolTipText("Type a message, then press Enter or click Send. Shift+Enter starts a new line.");
        // Enter sends, as in chat apps; Shift+Enter is the text area's usual new line, Ctrl+Enter still sends
        InputMap promptKeys = prompt.getInputMap(JComponent.WHEN_FOCUSED);
        promptKeys.put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), "send-prompt");
        promptKeys.put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, InputEvent.CTRL_DOWN_MASK), "send-prompt");
        promptKeys.put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, InputEvent.SHIFT_DOWN_MASK),
                javax.swing.text.DefaultEditorKit.insertBreakAction);
        prompt.getActionMap().put("send-prompt", new AbstractAction() {
            @Override public void actionPerformed(ActionEvent e) {
                if (send.isEnabled()) { send(); }
            }
        });
        send.setToolTipText("Send the message (Enter from the message field)");
        send.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        stop.setToolTipText("Stop the running request");
        // Send and Stop take turns in one slot, so the message box keeps its size
        int slotWidth = Math.max(send.getPreferredSize().width, stop.getPreferredSize().width) + 32;
        sendSlot.setOpaque(false);
        sendSlot.setPreferredSize(new Dimension(slotWidth, JoinedButtons.HEIGHT));
        sendSlot.add(send, "send");
        sendSlot.add(stop, "stop");
        JPanel slotColumn = new JPanel(new BorderLayout());
        slotColumn.setOpaque(false);
        slotColumn.add(sendSlot, BorderLayout.SOUTH);
        JScrollPane promptScroll = new JScrollPane(prompt);
        promptScroll.setBorder(BorderFactory.createEmptyBorder());
        attach.setToolTipText("<html>Attach text files (logs, .msl, .csv, Lua, .ini) or pictures to the next message.<br>"
                + "You can also paste a screenshot or copied files with Ctrl+V, or drop files here.</html>");
        attach.getAccessibleContext().setAccessibleName("Attach files");
        attach.putClientProperty("JButton.buttonType", "toolBarButton");
        attach.setPreferredSize(new Dimension(JoinedButtons.HEIGHT, JoinedButtons.HEIGHT));
        attach.addActionListener(e -> chooseAttachments());
        JPanel attachColumn = new JPanel(new BorderLayout());
        attachColumn.setOpaque(false);
        attachColumn.add(attach, BorderLayout.SOUTH);
        attachments.setBorder(BorderFactory.createEmptyBorder(2, 2, 6, 0));
        attachments.setOnChange(() -> {
            prompt.revalidate();
            updateControls();
        });
        // pasting or dropping files or a picture attaches them; text pastes as before
        prompt.setTransferHandler(AttachmentBar.handler(prompt.getTransferHandler(), this::attachFiles, this::attachPicture));
        chat.setTransferHandler(AttachmentBar.handler(null, this::attachFiles, this::attachPicture));
        JPanel box = new JPanel(new BorderLayout(8, 0));
        box.setBackground(cardBackground());
        box.setBorder(new FlatLineBorder(new Insets(6, 4, 6, 6), cardBorder(), 1, 8));
        box.add(attachments, BorderLayout.NORTH);
        box.add(attachColumn, BorderLayout.WEST);
        box.add(promptScroll, BorderLayout.CENTER);
        box.add(slotColumn, BorderLayout.EAST);

        JLabel privacy = muted(new JLabel(PRIVACY_NOTE));
        privacy.setFont(smallFont());
        JLabel keys = muted(new JLabel("Enter to send, Shift+Enter for a new line"));
        keys.setFont(smallFont());
        JPanel footer = new JPanel(new BorderLayout(12, 0));
        footer.setOpaque(false);
        footer.add(privacy, BorderLayout.CENTER);
        footer.add(keys, BorderLayout.EAST);
        JPanel input = new JPanel(new BorderLayout(0, 4));
        input.setOpaque(false);
        input.add(box, BorderLayout.CENTER);
        input.add(footer, BorderLayout.SOUTH);
        input.setMinimumSize(new Dimension(0, JoinedButtons.HEIGHT + 30));

        // drag the divider up to give the message box more room
        JSplitPane split = new JSplitPane(JSplitPane.VERTICAL_SPLIT, chatScroll, input);
        split.setResizeWeight(1);
        split.setContinuousLayout(true);
        split.setDividerSize(10);
        split.setBorder(BorderFactory.createEmptyBorder());
        split.setOpaque(false);

        JPanel page = new JPanel(new BorderLayout());
        page.setBorder(BorderFactory.createEmptyBorder(12, 16, 12, 16));
        page.add(split, BorderLayout.CENTER);
        JPanel chat = new JPanel(new BorderLayout());
        chat.add(header, BorderLayout.NORTH);
        chat.add(page, BorderLayout.CENTER);
        return chat;
    }

    /** One checklist row: badge and title, with the step's controls below when it has any. */
    private static JComponent step(Icon badge, JLabel title, JComponent trailing, JComponent body, boolean separated) {
        JPanel header = new JPanel(new BorderLayout(10, 0));
        header.setOpaque(false);
        header.add(new JLabel(badge), BorderLayout.WEST);
        header.add(title, BorderLayout.CENTER);
        if (trailing != null) { header.add(trailing, BorderLayout.EAST); }
        JPanel step = new JPanel(new BorderLayout(0, 10));
        step.setOpaque(false);
        step.setBorder(BorderFactory.createCompoundBorder(
                separated ? BorderFactory.createMatteBorder(0, 0, 1, 0, cardBorder()) : BorderFactory.createEmptyBorder(),
                BorderFactory.createEmptyBorder(12, 14, 12, 14)));
        step.add(header, BorderLayout.NORTH);
        if (body != null) {
            // the body lines up with the title, right of the badge
            body.setBorder(BorderFactory.createEmptyBorder(0, 32, 0, 0));
            step.add(body, BorderLayout.CENTER);
        }
        step.setAlignmentX(Component.LEFT_ALIGNMENT);
        return step;
    }

    private static JComponent bullet(String text) {
        JPanel bullet = new JPanel(new BorderLayout(8, 0));
        bullet.setOpaque(false);
        bullet.setBorder(BorderFactory.createEmptyBorder(2, 0, 2, 0));
        JLabel dot = new JLabel("\u2022");
        dot.setVerticalAlignment(SwingConstants.TOP);
        bullet.add(dot, BorderLayout.WEST);
        bullet.add(paragraph(text), BorderLayout.CENTER);
        return bullet;
    }

    /**
     * Read-only text that wraps to the width it is given. An HTML label would need a fixed CSS width, which Swing
     * scales differently from the layout, so the text would overflow the card.
     */
    private static JTextArea paragraph(String text) {
        JTextArea area = new JTextArea(text);
        area.setEditable(false);
        area.setFocusable(false);
        area.setLineWrap(true);
        area.setWrapStyleWord(true);
        area.setOpaque(false);
        area.setBorder(BorderFactory.createEmptyBorder());
        area.setFont(UIManager.getFont("Label.font"));
        area.setForeground(UIManager.getColor("Label.foreground"));
        return area;
    }

    /** {@code body} on a rounded card of fixed width, centred in the page; its height follows what is shown. */
    private static JComponent centredCard(JComponent body) {
        JPanel card = new JPanel(new BorderLayout()) {
            @Override public Dimension getPreferredSize() {
                return new Dimension(CARD_WIDTH, super.getPreferredSize().height);
            }
        };
        // FlatPanelUI paints a rounded background when the border is a FlatLineBorder with an arc
        card.setBorder(new FlatLineBorder(new Insets(24, 28, 24, 28), cardBorder(), 1, 10));
        card.setBackground(cardBackground());
        card.add(body, BorderLayout.CENTER);
        JPanel page = new JPanel(new GridBagLayout());
        page.setBorder(BorderFactory.createEmptyBorder(16, 16, 16, 16));
        page.add(card);
        JScrollPane scroll = new JScrollPane(page);
        scroll.setBorder(BorderFactory.createEmptyBorder());
        return scroll;
    }

    /**
     * Saved conversations, newest first, each with its time below the title; the open one is highlighted.
     * A click opens a conversation, the bin on the selected or hovered row deletes it, and so do Enter and Delete.
     */
    static final class ConversationList extends JList<TroubleshootingSessionStore.Summary> {
        private static final int BIN_WIDTH = 28;
        Consumer<TroubleshootingSessionStore.Summary> onOpen = summary -> { };
        Consumer<TroubleshootingSessionStore.Summary> onDelete = summary -> { };
        private int hovered = -1;

        ConversationList(ListModel<TroubleshootingSessionStore.Summary> model) {
            super(model);
            setOpaque(false);
            setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
            setCellRenderer(new Row());
            setBorder(BorderFactory.createEmptyBorder(0, 6, 8, 6));
            getAccessibleContext().setAccessibleName("Saved conversations");
            // a row keeps the sidebar's width; a long title ends in "..."
            setFixedCellWidth(1);
            setToolTipText("Click to open a saved conversation");
            MouseAdapter mouse = new MouseAdapter() {
                @Override public void mouseMoved(MouseEvent e) {
                    int index = rowAt(e.getPoint());
                    if (index != hovered) {
                        hovered = index;
                        repaint();
                    }
                }
                @Override public void mouseExited(MouseEvent e) {
                    hovered = -1;
                    repaint();
                }
                @Override public void mouseReleased(MouseEvent e) {
                    int index = rowAt(e.getPoint());
                    if (!isEnabled() || index < 0 || !SwingUtilities.isLeftMouseButton(e)) { return; }
                    Rectangle bounds = getCellBounds(index, index);
                    if (e.getX() >= bounds.x + bounds.width - BIN_WIDTH) {
                        onDelete.accept(getModel().getElementAt(index));
                    } else {
                        onOpen.accept(getModel().getElementAt(index));
                    }
                }
            };
            addMouseListener(mouse);
            addMouseMotionListener(mouse);
            getInputMap(WHEN_FOCUSED).put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), "open-conversation");
            getActionMap().put("open-conversation", new AbstractAction() {
                @Override public void actionPerformed(ActionEvent e) {
                    if (isEnabled() && getSelectedValue() != null) { onOpen.accept(getSelectedValue()); }
                }
            });
            getInputMap(WHEN_FOCUSED).put(KeyStroke.getKeyStroke(KeyEvent.VK_DELETE, 0), "delete-conversation");
            getActionMap().put("delete-conversation", new AbstractAction() {
                @Override public void actionPerformed(ActionEvent e) {
                    if (isEnabled() && getSelectedValue() != null) { onDelete.accept(getSelectedValue()); }
                }
            });
        }

        private int rowAt(Point point) {
            int index = locationToIndex(point);
            return index >= 0 && getCellBounds(index, index).contains(point) ? index : -1;
        }

        /** Highlights the conversation with this id, or none. */
        void showActive(String id) {
            for (int index = 0; index < getModel().getSize(); index++) {
                if (getModel().getElementAt(index).id.equals(id)) {
                    if (getSelectedIndex() != index) { setSelectedIndex(index); }
                    return;
                }
            }
            clearSelection();
        }

        @Override public String getToolTipText(MouseEvent event) {
            int index = rowAt(event.getPoint());
            if (index < 0) { return null; }
            Rectangle bounds = getCellBounds(index, index);
            return event.getX() >= bounds.x + bounds.width - BIN_WIDTH ? "Delete this conversation"
                    : getModel().getElementAt(index).title;
        }

        @Override protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            if (getModel().getSize() > 0) { return; }
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
                g2.setColor(themed("Label.disabledForeground", Color.GRAY));
                g2.setFont(getFont());
                Insets insets = getInsets();
                g2.drawString("No saved conversations yet", insets.left + 6, insets.top + g2.getFontMetrics().getAscent() + 4);
            } finally {
                g2.dispose();
            }
        }

        private final class Row extends JPanel implements ListCellRenderer<TroubleshootingSessionStore.Summary> {
            private final JLabel title = new JLabel();
            private final JLabel when = new JLabel();
            private boolean showBin;

            Row() {
                super(new GridLayout(2, 1));
                setBorder(BorderFactory.createEmptyBorder(6, 6, 6, BIN_WIDTH));
                when.setFont(smallFont());
                muted(when);
                add(title);
                add(when);
            }

            @Override public Component getListCellRendererComponent(JList<? extends TroubleshootingSessionStore.Summary> list,
                                                                    TroubleshootingSessionStore.Summary value, int index,
                                                                    boolean isSelected, boolean cellHasFocus) {
                title.setText(value.title);
                title.setFont(isSelected ? boldFont(0) : UIManager.getFont("Label.font"));
                title.setForeground(UIManager.getColor("Label.foreground"));
                when.setText(relativeTime(value.updatedAt));
                setOpaque(isSelected || index == hovered);
                setBackground(isSelected ? themed("Console.Status.accent.background", Color.LIGHT_GRAY)
                        : themed("List.hoverBackground", Color.LIGHT_GRAY));
                showBin = list.isEnabled() && (isSelected || index == hovered);
                return this;
            }

            @Override protected void paintComponent(Graphics g) {
                super.paintComponent(g);
                if (showBin) {
                    TroubleshootingIcons.paintTrash(g, getWidth() - BIN_WIDTH + 4, (getHeight() - 16) / 2);
                }
            }
        }
    }

    /** "Today, 14:02", "Yesterday, 09:15", "28 Oct" or "28 Oct 2025". */
    static String relativeTime(long epochMillis) {
        ZonedDateTime time = Instant.ofEpochMilli(epochMillis).atZone(ZoneId.systemDefault());
        LocalDate today = LocalDate.now(ZoneId.systemDefault());
        LocalDate day = time.toLocalDate();
        if (day.equals(today)) { return "Today, " + time.format(DateTimeFormatter.ofPattern("HH:mm")); }
        if (day.equals(today.minusDays(1))) { return "Yesterday, " + time.format(DateTimeFormatter.ofPattern("HH:mm")); }
        return time.format(DateTimeFormatter.ofPattern(day.getYear() == today.getYear() ? "d MMM" : "d MMM yyyy"));
    }

    /**
     * A button as tall as the console's other controls and a few pixels wider than its text needs. At
     * fractional display scaling (125%, 150%) Java paints text wider than it measures it, so an exactly
     * fitted label ends in "...".
     */
    private static JButton roomyButton(String text) {
        return new JButton(text) {
            @Override public Dimension getPreferredSize() {
                Dimension size = super.getPreferredSize();
                if (!isPreferredSizeSet()) {
                    size.width += Math.max(16, getFontMetrics(getFont()).stringWidth(getText()) / 8);
                    size.height = JoinedButtons.HEIGHT;
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

    private static JPanel row(JComponent... components) {
        JPanel row = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
        row.setOpaque(false);
        for (int index = 0; index < components.length; index++) {
            if (index > 0) { row.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP)); }
            row.add(components[index]);
        }
        return row;
    }

    private static JPanel verticalBox() {
        JPanel box = new JPanel();
        box.setLayout(new BoxLayout(box, BoxLayout.Y_AXIS));
        box.setOpaque(false);
        return box;
    }

    private static <T extends JComponent> T left(T component) {
        component.setAlignmentX(Component.LEFT_ALIGNMENT);
        return component;
    }

    private static <T extends JComponent> T muted(T component) {
        component.setForeground(UIManager.getColor("Label.disabledForeground"));
        return component;
    }

    private static Font boldFont(int larger) {
        Font base = UIManager.getFont("Label.font");
        return base == null ? null : base.deriveFont(Font.BOLD, base.getSize2D() + larger);
    }

    private static Font smallFont() {
        Font base = UIManager.getFont("Label.font");
        return base == null ? null : base.deriveFont(base.getSize2D() - 1);
    }

    /** A plain Color, not the theme's UIResource, so panels and read-only text keep it. */
    private static Color cardBackground() {
        return new Color(themed("Console.Card.background", Color.WHITE).getRGB());
    }

    private static Color cardBorder() {
        return themed("Console.Card.borderColor", Color.LIGHT_GRAY);
    }

    private static Color themed(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }

    public JPanel getContent() { return content; }

    /** The conversation and the message being typed, the text Ctrl+wheel resizes. */
    public JComponent[] getZoomableText() {
        return new JComponent[]{chat, prompt};
    }

    /** The saved-conversation list; package-private for tests. */
    ConversationList conversationsForTest() { return conversations; }

    /** The chosen account; package-private for tests. */
    Account selectedAccountForTest() { return (Account) accounts.getSelectedItem(); }

    /** Lets the user pick files to attach to the next message. */
    private void chooseAttachments() {
        JFileChooser chooser = new JFileChooser(attachFolder);
        chooser.setDialogTitle("Attach files");
        chooser.setMultiSelectionEnabled(true);
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        if (chooser.showOpenDialog(content) != JFileChooser.APPROVE_OPTION) {
            return;
        }
        attachFolder = chooser.getCurrentDirectory();
        List<Path> paths = new ArrayList<>();
        for (File file : chooser.getSelectedFiles()) {
            paths.add(file.toPath());
        }
        attachFiles(paths);
    }

    /** Reads files off the event thread; each lands as a chip, or the bar says why it could not be attached. */
    void attachFiles(List<Path> paths) {
        attachments.setNote(null);
        for (Path path : paths) {
            if (java.nio.file.Files.isDirectory(path)) {
                attachments.setNote(path.getFileName() + " is a folder; attach the files in it instead.");
                continue;
            }
            attachInBackground(() -> Attachment.fromFile(path));
        }
    }

    void attachPicture(Image picture) {
        attachments.setNote(null);
        String name = "Pasted picture " + (++pastedPictures) + ".png";
        attachInBackground(() -> Attachment.fromImage(picture, name));
    }

    private void attachInBackground(java.util.concurrent.Callable<Attachment> load) {
        new SwingWorker<Attachment, Void>() {
            @Override protected Attachment doInBackground() throws Exception {
                return load.call();
            }

            @Override protected void done() {
                if (closed) { return; }
                try {
                    attachments.attach(get());
                } catch (java.util.concurrent.ExecutionException e) {
                    attachments.setNote(e.getCause() instanceof IOException ? e.getCause().getMessage()
                            : "Could not attach the file.");
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }.execute();
    }

    /** The attachment bar; package-private for tests. */
    AttachmentBar attachmentsForTest() { return attachments; }

    /** A line from the tab itself between the messages, e.g. that a turn was stopped. */
    private void notice(String text) {
        if (closed || text == null) { return; }
        chat.addNotice(text);
    }

    private void browse(URI uri, Cancellation cancellation) {
        if (!AuthDialogLifecycle.mayBrowse(cancellation, running, closed)) { return; }
        signInUri = uri;
        signInLink.setText(uri.toString());
        signInLink.setCaretPosition(0);
        updateControls();
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

    /** The browser did not open: the sign-in card says so, and its link is the way in. */
    private AuthDialogHandle showFallbackNotice(URI uri, Runnable onCancel) {
        signInHint.setText("Your browser did not open. Copy this link into it:");
        signInHint.setForeground(themed("Console.Status.pending.foreground", Color.DARK_GRAY));
        return () -> {
            signInHint.setText("Browser didn't open? Copy this link:");
            muted(signInHint);
        };
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
        ConnectionStatusLogic.INSTANCE.removeListener(ecuListener);
        authDialogs.close();
        worker.execute(() -> {
            if (client != null) {
                try { client.close(); } catch (Exception ignored) { }
            }
        });
        worker.shutdown();
    }

}
