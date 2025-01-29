package com.opensr5.io;

import com.opensr5.ConfigurationImageMeta;
import org.yaml.snakeyaml.DumperOptions;
import org.yaml.snakeyaml.Yaml;
import org.yaml.snakeyaml.constructor.Constructor;

import java.io.*;

public class ConfigurationImageMetaYamlUtil {
    // `dump` method is not responsible for closing output stream, passed as argument
    static void dump(final ConfigurationImageMeta meta, final OutputStream os) throws IOException {
        final Yaml yaml = new Yaml(getDumperOptions());
        // We do not want print writer to close an output stream, so we do not use try-with-resources statement below:
        final PrintWriter osw = new PrintWriter(os);
        yaml.dump(meta, osw);
    }

    static <MetaType extends ConfigurationImageMeta> MetaType loadMeta(
        final Class<MetaType> clazz,
        final InputStream is
    ) {
        final Yaml yaml = new Yaml(new Constructor(clazz));
        return yaml.load(is);
    }

    private static DumperOptions getDumperOptions() {
        final DumperOptions options = new DumperOptions();
        options.setIndent(2);
        options.setPrettyFlow(true);
        options.setDefaultFlowStyle(DumperOptions.FlowStyle.BLOCK);
        options.setNonPrintableStyle(DumperOptions.NonPrintableStyle.ESCAPE);
        return options;
    }
}
