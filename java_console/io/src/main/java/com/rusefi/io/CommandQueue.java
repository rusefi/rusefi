package com.rusefi.io;

import com.devexperts.logging.Logging;
import com.rusefi.Listener;
import com.rusefi.config.generated.Integration;
import com.rusefi.util.ExitUtil;
import org.jetbrains.annotations.NotNull;

import java.util.*;
import java.util.concurrent.*;
import java.util.function.Consumer;

import static com.devexperts.logging.Logging.getLogging;

/**
 * This singleton keeps re-sending commands till a proper confirmation is received
 *
 * <p/>
 * Date: 1/7/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
@SuppressWarnings("FieldCanBeLocal")
public class CommandQueue {
    private static final Logging log = getLogging(CommandQueue.class);
    public static final String CONFIRMATION_PREFIX = "confirmation_";
    public static int DEFAULT_TIMEOUT = 500;
    private static final int COMMAND_CONFIRMATION_TIMEOUT = 1000;
    public static final int SLOW_CONFIRMATION_TIMEOUT = 5000;
    public static final Class<CommandQueue> COMMAND_QUEUE_CLASS = CommandQueue.class;
    private final LinkManager linkManager;

    private final BlockingQueue<IMethodInvocation> pendingCommands = new LinkedBlockingQueue<>();
    // we have concurrent access here at least in autotests
    private final List<Consumer<String>> commandListeners = new CopyOnWriteArrayList<>();

    private final Runnable runnable;

    private static boolean isSlowCommand(String cmd) {
        String lc = cmd.toLowerCase();
        return lc.startsWith("set " + Integration.CMD_ENGINE_TYPE) || lc.startsWith("writeconfig") || lc.startsWith("rewriteconfig");
    }

    public static int getTimeout(String cmd) {
        return isSlowCommand(cmd) ? SLOW_CONFIRMATION_TIMEOUT : COMMAND_CONFIRMATION_TIMEOUT;
    }

    public void addListener(Consumer<String> listener) {
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
        final IMethodInvocation command = pendingCommands.take();
        // got a command? let's send it!
        sendCommand(command);
    }

    /**
     * this method keeps retrying till a confirmation is received
     */
    private void sendCommand(final IMethodInvocation commandRequest) throws InterruptedException {
        String command = commandRequest.getCommand();

        CountDownLatch cl = new CountDownLatch(1);

        Consumer<String> listener = confirmStr -> {
            // only listen to replies for the correct command
            if (command.equals(confirmStr)) {
                cl.countDown();
            } else {
                throw new IllegalStateException("Was waiting for confirmation of " + command + " but got confirmation for " + confirmStr);
            }
        };

        commandListeners.add(listener);

        // Actually send the command now that we're listening
        linkManager.send(command, commandRequest.isFireEvent());

        // Wait for a reply
        int timeoutMs = commandRequest.getTimeout();
        cl.await(timeoutMs, TimeUnit.MILLISECONDS);

        commandListeners.remove(listener);

        if (cl.getCount() == 0) {
            commandRequest.getListener().onCommandConfirmation();
        } else {
            log.warn("No confirmation received for command '" + command + "' after timeout of " + timeoutMs + " ms");
        }
    }

    public static Listener<Throwable> ERROR_HANDLER = parameter -> ExitUtil.exit("CommandQueue error: " + parameter, -2);

    public CommandQueue(LinkManager linkManager) {
        this.linkManager = linkManager;
        runnable = new Runnable() {
            @SuppressWarnings("InfiniteLoopStatement")
            @Override
            public void run() {
                linkManager.messageListener.postMessage(COMMAND_QUEUE_CLASS, "SerialIO started");
                while (true) {
                    try {
                        sendPendingCommand();
                    } catch (Throwable e) {
                        log.error("Major connectivity error", e);
                        ERROR_HANDLER.onResult(e);
                    }
                }
            }
        };
        Thread thread = new Thread(runnable, "ECU Commands Queue");
        thread.setDaemon(true);
        thread.start();
    }

    public LinkManager getLinkManager() {
        return linkManager;
    }

    /**
     * this method handles command confirmations packed as
     * TODO: add example, todo: refactor method and add unit test
     */
    public void handleConfirmationMessage(final String message) {
        String confirmation = LinkManager.unpackConfirmation(message);
        if (confirmation == null)
            linkManager.messageListener.postMessage(CommandQueue.class, "Broken confirmation length: " + message);

        // Poke everyone listening for confirmation
        this.commandListeners.forEach(f -> f.accept(confirmation));

        if (LinkManager.LOG_LEVEL.isDebugEnabled())
            linkManager.messageListener.postMessage(CommandQueue.class, "got valid conf! " + confirmation + ", still pending: " + pendingCommands.size());
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
			this.commandListeners.forEach(c -> c.accept(command));
		}

        pendingCommands.add(new MethodInvocation(command, timeoutMs, listener, fireEvent));
    }

    public void addIfNotPresent(IMethodInvocation commandSender) {
        // technically this should be a critical locked section but for our use-case we do not care
        if (!pendingCommands.contains(commandSender))
            pendingCommands.add(commandSender);
    }

    static class MethodInvocation implements IMethodInvocation {
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

        @Override
        public String getCommand() {
            return command;
        }

        @Override
        public int getTimeout() {
            return timeoutMs;
        }

        @Override
        public InvocationConfirmationListener getListener() {
            return listener;
        }

        @Override
        public boolean isFireEvent() {
            return fireEvent;
        }

        @Override
        public String toString() {
            return "MethodInvocation{" +
                    "timeoutMs=" + timeoutMs +
                    ", command='" + command + '\'' +
                    '}';
        }
    }
}
