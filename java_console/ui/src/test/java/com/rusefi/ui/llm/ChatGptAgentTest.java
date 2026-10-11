package com.rusefi.ui.llm;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.StringReader;
import java.util.concurrent.CancellationException;
import java.util.concurrent.atomic.AtomicInteger;

import static com.rusefi.ui.llm.ChatGptClient.*;
import static org.junit.jupiter.api.Assertions.*;

@SuppressWarnings("unchecked")
class ChatGptAgentTest {
    @Test void streamsToolsThenReplaysCompleteReasoningAndCallIds() throws Exception {
        FakeTools tools = new FakeTools();
        JSONArray previous = array(object("role", "user", "content", "Earlier question"));
        JSONObject reasoning = object("type", "reasoning", "id", "rs_1", "encrypted_content", "opaque", "summary", array());
        JSONObject call = call("call_1", "ecu_info", "{}");
        JSONObject answer = object("type", "message", "role", "assistant", "content",
                array(object("type", "output_text", "text", "Firmware identified", "annotations", array())));
        AtomicInteger requests = new AtomicInteger();
        StringBuilder visible = new StringBuilder();
        JSONArray result = ChatGptAgent.run((input, definitions, delta, c) -> {
            assertEquals(1, definitions.size());
            if (requests.getAndIncrement() == 0) {
                assertEquals(2, input.size());
                return readResponse(new StringReader(completed(array(reasoning, call))), delta, c);
            }
            assertEquals(reasoning, input.get(2));
            assertEquals(call, input.get(3));
            JSONObject returned = (JSONObject) input.get(4);
            assertEquals("function_call_output", returned.get("type"));
            assertEquals("call_1", returned.get("call_id"));
            assertEquals("rusEFI-test", parseObject((String) returned.get("output")).get("signature"));
            return readResponse(new StringReader("data: {\"type\":\"response.output_text.delta\",\"delta\":\"Firmware identified\"}\n\n"
                    + completed(array(answer))), delta, c);
        }, tools, previous, "Identify the ECU", visible::append, ignored -> {}, new Cancellation());
        assertEquals(2, requests.get());
        assertEquals(1, tools.executed);
        assertEquals("Firmware identified", visible.toString());
        assertEquals(answer, result.get(5));
        assertEquals(1, previous.size(), "The caller commits a complete turn; the loop does not mutate old history");
    }

    @Test void streamedItemsWithEmptyCompletedOutputStillRunToolsAndAnswer() throws Exception {
        // Field capture 2026-10-08: a backend that streams items via response.output_item.done
        // but sends "output": [] in response.completed. Before the fix this made the whole turn
        // a silent no-op - no tool ran, no text was shown, no error was raised.
        FakeTools tools = new FakeTools();
        AtomicInteger requests = new AtomicInteger();
        String callStream = "data: " + object("type", "response.output_item.done", "output_index", 0,
                "item", call("call_1", "ecu_info", "{}")) + "\n\n" + completed(array());
        JSONObject answer = object("type", "message", "role", "assistant", "content",
                array(object("type", "output_text", "text", "Trigger looks healthy", "annotations", array())));
        String answerStream = "data: {\"type\":\"response.output_text.delta\",\"delta\":\"Trigger looks healthy\"}\n\n"
                + "data: " + object("type", "response.output_item.done", "output_index", 0, "item", answer) + "\n\n"
                + completed(array());
        StringBuilder visible = new StringBuilder();
        JSONArray result = ChatGptAgent.run((input, defs, delta, c) -> readResponse(new StringReader(
                        requests.getAndIncrement() == 0 ? callStream : answerStream), delta, c),
                tools, array(), "how do i troubleshoot trigger", visible::append, ignored -> {}, new Cancellation());
        assertEquals(1, tools.executed);
        assertEquals("Trigger looks healthy", visible.toString());
        assertEquals(answer, result.get(result.size() - 1));
    }

    @Test void neverExecutesPartialOrFailedStreams() {
        for (String ending : new String[]{"", "data: {\"type\":\"response.failed\"}\n\n",
                "data: {\"type\":\"response.incomplete\"}\n\n"}) {
            FakeTools tools = new FakeTools();
            String stream = "data: " + object("type", "response.output_item.done", "item", call("id", "ecu_info", "{}")) + "\n\n" + ending;
            assertThrows(IOException.class, () -> ChatGptAgent.run((input, defs, delta, c) ->
                            readResponse(new StringReader(stream), delta, c), tools, array(), "help", ignored -> {}, ignored -> {}, new Cancellation()));
            assertEquals(0, tools.executed);
        }
    }

    @Test void cancellationAndConnectionChangeDiscardCompletedToolRequests() {
        FakeTools tools = new FakeTools();
        Cancellation cancellation = new Cancellation();
        assertThrows(CancellationException.class, () -> ChatGptAgent.run((input, defs, delta, c) -> {
            c.cancel();
            return response(call("id", "ecu_info", "{}"));
        }, tools, array(), "help", ignored -> {}, ignored -> {}, cancellation));
        assertEquals(0, tools.executed);
        assertThrows(IOException.class, () -> ChatGptAgent.run((input, defs, delta, c) -> {
            tools.connected = false;
            return response(call("id", "ecu_info", "{}"));
        }, tools, array(), "help", ignored -> {}, ignored -> {}, new Cancellation()));
        assertEquals(0, tools.executed);
    }

    @Test void malformedArgumentsReturnErrorsWithoutCallingTools() throws Exception {
        for (String arguments : new String[]{"[]", "null", "{broken", repeat('x', 16385)}) {
            FakeTools tools = new FakeTools();
            AtomicInteger requests = new AtomicInteger();
            ChatGptAgent.run((input, defs, delta, c) -> {
                if (requests.getAndIncrement() == 0) {
                    return response(call("id", "ecu_info", arguments));
                }
                JSONObject result = (JSONObject) input.get(2);
                assertEquals(Boolean.FALSE, parseObject((String) result.get("output")).get("success"));
                return response(object("type", "message"));
            }, tools, array(), "help", ignored -> {}, ignored -> {}, new Cancellation());
            assertEquals(0, tools.executed);
        }
    }

    @Test void roundsCallsDuplicateIdsAndHistoryAreBounded() {
        FakeTools tools = new FakeTools();
        AtomicInteger requests = new AtomicInteger();
        assertThrows(IOException.class, () -> ChatGptAgent.run((input, defs, delta, c) ->
                        response(call("id" + requests.incrementAndGet(), "ecu_info", "{}")),
                tools, array(), "help", ignored -> {}, ignored -> {}, new Cancellation()));
        assertEquals(ChatGptAgent.MAX_ROUNDS, requests.get());
        assertEquals(ChatGptAgent.MAX_ROUNDS - 1, tools.executed);
        tools.executed = 0;
        assertThrows(IOException.class, () -> ChatGptAgent.run((input, defs, delta, c) ->
                        response(call("id", "ecu_info", "{}"), call("id", "ecu_info", "{}")),
                tools, array(), "help", ignored -> {}, ignored -> {}, new Cancellation()));
        assertEquals(1, tools.executed);
        tools.executed = 0;
        JSONArray calls = array();
        for (int i = 0; i < ChatGptAgent.MAX_CALLS + 1; i++) {
            calls.add(call("id" + i, "ecu_info", "{}"));
        }
        assertThrows(IOException.class, () -> ChatGptAgent.run((input, defs, delta, c) -> new Response("", calls),
                tools, array(), "help", ignored -> {}, ignored -> {}, new Cancellation()));
        assertEquals(ChatGptAgent.MAX_CALLS, tools.executed);
        assertThrows(IOException.class, () -> ChatGptAgent.run((input, defs, delta, c) -> {
            fail("Oversize history must not be sent");
            return null;
        }, tools, array(), repeat('x', ChatGptAgent.MAX_HISTORY), ignored -> {}, ignored -> {}, new Cancellation()));
    }

    @Test void largeToolResultsBecomeBoundedErrors() throws Exception {
        FakeTools tools = new FakeTools();
        tools.result = object("data", repeat('x', ChatGptAgent.MAX_RESULT + 1));
        AtomicInteger requests = new AtomicInteger();
        ChatGptAgent.run((input, defs, delta, c) -> {
            if (requests.getAndIncrement() == 0) {
                return response(call("id", "ecu_info", "{}"));
            }
            String output = (String) ((JSONObject) input.get(2)).get("output");
            assertTrue(output.length() < 200);
            assertEquals(Boolean.FALSE, parseObject(output).get("success"));
            return response(object("type", "message"));
        }, tools, array(), "help", ignored -> {}, ignored -> {}, new Cancellation());
    }

    private static class FakeTools implements ChatGptAgent.Tools {
        int executed;
        boolean connected = true;
        JSONObject result = object("signature", "rusEFI-test");
        @Override public JSONArray definitions() { return array(object("name", "ecu_info")); }
        @Override public void checkConnected() throws IOException {
            if (!connected) { throw new IOException("Disconnected"); }
        }
        @Override public JSONObject execute(String name, JSONObject args, Runnable checkCancellation) {
            checkCancellation.run();
            executed++;
            return result;
        }
    }

    private static JSONObject call(String id, String name, String args) {
        return object("type", "function_call", "call_id", id, "name", name, "arguments", args);
    }
    private static Response response(JSONObject... output) { return new Response("", array((Object[]) output)); }
    static JSONArray array(Object... values) {
        JSONArray result = new JSONArray();
        java.util.Collections.addAll(result, values);
        return result;
    }
    static String completed(JSONArray output) {
        return "data: " + object("type", "response.completed", "response", object("status", "completed", "output", output)) + "\n\n";
    }
    private static String repeat(char value, int count) {
        char[] chars = new char[count];
        java.util.Arrays.fill(chars, value);
        return new String(chars);
    }
}
