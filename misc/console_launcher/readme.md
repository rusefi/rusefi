rusEFI uses Launch4j 3.50 to create the Windows console launcher. From this
directory, build it with the Launch4j installation directory as the sole
argument:

```sh
bash ./build_launcher.sh /path/to/launch4j-3.50
```

The configuration writes `rusefi_updater.exe` beside itself and uses
`favicon.ico` from this directory. Full board and universal ZIPs bundle a
private JRE in `jre`; when present, that runtime takes precedence over
`JAVA_HOME` and `PATH`. The launcher requires 64-bit Java because bundled native libraries are x64. Keep
the minimum Java version at 11. Unix launcher behavior is unchanged.

See docs/frontend.md
