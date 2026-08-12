package com.rusefi.ts_plugin.auth;

import com.devexperts.logging.Logging;
import com.rusefi.server.JsonUserDetailsResolver;
import com.rusefi.server.UserDetails;
import com.rusefi.ts_plugin.ui.AuthTokenPanel;

import java.util.concurrent.CopyOnWriteArrayList;

public class InstanceAuthContext {
    private final static Logging log = Logging.getLogging(InstanceAuthContext.class);

    private static boolean isInitialized;

    public static CopyOnWriteArrayList<Listener> listeners = new CopyOnWriteArrayList<>();

    public static UserDetails self;

    public synchronized static void startup() {
        if (isInitialized)
            return;
        if (!AuthTokenPanel.hasToken()) {
            // exiting without marking initialized - UI has a chance to re-start the process once token is provided
            return;
        }
        log.info("Startup");
        isInitialized = true;
        new Thread(() -> {
            self = new JsonUserDetailsResolver().apply(AuthTokenPanel.getAuthToken());
            for (Listener listener : listeners)
                listener.onUserDetails(self);
        }).start();
    }

    public static boolean isOurController(int userId) {
        if (self == null)
            return false;
        return self.getUserId() == userId;
    }

    public interface Listener {
        void onUserDetails(UserDetails userDetails);
    }
}
