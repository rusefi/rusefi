package com.rusefi.core;

import com.devexperts.logging.Logging;

import javax.swing.*;
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Messages from the firmware and UI panels which want to display them
 *
 * Date: 4/27/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class MessagesCentral {
    private static final Logging log = getLogging(MessagesCentral.class);

    private static final MessagesCentral INSTANCE = new MessagesCentral();
    private final List<MessageListener> listeners = new CopyOnWriteArrayList<>();

    /**
     * Bounded backlog of recent messages
     * {@link #addListenerAndReplay}. It is NOT replayed to plain {@link #addListener} listeners:
     * reactive listeners (config fields, {@link com.rusefi.io.ConnectionStatusLogic}) would
     * re-trigger side effects on stale messages.
     * Only ever touched on the AWT event thread (from {@link #postMessage}'s and
     * {@link #addListenerAndReplay}'s {@code invokeLater} bodies), so no extra synchronization.
     */
    private static final int MAX_HISTORY_SIZE = 2000;
    private final Deque<Message> history = new ArrayDeque<>();

    private MessagesCentral() {
    }

    public static MessagesCentral getInstance() {
        return INSTANCE;
    }

    public void addListener(MessageListener listener) {
        listeners.add(listener);
    }

    /**
     * Registers a display listener and replays the buffered message backlog to it, so messages that
     * arrived before this listener existed - e.g. during the splash-screen auto-connect, before the
     * Messages tab is created - are still shown. See {@link #history} / #9738.
     */
    public void addListenerAndReplay(final MessageListener listener) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                // Replay first, then register: any message posted after this runnable is enqueued is
                // delivered live once we are in the list; anything earlier is in the backlog. Single
                // EDT thread guarantees exactly-once delivery with no gap.
                for (Message m : history) {
                    listener.onMessage(m.clazz, m.message);
                }
                listeners.add(listener);
            }
        });
    }

    public void postMessage(final Class clazz, final String message) {
        log.info("postMessage " + clazz.getSimpleName() + ": " + message);
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                recordHistory(clazz, message);
                for (MessageListener listener : listeners)
                    listener.onMessage(clazz, message);
            }
        });
    }

    private void recordHistory(final Class clazz, final String message) {
        history.addLast(new Message(clazz, message));
        while (history.size() > MAX_HISTORY_SIZE) {
            history.removeFirst();
        }
    }

    private static final class Message {
        final Class clazz;
        final String message;

        Message(final Class clazz, final String message) {
            this.clazz = clazz;
            this.message = message;
        }
    }

    public interface MessageListener {
        /**
         * all listeners are invoked on AWT thread
         */
        void onMessage(Class clazz, String message);
    }
}
