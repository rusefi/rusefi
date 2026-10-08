package com.rusefi.ui.llm;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.field.ScalarIniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.FieldType;
import com.rusefi.core.OutputChannelSnapshot;
import com.rusefi.core.SensorCentral;
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
import java.util.BitSet;
import java.util.Collections;
import java.util.Optional;
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
        IniFileModel ini = mock(IniFileModel.class);
        ScalarIniField rpm = new ScalarIniField("RPMValue", 0, "RPM", FieldType.UINT16, 1, "0", 0);
        when(ini.getAllOutputChannels()).thenReturn(Collections.singletonMap("RPMValue", rpm));
        when(ini.getOutputChannel("RPMValue")).thenReturn(rpm);
        ScalarIniField cranking = new ScalarIniField("cranking_rpm", 1, "RPM", FieldType.UINT8, 1, "0", 0);
        when(ini.getAllIniFields()).thenReturn(Collections.singletonMap("cranking_rpm", cranking));
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(1);
        when(meta.getPageSize(0)).thenReturn(16);
        when(ini.getMetaInfo()).thenReturn(meta);
        when(ini.getBlockingFactor()).thenReturn(16);
        when(protocol.getIniFile()).thenReturn(ini);
        when(protocol.getIniFileNullable()).thenReturn(ini);
        when(ini.findIniField("LUASCRIPT")).thenReturn(Optional.of(new StringIniField("luaScript", 4, 12)));
        when(protocol.readFromPage(0, 4, 12)).thenReturn(new byte[]{'r', 'e', 't', 'u', 'r', 'n', ' ', '1', '\n', 0, 0, 0});
        when(protocol.readFromPage(0, 1, 1)).thenReturn(new byte[]{(byte) 200});
        doAnswer(call -> { ((Runnable) call.getArgument(0)).run(); return null; }).when(link).submit(any(Runnable.class));
        SensorCentral sensors = SensorCentral.getInstance();
        sensors.reset();
        try (ConsoleEcuSession session = new ConsoleEcuSession(link)) {
            BitSet valid = new BitSet();
            valid.set(0, 2);
            sensors.grabSensorValues(new OutputChannelSnapshot(new byte[]{0, (byte) 250, 0}, valid,
                    Collections.emptySet(), sensors.getOutputChannelDemand().getGeneration(), true), ini, null);
            TroubleshootingTools tools = new TroubleshootingTools(session, directory);
            AtomicInteger rounds = new AtomicInteger();
            JSONArray history = ChatGptAgent.run((input, definitions, delta, c) -> {
                assertEquals(12, definitions.size());
                assertTrue(definitions.toJSONString().contains("get_lua"));
                assertTrue(definitions.toJSONString().contains("capture_live_log"));
                assertTrue(definitions.toJSONString().contains("capture_engine_sniffer"));
                assertTrue(definitions.toJSONString().contains("read_tune_fields"));
                assertTrue(definitions.toJSONString().contains("read_live_values"));
                assertTrue(definitions.toJSONString().contains("diagnostic_snapshot"));
                assertTrue(definitions.toJSONString().contains("search_knowledge"));
                assertTrue(definitions.toJSONString().contains("read_knowledge"));
                switch (rounds.getAndIncrement()) {
                    case 0:
                        return response(call("ecu", "diagnostic_snapshot", object("names", array("RPMValue"))),
                                call("tune", "read_tune_fields", object("names", array("cranking_rpm"))),
                                call("search", "search_knowledge", object("query", "cranking voltage")));
                    case 1:
                        JSONObject ecu = returned(input, "ecu");
                        assertEquals("test-ecu-signature", ecu.get("signature"));
                        assertEquals(250.0, ((JSONObject) ((JSONArray) ecu.get("values")).get(0)).get("value"));
                        assertEquals(1L, ecu.get("sampleId"));
                        assertFalse(((JSONArray) ecu.get("faults")).isEmpty());
                        JSONObject tune = returned(input, "tune");
                        assertEquals(Boolean.TRUE, tune.get("complete"));
                        assertEquals(200.0, ((JSONObject) ((JSONArray) tune.get("fields")).get(0)).get("value"));
                        JSONObject search = returned(input, "search");
                        JSONObject match = (JSONObject) ((JSONArray) search.get("matches")).get(0);
                        assertEquals("rusefi_documentation/Cranking.md:L2", match.get("citation"));
                        return response(call("read", "read_knowledge", object("path", match.get("path"), "start_line", 2, "max_lines", 2)),
                                call("lua", "get_lua", object("start_line", 1, "max_lines", 1)));
                    default:
                        JSONObject read = returned(input, "read");
                        JSONObject lua = returned(input, "lua");
                        assertEquals("ecu_ram", lua.get("source"));
                        assertEquals("return 1", ((JSONObject) ((JSONArray) lua.get("lines")).get(0)).get("text"));
                        assertEquals(64, ((String) lua.get("sha256")).length());
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
        } finally {
            sensors.reset();
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
