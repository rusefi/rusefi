package com.rusefi.ui;

import com.camick.RXTextUtilities;
import com.devexperts.logging.Logging;
import com.rusefi.CodeWalkthrough;
import com.rusefi.config.Field;
import com.rusefi.enums.live_data_e;
import com.rusefi.ldmp.StateDictionary;
import com.rusefi.livedata.LiveDataParserPanel;
import com.rusefi.livedata.ParseResult;
import com.rusefi.livedata.generated.CPP14Parser;
import com.rusefi.ui.util.UiUtils;
import net.miginfocom.swing.MigLayout;
import org.jetbrains.annotations.NotNull;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import javax.swing.text.BadLocationException;
import javax.swing.tree.*;
import java.awt.*;
import java.awt.event.*;

import static com.devexperts.logging.Logging.getLogging;


/**
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see LiveDataParserPanel
 * See LiveDataPaneSandbox
 */
public class LiveDataPane {
    private static final Logging log = getLogging(LiveDataPane.class);

    public static final String CPP_SUFFIX = ".cpp";

    /**
     * this is the panel we expose to the outside world
     */
    private final JPanel content = new JPanel(new BorderLayout());
    private boolean isPaused;

    private LiveDataPane(UIContext uiContext) {
        long start = System.currentTimeMillis();

        JPanel vertical = new JPanel(new MigLayout("wrap 1", "[grow,fill]"));
        vertical.setBorder(BorderFactory.createLineBorder(Color.orange));
        JScrollPane scroll = new JScrollPane(vertical, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        JPanel legend = populateLegend();

        DefaultMutableTreeNode root = new DefaultMutableTreeNode();
        for (live_data_e view : live_data_e.values()) {
            String fileName = StateDictionary.INSTANCE.getFileName(view) + CPP_SUFFIX;
            Field[] values = StateDictionary.INSTANCE.getFields(view);
            LiveDataParserPanel liveDataParserPanel = LiveDataParserPanel.createLiveDataParserPanel(uiContext, view, values, fileName);
            ParseResult parseResult = liveDataParserPanel.getParseResult();
            JPanel liveDataParserContent = liveDataParserPanel.getContent();

            DefaultMutableTreeNode child = new DefaultMutableTreeNode(fileName);
            child.setUserObject(new PanelAndName(liveDataParserContent, fileName));

            for (CPP14Parser.UnqualifiedIdContext functionId : parseResult.getFunctions()) {
                DefaultMutableTreeNode methodNode = new DefaultMutableTreeNode();
                methodNode.setUserObject(new SpecificMethod(liveDataParserPanel, functionId));
                child.add(methodNode);
            }

            root.add(child);

            vertical.add(liveDataParserContent, "grow, wrap");
        }

        JTree tree = new JTree(new DefaultTreeModel(root));
        tree.setRootVisible(false);
        tree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);

        MouseListener ml = new MouseAdapter() {
            public void mouseClicked(MouseEvent e) {
                DefaultMutableTreeNode node = (DefaultMutableTreeNode) tree.getLastSelectedPathComponent();
                Object payload = node.getUserObject();

                if (payload instanceof PanelAndName) {
                    PanelAndName panel = (PanelAndName) payload;
                    scrollToPanel(panel.panel, 0);
                } else if (payload instanceof SpecificMethod) {
                    SpecificMethod method = (SpecificMethod) payload;
                    JTextPane jTextPane = method.liveDataParserPanel.getText();
                    int position = RXTextUtilities.getStartOfLineOffset(jTextPane, method.method.getStart().getLine());
                    Rectangle r;
                    try {
                        r = jTextPane.modelToView(position);
                    } catch (BadLocationException ex) {
                        return;
                    }
                    scrollToPanel(method.liveDataParserPanel.getContent(), r.y);
                }
            }

            private void scrollToPanel(JPanel panel, int yOffset) {
                scroll.getVerticalScrollBar().setValue(panel.getLocation().y + yOffset);
                // we want focus there so that mouse wheel scrolling would be active
                scroll.requestFocus();
            }
        };
        tree.addMouseListener(ml);
        content.add(tree, BorderLayout.WEST);
        content.add(scroll, BorderLayout.CENTER);
        content.add(legend, BorderLayout.EAST);

/*
        JButton saveImage = UiUtils.createSaveImageButton();
        saveImage.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String fileName = Logger.getDate() + "_formulas.png";

                UiUtils.saveImageWithPrompt(fileName, formulaProxy, formulaProxy);
            }
        });
*/

        final JButton pauseButton = UiUtils.createPauseButton();
        pauseButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                isPaused = !isPaused;
                UiUtils.setPauseButtonText(pauseButton, isPaused);
            }
        });
        JPanel topButtonsPanel = new JPanel(new FlowLayout());
// todo: implement this        topButtonsPanel.add(pauseButton);
        content.add(topButtonsPanel, BorderLayout.NORTH);

        log.info("created in " + (System.currentTimeMillis() - start) + "ms");
    }

    public static InitOnFirstPaintPanel createLazy(UIContext uiContext) {
        InitOnFirstPaintPanel panel = new InitOnFirstPaintPanel() {
            @Override
            protected JPanel createContent() {
                return new LiveDataPane(uiContext).getContent();
            }
        };
        return panel;
    }

    @NotNull
    private JPanel populateLegend() {
        JPanel legend = new JPanel(new VerticalFlowLayout());
        legend.add(new JLabel("Legend:"));
        legend.add(createLabel(CodeWalkthrough.TRUE_CONDITION, "'true' condition"));
        legend.add(createLabel(CodeWalkthrough.FALSE_CONDITION, "'false' condition"));
        legend.add(createLabel(CodeWalkthrough.PASSIVE_CODE, "inactive code"));
        legend.add(createLabel(CodeWalkthrough.ACTIVE_STATEMENT, "active branch"));
        legend.add(createLabel(CodeWalkthrough.BROKEN_CODE, "No live data"));

        return legend;
    }

    @NotNull
    private JLabel createLabel(Color background, String text) {
        JLabel label = new JLabel(text);
        label.setOpaque(true);
        label.setBackground(background);
        return label;
    }

    public JPanel getContent() {
        return content;
    }

    static class PanelAndName {
        public final JPanel panel;
        public final String name;

        public PanelAndName(JPanel panel, String name) {
            this.panel = panel;
            this.name = name;
        }

        @Override
        public String toString() {
            return name;
        }
    }

    static class SpecificMethod {
        private final LiveDataParserPanel liveDataParserPanel;
        private final CPP14Parser.UnqualifiedIdContext method;

        public SpecificMethod(LiveDataParserPanel liveDataParserPanel, CPP14Parser.UnqualifiedIdContext method) {
            this.liveDataParserPanel = liveDataParserPanel;
            this.method = method;
        }

        public LiveDataParserPanel getLiveDataParserPanel() {
            return liveDataParserPanel;
        }

        @Override
        public String toString() {
            return method.getText();
        }
    }
}
