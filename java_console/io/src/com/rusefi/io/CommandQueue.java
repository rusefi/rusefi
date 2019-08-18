package com.rusefi.io;

import com.rusefi.FileLog;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.MessagesCentral;
import org.jetbrains.annotations.NotNull;

import java.util.*;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * This singleton keeps re-sending commands till a proper confirmation is received
 *
 * <p/>
 * Date: 1/7/13
 * (c) Andrey Belomutskiy
 */
@SuppressWarnings("FieldCanBeLocal")
public class CommandQueue {
    public static final String CONFIRMATION_PREFIX = "confirmation_";
    public static final int DEFAULT_TIMEOUT = 500;
    private static final int COMMAND_CONFIRMATION_TIMEOUT = 1000;
    public static final int SLOW_CONFIRMATION_TIMEOUT = 5000;
    public static final Class<CommandQueue> COMMAND_QUEUE_CLASS = CommandQueue.class;
    private final Object lock = new Object();
    /**
     * One complex use-case is when we send out a bunch of commands and then we need to handle all the configurations
     * correctly
     * TODO: TTL for confirmations?
     */
    private Set<String> pendingConfirmations = Collections.synchronizedSet(new HashSet<String>());

    private static final CommandQueue instance = new CommandQueue();
    private final BlockingQueue<MethodInvocation> pendingCommands = new LinkedBlockingQueue<>();
    private final List<CommandQueueListener> commandListeners = new ArrayList<>();

    private final Runnable runnable = new Runnable() {
        @SuppressWarnings("InfiniteLoopStatement")
        @Override
        public void run() {
            MessagesCentral.getInstance().postMessage(COMMAND_QUEUE_CLASS, "SerialIO started");
            while (true) {
                try {
                    sendPendingCommand();
                } catch (Throwable e) {
                    FileLog.MAIN.logException("CommandQueue error", e);
                    System.exit(-2);
                }
            }
        }
    };

    private static boolean isSlowCommand(String cmd) {
        String lc = cmd.toLowerCase();
        return lc.startsWith("set_engine_type") || lc.startsWith("writeconfig") || lc.startsWith("rewriteconfig");
    }

    public static int getTimeout(String cmd) {
        return isSlowCommand(cmd) ? SLOW_CONFIRMATION_TIMEOUT : COMMAND_CONFIRMATION_TIMEOUT;
    }

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
    private void sendCommand(final MethodInvocation commandRequest) throws InterruptedException {
        int counter = 0;
        String command = commandRequest.getCommand();

        while (!pendingConfirmations.contains(command)) {
            counter++;
//            FileLog.MAIN.logLine("templog sending " + command + " " + System.currentTimeMillis() + " " + new Date());
            LinkManager.send(command, commandRequest.fireEvent);
            long now = System.currentTimeMillis();
            synchronized (lock) {
                lock.wait(commandRequest.getTimeout());
            }
            long timeWaited = System.currentTimeMillis() - now;
            if (!pendingConfirmations.contains(command) && timeWaited < commandRequest.getTimeout() / 2) {
                /**
                 * there could be a log of un-releated confirmations, we do not need to send out
                 * a log of the same command
                 */
                long extraWaitTime = commandRequest.getTimeout() / 2 - timeWaited;
//                FileLog.MAIN.logLine("templog extraWaitTime: " + extraWaitTime);
                Thread.sleep(extraWaitTime);
            }
        }
        if (pendingConfirmations.contains(command)) {
            commandRequest.listener.onCommandConfirmation();
            pendingConfirmations.remove(command);
        }

        if (counter != 1)
            MessagesCentral.getInstance().postMessage(CommandQueue.class, "Took " + counter + " attempts");
    }

    private CommandQueue() {
        Thread thread = new Thread(runnable, "Commands Queue");
        thread.setDaemon(true);
        thread.start();
    }

    /**
     * this method handles command confirmations packed as
     * TODO: add example, todo: refactor method and add unit test
     */
    public void handleConfirmationMessage(final String message) {
        MessagesCentral mc = MessagesCentral.getInstance();
        String confirmation = LinkManager.unpackConfirmation(message);
        if (confirmation == null)
            mc.postMessage(CommandQueue.class, "Broken confirmation length: " + message);
        pendingConfirmations.add(confirmation);
        if (LinkManager.LOG_LEVEL.isDebugEnabled())
            mc.postMessage(CommandQueue.class, "got valid conf! " + confirmation + ", still pending: " + pendingCommands.size());

//        FileLog.MAIN.logLine("templog got valid conf " + confirmation + " " + System.currentTimeMillis() + " " + new Date());

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

    public void write(String command, InvocationConfirmationListener listener) {
        write(command, DEFAULT_TIMEOUT, listener, true);
    }

    public void write(String command, int timeoutMs, InvocationConfirmationListener listener) {
        write(command, timeoutMs, listener, true);
    }

    /**
     * Non-blocking command request
     * Command is placed in the queue where it would be until it is confirmed
     *
     * @param command   dev console command
     * @param timeoutMs retry timeout
     * @param fireEvent true if we want global even about this comment, i.e. recent commands list to know about this command
     */
    public void write(String command, int timeoutMs, InvocationConfirmationListener listener, boolean fireEvent) {

        if (fireEvent) {
            for (CommandQueueListener cql : commandListeners)
                cql.onCommand(command);
        }

        pendingCommands.add(new MethodInvocation(command, timeoutMs, listener, fireEvent));
    }

    static class MethodInvocation {
        private final String command;
        private final int timeoutMs;
        private final InvocationConfirmationListener listener;
        private final boolean fireEvent;

        MethodInvocation(String command, int timeoutMs, InvocationConfirmationListener listener, boolean fireEvent) {
            this.command = command;
            this.timeoutMs = timeoutMs;
            this.listener = listener;
            this.fireEvent = fireEvent;
        }

        public String getCommand() {
            return command;
        }

        public int getTimeout() {
            return timeoutMs;
        }

        @Override
        public String toString() {
            return "MethodInvocation{" +
                    "timeoutMs=" + timeoutMs +
                    ", command='" + command + '\'' +
                    '}';
        }
    }

    public interface CommandQueueListener {
        void onCommand(String command);
    }

    public static String disableCommand(String command) {
        return Fields.CMD_DISABLE + " " + command;
    }
}
