package com.rusefi.autoupdate;

/**
 * Manual harness that installs the TunerStudio plugin via {@link TsPluginInstaller}.
 */
public class TsPluginInstallerSandbox {
    public static void main(String[] args) {
        TsPluginInstaller.installTsPlugin();
    }
}
