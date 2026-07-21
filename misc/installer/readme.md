# Windows installers

rusEFI has a Windows Edition license for [install4j](https://www.ej-technologies.com/install4j). The current license and projects use install4j 11, not version 12.

This directory contains branded Windows installers for rusEFI bundles. The first product is uaEFI in `uaefi/`

## Scope

An installer wraps an already-built public bundle. It must not rebuild firmware or Java artifacts. Its responsibilities are:

- install the complete bundle, including firmware, INI, console, updater, drivers, and scripts;
- provide branded launchers and shortcuts;
- bundle the supported Windows JRE;
- request administrator privileges where driver installation or ECU updates require them;
- emit install4j update metadata and checksums.

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

Use a downloaded and extracted public bundle as the install4j input. Keep it outside the source tree, for example:

```text
../bundles/rusefi_bundle_<product>/rusefi.snapshot.<product>/
```

Set `allPathsRelative="true"` and use only project-relative paths in the install4j file. Do not commit developer-specific paths such as `C:/stuff/...`.

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
