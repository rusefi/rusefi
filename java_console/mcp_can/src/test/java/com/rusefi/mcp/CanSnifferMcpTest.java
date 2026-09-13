package com.rusefi.mcp;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.jupiter.api.Test;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;

class CanSnifferMcpTest {
    private static String call(String tool, String args) {
        return "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"tools/call\",\"params\":{\"name\":\""
                + tool + "\",\"arguments\":" + args + "}}\n";
    }

    private static List<JSONObject> run(String requests, CanSnifferMcp.SlcanFactory factory,
                                        String... options) throws Exception {
        ByteArrayOutputStream output = new ByteArrayOutputStream();
        CanSnifferMcp server = new CanSnifferMcp(new BufferedReader(new StringReader(requests)),
                new PrintStream(output, true, StandardCharsets.UTF_8), factory);
        server.configure(options);
        server.runForTests();
        List<JSONObject> results = new ArrayList<>();
        for (String line : output.toString(StandardCharsets.UTF_8).split("\n")) {
            JSONObject response = (JSONObject) new JSONParser().parse(line);
            assertFalse(response.containsKey("error"), line);
            results.add((JSONObject) response.get("result"));
        }
        return results;
    }

    private static JSONObject body(JSONObject result) {
        return (JSONObject) result.get("structuredContent");
    }

    @Test
    void slcanPacketsUseExistingFiltersAndPreserveFrameMetadata() throws Exception {
        FakeSlcan serial = new FakeSlcan("", "F00", "bad", "t1232AABB", "T123456781CC1234", "r3218");
        AtomicInteger opens = new AtomicInteger();
        List<JSONObject> results = run(call("connect", "{}")
                        + call("wait_for_packet", "{\"idFilter\":\"321\",\"timeoutMs\":2000}")
                        + call("read_packets", "{\"sinceSeq\":1,\"maxPackets\":1}")
                        + call("wait_for_packet", "{\"idFilter\":\"123\",\"dataRegex\":\"AA.*BB\",\"timeoutMs\":0}")
                        + call("read_packets", "{\"idFilter\":\"123\"}")
                        + call("wait_for_packet", "{\"sinceSeq\":3,\"timeoutMs\":0}")
                        + call("connect", "{}") + call("status", "{}"), port -> {
                    assertEquals("COM42", port);
                    opens.incrementAndGet();
                    return serial;
                }, "--backend", "slcan", "--port", "COM42");
        assertEquals(true, body(results.get(0)).get("connected"));
        JSONObject rtr = (JSONObject) body(results.get(1)).get("match");
        assertEquals(true, rtr.get("rtr"));
        assertEquals(8L, rtr.get("length"));
        assertEquals("", rtr.get("data"));
        JSONArray packets = (JSONArray) body(results.get(2)).get("packets");
        assertEquals(1, packets.size());
        JSONObject extended = (JSONObject) packets.get(0);
        assertEquals(2L, extended.get("seq"));
        assertEquals("0x12345678", extended.get("id"));
        assertEquals(true, extended.get("extended"));
        assertEquals(false, extended.get("rtr"));
        assertEquals("1234", extended.get("slcanTimestamp"));
        assertEquals(true, body(results.get(3)).get("success"));
        assertEquals(1, ((JSONArray) body(results.get(4)).get("packets")).size());
        assertEquals("timeout", body(results.get(5)).get("error"));
        assertEquals(1, opens.get());
        assertEquals("slcan", body(results.get(7)).get("backend"));
        assertEquals("COM42", body(results.get(7)).get("port"));
        assertEquals(3L, body(results.get(7)).get("totalReceived"));
        assertEquals(1, serial.closes.get());
        assertTrue(serial.polls.get() > 0);
    }

    @Test
    void autodetectionFailureIsAnErrorAndCanBeRetried() throws Exception {
        AtomicInteger opens = new AtomicInteger();
        FakeSlcan serial = new FakeSlcan("t1230");
        List<JSONObject> results = run(call("connect", "{}") + call("status", "{}")
                        + call("wait_for_packet", "{\"idFilter\":\"123\",\"timeoutMs\":2000}"), port -> {
                    assertNull(port);
                    if (opens.incrementAndGet() == 1) {
                        throw new IOException("No SLCAN port found");
                    }
                    return serial;
                }, "--backend", "slcan");
        assertEquals(true, results.get(0).get("isError"));
        assertEquals(false, body(results.get(1)).get("connected"));
        assertEquals("No SLCAN port found", body(results.get(1)).get("error"));
        assertEquals(true, body(results.get(2)).get("success"));
        assertEquals(2, opens.get());
        assertEquals(1, serial.closes.get());
    }

    @Test
    void readerFailureWakesWaitAndClosesConnection() throws Exception {
        FakeSlcan serial = new FakeSlcan();
        serial.failPoll = true;
        List<JSONObject> results = run(call("wait_for_packet", "{\"timeoutMs\":2000}")
                + call("status", "{}"), port -> serial, "--backend", "slcan");
        assertEquals(true, results.get(0).get("isError"));
        assertTrue(body(results.get(0)).get("error").toString().contains("unplugged"));
        assertEquals(false, body(results.get(1)).get("connected"));
        assertEquals(1, serial.closes.get());
    }

    @Test
    void defaultAndExplicitPcanOptionsDoNotOpenSerial() throws Exception {
        CanSnifferMcp.SlcanFactory forbidden = port -> { throw new AssertionError("Serial opened for PCAN"); };
        JSONObject defaults = body(run(call("status", "{}"), forbidden).get(0));
        assertEquals("pcan", defaults.get("backend"));
        assertEquals("PCAN_USBBUS1", defaults.get("channel"));
        JSONObject explicit = body(run(call("status", "{}"), forbidden,
                "--backend", "pcan", "--channel", "2").get(0));
        assertEquals("PCAN_USBBUS2", explicit.get("channel"));
    }

    @Test
    void rejectsAmbiguousOrInvalidOptions() {
        for (String[] options : new String[][]{
                {"--backend", "invalid"}, {"--backend"}, {"--port", "COM42"},
                {"--backend", "slcan", "--channel", "2"}, {"--unknown", "x"},
                {"--backend", "slcan", "--port", " "}}) {
            assertThrows(IllegalArgumentException.class, () -> run(call("status", "{}"),
                    port -> { throw new AssertionError("Unexpected connection"); }, options));
        }
    }

    private static final class FakeSlcan implements CanSnifferMcp.SlcanConnection {
        final LinkedBlockingQueue<String> lines = new LinkedBlockingQueue<>();
        final AtomicInteger closes = new AtomicInteger();
        final AtomicInteger polls = new AtomicInteger();
        boolean failPoll;

        FakeSlcan(String... lines) {
            this.lines.addAll(Arrays.asList(lines));
        }

        public String getPort() { return "COM42"; }

        public String readLine(int timeoutMs) {
            try {
                return lines.poll(timeoutMs, TimeUnit.MILLISECONDS);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                return null;
            }
        }

        public void pollStatus() throws IOException {
            polls.incrementAndGet();
            if (failPoll) {
                throw new IOException("unplugged");
            }
        }

        public void close() { closes.incrementAndGet(); }
    }
}
