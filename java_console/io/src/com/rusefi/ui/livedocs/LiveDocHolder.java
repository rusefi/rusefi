package com.rusefi.ui.livedocs;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.Field;

public abstract class LiveDocHolder {
    private final LiveDataContext id;
    private final RefreshActionsMap actions;
    private final Field[] values;

    public LiveDocHolder(LiveDataContext id, com.rusefi.ui.livedocs.RefreshActionsMap actions, Field[] values) {
        this.id = id;
        this.actions = actions;
        this.values = values;
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

    public int getStructSize() {
        return Field.getStructureSize(values);
    }

    @Override
    public String toString() {
        return "LiveDocHolder{" +
                "id=" + id +
                '}';
    }
}
