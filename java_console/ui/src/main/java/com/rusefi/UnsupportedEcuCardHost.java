package com.rusefi;

import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.io.UnsupportedEcuInfo;
import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.ui.util.URLLabel;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.function.Consumer;

/** Full-window, hardware-driven blocker shared by startup and the full console. */
public final class UnsupportedEcuCardHost implements LinkManager.EcuCompatibilityListener {
    private static final String NORMAL = "normal";
    private static final String UNSUPPORTED = "unsupported";

    private static final class Blocker {
        final UnsupportedEcuInfo info;
        final boolean typed;

        private Blocker(UnsupportedEcuInfo info, boolean typed) {
            this.info = info;
            this.typed = typed;
        }
    }

    private final PortScanner portScanner;
    private final JPanel content = new JPanel(new CardLayout());
    private final JPanel normalContent = new JPanel(new BorderLayout());
    private final JTextArea detectedTargets = new JTextArea();
    private final JTextArea detectedPorts = new JTextArea();
    private final JLabel bundleTarget = new JLabel();
    private final JButton downloadButton = new JButton("Download compatible bundle");
    private final Map<String, Blocker> blockers = new HashMap<>();
    private final Map<String, String> compatibleIdentities = new HashMap<>();
    private final Set<String> presentPorts = new HashSet<>();
    private final List<Consumer<Boolean>> blockingListeners = new CopyOnWriteArrayList<>();
    private final List<Consumer<PortResult>> compatiblePortListeners = new CopyOnWriteArrayList<>();

    private volatile boolean blocking;
    private volatile String downloadUrl;
    private boolean renderedBlocking;
    private long renderVersion;

    public UnsupportedEcuCardHost(ConnectivityContext connectivityContext, LinkManager linkManager) {
        portScanner = connectivityContext.getPortScanner();
        content.add(normalContent, NORMAL);
        content.add(createUnsupportedPanel(), UNSUPPORTED);
        linkManager.setEcuCompatibilityListener(this);
        portScanner.addListener(this::onHardwareChanged);
        onHardwareChanged(portScanner.getCurrentHardware());
    }

    public JComponent getContent() {
        return content;
    }

    public JPanel getNormalContent() {
        return normalContent;
    }

    public void setNormalContent(JComponent component) {
        normalContent.removeAll();
        normalContent.add(component, BorderLayout.CENTER);
        normalContent.revalidate();
        normalContent.repaint();
    }

    public boolean isBlocking() {
        return blocking;
    }

    public synchronized boolean isBlocked(String port) {
        return blockers.containsKey(port);
    }

    public void addBlockingListener(Consumer<Boolean> listener) {
        blockingListeners.add(listener);
        runOnEdt(() -> listener.accept(blocking));
    }

    public void addCompatiblePortListener(Consumer<PortResult> listener) {
        compatiblePortListeners.add(listener);
    }

    @Override
    public void onUnsupportedEcu(String port, UnsupportedEcuInfo info) {
        if (port == null) {
            return;
        }
        synchronized (this) {
            if (!isPortPresentLocked(port)) {
                return;
            }
            blockers.put(port, new Blocker(info, true));
            refreshLocked();
        }
        // A watchdog reconnect can discover a replacement ECU while the scanner still holds the old ECU.
        portScanner.invalidatePort(port);
    }

    @Override
    public void onCompatibleEcu(String port, RusEfiSignature signature) {
        if (port == null) {
            return;
        }
        boolean identityChanged;
        synchronized (this) {
            boolean wasBlocked = blockers.remove(port) != null;
            String identity = signature == null ? "" : signature.toString();
            String previousIdentity = compatibleIdentities.put(port, identity);
            identityChanged = wasBlocked
                || (previousIdentity != null && !previousIdentity.equals(identity));
            refreshLocked();
        }
        if (identityChanged) {
            // The live stream proves compatibility but cannot safely be probed for DFU/OpenBLT support.
            PortResult recoveredPort = new PortResult(port, SerialPortType.EcuUnknown);
            portScanner.cachePort(recoveredPort);
            for (Consumer<PortResult> listener : compatiblePortListeners) {
                listener.accept(recoveredPort);
            }
        }
    }

    private void onHardwareChanged(AvailableHardware hardware) {
        Map<String, PortResult> ports = new HashMap<>();
        for (PortResult port : hardware.getKnownPorts()) {
            ports.put(port.port, port);
        }

        synchronized (this) {
            presentPorts.clear();
            presentPorts.addAll(ports.keySet());
            blockers.entrySet().removeIf(entry -> !ports.containsKey(entry.getKey())
                && !(entry.getValue().typed && LinkManager.isSpecialNotSerial(entry.getKey())));
            compatibleIdentities.keySet().removeIf(port -> !ports.containsKey(port));
            for (PortResult port : ports.values()) {
                Blocker current = blockers.get(port.port);
                if (port.isUnsupportedEcu() && port.getUnsupportedEcuInfo() != null
                    && (current == null || !current.typed)) {
                    blockers.put(port.port, new Blocker(port.getUnsupportedEcuInfo(), false));
                } else if (port.isEcu() && current != null && !current.typed) {
                    blockers.remove(port.port);
                }
                if (port.isEcu() && port.getSignature() != null) {
                    compatibleIdentities.put(port.port, port.getSignature().toString());
                }
            }
            refreshLocked();
        }
    }

    private boolean isPortPresentLocked(String port) {
        if (presentPorts.contains(port)) {
            return true;
        }
        if (TcpConnector.isTcpPort(port)) {
            return TcpConnector.isPortOpened(port);
        }
        return !LinkManager.isSpecialNotSerial(port) && LinkManager.getCommPorts().contains(port);
    }

    private void refreshLocked() {
        List<Map.Entry<String, Blocker>> snapshot = new ArrayList<>(blockers.entrySet());
        snapshot.sort(Comparator.comparing(Map.Entry::getKey));
        blocking = !snapshot.isEmpty();
        long version = ++renderVersion;

        runOnEdt(() -> {
            synchronized (UnsupportedEcuCardHost.this) {
                if (version != renderVersion) {
                    return;
                }
            }
            render(snapshot);
            boolean snapshotBlocking = !snapshot.isEmpty();
            if (renderedBlocking != snapshotBlocking) {
                renderedBlocking = snapshotBlocking;
                for (Consumer<Boolean> listener : blockingListeners) {
                    listener.accept(snapshotBlocking);
                }
            }
        });
    }

    private void render(List<Map.Entry<String, Blocker>> snapshot) {
        StringBuilder targets = new StringBuilder();
        StringBuilder ports = new StringBuilder();
        String bundle = "";
        String resolvedDownloadUrl = null;
        for (Map.Entry<String, Blocker> entry : snapshot) {
            if (targets.length() > 0) {
                targets.append('\n');
                ports.append('\n');
            }
            UnsupportedEcuInfo info = entry.getValue().info;
            targets.append(info.getEcuTarget());
            ports.append(entry.getKey());
            bundle = info.getBundleTarget();
            resolvedDownloadUrl = info.getDownloadUrl();
        }
        detectedTargets.setText(targets.toString());
        detectedPorts.setText(ports.toString());
        bundleTarget.setText(bundle);
        downloadUrl = resolvedDownloadUrl;
        downloadButton.setEnabled(resolvedDownloadUrl != null && !resolvedDownloadUrl.trim().isEmpty());
        CardLayout cards = (CardLayout) content.getLayout();
        cards.show(content, snapshot.isEmpty() ? NORMAL : UNSUPPORTED);
        content.revalidate();
        content.repaint();
    }

    private JPanel createUnsupportedPanel() {
        Color surface = color("TextField.background", Color.WHITE);
        Color border = color("Component.borderColor", new Color(0xc8ccd2));
        Color muted = color("Label.disabledForeground", new Color(0x6b7280));
        Color danger = color("Actions.Red", new Color(0xb42318));
        Color accent = color("Component.accentColor", new Color(0x2f6fed));

        JPanel card = new JPanel();
        card.setLayout(new BoxLayout(card, BoxLayout.Y_AXIS));
        card.setBackground(surface);
        card.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(border),
            BorderFactory.createEmptyBorder(36, 44, 34, 44)));

        JLabel status = new JLabel("CONNECTION BLOCKED");
        status.setForeground(danger);
        status.setFont(status.getFont().deriveFont(Font.BOLD, status.getFont().getSize() * 0.9f));
        status.setAlignmentX(Component.LEFT_ALIGNMENT);
        card.add(status);
        card.add(Box.createVerticalStrut(14));

        JLabel title = new JLabel("Unsupported ECU", UIManager.getIcon("OptionPane.warningIcon"), JLabel.LEFT);
        title.setIconTextGap(14);
        title.setFont(title.getFont().deriveFont(Font.BOLD, title.getFont().getSize() * 2.0f));
        title.setAlignmentX(Component.LEFT_ALIGNMENT);
        card.add(title);
        card.add(Box.createVerticalStrut(14));

        JTextArea explanation = transparentText(
            "This console bundle does not support the detected controller. " +
                "Connection, tuning, and firmware updates are blocked to protect the ECU.");
        explanation.setColumns(48);
        explanation.setForeground(muted);
        explanation.setAlignmentX(Component.LEFT_ALIGNMENT);
        card.add(explanation);
        card.add(Box.createVerticalStrut(26));

        JPanel details = new JPanel(new GridBagLayout());
        details.setBackground(color("Panel.background", new Color(0xf5f6f8)));
        details.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(border),
            BorderFactory.createEmptyBorder(16, 18, 16, 18)));
        details.setAlignmentX(Component.LEFT_ALIGNMENT);

        JLabel targetHeader = new JLabel("ECU TARGET");
        JLabel portHeader = new JLabel("PORT");
        JLabel bundleHeader = new JLabel("CURRENT BUNDLE");
        for (JLabel header : new JLabel[]{targetHeader, portHeader, bundleHeader}) {
            header.setForeground(muted);
            header.setFont(header.getFont().deriveFont(Font.BOLD, header.getFont().getSize() * 0.85f));
        }

        for (JTextArea values : new JTextArea[]{detectedTargets, detectedPorts}) {
            values.setEditable(false);
            values.setOpaque(false);
            values.setFocusable(false);
            values.setFont(values.getFont().deriveFont(Font.BOLD, values.getFont().getSize() * 1.15f));
        }
        bundleTarget.setFont(bundleTarget.getFont().deriveFont(Font.BOLD));

        GridBagConstraints cell = new GridBagConstraints();
        cell.anchor = GridBagConstraints.WEST;
        cell.gridx = 0;
        cell.gridy = 0;
        cell.insets = new Insets(0, 0, 7, 32);
        details.add(targetHeader, cell);
        cell.gridx = 1;
        cell.weightx = 1;
        cell.insets = new Insets(0, 0, 7, 0);
        details.add(portHeader, cell);
        cell.gridx = 0;
        cell.gridy = 1;
        cell.weightx = 0;
        cell.insets = new Insets(0, 0, 14, 32);
        details.add(detectedTargets, cell);
        cell.gridx = 1;
        cell.weightx = 1;
        cell.insets = new Insets(0, 0, 14, 0);
        details.add(detectedPorts, cell);
        cell.gridx = 0;
        cell.gridy = 2;
        cell.weightx = 0;
        cell.insets = new Insets(0, 0, 0, 32);
        details.add(bundleHeader, cell);
        cell.gridx = 1;
        cell.weightx = 1;
        cell.insets = new Insets(0, 0, 0, 0);
        details.add(bundleTarget, cell);
        card.add(details);
        card.add(Box.createVerticalStrut(24));

        downloadButton.putClientProperty("JButton.buttonType", "roundRect");
        downloadButton.setBackground(accent);
        downloadButton.setForeground(Color.WHITE);
        downloadButton.setFont(downloadButton.getFont().deriveFont(Font.BOLD));
        downloadButton.setMargin(new Insets(10, 20, 10, 20));
        downloadButton.setAlignmentX(Component.LEFT_ALIGNMENT);
        downloadButton.setEnabled(false);
        downloadButton.addActionListener(e -> {
            String url = downloadUrl;
            if (url != null && !url.trim().isEmpty()) {
                URLLabel.open(url);
            }
        });
        card.add(downloadButton);
        card.add(Box.createVerticalStrut(16));

        JLabel instruction = new JLabel("Disconnect every unsupported ECU to continue.");
        instruction.setForeground(muted);
        instruction.setAlignmentX(Component.LEFT_ALIGNMENT);
        card.add(instruction);

        JPanel centered = new JPanel(new GridBagLayout());
        centered.setBorder(BorderFactory.createEmptyBorder(28, 28, 28, 28));
        centered.add(card, new GridBagConstraints());
        return centered;
    }

    private static JTextArea transparentText(String text) {
        JTextArea area = new JTextArea(text);
        area.setEditable(false);
        area.setOpaque(false);
        area.setFocusable(false);
        area.setLineWrap(true);
        area.setWrapStyleWord(true);
        area.setFont(UIManager.getFont("Label.font"));
        return area;
    }

    private static Color color(String key, Color fallback) {
        Color value = UIManager.getColor(key);
        return value != null ? value : fallback;
    }

    private static void runOnEdt(Runnable runnable) {
        if (SwingUtilities.isEventDispatchThread()) {
            runnable.run();
        } else {
            SwingUtilities.invokeLater(runnable);
        }
    }
}
