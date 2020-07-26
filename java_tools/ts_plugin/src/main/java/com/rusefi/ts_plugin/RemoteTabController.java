package com.rusefi.ts_plugin;

import com.rusefi.proxy.client.LocalApplicationProxy;

public enum RemoteTabController {
    /**
     * TunerStudio likes to close plugin panel, we need a singleton to preserve the state
     */
    INSTANCE;

    private State state = State.NOT_CONNECTED;
    private LocalApplicationProxy localApplicationProxy;

    public synchronized void setState(State state) {
        this.state = state;
        localApplicationProxy = null;
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
}
