package com.rusefi.ui.obd;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Comparator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

/**
 * Every OBD code seen during this Console session.
 * <p>
 * Firmware keeps a code in recentErrorCode[] only while it keeps being raised (see warningPeriod),
 * so this history is what lets the user notice a code which has already expired.
 */
public class ObdCodeHistory {
    public static class Item {
        public final int code;
        public final long firstSeenMillis;
        long lastSeenMillis;
        boolean active;
        /**
         * number of times the code went from inactive to active
         */
        int raisedCount;

        Item(int code, long nowMillis) {
            this.code = code;
            firstSeenMillis = nowMillis;
            lastSeenMillis = nowMillis;
        }

        public long getLastSeenMillis() {
            return lastSeenMillis;
        }

        public boolean isActive() {
            return active;
        }

        public int getRaisedCount() {
            return raisedCount;
        }
    }

    private static final Comparator<Item> ORDER = Comparator
        .comparing((Item item) -> !item.active)
        .thenComparing(Comparator.comparingLong((Item item) -> item.lastSeenMillis).reversed())
        .thenComparingInt(item -> item.code);

    private final Map<Integer, Item> items = new LinkedHashMap<>();
    private int lastErrorCode;

    /**
     * @param lastErrorCode sticky last code, 0 if none
     * @param activeCodes   non-zero content of recentErrorCode[]
     */
    public void update(int lastErrorCode, Collection<Integer> activeCodes, long nowMillis) {
        this.lastErrorCode = lastErrorCode;
        for (Item item : items.values()) {
            if (!activeCodes.contains(item.code)) {
                item.active = false;
            }
        }
        for (int code : activeCodes) {
            if (code == 0) {
                continue;
            }
            Item item = items.computeIfAbsent(code, c -> new Item(c, nowMillis));
            if (!item.active) {
                item.active = true;
                item.raisedCount++;
            }
            item.lastSeenMillis = nowMillis;
        }
        // the last code could have expired before we have connected
        if (lastErrorCode != 0) {
            items.computeIfAbsent(lastErrorCode, c -> new Item(c, nowMillis));
        }
    }

    /**
     * No more live data: nothing is known to be active
     */
    public void markAllInactive() {
        for (Item item : items.values()) {
            item.active = false;
        }
    }

    public void clear() {
        items.clear();
        lastErrorCode = 0;
    }

    public int getLastErrorCode() {
        return lastErrorCode;
    }

    public int getActiveCount() {
        int count = 0;
        for (Item item : items.values()) {
            if (item.active) {
                count++;
            }
        }
        return count;
    }

    /**
     * @return active codes first, then most recently seen first
     */
    public List<Item> getItems() {
        List<Item> result = new ArrayList<>(items.values());
        result.sort(ORDER);
        return result;
    }
}
