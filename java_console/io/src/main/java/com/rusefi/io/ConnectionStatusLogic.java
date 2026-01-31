package com.rusefi.io;

import com.rusefi.config.generated.Integration;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import org.jetbrains.annotations.NotNull;

import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class ConnectionStatusLogic {
    @NotNull
    private ConnectionStatusValue value = ConnectionStatusValue.NOT_CONNECTED;

    public void executeOnceConnected(Runnable r) {
        /*
         * This would request initial value
         */
        if (isConnected()) {
            r.run();
        } else {
            addListener(isConnected -> {
                if (getValue() == ConnectionStatusValue.CONNECTED)
                    r.run();
            });
        }
    }

    public static final ConnectionStatusLogic INSTANCE = new ConnectionStatusLogic();
    private final List<Listener> listeners = new CopyOnWriteArrayList<>();

    private ConnectionStatusLogic() {
        SensorCentral.getInstance().addListener(Sensor.SECONDS, value -> markConnected());

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (message.startsWith(Integration.CRITICAL_PREFIX))
                    markConnected();
            }
        });
    }

    public void markConnected() {
        if (value == ConnectionStatusValue.NOT_CONNECTED)
            setValue(ConnectionStatusValue.LOADING);
    }

    public void setValue(@NotNull ConnectionStatusValue value) {
        if (value == this.value)
            return;
        this.value = value;
        for (Listener listener : listeners)
            listener.onConnectionStatus(isConnected());
    }

    public boolean isConnected() {
        return value != ConnectionStatusValue.NOT_CONNECTED;
    }

    @NotNull
    public ConnectionStatusValue getValue() {
        return value;
    }

    /**
     * todo: note that we do not have removeListener! in general we are not great in terms of memory leaks in
     * case of dynamic UI elements like detachable gauges
     * @see #setValue
     */
    public void addListener(Listener listener) {
        listeners.add(listener);
    }

    public interface Listener {
        void onConnectionStatus(boolean isConnected);
    }
}
