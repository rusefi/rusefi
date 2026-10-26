package com.rusefi.ui.llm;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;

import static com.rusefi.ui.llm.ChatGptClient.object;
import static org.junit.jupiter.api.Assertions.*;

@SuppressWarnings("unchecked")
class TroubleshootingSessionStoreTest {
    @TempDir Path directory;

    @Test void savesCompletedHistoryTranscriptAndEvidenceAcrossRestart() throws Exception {
        Path sessions = directory.resolve("sessions");
        TroubleshootingSessionStore store = new TroubleshootingSessionStore(sessions);
        DiagnosticCaseStore evidence = new DiagnosticCaseStore(directory.resolve("cases"));
        JSONObject result = object("samples", "captured log", "success", true);
        evidence.retain("capture_live_log", object(), result);
        Object evidenceId = result.get("evidence_id");
        JSONArray history = new JSONArray();
        history.add(object("role", "user", "content", "Review this log"));
        history.add(object("type", "function_call_output", "call_id", "call-1",
                "output", result.toJSONString()));
        TroubleshootingSessionStore.Session first = TroubleshootingSessionStore.Session
                .start("account-1", "model-1", "Review this log\nwith more detail")
                .completed("model-1", history, "> Review this log\nThe log shows RPM.\n", evidence.snapshot());
        store.save(first);

        TroubleshootingSessionStore reopened = new TroubleshootingSessionStore(sessions);
        List<TroubleshootingSessionStore.Summary> recent = reopened.list("account-1");
        assertEquals(1, recent.size());
        assertTrue(recent.get(0).toString().contains("Review this log"));
        TroubleshootingSessionStore.Session loaded = reopened.load("account-1", recent.get(0).id);
        assertEquals(first.id, loaded.id);
        assertEquals("model-1", loaded.model);
        assertEquals("Review this log", loaded.title);
        assertEquals(first.transcript, loaded.transcript);
        assertEquals(history, loaded.history);
        DiagnosticCaseStore retained = DiagnosticCaseStore.restore(directory.resolve("cases"), loaded.evidence);
        JSONArray ids = new JSONArray();
        ids.add(evidenceId);
        JSONObject exported = retained.export(object("findings", "RPM observed", "hypotheses", "Unconfirmed",
                "next_measurements", "Measure voltage", "evidence_ids", ids), object(), () -> {});
        assertEquals(Boolean.TRUE, exported.get("success"));

        JSONArray secondHistory = new JSONArray();
        secondHistory.addAll(history);
        secondHistory.add(object("role", "user", "content", "Follow up"));
        reopened.save(loaded.completed("model-2", secondHistory, loaded.transcript + "> Follow up\n", retained.snapshot()));
        assertEquals(1, reopened.list("account-1").size());
        assertEquals(3, reopened.load("account-1", first.id).history.size());
    }

    @Test void listsOnlySelectedAccountAndDeletesOnlyItsOwnSessions() throws Exception {
        TroubleshootingSessionStore store = new TroubleshootingSessionStore(directory.resolve("sessions"));
        TroubleshootingSessionStore.Session one = TroubleshootingSessionStore.Session.start("one", "model", "First");
        TroubleshootingSessionStore.Session two = TroubleshootingSessionStore.Session.start("two", "model", "Second");
        store.save(one);
        store.save(two);
        assertEquals(1, store.list("one").size());
        assertEquals("First", store.list("one").get(0).title);
        assertThrows(IOException.class, () -> store.load("one", two.id));
        assertThrows(IOException.class, () -> store.delete("one", two.id));
        assertThrows(IOException.class, () -> store.load("one", "../accounts"));
        store.delete("one", one.id);
        assertTrue(store.list("one").isEmpty());
        assertEquals(1, store.list("two").size());
    }

    @Test void damagedSessionDoesNotHideHealthySessionsAndSymlinkIsRejected() throws Exception {
        Path sessions = directory.resolve("sessions");
        TroubleshootingSessionStore store = new TroubleshootingSessionStore(sessions);
        TroubleshootingSessionStore.Session healthy = TroubleshootingSessionStore.Session.start("one", "model", "Healthy");
        store.save(healthy);
        String damagedId = java.util.UUID.randomUUID().toString();
        Files.write(sessions.resolve(damagedId + ".json"), "invalid JSON".getBytes(java.nio.charset.StandardCharsets.UTF_8));
        assertEquals(1, store.list("one").size());
        assertThrows(IOException.class, () -> store.load("one", damagedId));
        Path redirected = directory.resolve("redirected");
        try {
            Files.createSymbolicLink(redirected, sessions);
        } catch (UnsupportedOperationException | IOException | SecurityException unavailable) {
            return;
        }
        TroubleshootingSessionStore unsafe = new TroubleshootingSessionStore(redirected);
        assertThrows(IOException.class, () -> unsafe.list("one"));
        assertThrows(IOException.class, () -> unsafe.save(healthy));
    }
}
