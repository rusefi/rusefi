# Java LLM tab sandbox

Run from the repository root:

```sh
./gradlew :ui:runLLMTabSandbox
```

Or run `com.rusefi.ui.LLMTabSandbox.main()` with the `:ui` test runtime
classpath in your IDE. It opens a standalone Swing window for authentication
and UI development without opening an ECU port. The launcher remains in the
test source set; its shared UI, authentication and JWT dependency are now
packaged in the Console JAR. Troubleshooting requests require the connected
Console integration, whose tab is currently disabled by a `false &&` guard in
`ConsoleUI`. See [Console troubleshooting assistant](llm-troubleshooting.md).

The sandbox first prepares a valid cached source ZIP. If none is usable, it
shows only the source download prompt, **Start Download**, progress and status.
Click **Start Download** and wait for download/extraction to complete before
the account and chat controls appear. A failed attempt can be retried. To use
isolated account and source directories for development:

```sh
./gradlew :ui:runLLMTabSandbox --args="/tmp/rusefi-chatgpt-dev /tmp/rusefi-source-cache"
```

1. Click **Continue with ChatGPT** and complete sign-in in your system browser.
   If the browser cannot open, copy the link from the dialog. The browser must
   reach the Java process's `127.0.0.1` loopback interface. Sign-in expires after
   five minutes; **Stop** cancels it.
2. Approve ChatGPT plan usage for the app. Eligibility and usage limits are
   controlled by OpenAI and your account. Identity-only sign-in leaves Send
   disabled. **Continue with ChatGPT** reauthorizes the selected connection;
   **Add account** creates a separate account/workspace registration.
   New registrations send the app name **rusEFI Updater** to ChatGPT. Returning
   sign-ins reuse the saved client ID and do not send a new name hint.
3. Choose a model from the account's catalog. The standalone launcher has no
   ECU connection, so **Send** asks you to connect in Console. When the connected
   Console tab is enabled, Send streams replies and invokes read-only ECU tools.
   Enter inserts a new line.
4. **Stop** cancels the current operation. Failed, incomplete or stopped replies
   remain visible but are excluded from later conversation context.
   **New conversation** clears the terminal and conversation context. Switching
   connections also clears context. Only complete turns enter the in-memory
   history; chat transcripts are not saved to disk by the sandbox.
5. Review usage limits in **ChatGPT Settings > Usage**. **Sign out** attempts session
   revocation and clears local tokens while retaining the client registration.
   If revocation cannot be confirmed, the terminal tells you to disconnect the
   app in ChatGPT Settings.

Saved connections are loaded on restart; select one to load its models and
check model access. The client refreshes expiring tokens automatically.
Reselect an account to retry a failed model-catalog request.

## Local storage and protocol

The default directory is `~/.rusefi/llm-access`. Override it with the first main
argument, or with Gradle's `--args="/absolute/private/directory"`. The directory
contains a stable installation host ID and separate account registrations in
`accounts.json`, plus `session.lock`. Account credentials are sensitive: do not
share these files. Files are atomically replaced and restricted to their owner
(POSIX permissions or Windows ACLs); storage fails if those protections are
unavailable. Only one sandbox may use a given directory at a time, preventing
concurrent refresh-token rotation.

When upgrading from the old `~/.rusefi/llm-sandbox` location, close the sandbox
and move that directory to `~/.rusefi/llm-access` to retain the installation UUID
and saved authorizations. Do not overwrite an existing account store.

The implementation follows the public-client OAuth flow in
[the Sign in with ChatGPT cookbook](https://developers.openai.com/cookbook/articles/sign-in-with-chatgpt)
and [the registration guide](https://developers.openai.com/siwc/token-sharing-open-source/sign-in):
loopback callback, fresh state/nonce/PKCE, an issued dynamic client ID retained
before code exchange, and Nimbus verification of RS256 signature, issuer,
audience, expiry and nonce. It never uses an ID token as an API credential.

Model discovery and inference use the public `/v1/models` and `/v1/responses`
endpoints with the granted OAuth access token. Each inference request sets
`store: false` and `stream: true`; success requires `response.completed`.
See [models and inference](https://developers.openai.com/siwc/token-sharing-open-source/models-and-inference).
The connected Console provides the read-only tools documented in the
[troubleshooting guide](llm-troubleshooting.md). Authorization URLs omit `id_token_hint`
so the browser-fallback dialog does not expose a retained identity token.

## Validation

```sh
./gradlew :ui:test --tests com.rusefi.ui.llm.ChatGptClientTest --max-workers=12
```

Tests exercise a local loopback callback with a fake OAuth/API transport and
locally signed JWTs. They do not contact OpenAI or consume ChatGPT usage.
Live browser sign-in and inference require an eligible account and remain a
manual check. Windows ACL behavior also requires validation on Windows.
