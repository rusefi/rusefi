package com.rusefi.ui.obd;

import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class ObdCodeHistoryTest {
    @Test
    public void codesStayInHistoryAfterExpiry() {
        ObdCodeHistory history = new ObdCodeHistory();
        history.update(335, Arrays.asList(335, 6700), 1000);
        assertEquals(2, history.getActiveCount());

        // 335 expired in firmware, 6700 still raised
        history.update(6700, Collections.singletonList(6700), 2000);
        List<ObdCodeHistory.Item> items = history.getItems();
        assertEquals(2, items.size());
        assertEquals(6700, items.get(0).code);
        assertTrue(items.get(0).isActive());
        assertEquals(2000, items.get(0).getLastSeenMillis());
        assertEquals(335, items.get(1).code);
        assertFalse(items.get(1).isActive());
        assertEquals(1000, items.get(1).getLastSeenMillis());
        assertEquals(1, history.getActiveCount());

        // raised again
        history.update(335, Arrays.asList(6700, 335), 3000);
        ObdCodeHistory.Item crank = find(history, 335);
        assertEquals(2, crank.getRaisedCount());
        assertEquals(1000, crank.firstSeenMillis);
        assertEquals(1, find(history, 6700).getRaisedCount());
    }

    @Test
    public void lastErrorCodeOnly() {
        ObdCodeHistory history = new ObdCodeHistory();
        // code expired before we connected, only the sticky lastErrorCode reports it
        history.update(121, Collections.emptyList(), 1000);
        assertEquals(121, history.getLastErrorCode());
        ObdCodeHistory.Item item = find(history, 121);
        assertFalse(item.isActive());
        assertEquals(0, item.getRaisedCount());
    }

    @Test
    public void disconnectAndClear() {
        ObdCodeHistory history = new ObdCodeHistory();
        history.update(335, Collections.singletonList(335), 1000);
        history.markAllInactive();
        assertEquals(0, history.getActiveCount());
        assertEquals(1, history.getItems().size());

        history.clear();
        assertTrue(history.getItems().isEmpty());
        assertEquals(0, history.getLastErrorCode());
    }

    private static ObdCodeHistory.Item find(ObdCodeHistory history, int code) {
        for (ObdCodeHistory.Item item : history.getItems()) {
            if (item.code == code) {
                return item;
            }
        }
        throw new AssertionError("No " + code);
    }
}
