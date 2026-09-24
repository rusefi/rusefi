package com.rusefi.mcp;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.io.StringReader;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

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
        assertTrue(names.contains("capture_engine_sniffer"));
        assertTrue(names.contains("mount_to_ecu"));
        assertTrue(names.contains("mount_to_pc"));
        assertTrue(names.contains("start_data_logging"));
        assertTrue(names.contains("stop_data_logging"));
        assertTrue(names.contains("data_logging_status"));
        assertTrue(names.contains("convert_log_to_csv"));
        assertTrue(names.contains("read_messages"));
        assertTrue(names.contains("wait_for_message"));
        assertTrue(names.contains("read_tune"));
        assertTrue(names.contains("write_tune"));
        assertTrue(names.contains("ecu_info"));
        assertTrue(names.contains("connect"));
        assertTrue(names.contains("download_bundle"));
        assertTrue(names.contains("update_firmware"));
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
    void captureAndMountToolsValidateTimeoutBeforeConnecting() throws Exception {
        for (String name : new String[]{"mount_to_ecu", "mount_to_pc", "capture_engine_sniffer"}) {
            for (long timeout : new long[]{0, -1, 120001}) {
                String[] responses = drive(jsonRpc(1, "tools/call",
                        "{\"name\":\"" + name + "\",\"arguments\":{\"timeoutMs\":" + timeout + "}}") + "\n");
                JSONObject result = (JSONObject) parse(responses[0]).get("result");
                JSONObject body = (JSONObject) result.get("structuredContent");
                assertEquals(false, body.get("success"));
                assertTrue(body.get("error").toString().contains("timeoutMs"));
            }
        }
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

    @Test
    void writeTuneRequiresAHostPathBeforeConnecting() throws Exception {
        for (String arguments : new String[]{"{}", "{\"path\":\"\"}", "{\"path\":42}"}) {
            String input = jsonRpc(1, "tools/call",
                    "{\"name\":\"write_tune\",\"arguments\":" + arguments + "}") + "\n";
            JSONObject envelope = (JSONObject) parse(drive(input)[0]).get("result");
            JSONObject body = (JSONObject) envelope.get("structuredContent");
            assertEquals(Boolean.TRUE, envelope.get("isError"));
            assertEquals(Boolean.FALSE, body.get("success"));
            assertTrue(body.get("error").toString().contains("path"));
        }
    }

    @Test
    @SuppressWarnings("unchecked")
    void convertsLogWithoutEcuAndReturnsConversionFailures(@TempDir Path directory) throws Exception {
        Path source = directory.resolve("capture.msl");
        Files.write(source, "Time\tRPM\ns\trpm\n0.5\t1200\n".getBytes(StandardCharsets.UTF_8));
        JSONObject args = new JSONObject();
        args.put("inputPath", source.toString());
        JSONObject params = new JSONObject();
        params.put("name", "convert_log_to_csv");
        params.put("arguments", args);
        String request = jsonRpc(1, "tools/call", params.toJSONString()) + "\n";
        JSONObject envelope = (JSONObject) parse(drive(request)[0]).get("result");
        JSONObject body = (JSONObject) envelope.get("structuredContent");
        assertEquals(false, envelope.get("isError"));
        assertEquals(true, body.get("success"));
        assertEquals(1L, body.get("recordCount"));
        assertEquals(2L, body.get("fieldCount"));
        assertEquals("msl", body.get("inputFormat"));
        assertEquals(directory.resolve("capture.csv").toString(), body.get("path"));
        assertEquals("Time (s),RPM (rpm)\n0.5,1200\n",
                new String(Files.readAllBytes(directory.resolve("capture.csv")), StandardCharsets.UTF_8));
        // Existing output -> tool error, not a successful or partial conversion.
        envelope = (JSONObject) parse(drive(request)[0]).get("result");
        assertEquals(true, envelope.get("isError"));
        for (Object invalid : new Object[]{null, "", 42L}) {
            args.put("inputPath", invalid);
            envelope = (JSONObject) parse(drive(jsonRpc(2, "tools/call", params.toJSONString()) + "\n")[0]).get("result");
            assertEquals(true, envelope.get("isError"));
        }
    }

    @Test
    void updateFirmwareRejectsInvalidImagesBeforeConnecting(@TempDir Path directory) throws Exception {
        Path invalid = directory.resolve("invalid.srec");
        Files.writeString(invalid, "not firmware");
        for (String arguments : new String[]{
                "{\"firmwarePath\":\"\"}", "{\"firmwarePath\":null}", "{\"firmwarePath\":42}",
                "{\"firmwarePath\":\"" + JSONObject.escape(invalid.toString()) + "\"}",
                "{\"firmwarePath\":\"" + JSONObject.escape(directory.resolve("missing.srec").toString()) + "\"}"}) {
            JSONObject envelope = (JSONObject) parse(drive(jsonRpc(1, "tools/call",
                    "{\"name\":\"update_firmware\",\"arguments\":" + arguments + "}") + "\n")[0]).get("result");
            assertEquals(true, envelope.get("isError"));
            assertEquals(false, ((JSONObject) envelope.get("structuredContent")).get("success"));
        }
    }

    @Test
    void downloadBundleRejectsInvalidArgumentsThroughMcp() throws Exception {
        for (String arguments : new String[]{"{}", "{\"board\":\"../uaefi\"}", "{\"universal\":\"true\"}"}) {
            JSONObject envelope = (JSONObject) parse(drive(jsonRpc(1, "tools/call",
                    "{\"name\":\"download_bundle\",\"arguments\":" + arguments + "}") + "\n")[0]).get("result");
            assertEquals(true, envelope.get("isError"));
            assertEquals(false, ((JSONObject) envelope.get("structuredContent")).get("success"));
        }
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
