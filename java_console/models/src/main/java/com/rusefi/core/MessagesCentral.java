package com.rusefi.core;

import com.devexperts.logging.Logging;

import javax.swing.*;
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

    private MessagesCentral() {
    }

    public static MessagesCentral getInstance() {
        return INSTANCE;
    }

    public void addListener(MessageListener listener) {
        listeners.add(listener);
    }

    public void postMessage(final Class clazz, final String message) {
        log.info("postMessage " + clazz.getSimpleName() + ": " + message);
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                for (MessageListener listener : listeners)
                    listener.onMessage(clazz, message);
            }
        });
    }

    public interface MessageListener {
        /**
         * all listeners are invoked on AWT thread
         */
        void onMessage(Class clazz, String message);
    }
}
