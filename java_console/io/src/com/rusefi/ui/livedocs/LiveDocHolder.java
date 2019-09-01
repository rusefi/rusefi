package com.rusefi.ui.livedocs;

import com.rusefi.binaryprotocol.BinaryProtocol;

public abstract class LiveDocHolder {
    private final LiveDataContext id;
    private final RefreshActionsMap actions;

    public LiveDocHolder(LiveDataContext id, RefreshActionsMap actions) {
        this.id = id;
        this.actions = actions;
    }

    public RefreshActionsMap getActions() {
        return actions;
    }

    public void update(BinaryProtocol binaryProtocol, LiveDataContext context, byte[] response) {
        for (RefreshActions action : actions.getActions().get(context))
            action.refresh(binaryProtocol, response);
    }

    public abstract boolean isVisible();

    public LiveDataContext getId() {
        return id;
    }

    @Override
    public String toString() {
        return "LiveDocHolder{" +
                "id=" + id +
                '}';
    }
}
