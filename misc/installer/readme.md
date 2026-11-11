# Windows installers

rusEFI has a Windows Edition license for [install4j](https://www.ej-technologies.com/install4j). The current license and projects use install4j 11, not version 12.

This directory contains branded Windows installers for rusEFI bundles:

| Product | Project | Bundle input |
|---------|---------|--------------|
| rusEFI Universal | `universal/universal-updater.install4j` | `rusefi_bundle_universal/rusefi.snapshot.universal` |
| uaEFI | `uaefi/uaefi-updater.install4j` | `rusefi_bundle_uaefi/rusefi.snapshot.uaefi` |

## Scope

An installer wraps an already-built public bundle. It must not rebuild firmware or Java artifacts. Its responsibilities are:

- install the complete bundle, including firmware, INI, console, updater, drivers, and scripts;
- provide branded launchers and shortcuts;
- bundle the supported Windows JRE;
- request administrator privileges where driver installation or ECU updates require them;
- emit install4j update metadata and checksums.

## bundled Java runtime

Full Windows board and universal ZIPs contain a private `jre/` directory. Their
launcher prefers that runtime, so no separate Java installation is needed.
Autoupdate ZIPs omit it; Linux/macOS launcher behavior is unchanged.

`firmware/bin/windows_jre.json` pins the Temurin 17 archive and SHA-256.
`firmware/bin/prepare_windows_jre.py` verifies and stages it with the runtime's
legal notices. `WINDOWS_JRE_CACHE` selects the download cache;
`WINDOWS_JRE_ARCHIVE` permits an already-downloaded archive, still checked
against the pin; `WINDOWS_JRE_MANIFEST` selects an alternate pin manifest.
Board bundles use the same settings through `make` as the universal assembler.

The universal installer retains its existing install4j-managed Zulu 17.0.14
runtime, configured by `application/jreBundles`. It excludes the imported ZIP's
`jre/` from its file set so only the install4j-managed runtime is installed.
No manually prepared JRE tarball is required. The portable ZIP's Temurin runtime
and the installer's runtime are selected independently.

Console autoupdates deliberately exclude the JRE and launcher. Release
maintainers update the portable runtime pin and the install4j runtime selection
for Java security releases, verify native-library compatibility, and publish fresh full
bundles/installers. Installed users rerun the installer; portable users extract
a fresh full bundle into a new directory rather than overlaying an old JRE.
Console releases must continue supporting the shipped Java major version.

For the universal installer, stage the public bundle in the work-area layout
below and build with install4j 11. Its bundle input is project-relative and works
in the GUI as well as on the command line:

```bash
install4jc --fail-on-warning -d /path/to/output \
  misc/installer/universal/universal-updater.install4j
```

## Proposed layout

Keep each product independent so identities, versions, artwork, and bundle paths cannot leak between installers:

```text
misc/installer/
  readme.md
  <product>/
    <product>-updater.install4j
    installation.properties
    <product>-16.png
    <product>-32.png
    <product>-48.png
    <product>-256.png
```

Generated EXEs, `md5sums`, `sha256sums`, and generated `updates.xml` should be ignored or published as release artifacts according to the release workflow. Do not commit downloaded bundle contents.

## Bundle input

Cache downloaded public bundles outside the checkout, then copy the universal
ZIP and extract it into this staging layout, as specified by
[#10530](https://github.com/rusefi/rusefi/issues/10530) and
[#10531](https://github.com/rusefi/rusefi/pull/10531):

```text
misc/installer/work-area/
  rusefi_bundle_universal.zip
  rusefi_bundle_universal/
    rusefi.snapshot.universal/
      rusefi_updater.exe
      console/rusefi_console.jar
      jre/
```

`work-area/` is ignored staging input, not source or release output. Set
`allPathsRelative="true"` and use project-relative paths in the install4j file.
The Launch4j output/icon paths are relative to `misc/console_launcher/`; they
do not change the installer work-area or the installed bundle layout. Do not
commit developer-specific paths such as `C:/stuff/...`.

Package the extracted bundle root with one `dirEntry` instead of listing individual files. Firmware names contain changing dates and hashes, so references to a specific SREC become stale on every bundle refresh. Packaging the directory also prevents accidentally omitting drivers, INI files, or updater support files.

## Launchers

The current public console bundle places both entry points in `console/rusefi_console.jar`:

| Launcher purpose | Main class |
|------------------|------------|
| Download fresh bundle and update ECU | `com.rusefi.autoupdate.Autoupdate` |
| Start console/update ECU | `com.rusefi.Launcher` |

Do not reference `rusefi_autoupdate.jar` unless the selected bundle actually contains it. Current bundles include the autoupdate class in `rusefi_console.jar`.

If the launcher working directory is `console`, its install4j classpath location is still relative to the installation root, for example `console/rusefi_console.jar`.

`installation.properties` can tell the console which installed launcher to use:

```properties
console-exe-file=../<Product>-Update-ECU.exe
```

## Product identity and versions

Every installer needs its own install4j `applicationId`. Never copy an ID from another product: Windows and install4j will otherwise treat both products as the same installation.

Keep the installer version monotonic. Do not reset a copied project to `1.0.0.0` if an older published installer has a higher version. Rebuilds that will be published should advance the version so update clients and Windows can distinguish them.

## Icons and console branding

Windows executable icons and the logo rendered inside the Java console are separate resources.

For Windows executables:

- start from square artwork; pad rectangular artwork instead of stretching it;
- provide explicit 16, 32, 48, and 256 pixel PNG files;
- assign all sizes to every launcher through `iconImageFiles`;
- set `useCustomIcon=true` and `customIconImageFiles` on the install4j `InstallerApplication` for the outer installer EXE;
- do not assume a file added to the directory is used - install4j assets have no effect until referenced by the project.

Setting `CreateDesktopLinkAction.winIconFile` changes only the shortcut icon. It does not brand the downloaded installer EXE. A shortcut normally inherits the target launcher's icon, so an explicit shortcut ICO is unnecessary when the launcher is configured correctly.

The Java console loads `/com/rusefi/logo.png` from `rusefi_console.jar`. Normally branding belongs in the Java build. For a one-off installer made from an unsigned public JAR, that resource can be replaced directly without rebuilding Java, but this is ephemeral: extracting a fresh bundle overwrites the modified JAR. Verify that the JAR is unsigned before modifying it.

## Build

Build from the product's installer directory with install4j 11:

```bash
/path/to/install4j11/bin/install4jc --fail-on-warning <product>-updater.install4j
```

`--fail-on-warning` is required. Missing bundle files and stale image paths are otherwise only warnings until launcher compilation or, worse, an incomplete installer is produced.

Use `--test --fail-on-warning` for a dry run. Note that install4j may still regenerate checksum/update metadata during a dry run, so inspect the worktree afterward.

## Validation

Minimum validation for each installer:

1. Validate project XML with `xmllint --noout`.
2. Run an install4j strict dry run.
3. Run the full strict build.
4. Confirm the media contains both launchers, the console JAR, firmware, INI, updater, and drivers.
5. Compare the generated EXE size and SHA-256 with `updates.xml`.
6. Inspect PE group-icon resources in the outer installer and each packaged launcher with `wrestool` and `icotool`; confirm 16/32/48/256 entries.
7. Install on Windows and test Explorer/UAC branding, both launchers, ECU update, uninstall, and Add/Remove Programs.

Windows caches icons aggressively. When validating a changed icon at the same path, rename the EXE or clear the Windows icon cache before concluding that the new resource was not embedded.

## Known working install4j XML pattern

The important pieces are shown below; IDs, names, paths, and versions must be product-specific:

```xml
<application name="Product Updater" applicationId="UNIQUE-ID" version="MONOTONIC-VERSION" allPathsRelative="true">
  <jreBundles jdkProviderId="Zulu" release="17/17.0.14" />
</application>

<entries>
  <dirEntry mountPoint="ROOT_MOUNT" file="../../relative/path/to/extracted/bundle" />
  <fileEntry mountPoint="CONSOLE_MOUNT" file="./installation.properties" />
</entries>

<iconImageFiles>
  <file path="./product-16.png" />
  <file path="./product-32.png" />
  <file path="./product-48.png" />
  <file path="./product-256.png" />
</iconImageFiles>
```
