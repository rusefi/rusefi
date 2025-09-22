package com.rusefi.ts_plugin.ui;


/*
public enum RemoteTabController {
    /**
     * TunerStudio likes to close plugin panel, we need a singleton to preserve the state
     */
/*
    INSTANCE;

    private State state = State.NOT_CONNECTED;
    private LocalApplicationProxy localApplicationProxy;

    public final CopyOnWriteArrayList<Listener> listeners = new CopyOnWriteArrayList<>();

    public synchronized void setState(State state) {
        localApplicationProxy = null;
        if (state != this.state) {
            this.state = state;
            for (Listener listener : listeners)
                listener.onChange(state);
        }
    }

    public synchronized State getState() {
        return state;
    }

    public synchronized void setConnected(LocalApplicationProxy localApplicationProxy) {
        setState(State.CONNECTED);
        this.localApplicationProxy = localApplicationProxy;
    }

    public LocalApplicationProxy getLocalApplicationProxy() {
        return localApplicationProxy;
    }

    public enum State {
        NOT_CONNECTED,
        CONNECTING,
        CONNECTED
    }

    interface Listener {
        void onChange(State state);
    }
}
*/
