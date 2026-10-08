package com.rusefi.ui.llm;

import com.nimbusds.jose.*;
import com.nimbusds.jose.crypto.RSASSASigner;
import com.nimbusds.jose.jwk.*;
import com.nimbusds.jose.jwk.gen.RSAKeyGenerator;
import com.nimbusds.jwt.JWTClaimsSet;
import com.nimbusds.jwt.SignedJWT;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.nio.file.attribute.PosixFilePermissions;
import java.util.*;
import java.util.concurrent.CancellationException;

import static org.junit.jupiter.api.Assertions.*;

@SuppressWarnings("unchecked")
class ChatGptClientTest {
    @TempDir Path directory;

    @Test void authorizationUsesFreshPkceAndStableHost() throws Exception {
        ChatGptClient.Attempt first = new ChatGptClient.Attempt("", "urn:uuid:host");
        ChatGptClient.Attempt second = new ChatGptClient.Attempt("client", "urn:uuid:host");
        Map<String, String> query = ChatGptClient.parseQuery(first.authorizationUri("http://127.0.0.1:1234/auth/callback").getRawQuery());
        assertEquals("dynamic_agent_client", query.get("client_id"));
        assertEquals("rusEFI Updater", query.get("agent_name_hint"));
        assertEquals("urn:uuid:host", query.get("ext_agent_host_id"));
        assertEquals("S256", query.get("code_challenge_method"));
        assertEquals(Base64.getUrlEncoder().withoutPadding().encodeToString(java.security.MessageDigest.getInstance("SHA-256")
                .digest(first.verifier.getBytes(StandardCharsets.US_ASCII))), query.get("code_challenge"));
        assertNotEquals(first.state, second.state);
        assertNotEquals(first.nonce, second.nonce);
        assertNotEquals(first.verifier, second.verifier);
        assertFalse(ChatGptClient.parseQuery(second.authorizationUri("http://127.0.0.1:5678/auth/callback").getRawQuery()).containsKey("agent_name_hint"));
    }

    @Test void callbackRejectsForgeryDenialMissingRegistrationAndClientSwap() throws Exception {
        ChatGptClient.Attempt attempt = new ChatGptClient.Attempt("", "host");
        Map<String, String> callback = new HashMap<>();
        callback.put("state", "wrong"); callback.put("code", "code"); callback.put("client_id", "issued");
        assertThrows(IOException.class, () -> attempt.validateCallback(callback));
        callback.put("state", attempt.state); callback.put("error", "access_denied");
        assertThrows(IOException.class, () -> attempt.validateCallback(callback));
        callback.remove("error"); callback.remove("client_id");
        assertThrows(IOException.class, () -> attempt.validateCallback(callback));
        callback.put("client_id", ChatGptClient.DYNAMIC_CLIENT);
        assertThrows(IOException.class, () -> attempt.validateCallback(callback));
        callback.put("client_id", "issued");
        assertEquals("issued", attempt.validateCallback(callback));
        ChatGptClient.Attempt returning = new ChatGptClient.Attempt("issued", "host");
        callback.put("state", returning.state); callback.put("client_id", "other");
        assertThrows(IOException.class, () -> returning.validateCallback(callback));
        callback.remove("client_id");
        assertEquals("issued", returning.validateCallback(callback));
        assertThrows(IOException.class, () -> ChatGptClient.parseQuery("state=a&state=b"));
    }

    @Test void validatesSignatureIssuerAudienceNonceExpiryAndSubject() throws Exception {
        RSAKey key = new RSAKeyGenerator(2048).keyID("test").generate();
        JSONObject jwks = ChatGptClient.parseObject(new JWKSet(key.toPublicJWK()).toString());
        JWTClaimsSet valid = claims("client", "nonce").build();
        assertEquals("subject", ChatGptClient.verifyIdentity(sign(key, valid), "client", "nonce", jwks).getSubject());
        List<JWTClaimsSet> invalid = Arrays.asList(
                new JWTClaimsSet.Builder(valid).issuer("https://attacker.example").build(),
                new JWTClaimsSet.Builder(valid).audience("other").build(),
                new JWTClaimsSet.Builder(valid).claim("nonce", "other").build(),
                new JWTClaimsSet.Builder(valid).expirationTime(new Date(0)).build(),
                new JWTClaimsSet.Builder(valid).subject("").build(),
                new JWTClaimsSet.Builder(valid).claim("azp", "other").build(),
                new JWTClaimsSet.Builder(valid).issueTime(new Date(System.currentTimeMillis() + 120000)).build(),
                new JWTClaimsSet.Builder(valid).audience(Arrays.asList("client", "other")).build());
        for (JWTClaimsSet bad : invalid) {
            assertThrows(IOException.class, () -> ChatGptClient.verifyIdentity(sign(key, bad), "client", "nonce", jwks));
        }
        RSAKey attacker = new RSAKeyGenerator(2048).keyID("test").generate();
        assertThrows(IOException.class, () -> ChatGptClient.verifyIdentity(sign(attacker, valid), "client", "nonce", jwks));
        assertThrows(IOException.class, () -> ChatGptClient.verifyIdentity("secret-invalid-token", "client", "nonce", jwks));
    }

    @Test void streamRequiresCompletedAndKeepsUnicodeAndMultilineData() throws Exception {
        String stream = ": heartbeat\r\nevent: response.output_text.delta\r\ndata: {\"type\":\"response.output_text.delta\",\r\n"
                + "data: \"delta\":\"Hello \\u03bb\\n\"}\r\n\r\n" + completed();
        StringBuilder visible = new StringBuilder();
        assertEquals("Hello λ\n", ChatGptClient.readEvents(new StringReader(stream), visible::append, new ChatGptClient.Cancellation()));
        assertEquals("Hello λ\n", visible.toString());
        for (String ending : Arrays.asList("", "data: [DONE]\n\n", event("response.failed"), event("response.incomplete"), event("error"))) {
            assertThrows(IOException.class, () -> ChatGptClient.readEvents(new StringReader(delta("partial") + ending),
                    ignored -> {}, new ChatGptClient.Cancellation()));
        }
        ChatGptClient.Cancellation cancellation = new ChatGptClient.Cancellation();
        cancellation.cancel();
        assertThrows(CancellationException.class, () -> ChatGptClient.readEvents(new StringReader(stream), ignored -> {}, cancellation));
    }

    @Test void streamedOutputItemsBackFillEmptyCompletedOutput() throws Exception {
        // Field capture 2026-10-08 (gpt-6.1 via the ChatGPT plan endpoint): the function call is
        // streamed through response.output_item.* events while the closing response.completed
        // event carries "output": []. Items assembled from the stream must back-fill the turn.
        JSONObject item = ChatGptClient.object("id", "fc_1", "type", "function_call", "status", "completed",
                "arguments", "{\"query\":\"trigger\"}", "call_id", "call_1", "name", "search_knowledge");
        String stream = "data: " + ChatGptClient.object("type", "response.output_item.added", "output_index", 0,
                        "item", ChatGptClient.object("id", "fc_1", "type", "function_call", "status", "in_progress",
                                "arguments", "", "call_id", "call_1", "name", "search_knowledge")) + "\n\n"
                + "data: " + ChatGptClient.object("type", "response.function_call_arguments.delta",
                        "item_id", "fc_1", "delta", "{\"query\":\"trigger\"}") + "\n\n"
                + "data: " + ChatGptClient.object("type", "response.output_item.done", "output_index", 0, "item", item) + "\n\n"
                + ChatGptAgentTest.completed(new JSONArray());
        StringBuilder visible = new StringBuilder();
        ChatGptClient.Response response = ChatGptClient.readResponse(new StringReader(stream),
                visible::append, new ChatGptClient.Cancellation());
        assertEquals("", visible.toString(), "function-call argument deltas are not user-visible text");
        assertEquals(ChatGptAgentTest.array(item), response.output);
        // A non-empty completed output remains authoritative over the streamed items.
        String both = "data: " + ChatGptClient.object("type", "response.output_item.done", "output_index", 0,
                "item", ChatGptClient.object("type", "message")) + "\n\n"
                + ChatGptAgentTest.completed(ChatGptAgentTest.array(item));
        assertEquals(ChatGptAgentTest.array(item), ChatGptClient.readResponse(new StringReader(both),
                ignored -> {}, new ChatGptClient.Cancellation()).output);
    }

    @Test void scopeIsGrantedNotAssumedAndRotatedTokensReplaceTogether() throws Exception {
        assertFalse(ChatGptClient.hasPlanScope("openid profile"));
        assertFalse(ChatGptClient.hasPlanScope("chatgpt.tokens.use.direct.extra"));
        JSONObject profile = ChatGptClient.object("scope", "openid " + ChatGptClient.PLAN_SCOPE, "refresh_token", "old-refresh");
        JSONObject response = ChatGptClient.object("access_token", "new-access", "refresh_token", "new-refresh", "expires_in", 3600, "token_type", "Bearer");
        ChatGptClient.updateTokens(profile, response, true);
        assertEquals("new-refresh", profile.get("refresh_token"));
        assertTrue(ChatGptClient.hasPlanScope((String) profile.get("scope")));
        response.put("scope", "openid");
        ChatGptClient.updateTokens(profile, response, true);
        assertFalse(ChatGptClient.hasPlanScope((String) profile.get("scope")));
        response.remove("access_token");
        assertThrows(IOException.class, () -> ChatGptClient.updateTokens(profile, response, true));
    }

    @Test void storeRetainsHostAndExcludesConcurrentProcesses() throws Exception {
        String host;
        try (ChatGptStore store = new ChatGptStore(directory)) {
            host = (String) store.data.get("host_id");
            assertTrue(host.startsWith("urn:uuid:"));
            assertThrows(IOException.class, () -> new ChatGptStore(directory));
            if (Files.getFileStore(directory).supportsFileAttributeView("posix")) {
                assertEquals(PosixFilePermissions.fromString("rw-------"), Files.getPosixFilePermissions(directory.resolve("accounts.json")));
                assertEquals(PosixFilePermissions.fromString("rwx------"), Files.getPosixFilePermissions(directory));
            }
        }
        try (ChatGptStore reopened = new ChatGptStore(directory)) {
            assertEquals(host, reopened.data.get("host_id"));
        }
    }

    @Test void malformedStoreIsRejectedWithoutDiscardingItsContents() throws Exception {
        Path file = directory.resolve("accounts.json");
        String invalid = "{\"host_id\":\"urn:uuid:test\",\"profiles\":{\"id\":\"invalid\"}}";
        Files.write(file, invalid.getBytes(StandardCharsets.UTF_8));
        assertThrows(IOException.class, () -> new ChatGptStore(directory));
        assertEquals(invalid, new String(Files.readAllBytes(file), StandardCharsets.UTF_8));
    }

    @Test void httpTransportSendsBearerButDoesNotFollowRedirectsOrExposeErrorBodies() throws Exception {
        com.sun.net.httpserver.HttpServer server = com.sun.net.httpserver.HttpServer.create(new InetSocketAddress("127.0.0.1", 0), 0);
        List<String> authorizations = new ArrayList<>();
        server.createContext("/stream", exchange -> {
            authorizations.add(exchange.getRequestHeaders().getFirst("Authorization"));
            byte[] bytes = (delta("HTTP reply") + completed()).getBytes(StandardCharsets.UTF_8);
            exchange.getResponseHeaders().set("Content-Type", "text/event-stream");
            exchange.sendResponseHeaders(200, bytes.length);
            try (OutputStream out = exchange.getResponseBody()) { out.write(bytes); }
            exchange.close();
        });
        server.createContext("/redirect", exchange -> {
            exchange.getResponseHeaders().set("Location", "/stream");
            exchange.sendResponseHeaders(302, -1);
            exchange.close();
        });
        server.createContext("/error", exchange -> {
            byte[] bytes = "private-credential-material".getBytes(StandardCharsets.UTF_8);
            exchange.sendResponseHeaders(401, bytes.length);
            try (OutputStream out = exchange.getResponseBody()) { out.write(bytes); }
            exchange.close();
        });
        server.start();
        try {
            String base = "http://127.0.0.1:" + server.getAddress().getPort();
            ChatGptClient.HttpTransport transport = new ChatGptClient.HttpTransport();
            ChatGptClient.Cancellation cancellation = new ChatGptClient.Cancellation();
            try (Reader input = transport.request("POST", base + "/stream", "test-token", "application/json", "{}", cancellation)) {
                assertEquals("HTTP reply", ChatGptClient.readEvents(input, ignored -> {}, cancellation));
            }
            assertThrows(IOException.class, () -> transport.request("GET", base + "/redirect", "test-token", "application/json", null, cancellation));
            assertEquals(Collections.singletonList("Bearer test-token"), authorizations);
            IOException failure = assertThrows(IOException.class, () -> transport.request("GET", base + "/error", "test-token", "application/json", null, cancellation));
            assertTrue(failure.getMessage().contains("401"));
            assertFalse(failure.getMessage().contains("private-credential-material"));
        } finally { server.stop(0); }
    }

    @Test void completeLoginModelDiscoveryRefreshResponseAndLogout() throws Exception {
        FakeTransport transport = new FakeTransport();
        String id;
        try (ChatGptClient client = new ChatGptClient(directory, transport)) {
            id = client.signIn(null, transport::browser, new ChatGptClient.Cancellation());
            assertTrue(client.accounts().get(0).planEnabled);
            assertEquals("available", client.models(id, new ChatGptClient.Cancellation()).get(0).slug);
            assertEquals(1, transport.refreshes); // Initial access token deliberately expires immediately.
            StringBuilder output = new StringBuilder();
            assertEquals("Hello", client.respond(id, "available", new JSONArray(), output::append, new ChatGptClient.Cancellation()));
            assertEquals("Hello", output.toString());
            assertEquals(Boolean.FALSE, transport.inference.get("store"));
            assertEquals(Boolean.TRUE, transport.inference.get("stream"));
            assertEquals("available", transport.inference.get("model"));
        }
        try (ChatGptClient reopened = new ChatGptClient(directory, transport)) {
            assertEquals(id, reopened.accounts().get(0).id);
            assertTrue(reopened.accounts().get(0).connected);
            assertTrue(reopened.signOut(id, new ChatGptClient.Cancellation()));
            assertFalse(reopened.accounts().get(0).connected);
            reopened.signIn(id, transport::browser, new ChatGptClient.Cancellation());
            assertEquals("issued-client", transport.authorization.get("client_id"));
            assertEquals(1, reopened.accounts().size());
        }
    }

    @Test void issuedClientSurvivesFailedCodeExchange() throws Exception {
        FakeTransport transport = new FakeTransport();
        transport.failExchange = true;
        try (ChatGptClient client = new ChatGptClient(directory, transport)) {
            assertThrows(IOException.class, () -> client.signIn(null, transport::browser, new ChatGptClient.Cancellation()));
            assertEquals(1, client.accounts().size());
            assertFalse(client.accounts().get(0).connected);
            transport.failExchange = false;
            client.signIn(client.accounts().get(0).id, transport::browser, new ChatGptClient.Cancellation());
            assertEquals("issued-client", transport.authorization.get("client_id"));
        }
    }

    @Test void toolInferenceAdvertisesSchemasAndRetainsOpaqueResponseItems() throws Exception {
        FakeTransport transport = new FakeTransport();
        JSONArray output = ChatGptAgentTest.array(ChatGptClient.object("type", "reasoning", "encrypted_content", "opaque"),
                ChatGptClient.object("type", "function_call", "call_id", "call_1", "name", "ecu_info", "arguments", "{}"));
        transport.responseStream = ChatGptAgentTest.completed(output);
        JSONArray tools = ChatGptAgentTest.array(ChatGptClient.object("type", "function", "name", "ecu_info"));
        try (ChatGptClient client = new ChatGptClient(directory, transport)) {
            String id = client.signIn(null, transport::browser, new ChatGptClient.Cancellation());
            ChatGptClient.Response response = client.respondWithTools(id, "available", new JSONArray(), tools,
                    ignored -> {}, new ChatGptClient.Cancellation());
            assertEquals(output, response.output);
            assertEquals(tools, transport.inference.get("tools"));
            assertEquals(Boolean.FALSE, transport.inference.get("parallel_tool_calls"));
            assertEquals(Boolean.FALSE, transport.inference.get("store"));
            assertEquals(Boolean.TRUE, transport.inference.get("stream"));
            assertEquals(ChatGptAgentTest.array("reasoning.encrypted_content"), transport.inference.get("include"));
            assertEquals(ChatGptAgent.INSTRUCTIONS, transport.inference.get("instructions"));
            transport.responseStream = completed();
            assertThrows(IOException.class, () -> client.respondWithTools(id, "available", new JSONArray(), tools,
                    ignored -> {}, new ChatGptClient.Cancellation()));
            // Completed with zero items anywhere (no streamed fallback either) is an error, not success.
            transport.responseStream = ChatGptAgentTest.completed(new JSONArray());
            assertThrows(IOException.class, () -> client.respondWithTools(id, "available", new JSONArray(), tools,
                    ignored -> {}, new ChatGptClient.Cancellation()));
        }
    }

    @Test void lastChosenConnectionPersistsAcrossReopen() throws Exception {
        FakeTransport transport = new FakeTransport();
        String id;
        try (ChatGptClient client = new ChatGptClient(directory, transport)) {
            assertEquals("", client.lastAccount());
            id = client.signIn(null, transport::browser, new ChatGptClient.Cancellation());
            client.rememberLastAccount(id);
            assertEquals(id, client.lastAccount());
        }
        try (ChatGptClient reopened = new ChatGptClient(directory, transport)) {
            assertEquals(id, reopened.lastAccount());
        }
    }

    @Test void reauthorizationCannotReplaceAConnectionsIdentity() throws Exception {
        FakeTransport transport = new FakeTransport();
        try (ChatGptClient client = new ChatGptClient(directory, transport)) {
            String id = client.signIn(null, transport::browser, new ChatGptClient.Cancellation());
            String original = new String(Files.readAllBytes(directory.resolve("accounts.json")), StandardCharsets.UTF_8);
            transport.subject = "someone-else";
            assertThrows(IOException.class, () -> client.signIn(id, transport::browser, new ChatGptClient.Cancellation()));
            assertTrue(client.accounts().get(0).connected);
            assertEquals(original, new String(Files.readAllBytes(directory.resolve("accounts.json")), StandardCharsets.UTF_8));
        }
    }

    @Test void cancellingSignInClosesTheCallbackListenerWithoutSavingTokens() throws Exception {
        FakeTransport transport = new FakeTransport();
        ChatGptClient.Cancellation cancellation = new ChatGptClient.Cancellation();
        URI[] callback = new URI[1];
        try (ChatGptClient client = new ChatGptClient(directory, transport)) {
            assertThrows(CancellationException.class, () -> client.signIn(null, uri -> {
                try {
                    callback[0] = URI.create(ChatGptClient.parseQuery(uri.getRawQuery()).get("redirect_uri"));
                    cancellation.cancel();
                } catch (IOException e) { throw new AssertionError(e); }
            }, cancellation));
            assertTrue(client.accounts().isEmpty());
            try (java.net.Socket socket = new java.net.Socket()) {
                assertThrows(IOException.class, () -> socket.connect(new InetSocketAddress(callback[0].getHost(), callback[0].getPort()), 1000));
            }
        }
    }

    @Test void missingPlanScopeBlocksInferenceAndFailedRevocationClearsLocalCredentials() throws Exception {
        FakeTransport transport = new FakeTransport();
        transport.scope = "openid email";
        try (ChatGptClient client = new ChatGptClient(directory, transport)) {
            String id = client.signIn(null, transport::browser, new ChatGptClient.Cancellation());
            assertTrue(client.accounts().get(0).connected);
            assertFalse(client.accounts().get(0).planEnabled);
            assertThrows(IOException.class, () -> client.respond(id, "any", new JSONArray(), ignored -> {}, new ChatGptClient.Cancellation()));
            assertNull(transport.inference);
            transport.failRevocation = true;
            assertFalse(client.signOut(id, new ChatGptClient.Cancellation()));
            assertFalse(client.accounts().get(0).connected);
        }
        String saved = new String(Files.readAllBytes(directory.resolve("accounts.json")), StandardCharsets.UTF_8);
        assertFalse(saved.contains("refresh_token"));
        assertFalse(saved.contains("access_token"));
        assertFalse(saved.contains("id_token"));
        assertTrue(saved.contains("issued-client"));
    }

    private static JWTClaimsSet.Builder claims(String audience, String nonce) {
        return new JWTClaimsSet.Builder().issuer(ChatGptClient.ISSUER).subject("subject").audience(audience)
                .issueTime(new Date()).expirationTime(new Date(System.currentTimeMillis() + 3600000))
                .claim("nonce", nonce).claim("email", "example@example.com");
    }

    private static String sign(RSAKey key, JWTClaimsSet claims) throws Exception {
        SignedJWT token = new SignedJWT(new JWSHeader.Builder(JWSAlgorithm.RS256).keyID("test").build(), claims);
        token.sign(new RSASSASigner(key));
        return token.serialize();
    }

    private static String delta(String text) { return "data: " + ChatGptClient.object("type", "response.output_text.delta", "delta", text) + "\n\n"; }
    private static String event(String type) { return "data: {\"type\":\"" + type + "\"}\n\n"; }
    private static String completed() { return "data: {\"type\":\"response.completed\",\"response\":{\"status\":\"completed\"}}\n\n"; }

    private static final class FakeTransport implements ChatGptClient.Transport {
        final RSAKey key = new RSAKeyGenerator(2048).keyID("test").generate();
        Map<String, String> authorization;
        JSONObject inference;
        String responseStream;
        int refreshes;
        boolean failExchange;
        boolean failRevocation;
        String subject = "subject";
        String scope = "openid " + ChatGptClient.PLAN_SCOPE;
        FakeTransport() throws Exception { }

        void browser(URI uri) {
            try {
                authorization = ChatGptClient.parseQuery(uri.getRawQuery());
                String redirect = authorization.get("redirect_uri");
                // A forged callback must not consume the pending attempt.
                assertEquals(400, callback(redirect, "forged"));
                assertEquals(200, callback(redirect, authorization.get("state")));
            } catch (Exception e) { throw new AssertionError(e); }
        }

        private int callback(String redirect, String state) throws Exception {
            HttpURLConnection connection = (HttpURLConnection) new URL(redirect + "?" + ChatGptClient.form(
                    "state", state, "code", "one-time-code", "client_id", "issued-client")).openConnection();
            connection.setConnectTimeout(2000); connection.setReadTimeout(2000);
            try { return connection.getResponseCode(); } finally { connection.disconnect(); }
        }

        @Override public Reader request(String method, String url, String token, String contentType, String body,
                                        ChatGptClient.Cancellation cancellation) throws Exception {
            cancellation.check();
            if (url.endsWith("/oauth/token")) {
                Map<String, String> values = ChatGptClient.parseQuery(body);
                assertEquals("issued-client", values.get("client_id"));
                assertEquals(ChatGptClient.RESOURCE, values.get("resource"));
                if ("refresh_token".equals(values.get("grant_type"))) {
                    refreshes++;
                    assertEquals("refresh-1", values.get("refresh_token"));
                    assertFalse(values.containsKey("scope"));
                    return new StringReader(ChatGptClient.object("access_token", "access-2", "refresh_token", "refresh-2",
                            "token_type", "Bearer", "expires_in", 3600).toJSONString());
                }
                if (failExchange) { throw new IOException("Exchange failed."); }
                assertEquals(authorization.get("redirect_uri"), values.get("redirect_uri"));
                return new StringReader(ChatGptClient.object("access_token", "access-1", "refresh_token", "refresh-1",
                        "id_token", sign(key, claims("issued-client", authorization.get("nonce")).subject(subject).build()),
                        "scope", scope, "expires_in", 1, "token_type", "Bearer").toJSONString());
            }
            if (url.endsWith("jwks.json")) { return new StringReader(new JWKSet(key.toPublicJWK()).toString()); }
            if (url.endsWith("/models")) {
                assertEquals("access-2", token);
                return new StringReader("{\"models\":[{\"slug\":\"hidden\",\"visibility\":\"hide\"},"
                        + "{\"slug\":\"available\",\"visibility\":\"list\",\"display_name\":\"Available\"}]}");
            }
            if (url.endsWith("/responses")) {
                assertEquals("access-2", token);
                assertEquals("application/json", contentType);
                inference = ChatGptClient.parseObject(body);
                return new StringReader(responseStream == null ? delta("Hello") + completed() : responseStream);
            }
            if (url.endsWith("openid-configuration")) {
                return new StringReader("{\"revocation_endpoint\":\"https://auth.openai.com/revoke\"}");
            }
            if (url.endsWith("/revoke")) {
                if (failRevocation) { throw new IOException("Offline"); }
                assertEquals("refresh_token", ChatGptClient.parseQuery(body).get("token_type_hint"));
                return new StringReader("");
            }
            throw new AssertionError("Unexpected endpoint " + url);
        }
    }
}
