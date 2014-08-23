package com.irnems.core;

import com.irnems.FileLog;

import javax.swing.*;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * Messages from the firmware and UI panels which want to display them
 *
 * Date: 4/27/13
 * (c) Andrey Belomutskiy
 */
public class MessagesCentral {
    private static final MessagesCentral INSTANCE = new MessagesCentral();
    private final List<MessageListener> listeners = new CopyOnWriteArrayList<MessageListener>();

    private MessagesCentral() {
    }

    public static MessagesCentral getInstance() {
        return INSTANCE;
    }

    public void addListener(MessageListener listener) {
        listeners.add(listener);
    }

    public void postMessage(final Class clazz, final String message) {
        FileLog.MAIN.logLine("postMessage " + clazz.getSimpleName() + ": " + message);
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
