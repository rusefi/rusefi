package com.rusefi.core.test;

import com.rusefi.core.EngineState;
import com.rusefi.io.LinkDecoder;
import com.rusefi.config.generated.Integration;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;
import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.*;

/**
 * @author Andrey Belomutskiy
 *         12/26/12
 */
public class EngineStateTest {
    @Test void observersCoexistWithOwnersAndReceiveEveryKeyInMixedLines() throws Exception {
        List<String> values = new ArrayList<>();
        EngineState state = new EngineState(new EngineState.EngineStateListenerImpl());
        state.registerStringValueAction("a", v -> values.add("owner:" + v));
        try (AutoCloseable first = state.addStringValueObserver("A", v -> values.add("first:" + v));
             AutoCloseable second = state.addStringValueObserver("a", v -> values.add("second:" + v));
             AutoCloseable other = state.addStringValueObserver("b", v -> values.add("other:" + v))) {
            deliver(state, "a", "one", "b", "two", "a", "three");
            assertEquals(Arrays.asList("owner:one", "first:one", "second:one", "other:two",
                    "owner:three", "first:three", "second:three"), values);
        }
        values.clear();
        deliver(state, "a", "four");
        assertEquals(Arrays.asList("owner:four"), values);
    }

    @Test void observersAllowLateOwnersAndOwnerRemovalWithoutLosingTheirSubscription() throws Exception {
        List<String> observed = new ArrayList<>();
        EngineState state = new EngineState(new EngineState.EngineStateListenerImpl());
        AutoCloseable token = state.addStringValueObserver("chart", observed::add);
        deliver(state, "chart", "first");
        state.registerStringValueAction("chart", ignored -> {});
        deliver(state, "chart", "second");
        state.removeAction("chart");
        deliver(state, "chart", "third");
        token.close();
        token.close();
        state.registerStringValueAction("chart", ignored -> {});
        deliver(state, "chart", "fourth");
        assertEquals(Arrays.asList("first", "second", "third"), observed);
    }

    @Test void failingObserverDoesNotBreakOtherObserversOrTheOwner() throws Exception {
        List<String> values = new ArrayList<>();
        EngineState state = new EngineState(new EngineState.EngineStateListenerImpl());
        state.registerStringValueAction("a", values::add);
        try (AutoCloseable bad = state.addStringValueObserver("a", ignored -> { throw new IllegalStateException("test"); });
             AutoCloseable good = state.addStringValueObserver("a", v -> values.add("observed:" + v))) {
            deliver(state, "a", "one", "a", "two");
            assertEquals(Arrays.asList("one", "observed:one", "two", "observed:two"), values);
        }
    }

    private static void deliver(EngineState state, String... tokens) {
        state.processNewData(String.join(Integration.LOG_DELIMITER, tokens) + Integration.LOG_DELIMITER + "\r\n", LinkDecoder.VOID);
    }

    @Test
    public void startsWithIgnoreCase() {
        assertTrue(EngineState.startWithIgnoreCase("HELLO", "he"));
        assertFalse(EngineState.startWithIgnoreCase("HELLO", "hellllll"));
        assertFalse(EngineState.startWithIgnoreCase("HELLO", "ha"));
    }
}
