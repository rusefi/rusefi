package com.rusefi.ui.livedocs;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class RefreshActionsMap {
    private final Map<LiveDataContext, List<RefreshActions>> actions = new HashMap<>();

    public void put(LiveDataContext context, RefreshActions refreshAction) {
        actions.putIfAbsent(context, new ArrayList<>());
        actions.get(context).add(refreshAction);
    }

    public void addAll(RefreshActionsMap actions) {
        for (Map.Entry<LiveDataContext, List<RefreshActions>> e : actions.actions.entrySet()) {
            for (RefreshActions action : e.getValue())
                put(e.getKey(), action);
        }
    }

    public Map<LiveDataContext, List<RefreshActions>> getActions() {
        return actions;
    }
}
