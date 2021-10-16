package com.rusefi.output;

import com.rusefi.ConfigDefinition;
import com.rusefi.ReaderState;
import com.rusefi.util.LazyFile;

import java.io.IOException;
import java.util.Arrays;

import static com.rusefi.ConfigDefinition.EOL;

public class FileFsioSettingsConsumer extends FsioSettingsConsumer {
    private final LazyFile enumFile;
    private final LazyFile gettersFile;
    private final LazyFile namesFile;
    private final LazyFile stringsFile;

    public FileFsioSettingsConsumer(ReaderState state,
                                    String destCFsioConstantsFileName,
                                    String destCFsioGettersFileName,
                                    String namesCFileName,
                                    String stringsCFileName) {
        super(state);
        enumFile = new LazyFile(destCFsioConstantsFileName);
        gettersFile = new LazyFile(destCFsioGettersFileName);
        namesFile = new LazyFile(namesCFileName);
        stringsFile = new LazyFile(stringsCFileName);
    }

    @Override
    public void startFile() {
        for (LazyFile file : Arrays.asList(enumFile, gettersFile, stringsFile, namesFile)) {
            file.write("// this file " + ConfigDefinition.MESSAGE + ConfigDefinition.EOL + EOL);
            file.write("// by " + getClass() + EOL);
        }
    }

    @Override
    public void endFile() throws IOException {
        enumFile.write(getEnumDefinition());
        enumFile.close();

        gettersFile.write(getContent());
        gettersFile.close();

        namesFile.write(getNames());
        namesFile.close();

        stringsFile.write(getStrings());
        stringsFile.close();
    }
}
