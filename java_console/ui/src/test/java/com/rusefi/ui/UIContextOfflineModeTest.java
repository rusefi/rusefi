package com.rusefi.ui;

import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

/**
 * The offline-mode flag + listener notification that drives the connection-status tooltip, the
 * Tuning toolbar state label and the Pinout offline board (#9730).
 */
public class UIContextOfflineModeTest {

    @Test
    void defaultsToOnline() {
        assertFalse(new UIContext().isOfflineMode());
    }

    @Test
    void setterUpdatesFlagAndNotifiesListener() {
        UIContext ctx = new UIContext();
        List<Boolean> events = new ArrayList<>();
        ctx.addOfflineModeListener(events::add);

        ctx.setOfflineMode(true);
        assertTrue(ctx.isOfflineMode());
        assertEquals(List.of(Boolean.TRUE), events);

        ctx.setOfflineMode(false);
        assertFalse(ctx.isOfflineMode());
        assertEquals(List.of(Boolean.TRUE, Boolean.FALSE), events);
    }

    @Test
    void notifiesEveryListener() {
        UIContext ctx = new UIContext();
        List<Boolean> a = new ArrayList<>();
        List<Boolean> b = new ArrayList<>();
        ctx.addOfflineModeListener(a::add);
        ctx.addOfflineModeListener(b::add);

        ctx.setOfflineMode(true);

        assertEquals(List.of(Boolean.TRUE), a);
        assertEquals(List.of(Boolean.TRUE), b);
    }
}
