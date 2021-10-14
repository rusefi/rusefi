package com.rusefi.ui.livedocs;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.enums.live_data_e;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 */
public abstract class LiveDocHolder {
    private final live_data_e id;
    private final RefreshActionsMap actions;

    public LiveDocHolder(live_data_e id, RefreshActionsMap actions) {
        this.id = id;
        this.actions = actions;
    }

    public RefreshActionsMap getActions() {
        return actions;
    }

    public void update(BinaryProtocol binaryProtocol, live_data_e context, byte[] response) {
        for (RefreshActions action : actions.getActions().get(context))
            action.refresh(binaryProtocol, response);
    }

    public abstract boolean isVisible();

    public live_data_e getId() {
        return id;
    }

    @Override
    public String toString() {
        return "LiveDocHolder{" +
                "id=" + id +
                '}';
    }
}
