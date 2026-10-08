package com.rusefi.ui.llm;

import com.rusefi.mcp.ConsoleEcuSession;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.IOException;
import java.nio.file.Path;

/** One allowlist for ECU observations and local knowledge, bound to the current Console session. */
final class TroubleshootingTools implements ChatGptAgent.Tools {
    private final ConsoleEcuSession ecu;
    private final LocalKnowledgeTools knowledge;

    TroubleshootingTools(ConsoleEcuSession ecu, Path directory) {
        this.ecu = ecu;
        knowledge = new LocalKnowledgeTools(directory);
    }

    @Override @SuppressWarnings("unchecked") public JSONArray definitions() {
        JSONArray definitions = ecu.definitions();
        definitions.addAll(LocalKnowledgeTools.definitions());
        return definitions;
    }

    @Override public void checkConnected() throws IOException { ecu.checkConnected(); }

    @Override public JSONObject execute(String name, JSONObject arguments, Runnable cancellation) throws Exception {
        cancellation.run();
        checkConnected();
        JSONObject result = LocalKnowledgeTools.handles(name) ? knowledge.execute(name, arguments, cancellation)
                : ecu.execute(name, arguments, cancellation);
        cancellation.run();
        checkConnected();
        return result;
    }
}
