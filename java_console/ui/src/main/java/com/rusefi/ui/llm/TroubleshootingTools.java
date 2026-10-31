package com.rusefi.ui.llm;

import com.rusefi.mcp.ConsoleEcuSession;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.IOException;
import java.nio.file.Path;
import java.util.HashSet;
import java.util.Set;

import static com.rusefi.ui.llm.ChatGptClient.object;

/** One allowlist for ECU observations and local knowledge, bound to the current Console session. */
final class TroubleshootingTools implements ChatGptAgent.Tools {
    private final ConsoleEcuSession ecu;
    private final LocalKnowledgeTools knowledge;
    private final DiagnosticCaseStore cases;
    private final Set<String> evidenceTools = new HashSet<>();

    TroubleshootingTools(ConsoleEcuSession ecu, Path directory, DiagnosticCaseStore cases) {
        this.ecu = ecu;
        this.cases = cases;
        knowledge = new LocalKnowledgeTools(directory);
        for (Object definition : ecu.definitions()) {
            evidenceTools.add((String) ((JSONObject) definition).get("name"));
        }
    }

    @Override @SuppressWarnings("unchecked") public JSONArray definitions() {
        JSONArray definitions = ecu.definitions();
        definitions.addAll(LocalKnowledgeTools.definitions());
        definitions.add(DiagnosticCaseStore.definition());
        return definitions;
    }

    @Override public void checkConnected() throws IOException { ecu.checkConnected(); }

    @Override public JSONObject execute(String name, JSONObject arguments, Runnable cancellation) throws Exception {
        cancellation.run();
        checkConnected();
        if ("export_diagnostic_case".equals(name)) {
            JSONObject identity = ecu.execute("ecu_info", object(), cancellation);
            return cases.export(arguments, identity, () -> {
                cancellation.run();
                checkConnected();
            });
        }
        JSONObject result = LocalKnowledgeTools.handles(name) ? knowledge.execute(name, arguments, cancellation)
                : ecu.execute(name, arguments, cancellation);
        cancellation.run();
        checkConnected();
        if (LocalKnowledgeTools.handles(name) || evidenceTools.contains(name)) {
            cases.retain(name, arguments, result);
        }
        return result;
    }
}
