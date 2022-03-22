package com.rusefi.ui.livedocs;

import com.rusefi.enums.live_data_e;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 */
public abstract class LiveDocHolder {
    private final live_data_e id;
    private final RefreshActions action;

    public LiveDocHolder(live_data_e id, RefreshActions action) {
        this.id = id;
        this.action = action;
    }

    public void update(byte[] response) {
        action.refresh(response);
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
