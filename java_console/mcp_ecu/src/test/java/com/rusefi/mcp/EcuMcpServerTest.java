package com.rusefi.mcp;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.jupiter.api.Test;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.io.StringReader;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Smoke test for the JSON-RPC / MCP protocol layer of {@link EcuMcpServer}.
 *
 * <p>Drives the server with no ECU attached, so only protocol-level methods that
 * don't require {@code ensureConnected(...)} are exercised: {@code initialize},
 * {@code tools/list}, {@code ping}, plus an unknown-method error path. ECU-touching
 * tools are covered separately by {@code LuaService} tests / integration tests.
 */
class EcuMcpServerTest {

    @Test
    void initializeAndListTools() throws Exception {
        String input = String.join("\n",
                jsonRpc(1, "initialize", "{\"protocolVersion\":\"2024-11-05\"}"),
                jsonRpc(null, "notifications/initialized", "{}"),
                jsonRpc(2, "tools/list", "{}"),
                jsonRpc(3, "ping", "{}"),
                jsonRpc(4, "no_such_method", "{}")
        ) + "\n";

        String[] responses = drive(input);
        // We expect 4 responses (notifications/initialized has no id and produces none).
        assertEquals(4, responses.length, "got: " + String.join(" | ", responses));

        JSONObject init = parse(responses[0]);
        assertEquals(1L, ((Number) init.get("id")).longValue());
        JSONObject initResult = (JSONObject) init.get("result");
        assertEquals("2024-11-05", initResult.get("protocolVersion"));
        assertNotNull(((JSONObject) initResult.get("capabilities")).get("tools"));

        JSONObject toolsList = parse(responses[1]);
        JSONArray tools = (JSONArray) ((JSONObject) toolsList.get("result")).get("tools");
        assertTrue(tools.size() >= 13, "expected our 13 tools, got " + tools.size());
        // Make sure the headline tools are present.
        java.util.Set<String> names = new java.util.HashSet<>();
        for (Object t : tools) names.add((String) ((JSONObject) t).get("name"));
        assertTrue(names.contains("set_lua"));
        assertTrue(names.contains("get_lua"));
        assertTrue(names.contains("lua_reset"));
        assertTrue(names.contains("send_command"));
        assertTrue(names.contains("command"));
        assertTrue(names.contains("read_output_channel"));
        assertTrue(names.contains("read_messages"));
        assertTrue(names.contains("wait_for_message"));
        assertTrue(names.contains("read_tune"));
        assertTrue(names.contains("ecu_info"));
        assertTrue(names.contains("connect"));
        assertTrue(names.contains("reboot"));
        assertTrue(names.contains("reboot_to_blt"));

        // ping returns an empty result object.
        JSONObject ping = parse(responses[2]);
        assertNotNull(ping.get("result"));

        // unknown method => error -32601.
        JSONObject unknown = parse(responses[3]);
        JSONObject err = (JSONObject) unknown.get("error");
        assertNotNull(err);
        assertEquals(-32601L, ((Number) err.get("code")).longValue());
    }

    @Test
    void readMessagesEmptyBufferReturnsZeroMessages() throws Exception {
        String input = jsonRpc(1, "tools/call",
                "{\"name\":\"read_messages\",\"arguments\":{\"maxLines\":50}}") + "\n";
        String[] responses = drive(input);
        assertEquals(1, responses.length);
        JSONObject resp = parse(responses[0]);
        JSONObject envelope = (JSONObject) resp.get("result");
        JSONObject structured = (JSONObject) envelope.get("structuredContent");
        JSONArray messages = (JSONArray) structured.get("messages");
        assertNotNull(messages);
        assertEquals(0, messages.size());
    }

    @Test
    void waitForMessageTimesOutQuickly() throws Exception {
        String input = jsonRpc(1, "tools/call",
                "{\"name\":\"wait_for_message\",\"arguments\":{\"regex\":\"NEVER_HAPPENS\",\"timeoutMs\":50}}") + "\n";
        long before = System.currentTimeMillis();
        String[] responses = drive(input);
        long elapsed = System.currentTimeMillis() - before;
        assertEquals(1, responses.length);
        JSONObject resp = parse(responses[0]);
        JSONObject envelope = (JSONObject) resp.get("result");
        JSONObject structured = (JSONObject) envelope.get("structuredContent");
        assertEquals(Boolean.FALSE, structured.get("success"));
        assertEquals("timeout", structured.get("error"));
        // Should return promptly after the 50ms timeout (give generous slack for CI).
        assertTrue(elapsed < 5_000, "wait_for_message blocked too long: " + elapsed + "ms");
    }

    // ---- helpers ----

    private static String[] drive(String input) throws Exception {
        BufferedReader in = new BufferedReader(new StringReader(input));
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        PrintStream out = new PrintStream(baos, true, "UTF-8");
        EcuMcpServer server = new EcuMcpServer(null, in, out);
        server.runForTests();
        String all = baos.toString(StandardCharsets.UTF_8.name()).trim();
        if (all.isEmpty()) return new String[0];
        return all.split("\\r?\\n");
    }

    private static String jsonRpc(Object id, String method, String paramsJson) {
        StringBuilder sb = new StringBuilder("{\"jsonrpc\":\"2.0\",");
        if (id != null) sb.append("\"id\":").append(id).append(',');
        sb.append("\"method\":\"").append(method).append("\"");
        if (paramsJson != null && !paramsJson.isEmpty())
            sb.append(",\"params\":").append(paramsJson);
        sb.append('}');
        return sb.toString();
    }

    private static JSONObject parse(String s) throws Exception {
        return (JSONObject) new JSONParser().parse(s);
    }
}
