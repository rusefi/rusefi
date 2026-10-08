package com.rusefi.ui.llm;

import com.nimbusds.jose.JWSAlgorithm;
import com.nimbusds.jose.jwk.JWKSet;
import com.nimbusds.jose.jwk.source.ImmutableJWKSet;
import com.nimbusds.jose.proc.JWSVerificationKeySelector;
import com.nimbusds.jose.proc.SecurityContext;
import com.nimbusds.jwt.JWTClaimsSet;
import com.nimbusds.jwt.proc.DefaultJWTClaimsVerifier;
import com.nimbusds.jwt.proc.DefaultJWTProcessor;
import com.sun.net.httpserver.HttpServer;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.security.SecureRandom;
import java.util.*;
import java.util.concurrent.*;
import java.util.function.Consumer;

/** Direct, public-client ChatGPT OAuth + Responses API; call from one worker, never the EDT. */
@SuppressWarnings("unchecked") // json-simple exposes raw collections.
public final class ChatGptClient implements Closeable {
    // Never log tokens, prompts, reply text or tool arguments; URLs, sizes and event names only.
    private static final com.devexperts.logging.Logging log = com.devexperts.logging.Logging.getLogging(ChatGptClient.class);
    static final String ISSUER = "https://auth.openai.com";
    static final String RESOURCE = "https://api.openai.com/v1";
    static final String DYNAMIC_CLIENT = "dynamic_agent_client";
    static final String PLAN_SCOPE = "chatgpt.tokens.use.direct";
    private final ChatGptStore store;
    private final JSONObject profiles;
    private final Transport transport;

    public ChatGptClient(Path directory) throws Exception {
        this(directory, new HttpTransport());
    }

    ChatGptClient(Path directory, Transport transport) throws Exception {
        this.store = new ChatGptStore(directory);
        this.profiles = (JSONObject) store.data.get("profiles");
        this.transport = transport;
    }

    public static final class Account {
        public final String id;
        public final String label;
        public final boolean connected;
        public final boolean planEnabled;

        Account(String id, JSONObject profile) {
            this.id = id;
            connected = !string(profile, "access_token").isEmpty();
            planEnabled = connected && hasPlanScope(string(profile, "scope"));
            label = string(profile, "label") + " - " + string(profile, "email")
                    + (connected ? "" : " (signed out)");
        }

        @Override public String toString() { return label; }
    }

    public static final class Model {
        public final String slug;
        private final String name;

        Model(String slug, String name) { this.slug = slug; this.name = name; }
        @Override public String toString() { return name; }
    }

    public List<Account> accounts() {
        List<Account> result = new ArrayList<>();
        for (Object key : profiles.keySet()) {
            result.add(new Account((String) key, (JSONObject) profiles.get(key)));
        }
        result.sort(Comparator.comparing(account -> account.label));
        return result;
    }

    public String signIn(String accountId, Consumer<URI> openBrowser, Cancellation cancellation) throws Exception {
        JSONObject old = accountId == null ? new JSONObject() : profile(accountId);
        Attempt attempt = new Attempt(string(old, "client_id"), string(store.data, "host_id"));
        CompletableFuture<Map<String, String>> callback = new CompletableFuture<>();
        HttpServer server = HttpServer.create(new InetSocketAddress("127.0.0.1", 0), 0);
        ExecutorService listener = Executors.newSingleThreadExecutor(r -> {
            Thread thread = new Thread(r, "chatgpt-loopback");
            thread.setDaemon(true);
            return thread;
        });
        server.setExecutor(listener);
        String redirect = "http://127.0.0.1:" + server.getAddress().getPort() + "/auth/callback";
        server.createContext("/auth/callback", exchange -> {
            int status = 400;
            String message = "Sign-in could not be verified. Return to the rusEFI Troubleshooting tab.";
            try {
                if ("GET".equals(exchange.getRequestMethod()) && "/auth/callback".equals(exchange.getRequestURI().getPath())) {
                    Map<String, String> values = parseQuery(exchange.getRequestURI().getRawQuery());
                    if (attempt.state.equals(values.get("state")) && !callback.isDone()) {
                        callback.complete(values);
                        status = 200;
                        message = "You can close this browser tab and return to the rusEFI Troubleshooting tab.";
                    }
                }
            } catch (Exception ignored) {
                // Never echo callback URLs, codes or tokens into diagnostics.
            }
            byte[] bytes = message.getBytes(StandardCharsets.UTF_8);
            exchange.getResponseHeaders().set("Content-Type", "text/plain; charset=utf-8");
            exchange.getResponseHeaders().set("Cache-Control", "no-store");
            exchange.getResponseHeaders().set("Referrer-Policy", "no-referrer");
            exchange.sendResponseHeaders(status, bytes.length);
            try (OutputStream output = exchange.getResponseBody()) { output.write(bytes); }
            finally { exchange.close(); }
        });
        server.start();
        try {
            cancellation.check();
            // No id_token_hint: the URL may be copied by the UI if Desktop.browse is unavailable.
            openBrowser.accept(attempt.authorizationUri(redirect));
            long deadline = System.nanoTime() + TimeUnit.MINUTES.toNanos(5);
            Map<String, String> values;
            while (true) {
                cancellation.check();
                if (System.nanoTime() >= deadline) {
                    throw new IOException("Sign-in timed out. Click Continue with ChatGPT to try again.");
                }
                try {
                    values = callback.get(200, TimeUnit.MILLISECONDS);
                    break;
                } catch (TimeoutException ignored) { }
            }
            String clientId = attempt.validateCallback(values);
            String id = accountId == null ? UUID.randomUUID().toString() : accountId;
            JSONObject registration = copy(old);
            registration.put("client_id", clientId);
            if (accountId == null) {
                registration.put("label", "Connection " + (profiles.size() + 1));
            }
            // Retain issued registration even if code exchange fails/invalid_grant occurs.
            profiles.put(id, registration);
            store.save();
            JSONObject tokens = json("POST", ISSUER + "/api/accounts/oauth/token", null,
                    form("grant_type", "authorization_code", "client_id", clientId, "code", values.get("code"),
                            "code_verifier", attempt.verifier, "redirect_uri", redirect, "resource", RESOURCE), cancellation);
            JSONObject jwks = json("GET", ISSUER + "/.well-known/jwks.json", null, null, cancellation);
            JWTClaimsSet identity = verifyIdentity(string(tokens, "id_token"), clientId, attempt.nonce, jwks);
            if (!string(old, "subject").isEmpty() && !string(old, "subject").equals(identity.getSubject())) {
                throw new IOException("Returned account does not match this saved connection. Add a new connection instead.");
            }
            JSONObject updated = copy(registration);
            updated.put("subject", identity.getSubject());
            updated.put("issuer", identity.getIssuer());
            updated.put("email", Optional.ofNullable(identity.getStringClaim("email")).orElse(identity.getSubject()));
            updateTokens(updated, tokens, false);
            cancellation.check();
            profiles.put(id, updated);
            store.save();
            return id;
        } finally {
            server.stop(0);
            listener.shutdownNow();
        }
    }

    static JWTClaimsSet verifyIdentity(String token, String clientId, String nonce, JSONObject jwks) throws IOException {
        try {
            DefaultJWTProcessor<SecurityContext> processor = new DefaultJWTProcessor<>();
            processor.setJWSKeySelector(new JWSVerificationKeySelector<>(JWSAlgorithm.RS256,
                    new ImmutableJWKSet<>(JWKSet.parse(jwks.toJSONString()))));
            DefaultJWTClaimsVerifier<SecurityContext> verifier = new DefaultJWTClaimsVerifier<>(
                    new HashSet<>(Collections.singletonList(clientId)),
                    new JWTClaimsSet.Builder().issuer(ISSUER).claim("nonce", nonce).build(),
                    new HashSet<>(Arrays.asList("sub", "exp", "iat", "aud", "iss", "nonce")), null);
            verifier.setMaxClockSkew(5);
            processor.setJWTClaimsSetVerifier(verifier);
            JWTClaimsSet claims = processor.process(token, null);
            if (claims.getSubject() == null || claims.getSubject().isEmpty()
                    || claims.getIssueTime().getTime() > System.currentTimeMillis() + 5000
                    || ((claims.getAudience().size() > 1 || claims.getClaim("azp") != null)
                        && !clientId.equals(claims.getStringClaim("azp")))) {
                throw new IllegalArgumentException();
            }
            return claims;
        } catch (Exception e) {
            throw new IOException("ChatGPT identity verification failed. Please sign in again.");
        }
    }

    public List<Model> models(String id, Cancellation cancellation) throws Exception {
        JSONObject result = json("GET", RESOURCE + "/models", accessToken(id, cancellation), null, cancellation);
        List<Model> models = new ArrayList<>();
        Object catalog = result.get("models");
        if (!(catalog instanceof JSONArray)) {
            throw new IOException("ChatGPT returned an unsupported model catalog.");
        }
        for (Object item : (JSONArray) catalog) {
            JSONObject model = (JSONObject) item;
            String slug = string(model, "slug");
            if ("list".equals(string(model, "visibility")) && !slug.isEmpty()) {
                models.add(new Model(slug, string(model, "display_name").isEmpty() ? slug : string(model, "display_name")));
            }
        }
        return models;
    }

    /** Returns only after response.completed. Callers must not commit partial replies to conversation history. */
    public String respond(String id, String model, JSONArray history, Consumer<String> delta, Cancellation cancellation) throws Exception {
        JSONObject body = object("model", model, "input", history, "store", false, "stream", true);
        try (Reader reader = transport.request("POST", RESOURCE + "/responses", accessToken(id, cancellation),
                "application/json", body.toJSONString(), cancellation)) {
            return readEvents(reader, delta, cancellation);
        }
    }

    public Response respondWithTools(String id, String model, JSONArray history, JSONArray tools,
                                     Consumer<String> delta, Cancellation cancellation) throws Exception {
        JSONObject body = object("model", model, "input", history, "store", false, "stream", true,
                "tools", tools, "parallel_tool_calls", false, "instructions", ChatGptAgent.INSTRUCTIONS);
        JSONArray include = new JSONArray();
        include.add("reasoning.encrypted_content");
        body.put("include", include);
        try (Reader reader = transport.request("POST", RESOURCE + "/responses", accessToken(id, cancellation),
                "application/json", body.toJSONString(), cancellation)) {
            Response response = readResponse(reader, delta, cancellation);
            if (response.output == null || response.output.isEmpty()) {
                throw new IOException("ChatGPT completed without response items.");
            }
            return response;
        }
    }

    static final class Response {
        final String text;
        final JSONArray output;

        Response(String text, JSONArray output) {
            this.text = text;
            this.output = output;
        }
    }

    private String accessToken(String id, Cancellation cancellation) throws Exception {
        JSONObject current = profile(id);
        if (!hasPlanScope(string(current, "scope"))) {
            throw new IOException("ChatGPT plan usage is not enabled. Continue with ChatGPT and grant plan usage.");
        }
        if (string(current, "access_token").isEmpty()) {
            throw new IOException("Please continue with ChatGPT to sign in.");
        }
        if (((Number) current.get("expires_at")).longValue() <= System.currentTimeMillis() + 60000) {
            if (string(current, "refresh_token").isEmpty()) {
                throw new IOException("Session expired. Continue with ChatGPT to sign in again.");
            }
            log.info("access token expired, refreshing");
            JSONObject tokens = json("POST", ISSUER + "/api/accounts/oauth/token", null,
                    form("grant_type", "refresh_token", "client_id", string(current, "client_id"),
                            "refresh_token", string(current, "refresh_token"), "resource", RESOURCE), cancellation);
            JSONObject updated = copy(current);
            updateTokens(updated, tokens, true);
            profiles.put(id, updated);
            store.save();
            current = updated;
        }
        if (!hasPlanScope(string(current, "scope"))) {
            throw new IOException("ChatGPT plan permission is no longer granted. Sign in again.");
        }
        return string(current, "access_token");
    }

    static void updateTokens(JSONObject profile, JSONObject tokens, boolean refresh) throws IOException {
        if (!"Bearer".equalsIgnoreCase(string(tokens, "token_type")) || string(tokens, "access_token").isEmpty()
                || !(tokens.get("expires_in") instanceof Number) || ((Number) tokens.get("expires_in")).longValue() <= 0) {
            throw new IOException("ChatGPT returned an incomplete token response.");
        }
        profile.put("access_token", tokens.get("access_token"));
        profile.put("expires_at", System.currentTimeMillis() + ((Number) tokens.get("expires_in")).longValue() * 1000);
        for (String field : Arrays.asList("refresh_token", "id_token", "scope")) {
            if (tokens.get(field) instanceof String) {
                profile.put(field, tokens.get(field));
            } else if (!refresh) {
                profile.remove(field);
            }
        }
    }

    /** Always clears local tokens; false means remote revocation could not be confirmed. */
    public boolean signOut(String id, Cancellation cancellation) throws Exception {
        JSONObject current = profile(id);
        boolean revoked = true;
        try {
            if (!string(current, "refresh_token").isEmpty()) {
                JSONObject discovery = json("GET", ISSUER + "/.well-known/openid-configuration", null, null, cancellation);
                String endpoint = string(discovery, "revocation_endpoint");
                URI uri = URI.create(endpoint);
                if (!"https".equals(uri.getScheme()) || !"auth.openai.com".equals(uri.getHost())
                        || uri.getUserInfo() != null || (uri.getPort() != -1 && uri.getPort() != 443)) {
                    throw new IOException("Invalid revocation endpoint.");
                }
                try (Reader ignored = transport.request("POST", endpoint, null, "application/x-www-form-urlencoded",
                        form("token", string(current, "refresh_token"), "token_type_hint", "refresh_token",
                                "client_id", string(current, "client_id")), cancellation)) { }
            }
        } catch (Exception e) {
            revoked = false;
        } finally {
            for (String field : Arrays.asList("access_token", "refresh_token", "id_token", "scope", "expires_at")) {
                current.remove(field);
            }
            store.save();
        }
        return revoked;
    }

    static String readEvents(Reader input, Consumer<String> delta, Cancellation cancellation) throws Exception {
        return readResponse(input, delta, cancellation).text;
    }

    static Response readResponse(Reader input, Consumer<String> delta, Cancellation cancellation) throws Exception {
        BufferedReader reader = new BufferedReader(input);
        StringBuilder data = new StringBuilder();
        StringBuilder answer = new StringBuilder();
        // Some backends send "output": [] in response.completed and deliver the items only as
        // response.output_item.done events (observed 2026-10-08 with gpt-6.1 on the plan endpoint).
        JSONArray streamed = new JSONArray();
        String line;
        while ((line = reader.readLine()) != null) {
            cancellation.check();
            if (line.isEmpty()) {
                if (data.length() != 0) {
                    JSONObject event = parseObject(data.toString());
                    data.setLength(0);
                    String type = string(event, "type");
                    if ("response.output_text.delta".equals(type) || "response.refusal.delta".equals(type)) {
                        String text = string(event, "delta");
                        answer.append(text);
                        if (answer.length() > 256 * 1024) {
                            throw new IOException("ChatGPT reply exceeded the text limit.");
                        }
                        delta.accept(text);
                    } else if ("response.output_item.done".equals(type)) {
                        if (event.get("item") instanceof JSONObject) {
                            if (streamed.size() >= 256) {
                                throw new IOException("ChatGPT returned too many response items.");
                            }
                            streamed.add(event.get("item"));
                        }
                    } else if ("response.completed".equals(type)) {
                        JSONObject response = (JSONObject) event.get("response");
                        if (response == null || !"completed".equals(string(response, "status"))) {
                            throw new IOException("Response did not complete successfully.");
                        }
                        Object output = response.get("output");
                        JSONArray items = output instanceof JSONArray ? (JSONArray) output : null;
                        boolean assembled = (items == null || items.isEmpty()) && !streamed.isEmpty();
                        if (assembled) {
                            items = streamed;
                        }
                        log.info("response completed: " + (items == null ? "no output array" : items.size() + " items")
                                + (assembled ? " (assembled from streamed output_item.done events)" : "")
                                + ", " + answer.length() + " text chars");
                        return new Response(answer.toString(), items);
                    } else if ("response.incomplete".equals(type)) {
                        log.error("response incomplete");
                        throw new IOException("ChatGPT returned an incomplete response; the partial reply was not saved.");
                    } else if ("response.failed".equals(type) || "error".equals(type)) {
                        JSONObject details = event.get("response") instanceof JSONObject ? (JSONObject) event.get("response") : event;
                        details = details.get("error") instanceof JSONObject ? (JSONObject) details.get("error") : details;
                        String code = string(details, "code");
                        log.error("response " + type + ", code=" + code);
                        if ("subscription_sharing_usage_limit_exceeded".equals(code) || "subscription_sharing_usage_unavailable".equals(code)) {
                            throw new IOException("ChatGPT plan usage is unavailable or its limit was reached. Open ChatGPT Settings > Usage.");
                        }
                        throw new IOException("ChatGPT response failed; the partial reply was not saved. Try again or check ChatGPT Settings > Usage.");
                    }
                }
            } else if (line.startsWith("data:")) {
                String value = line.substring(5);
                if (value.startsWith(" ")) { value = value.substring(1); }
                if ("[DONE]".equals(value)) { break; }
                if (data.length() > 0) { data.append('\n'); }
                data.append(value);
                if (data.length() > 4 * 1024 * 1024) {
                    throw new IOException("ChatGPT stream event exceeded the size limit.");
                }
            }
        }
        cancellation.check();
        throw new IOException("Connection ended before response.completed; the partial reply was not saved.");
    }

    private JSONObject json(String method, String url, String token, String body, Cancellation cancellation) throws Exception {
        try (Reader reader = transport.request(method, url, token, "application/x-www-form-urlencoded", body, cancellation)) {
            StringBuilder text = new StringBuilder();
            char[] buffer = new char[4096];
            int count;
            while ((count = reader.read(buffer)) != -1) {
                cancellation.check();
                text.append(buffer, 0, count);
                if (text.length() > 4 * 1024 * 1024) { throw new IOException("ChatGPT response was too large."); }
            }
            return parseObject(text.toString());
        }
    }

    private JSONObject profile(String id) throws IOException {
        JSONObject profile = (JSONObject) profiles.get(id);
        if (profile == null) { throw new IOException("Select a saved ChatGPT connection."); }
        return profile;
    }

    static JSONObject parseObject(String text) throws IOException {
        try {
            Object parsed = new JSONParser().parse(text);
            if (!(parsed instanceof JSONObject)) { throw new IllegalArgumentException(); }
            return (JSONObject) parsed;
        } catch (Exception e) {
            throw new IOException("ChatGPT returned an invalid JSON response.");
        }
    }

    static boolean hasPlanScope(String scope) { return Arrays.asList(scope.split("\\s+")).contains(PLAN_SCOPE); }
    static String string(JSONObject object, String key) { return object.get(key) instanceof String ? (String) object.get(key) : ""; }
    static JSONObject copy(JSONObject original) { JSONObject result = new JSONObject(); result.putAll(original); return result; }

    @SuppressWarnings("unchecked")
    public static JSONObject object(Object... pairs) {
        JSONObject result = new JSONObject();
        for (int i = 0; i < pairs.length; i += 2) { result.put(pairs[i], pairs[i + 1]); }
        return result;
    }

    static String form(String... pairs) throws UnsupportedEncodingException {
        StringJoiner result = new StringJoiner("&");
        for (int i = 0; i < pairs.length; i += 2) {
            result.add(URLEncoder.encode(pairs[i], "UTF-8") + "=" + URLEncoder.encode(pairs[i + 1], "UTF-8"));
        }
        return result.toString();
    }

    static Map<String, String> parseQuery(String raw) throws IOException {
        Map<String, String> result = new HashMap<>();
        if (raw == null || raw.length() > 16384) { throw new IOException("Invalid sign-in callback."); }
        for (String pair : raw.split("&")) {
            String[] parts = pair.split("=", 2);
            String key = URLDecoder.decode(parts[0], "UTF-8");
            if (parts.length != 2 || result.put(key, URLDecoder.decode(parts[1], "UTF-8")) != null) {
                throw new IOException("Invalid sign-in callback.");
            }
        }
        return result;
    }

    static final class Attempt {
        final String state = random();
        final String nonce = random();
        final String verifier = random();
        final String clientId;
        final String hostId;
        Attempt(String clientId, String hostId) { this.clientId = clientId; this.hostId = hostId; }

        URI authorizationUri(String redirect) throws Exception {
            String query = form("client_id", clientId.isEmpty() ? DYNAMIC_CLIENT : clientId,
                    "ext_agent_host_id", hostId, "response_type", "code", "redirect_uri", redirect,
                    "scope", "openid profile email offline_access resource.invoke " + PLAN_SCOPE,
                    "resource", RESOURCE, "state", state, "nonce", nonce,
                    "code_challenge_method", "S256", "code_challenge", Base64.getUrlEncoder().withoutPadding()
                            .encodeToString(MessageDigest.getInstance("SHA-256").digest(verifier.getBytes(StandardCharsets.US_ASCII))));
            if (clientId.isEmpty()) { query += "&" + form("agent_name_hint", "rusEFI Updater"); }
            return URI.create(ISSUER + "/api/accounts/authorize?" + query);
        }

        String validateCallback(Map<String, String> values) throws IOException {
            if (!state.equals(values.get("state"))) { throw new IOException("Sign-in state did not match."); }
            if (values.containsKey("error")) { throw new IOException("ChatGPT sign-in was declined or failed. Please try again."); }
            if (values.getOrDefault("code", "").isEmpty()) { throw new IOException("Sign-in did not return a code."); }
            String issued = values.getOrDefault("client_id", clientId);
            if (issued.isEmpty() || DYNAMIC_CLIENT.equals(issued) || (!clientId.isEmpty() && !clientId.equals(issued))) {
                throw new IOException("Sign-in returned an invalid client registration.");
            }
            return issued;
        }

        private static String random() {
            byte[] bytes = new byte[32];
            new SecureRandom().nextBytes(bytes);
            return Base64.getUrlEncoder().withoutPadding().encodeToString(bytes);
        }
    }

    interface Transport {
        Reader request(String method, String url, String token, String contentType, String body, Cancellation cancellation) throws Exception;
    }

    public static final class Cancellation {
        private volatile boolean cancelled;
        private volatile HttpURLConnection connection;
        public void cancel() {
            cancelled = true;
            HttpURLConnection active = connection;
            if (active != null) {
                Thread closer = new Thread(active::disconnect, "chatgpt-cancel");
                closer.setDaemon(true);
                closer.start();
            }
        }
        public boolean isCancelled() { return cancelled; }
        void check() { if (cancelled || Thread.currentThread().isInterrupted()) { throw new CancellationException(); } }
    }

    static final class HttpTransport implements Transport {
        @Override
        public Reader request(String method, String url, String token, String contentType, String body, Cancellation cancellation) throws Exception {
            cancellation.check();
            HttpURLConnection connection = (HttpURLConnection) new URL(url).openConnection();
            cancellation.connection = connection;
            try {
                cancellation.check();
                connection.setInstanceFollowRedirects(false);
                connection.setConnectTimeout(15000);
                connection.setReadTimeout(60000);
                connection.setRequestMethod(method);
                connection.setRequestProperty("Accept", "application/json, text/event-stream");
                if (token != null) { connection.setRequestProperty("Authorization", "Bearer " + token); }
                if (body != null) {
                    connection.setDoOutput(true);
                    connection.setRequestProperty("Content-Type", contentType);
                    byte[] bytes = body.getBytes(StandardCharsets.UTF_8);
                    connection.setFixedLengthStreamingMode(bytes.length);
                    try (OutputStream out = connection.getOutputStream()) { out.write(bytes); }
                }
                int status = connection.getResponseCode();
                log.info(method + " " + url + ": HTTP " + status);
                if (status < 200 || status >= 300) {
                    // Do not echo an auth response body, which may contain credentials or callback data.
                    throw new IOException("ChatGPT request failed (HTTP " + status + "). "
                            + (status == 401 || (status == 400 && url.endsWith("/oauth/token")) ? "Continue with ChatGPT to sign in again."
                            : status == 403 || status == 429 ? "Check plan access in ChatGPT Settings > Usage." : "Please try again."));
                }
                return new FilterReader(new InputStreamReader(connection.getInputStream(), StandardCharsets.UTF_8)) {
                    @Override public void close() throws IOException {
                        try { super.close(); }
                        finally { connection.disconnect(); cancellation.connection = null; }
                    }
                };
            } catch (Exception e) {
                connection.disconnect();
                cancellation.connection = null;
                throw e;
            }
        }
    }

    @Override public void close() throws IOException { store.close(); }
}
