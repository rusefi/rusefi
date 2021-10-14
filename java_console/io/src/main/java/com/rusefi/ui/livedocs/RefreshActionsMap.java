package com.rusefi.ui.livedocs;

import com.rusefi.enums.live_data_e;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class RefreshActionsMap {
    private final Map<live_data_e, List<RefreshActions>> actions = new HashMap<>();

    public void put(live_data_e context, RefreshActions refreshAction) {
        actions.putIfAbsent(context, new ArrayList<>());
        actions.get(context).add(refreshAction);
    }

    public void addAll(RefreshActionsMap actions) {
        for (Map.Entry<live_data_e, List<RefreshActions>> e : actions.actions.entrySet()) {
            for (RefreshActions action : e.getValue())
                put(e.getKey(), action);
        }
    }

    public Map<live_data_e, List<RefreshActions>> getActions() {
        return actions;
    }
}
