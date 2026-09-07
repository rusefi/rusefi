package com.rusefi.mcp;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;
import java.io.StringReader;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

/**
 * Runs a live {@link EcuMcpServer} dispatch loop on its own thread and lets a test exchange JSON-RPC lines with it
 * the way an MCP client would over stdio: {@link #call(String, String, long)} pushes one {@code tools/call} request
 * and blocks for its response line. Unlike the one-shot {@code StringReader} approach used by
 * {@link EcuMcpServerTest} this allows request/response round trips (connect first, then read something).
 */
final class McpStdioHarness implements AutoCloseable {
    /** Sentinel pushed into the request queue to make the server see end-of-stdin. */
    private static final String END_OF_INPUT = "<END_OF_INPUT>";

    private final BlockingQueue<String> requests = new LinkedBlockingQueue<>();
    private final BlockingQueue<String> responses = new LinkedBlockingQueue<>();
    private final AtomicReference<Throwable> failure = new AtomicReference<>();
    private final AtomicLong ids = new AtomicLong();
    private final Thread thread;

    McpStdioHarness() throws Exception {
        BufferedReader in = new BufferedReader(new StringReader("")) {
            @Override
            public String readLine() {
                try {
                    String line = requests.take();
                    return END_OF_INPUT.equals(line) ? null : line;
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    return null;
                }
            }
        };
        OutputStream lineSink = new OutputStream() {
            private final ByteArrayOutputStream pending = new ByteArrayOutputStream();

            @Override
            public synchronized void write(int b) throws IOException {
                if (b == '\n') {
                    responses.add(pending.toString("UTF-8"));
                    pending.reset();
                } else if (b != '\r') {
                    pending.write(b);
                }
            }
        };
        PrintStream out = new PrintStream(lineSink, true, "UTF-8");
        EcuMcpServer server = new EcuMcpServer(null, in, out);
        thread = new Thread(() -> {
            try {
                server.runForTests();
            } catch (Throwable t) {
                failure.set(t);
            }
        }, "EcuMcpServer under test");
        thread.setDaemon(true);
        thread.start();
    }

    /**
     * @return the {@code structuredContent} body of the tool result
     */
    JSONObject call(String tool, String argumentsJson, long timeoutMs) throws Exception {
        long id = ids.incrementAndGet();
        requests.add("{\"jsonrpc\":\"2.0\",\"id\":" + id + ",\"method\":\"tools/call\",\"params\":{\"name\":\"" + tool
                + "\",\"arguments\":" + argumentsJson + "}}");
        String line = responses.poll(timeoutMs, TimeUnit.MILLISECONDS);
        assertNotNull(line, "no response to '" + tool + "' within " + timeoutMs + "ms");
        JSONObject response = (JSONObject) new JSONParser().parse(line);
        assertEquals(id, ((Number) response.get("id")).longValue(), line);
        JSONObject result = (JSONObject) response.get("result");
        assertNotNull(result, "JSON-RPC error: " + line);
        JSONObject body = (JSONObject) result.get("structuredContent");
        assertNotNull(body, "no structuredContent: " + line);
        return body;
    }

    /**
     * Closes the server's stdin, which makes {@link EcuMcpServer} close its {@code LinkManager} and return.
     */
    @Override
    public void close() throws Exception {
        requests.add(END_OF_INPUT);
        thread.join(30_000);
        if (failure.get() != null) {
            throw new AssertionError("EcuMcpServer loop failed", failure.get());
        }
    }
}
