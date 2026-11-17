package com.rusefi.ui.llm;

import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class ChatTranscriptTest {
    @Test
    void savedTranscriptSplitsIntoBubblesAndChips() {
        ChatTranscript chat = new ChatTranscript("empty");
        chat.load("> Engine stumbles at 3000 rpm\non boost\n\n"
                + "\n[Tool: read_live_values]\n"
                + "\n[Tool: search_knowledge]\n"
                + "Dwell looks short.\nCheck battery voltage.\n"
                + "\n"
                + "> Export it\n\n"
                + "\n[Tool: export_diagnostic_case]\n"
                + "\n[Diagnostic case saved: C:\\cases\\case.json]\n"
                + "Saved.\n"
                + "[Turn discarded: stopped or Console connection changed.]\n");
        assertEquals(Arrays.asList(
                "user: Engine stumbles at 3000 rpm\non boost",
                "assistant[ECU: read live values, Search wiki and source]: Dwell looks short.\nCheck battery voltage.",
                "user: Export it",
                "assistant[Export diagnostic case, Diagnostic case saved]: Saved.",
                "notice: Turn discarded: stopped or Console connection changed."), chat.describeForTest());
    }

    @Test
    void liveTurnStreamsIntoOneReply() {
        ChatTranscript chat = new ChatTranscript("empty");
        chat.addUser("Why does it stall?");
        chat.addProgress("\n[Tool: diagnostic_snapshot]\n");
        chat.appendAssistant("\n\nIdle ");
        chat.appendAssistant("target is low.");
        chat.addNotice("[Could not save this conversation locally: disk full]");
        assertEquals(Arrays.asList(
                "user: Why does it stall?",
                "assistant[ECU: diagnostic snapshot]: Idle target is low.",
                "notice: Could not save this conversation locally: disk full"), chat.describeForTest());
        chat.clear();
        assertTrue(chat.describeForTest().isEmpty());
    }

    @Test
    void unknownToolsAreNamedInWords() {
        assertEquals("some new tool", ChatTranscript.ToolChip.describe("some_new_tool"));
    }
}
