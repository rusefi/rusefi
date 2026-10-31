package com.rusefi.ui.llm;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.FilterReader;
import java.io.PrintWriter;
import java.io.Reader;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import java.util.Locale;

import static com.rusefi.ui.llm.ChatGptClient.object;

/**
 * Headless reproduction of one AI Troubleshooting turn against the real ChatGPT account store,
 * without an ECU: ECU tools are absent, knowledge tools are real. Prints a timestamped trace of
 * HTTP requests, tool calls, streamed deltas and the final outcome; raw SSE events are copied to
 * a file for offline inspection. Never prints tokens or Authorization data.
 *
 * Usage: LlmTurnProbe [sseLogFile] [prompt]
 * Close the Console first: the account store takes an exclusive lock.
 */
public final class LlmTurnProbe {
    private static final long START = System.nanoTime();

    public static void main(String[] args) throws Exception {
        Path store = Paths.get(System.getProperty("user.home"), ".rusefi", "llm-access");
        Path knowledge = Paths.get(System.getProperty("user.home"), ".rusefi", "llm-temp");
        Path sseLog = args.length > 0 ? Paths.get(args[0]) : Files.createTempFile("llm-probe-sse-", ".log");
        String prompt = args.length > 1 ? args[1] : "how do i troubleshoot trigger";
        log("store=" + store + " knowledge=" + knowledge + " sseLog=" + sseLog);
        try (PrintWriter sse = new PrintWriter(Files.newBufferedWriter(sseLog, StandardCharsets.UTF_8))) {
            ChatGptClient.Transport http = new ChatGptClient.HttpTransport();
            ChatGptClient.Transport tee = (method, url, token, contentType, body, cancellation) -> {
                log("HTTP " + method + " " + url + (body == null ? "" : " bodyChars=" + body.length()));
                long sent = System.nanoTime();
                Reader inner = http.request(method, url, token, contentType, body, cancellation);
                log("HTTP status OK in " + ms(sent) + " ms, reading stream...");
                return new FilterReader(inner) {
                    boolean first = true;
                    @Override public int read(char[] buffer, int offset, int length) throws java.io.IOException {
                        int count = super.read(buffer, offset, length);
                        if (count > 0) {
                            if (first) { first = false; log("first stream bytes after " + ms(sent) + " ms"); }
                            sse.write(buffer, offset, count);
                            sse.flush();
                        }
                        return count;
                    }
                };
            };
            try (ChatGptClient client = new ChatGptClient(store, tee)) {
                ChatGptClient.Cancellation cancellation = new ChatGptClient.Cancellation();
                String id = null;
                for (ChatGptClient.Account account : client.accounts()) {
                    log("account: " + account.label + " connected=" + account.connected + " plan=" + account.planEnabled);
                    if (account.planEnabled && id == null) { id = account.id; }
                }
                if (id == null) { log("no plan-enabled account; sign in from the Console first"); return; }
                List<ChatGptClient.Model> models = client.models(id, cancellation);
                for (ChatGptClient.Model model : models) { log("model: " + model.slug + " (" + model + ")"); }
                if (models.isEmpty()) { log("no models"); return; }
                ChatGptClient.Model model = models.get(0); // LLMTab's combo box auto-selects the first entry.
                log("using model " + model.slug);
                LocalKnowledgeTools local = new LocalKnowledgeTools(knowledge);
                ChatGptAgent.Tools tools = new ChatGptAgent.Tools() {
                    @Override public JSONArray definitions() { return LocalKnowledgeTools.definitions(); }
                    @Override public void checkConnected() { }
                    @Override public JSONObject execute(String name, JSONObject arguments, Runnable check) {
                        log("tool call: " + name + " args=" + arguments.toJSONString());
                        long started = System.nanoTime();
                        JSONObject result = LocalKnowledgeTools.handles(name) ? local.execute(name, arguments, check)
                                : object("success", false, "error", "Tool is not available in this probe.");
                        log("tool done: " + name + " in " + ms(started) + " ms, resultChars=" + result.toJSONString().length());
                        return result;
                    }
                };
                final String turnId = id;
                try {
                    JSONArray history = ChatGptAgent.run(
                            (input, definitions, delta, c) -> client.respondWithTools(turnId, model.slug, input, definitions, delta, c),
                            tools, new JSONArray(), prompt,
                            delta -> log("delta " + delta.length() + " chars: " + preview(delta)),
                            progress -> log("progress: " + progress.trim()), cancellation);
                    log("TURN COMPLETE: " + history.size() + " history items");
                    for (Object item : history) {
                        JSONObject entry = (JSONObject) item;
                        Object type = entry.containsKey("role") ? entry.get("role") : entry.get("type");
                        log("  item: " + type);
                    }
                } catch (Exception e) {
                    log("TURN FAILED after " + ms(START) + " ms: " + e.getClass().getSimpleName() + ": " + e.getMessage());
                    log("cancelled=" + cancellation.isCancelled());
                }
            }
        }
    }

    private static String preview(String text) {
        String flat = text.replace("\n", "\\n");
        return flat.length() <= 120 ? flat : flat.substring(0, 120) + "...";
    }

    private static long ms(long since) { return (System.nanoTime() - since) / 1_000_000; }

    private static void log(String message) {
        System.out.println(String.format(Locale.ROOT, "[%8.3fs] %s", (System.nanoTime() - START) / 1e9, message));
    }
}
