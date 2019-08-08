package com.rusefi.ui.livedocs;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.Field;

import java.util.List;

public abstract class LiveDocHolder {
    private final int id;
    private final List<RefreshActions> actions;
    private final Field[] values;

    public LiveDocHolder(int id, List<RefreshActions> actions, Field[] values) {
        this.id = id;
        this.actions = actions;
        this.values = values;
    }

    public void update(BinaryProtocol binaryProtocol, byte[] response) {
        for (RefreshActions action : actions)
            action.refresh(binaryProtocol, response);
    }

    public abstract boolean isVisible();

    public Integer getId() {
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
