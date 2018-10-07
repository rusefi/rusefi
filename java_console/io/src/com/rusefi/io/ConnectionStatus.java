package com.rusefi.io;

import com.rusefi.Timeouts;
import com.rusefi.core.EngineTimeListener;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * todo: eliminate logic duplication with {@link ConnectionWatchdog}
 */
public class ConnectionStatus {
    // todo: react to any message as connected? how to know if message from controller, not internal message?
    public static final String FATAL_MESSAGE_PREFIX = "FATAL";
    @NotNull
    private Value value = Value.NOT_CONNECTED;

    public void executeOnceConnected(Runnable r) {
        /*
         * This would request initial value
         */
        if (isConnected()) {
            r.run();
        } else {
            addListener(isConnected -> {
                if (getValue() == Value.CONNECTED)
                    r.run();
            });
        }
    }

    public enum Value {
        NOT_CONNECTED,
        LOADING,
        CONNECTED
    }

    public static ConnectionStatus INSTANCE = new ConnectionStatus();
    private List<Listener> listeners = new CopyOnWriteArrayList<>();

    private final Timer timer = new Timer(Timeouts.CS_TIMEOUT, new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
//            setValue(Value.NOT_CONNECTED);
        }
    });

    private ConnectionStatus() {

        LinkManager.engineState.timeListeners.add(new EngineTimeListener() {
            @Override
            public void onTime(double time) {
                markConnected();
            }
        });

        SensorCentral.getInstance().addListener(Sensor.TIME_SECONDS, new SensorCentral.SensorListener() {
            @Override
            public void onSensorUpdate(double value) {
                markConnected();
            }
        });

        MessagesCentral.getInstance().addListener(new MessagesCentral.MessageListener() {
            @Override
            public void onMessage(Class clazz, String message) {
                if (message.startsWith(FATAL_MESSAGE_PREFIX))
                    markConnected();
            }
        });
    }

    public void markConnected() {
        if (value == Value.NOT_CONNECTED)
            setValue(Value.LOADING);
        /**
         * this timer will catch engine inactivity and display a warning
         */
        timer.restart();
    }

    public void setValue(@NotNull Value value) {
        if (value == this.value)
            return;
        this.value = value;
        for (Listener listener : listeners)
            listener.onConnectionStatus(isConnected());
    }

    public boolean isConnected() {
        return value != Value.NOT_CONNECTED;
    }

    @NotNull
    public Value getValue() {
        return value;
    }

    /**
     * @see #setValue
     */
    public void addListener(Listener listener) {
        listeners.add(listener);
    }

    public interface Listener {
        void onConnectionStatus(boolean isConnected);
    }
}