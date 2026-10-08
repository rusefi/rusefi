package com.rusefi.ui.llm;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.mcp.ConsoleEcuSession;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.concurrent.atomic.AtomicInteger;

import static com.rusefi.ui.llm.ChatGptClient.*;
import static com.rusefi.ui.llm.ChatGptAgentTest.array;
import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class TroubleshootingToolsTest {
    @TempDir Path directory;

    @Test void combinesEcuEvidenceSearchAndCitedReadThroughTheAgentLoop() throws Exception {
        Files.createDirectories(directory.resolve("rusefi_documentation"));
        Files.write(directory.resolve("rusefi_documentation/Cranking.md"),
                "# Cranking\nCheck cranking voltage and RPM.\n![Diagram](Images/start.png)\n".getBytes(StandardCharsets.UTF_8));
        LinkManager link = mock(LinkManager.class);
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        protocol.signature = "test-ecu-signature";
        when(link.getBinaryProtocol()).thenReturn(protocol);
        try (ConsoleEcuSession session = new ConsoleEcuSession(link)) {
            TroubleshootingTools tools = new TroubleshootingTools(session, directory);
            AtomicInteger rounds = new AtomicInteger();
            JSONArray history = ChatGptAgent.run((input, definitions, delta, c) -> {
                assertEquals(6, definitions.size());
                assertTrue(definitions.toJSONString().contains("search_knowledge"));
                assertTrue(definitions.toJSONString().contains("read_knowledge"));
                switch (rounds.getAndIncrement()) {
                    case 0:
                        return response(call("ecu", "ecu_info", object()),
                                call("search", "search_knowledge", object("query", "cranking voltage")));
                    case 1:
                        JSONObject ecu = returned(input, "ecu");
                        assertEquals("test-ecu-signature", ecu.get("signature"));
                        JSONObject search = returned(input, "search");
                        JSONObject match = (JSONObject) ((JSONArray) search.get("matches")).get(0);
                        assertEquals("rusefi_documentation/Cranking.md:L2", match.get("citation"));
                        return response(call("read", "read_knowledge", object("path", match.get("path"), "start_line", 2, "max_lines", 2)));
                    default:
                        JSONObject read = returned(input, "read");
                        assertEquals("rusefi_documentation/Cranking.md:L2-L3", read.get("citation"));
                        assertEquals("unverified", ((JSONObject) read.get("provenance")).get("ecu_match"));
                        assertEquals("Check cranking voltage and RPM.", ((JSONObject) ((JSONArray) read.get("lines")).get(0)).get("text"));
                        return response(object("type", "message", "role", "assistant", "content", array(object("type", "output_text",
                                "text", "ECU test-ecu-signature: check cranking voltage and RPM (rusefi_documentation/Cranking.md:L2-L3). Source/ECU match is unverified."))));
                }
            }, tools, array(), "The engine cranks but will not start", ignored -> {}, ignored -> {}, new Cancellation());
            assertEquals(3, rounds.get());
            JSONObject answer = (JSONObject) history.get(history.size() - 1);
            JSONObject content = (JSONObject) ((JSONArray) answer.get("content")).get(0);
            assertTrue(((String) content.get("text")).contains("rusefi_documentation/Cranking.md:L2-L3"));
            assertEquals(Boolean.FALSE, tools.execute("send_command", object("command", "reboot"), () -> {}).get("success"));
        }
        verify(link, never()).close();
        verify(protocol, never()).close();
    }

    @Test void replacedConnectionBlocksKnowledgeAndMissingCacheDoesNotBlockEcuTools() throws Exception {
        LinkManager link = mock(LinkManager.class);
        BinaryProtocol protocol = mock(BinaryProtocol.class);
        protocol.signature = "test-ecu";
        when(link.getBinaryProtocol()).thenReturn(protocol);
        try (ConsoleEcuSession session = new ConsoleEcuSession(link)) {
            TroubleshootingTools tools = new TroubleshootingTools(session, directory.resolve("missing"));
            assertEquals(Boolean.FALSE, tools.execute("search_knowledge", object("query", "cranking"), () -> {}).get("success"));
            assertEquals("test-ecu", tools.execute("ecu_info", object(), () -> {}).get("signature"));
            when(link.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class));
            assertThrows(IOException.class, () -> tools.execute("search_knowledge", object("query", "cranking"), () -> {}));
        }
    }

    private static JSONObject call(String id, String name, JSONObject args) {
        return object("type", "function_call", "call_id", id, "name", name, "arguments", args.toJSONString());
    }
    private static Response response(JSONObject... output) { return new Response("", array((Object[]) output)); }
    private static JSONObject returned(JSONArray input, String id) throws IOException {
        for (Object item : input) {
            JSONObject value = (JSONObject) item;
            if ("function_call_output".equals(value.get("type")) && id.equals(value.get("call_id"))) {
                return parseObject((String) value.get("output"));
            }
        }
        throw new AssertionError("Missing tool result: " + id);
    }
}
