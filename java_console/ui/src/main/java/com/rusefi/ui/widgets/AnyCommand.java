package com.rusefi.ui.widgets;

import com.fathzer.soft.javaluator.DoubleEvaluator;
import com.rusefi.FileLog;
import com.rusefi.InfixConverter;
import com.rusefi.NamedThreadFactory;
import com.rusefi.core.MessagesCentral;
import com.rusefi.functional_tests.EcuTestHelper;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.RecentCommands;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.JTextFieldWithWidth;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.JTextComponent;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Function;

/**
 * Date: 3/20/13
 * Andrey Belomutskiy, (c) 2013-2020
 * @see com.rusefi.CommandControl for hard-coded commands
 */
public class AnyCommand {
    private final static ThreadFactory THREAD_FACTORY = new NamedThreadFactory("AnyCommand");
    public static final String KEY = "last_value";
    // todo: kill while killing FSIO
    private static final String DECODE_RPN = "decode_rpn";

    private final UIContext uiContext;
    private final JTextComponent text;
    private JPanel content = new JPanel(new FlowLayout(FlowLayout.LEFT));
    private boolean reentrant;
    private Listener listener;

    private AnyCommand(UIContext uiContext, final JTextComponent text, final Node config, String defaultCommand, final boolean listenToCommands, boolean withCommandCaption) {
        this.uiContext = uiContext;
        this.text = text;
        installCtrlEnterAction();
        text.setText(defaultCommand);
        content.setBorder(BorderFactory.createLineBorder(Color.PINK));
        if (withCommandCaption) {
            content.add(new JLabel("Command: "));
        }
        content.add(text);
        JButton go = new JButton("Go");
        go.setContentAreaFilled(false);
        go.addActionListener(e -> send());
        content.add(go);

        uiContext.getCommandQueue().addListener(command -> {
            if (listenToCommands && !reentrant && !RecentCommands.isBoringCommand(command))
                text.setText(command);
        });

        text.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                changedUpdate(null);
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                boolean isOk = isValidInput(text.getText());
                text.setBorder(isOk ? null : BorderFactory.createLineBorder(Color.red));
                config.setProperty(KEY, text.getText());
            }
        });

//        text.setInputVerifier(new InputVerifier() {
//            @Override
//            public boolean verify(JComponent input) {
//                boolean isOk = isValidInput(text);
//                text.setBorder(isOk ? null : BorderFactory.createLineBorder(Color.red));
//                return isOk;
//            }
//        });
        // todo: limit the length of text in the text field
    }

    private void installCtrlEnterAction() {
        text.setToolTipText("Ctrl-Enter to send");
        text.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER,
                java.awt.event.InputEvent.CTRL_DOWN_MASK),
                "sendKey");
        text.getActionMap().put("sendKey", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                send();
            }
        });
    }

    private void send() {
        String multiLine = text.getText();
        for (String line : multiLine.split("\n")) {
            sendCommand(line);
        }
    }

    private void sendCommand(String rawCommand) {
        if (!isValidInput(rawCommand))
            return;
        String cmd = prepareCommand(rawCommand, uiContext.getLinkManager());
        if (cmd == null) {
            /**
             * {@link #DECODE_RPN} for example does not send out anything
             */
            return;
        }
        if (listener != null)
            listener.onSend();
        int timeout = CommandQueue.getTimeout(cmd);
        reentrant = true;
        uiContext.getCommandQueue().write(cmd, timeout);
        reentrant = false;
    }

    public static String prepareCommand(String rawCommand, LinkManager linkManager) {
        try {
            if (rawCommand.startsWith("eval" + " ")) {
                String result = prepareEvalCommand(rawCommand);
                if (result.equals(rawCommand)) {
                    // result was not translated
                    MessagesCentral.getInstance().postMessage(AnyCommand.class, "Not valid expression");
                    MessagesCentral.getInstance().postMessage(AnyCommand.class, "Please try eval \"2 + 2\"");
                    MessagesCentral.getInstance().postMessage(AnyCommand.class, "For RPN use rpn_eval \"2 2 +\"");
                }
                return result;
            } else if (rawCommand.toLowerCase().startsWith("stim_check" + " ")) {
                handleStimulationSelfCheck(rawCommand, linkManager);
                return null;
            } else if (rawCommand.toLowerCase().startsWith(DECODE_RPN + " ")) {
                handleDecodeRpn(rawCommand);
                return null;
            } else if (rawCommand.toLowerCase().startsWith("set_fsio_expression" + " ")) {
                return prepareSetFsioCommand(rawCommand);
            } else {
                return rawCommand;
            }
        } catch (Throwable e) {
            FileLog.MAIN.log(e);
            return rawCommand;
        }
    }

    /**
     * stim_check 3000 5 30
     * would set RPM to 3000, give it 5 seconds to settle, and test for 30 seconds
     */
    private static void handleStimulationSelfCheck(String rawCommand, LinkManager linkManager) {
        String[] parts = rawCommand.split(" ", 4);
        if (parts.length != 4) {
            MessagesCentral.getInstance().postMessage(AnyCommand.class, "Invalid command length " + parts);
            return; // let's ignore invalid command
        }
        int rpm = Integer.parseInt(parts[1]);
        int settleTime = Integer.parseInt(parts[2]);
        int durationTime = Integer.parseInt(parts[3]);
        THREAD_FACTORY.newThread(new Runnable() {
            @Override
            public void run() {
                MessagesCentral.getInstance().postMessage(AnyCommand.class, "Will test with RPM " + rpm + ", settle time" + settleTime + "s and duration" + durationTime + "s");
                Function<String, Object> callback = new Function<String, Object>() {
                    @Override
                    public Object apply(String status) {
                        if (status == null) {
                            MessagesCentral.getInstance().postMessage(AnyCommand.class, rpm + " worked!");
                        } else {
                            MessagesCentral.getInstance().postMessage(AnyCommand.class, rpm + " failed " + status);
                        }
                        return null;
                    }
                };
                EcuTestHelper.assertRpmDoesNotJump(rpm, settleTime, durationTime, callback, linkManager.getCommandQueue());
            }
        }).start();
    }

    private static String prepareSetFsioCommand(String rawCommand) {
        String[] parts = rawCommand.split(" ", 3);
        if (parts.length != 3)
            return rawCommand; // let's ignore invalid command
        return "set_rpn_expression " + parts[1] + " " + quote(infix2postfix(unquote(parts[2])));
    }

    private static void handleDecodeRpn(String rawCommand) {
        String[] parts = rawCommand.split(" ", 2);
        if (parts.length != 2) {
            MessagesCentral.getInstance().postMessage(AnyCommand.class, "Failed to parse, one argument expected");
            return;
        }
        String argument = unquote(parts[1]);
        String humanForm = InfixConverter.getHumanInfixFormOrError(argument);
        MessagesCentral.getInstance().postMessage(AnyCommand.class, "Human form is \"" + humanForm + "\"");
    }

    public static String prepareEvalCommand(String rawCommand) {
        String[] parts = rawCommand.split(" ", 2);
        if (parts.length != 2)
            return rawCommand; // let's ignore invalid command

        try {
            return "rpn_eval" + " " + quote(infix2postfix(unquote(parts[1])));
        } catch (IllegalArgumentException e) {
            return rawCommand;
        }
    }

    private static String quote(String s) {
        return "\"" + s + "\"";
    }

    private static String infix2postfix(String infixExpression) {
        return DoubleEvaluator.process(infixExpression).getPosftfixExpression();
    }

    public static String unquote(String quoted) {
        quoted = quoted.trim().replace('\u201C', '"').replace('\u201D', '"');
        if (quoted.charAt(0) == '"')
            return quoted.substring(1, quoted.length() - 1);
        return quoted; // ignoring invalid input
    }

    private static boolean isValidInput(String text) {
        boolean isOk = true;
        for (char c : text.toCharArray()) {
            if (c > 127) {
                /**
                 * https://sourceforge.net/p/rusefi/tickets/63/
                 * only English characters are accepted - we need to reject two-byte unicode stuff
                 */
                isOk = false;
                break;
            }
        }
        return isOk;
    }

    public JComponent getContent() {
        return content;
    }

    public void setContent(JPanel content) {
        this.content = content;
    }

    public void requestFocus() {
        text.requestFocus();
    }

    interface Listener {
        void onSend();
    }

    public static AnyCommand createField(UIContext uiContext, Node config, boolean listenToCommands, boolean withCommandCaption) {
        return createField(uiContext, config, config.getProperty(KEY), listenToCommands, withCommandCaption);
    }

    public static AnyCommand createField(UIContext uiContext, Node config, String defaultCommand, boolean listenToCommands, boolean withCommandCaption) {
        final JTextField text = new JTextFieldWithWidth(200);

        final AnyCommand command = new AnyCommand(uiContext, text, config, defaultCommand, listenToCommands, withCommandCaption);
        text.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                command.send();
            }
        });

        final AtomicInteger index = new AtomicInteger();
        command.listener = new Listener() {
            @Override
            public void onSend() {
                index.set(0);
            }
        };

        text.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_UP) {
                    String command = RecentCommands.getRecent(index.incrementAndGet());
                    text.setText(command);
                } else if (e.getKeyCode() == KeyEvent.VK_DOWN) {
                    String command = RecentCommands.getRecent(index.decrementAndGet());
                    text.setText(command);
                }
            }
        });

        return command;
    }

    public static AnyCommand createArea(UIContext uiContext, Node config, String defaultCommand, boolean listenToCommands, boolean withCommandCaption) {
        final JTextArea text = new JTextArea(3, 20) {
            @Override
            public Dimension getPreferredSize() {
                Dimension size = super.getPreferredSize();
                return new Dimension(200, size.height);
            }
        };
//        text.setMax

        return new AnyCommand(uiContext, text, config, defaultCommand, listenToCommands, withCommandCaption);
    }
}
