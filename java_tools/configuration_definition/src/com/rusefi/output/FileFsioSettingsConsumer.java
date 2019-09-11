package com.rusefi.output;

import com.rusefi.ConfigDefinition;
import com.rusefi.ReaderState;
import com.rusefi.util.LazyFile;

import java.io.IOException;

import static com.rusefi.ConfigDefinition.EOL;

public class FileFsioSettingsConsumer extends FsioSettingsConsumer {
    private final LazyFile enumFile;

    public FileFsioSettingsConsumer(ReaderState state, String destCFsioConstantsFileName) {
        super(state);
        enumFile = new LazyFile(destCFsioConstantsFileName);
    }

    @Override
    public void startFile() {
        enumFile.write("// this file " + ConfigDefinition.MESSAGE + ConfigDefinition.EOL + EOL);
        enumFile.write("// by " + getClass() + EOL);
    }

    @Override
    public void endFile() throws IOException {
        enumFile.write(enumDefinition.toString());
        enumFile.close();
    }
}
