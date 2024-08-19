package com.rusefi.output;

import com.devexperts.logging.Logging;
import com.rusefi.*;
import com.rusefi.newparse.DefinitionsState;
import com.rusefi.newparse.parsing.Definition;

import java.io.IOException;

/**
 * Configuration consumer which writes Signature header file
 *
 * See also gen_signature.sh
 */
public class SignatureConsumer implements ConfigurationConsumer {
    private final static Logging log = Logging.getLogging(SignatureConsumer.class);
    private final String destHeader;
    private final VariableRegistry registry;

    public SignatureConsumer(String destHeader, VariableRegistry registry) {
        log.info("Writing Signature header to " + destHeader);
        this.destHeader = destHeader;
        this.registry = registry;
    }

    public static void storeUniqueBuildId(ReaderStateImpl state, DefinitionsState parseState, String tsPath, FirmwareVersion uniqueId) {
        // store a hash as a built-in variable

        // nasty trick - do not insert signature into live data files
        if (tsPath != null) {
            parseState.addDefinition(state.getVariableRegistry(),
                    ConfigDefinition.SIGNATURE_HASH, uniqueId.encode(), Definition.OverwritePolicy.NotAllowed);
        }
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
      CHeaderConsumer.writeDefinesToFile(registry, ConfigDefinitionRootOutputFolder.getValue() + destHeader, "by SignatureConsumer");
    }
}
