package com.rusefi.io;

import com.rusefi.core.MessagesCentral;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * This class keeps re-sending a command till a proper confirmation is received
 * <p/>
 * Date: 1/7/13
 * (c) Andrey Belomutskiy
 */
@SuppressWarnings("FieldCanBeLocal")
public class CommandQueue {
    private static final String CONFIRMATION_PREFIX = "confirmation_";
    public static final int DEFAULT_TIMEOUT = 300;
    private final Object lock = new Object();
    private String latestConfirmation;

    private static final CommandQueue instance = new CommandQueue();
    private final BlockingQueue<MethodInvocation> pendingCommands = new LinkedBlockingQueue<MethodInvocation>();
    private final List<CommandQueueListener> commandListeners = new ArrayList<>();

    private final Runnable runnable = new Runnable() {
        @SuppressWarnings("InfiniteLoopStatement")
        @Override
        public void run() {
            MessagesCentral.getInstance().postMessage(CommandQueue.class, "SerialIO started");
            while (true) {
                try {
                    sendPendingCommand();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                    throw new IllegalStateException(e);
                }
            }
        }
    };

    public void addListener(CommandQueueListener listener) {
        commandListeners.add(listener);
    }

    /**
     * this method is always invoked on 'Commands Queue' thread {@link #runnable}
     *
     * @throws InterruptedException
     */
    private void sendPendingCommand() throws InterruptedException {
        /**
         * here we block in case there is no command to send
         */
        @NotNull
        final MethodInvocation command = pendingCommands.take();
        // got a command? let's send it!
        sendCommand(command);
    }

    /**
     * this method keeps retrying till a confirmation is received
     */
    private void sendCommand(final MethodInvocation pair) throws InterruptedException {
        int counter = 0;
        latestConfirmation = null;
        String command = pair.getText();

        while (!command.equals(latestConfirmation)) {
            counter++;
            LinkManager.send(command);
            synchronized (lock) {
                lock.wait(pair.getTimeout());
            }
        }
        if (command.equals(latestConfirmation))
            pair.listener.onCommandConfirmation();

        if (counter != 1)
            MessagesCentral.getInstance().postMessage(CommandQueue.class, "Took " + counter + " attempts");
    }

    private CommandQueue() {
        Thread thread = new Thread(runnable, "Commands Queue");
        thread.setDaemon(true);
        thread.start();
        final MessagesCentral mc = MessagesCentral.getInstance();
        mc.addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (message.startsWith(CONFIRMATION_PREFIX))
                    handleConfirmationMessage(message, mc);
            }
        });
    }

    /**
     * this method handles command confirmations packed as
     * TODO: add example, todo: refactor method and add unit test
     */
    private void handleConfirmationMessage(String message, MessagesCentral mc) {
        String confirmation = message.substring(CONFIRMATION_PREFIX.length());
        int index = confirmation.indexOf(":");
        if (index < 0) {
            mc.postMessage(CommandQueue.class, "Broken confirmation: " + confirmation);
            return;
        }
        String number = confirmation.substring(index + 1);
        int length;
        try {
            length = Integer.parseInt(number);
        } catch (NumberFormatException e) {
            mc.postMessage(CommandQueue.class, "Broken confirmation length: " + confirmation);
            return;
        }
        if (length != index) {
            mc.postMessage(CommandQueue.class, "Broken confirmation length: " + confirmation);
            return;
        }
        latestConfirmation = confirmation.substring(0, length);
        mc.postMessage(CommandQueue.class, "got valid conf! " + latestConfirmation);
        synchronized (lock) {
            lock.notifyAll();
        }
    }

    public static CommandQueue getInstance() {
        return instance;
    }

    public void write(String command) {
        write(command, DEFAULT_TIMEOUT);
    }

    public void write(String command, int timeout) {
        write(command, timeout, InvocationConfirmationListener.VOID);
    }

    /**
     * Non-blocking command request
     */
    public void write(String command, int timeoutMs, InvocationConfirmationListener listener) {

        for (CommandQueueListener cql : commandListeners)
            cql.onCommand(command);

        pendingCommands.add(new MethodInvocation(command, timeoutMs, listener));
    }

    static class MethodInvocation {
        private final String text;
        private final int timeoutMs;
        private final InvocationConfirmationListener listener;

        MethodInvocation(String text, int timeoutMs, InvocationConfirmationListener listener) {
            this.text = text;
            this.timeoutMs = timeoutMs;
            this.listener = listener;
        }

        public String getText() {
            return text;
        }

        public int getTimeout() {
            return timeoutMs;
        }
    }

    public interface CommandQueueListener {
        void onCommand(String command);
    }
}
