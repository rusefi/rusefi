# TunerStudio User Folder Integrations

This lists every place where rusEFI Java code reads or writes TunerStudio's
per-user files. Paths not covered here (TunerStudio's communication protocol, the
`ControllerAccess` plugin API, rusEFI's own `~/.rusEFI/` folder) are listed only
where they connect to a TunerStudio folder.

TunerStudio keeps two per-user locations:

| Location | Constant / helper | Contents |
|---|---|---|
| `~/.efiAnalytics/` | `TsHelper.TS_ROOT` (`java_console/shared_io/.../ts/TsHelper.java`) | Application state: `tsUser.properties`, `TunerStudio/config/ecuDef/`, `TunerStudio/plugins/` |
| Projects folder, normally `<Documents>/TunerStudioProjects/` | `projectsDir` key in `tsUser.properties` | One folder per project: `projectCfg/`, `CurrentTune.msq`, `DataLogs/`, `dashboard/`, ... |

`TsHelper` also defines the project-relative names `CURRENT_TUNE_MSQ`
(`CurrentTune.msq`) and `MAIN_CONTROLLER_PATH` (`projectCfg/mainController.ini`).

## Summary

| # | Integration | TS path | R/W | Code | Trigger |
|---|---|---|---|---|---|
| 1 | Plugin launcher install | `~/.efiAnalytics/TunerStudio/plugins/rusefi_ts_plugin_launcher.jar` | W, delete | `TsPluginInstaller.installTsPlugin()` | Autoupdate bundle unzip |
| 2 | Developer plugin install | `~/.efianalytics/TunerStudio/plugins/` | W | Gradle `launcher_local_install` | Manual Gradle task |
| 3 | ecuDef cache priming | `~/.efiAnalytics/TunerStudio/config/ecuDef/<signature>.ini` | W | `PrimeTunerStudioCache.prime()` | Console start-up; every `.ini` resolved by `RealIniFileProvider` |
| 4 | ecuDef cache as `.ini` source | same | R | `PrimeTunerStudioCache.findInTunerStudioCache()` | `RealIniFileProvider.provide()` fallback step 4 |
| 5 | TS project auto-creation | `~/TunerStudioProjects/<name>/...` + `tsUser.properties` | R/W | `TsProjectCreator` | Console connect, opt-in |
| 6 | Post-migration project check | `tsUser.properties`, `<lastProjectPath>/projectCfg/mainController.ini` | R | `TsProjectUpdater` | End of tune migration |
| 7 | Plugin: project tune and `.ini` | `<projectsDir>/<project>/CurrentTune.msq`, `.../projectCfg/mainController.ini` | R | `TsTuneReader`, `MetaDataCache`, `TuneUploder`, `TuneUploadTab`, `UploaderStatus` | TS plugin tune upload |
| 8 | Plugin: data logs | `<projectsDir>/<project>/DataLogs/*.mlg` | R, delete | `LogUploadSelector.getLogsFolderDir()` | TS plugin log upload |

The TunerStudio *installation* (not user) folder is only located, never
modified: `TunerStudioHelper.defaultTsCandidatePaths()` probes `Program Files`
on Windows and `~/TunerStudioMS`, `~/.TunerStudioMS` elsewhere to launch
TunerStudio, remembering the result in the console's own settings.

## 1. Plugin launcher install

`java_console/autoupdate/.../TsPluginInstaller.java`, called from
`Autoupdate.tryInstallTsPlugin()` after a bundle is unzipped. It copies
`rusefi_ts_plugin_launcher.jar` from the bundle's working directory into
`~/.efiAnalytics/TunerStudio/plugins/` (creating the folder), overwriting any
existing copy, and deletes the legacy `rusefi_plugin_launcher.jar`. A missing
source JAR is logged and skipped. `NoClassDefFoundError` is caught because the
updater may have replaced its own JAR (see "Java Version Constants" in
`CLAUDE.md`).

TunerStudio loads the launcher (`TsPluginLauncher`, manifest attribute
`ApplicationPlugin`). The launcher then fetches the plugin body into
`~/.rusEFI/` (`TsPluginBodyFetcher.LOCAL_JAR_FILE_NAME`), not into a
TunerStudio folder.

## 2. Developer plugin install

`java_tools/ts_plugin_launcher/build.gradle`, task `launcher_local_install`,
copies the shadow JAR to `user.home + '/.efianalytics/TunerStudio/plugins'`.
The folder name is spelled in lower case, unlike `TsHelper.TS_ROOT`
(`.efiAnalytics`). On Windows the file system ignores the difference; on Linux
and macOS this task writes to a different folder from the one the console
installer uses.

## 3. ecuDef cache priming

`java_console/io/.../opensr5/ini/PrimeTunerStudioCache.java`. TunerStudio stores
each ECU definition it has loaded as
`~/.efiAnalytics/TunerStudio/config/ecuDef/<signature without spaces>.ini`.
`prime()` copies a rusEFI `.ini` there so that TunerStudio can open a project
for that firmware without downloading or importing the definition. It:

- does nothing if the `ecuDef` folder does not exist (TunerStudio never ran);
- never overwrites an existing file with that name.

Callers:

- `StartupFrame` constructor starts the `ecuDef primer` thread with
  `primeWithLocalFile()`, which primes the `.ini` found in the current folder or
  `IniFileReader.INI_FILE_PATH` (the bundle's `.ini`);
- `RealIniFileProvider.provide()` primes every `.ini` it successfully resolves
  and parses for a connected ECU.

## 4. ecuDef cache as an `.ini` source

`PrimeTunerStudioCache.findInTunerStudioCache()` is step 4 of
`RealIniFileProvider.provide()`, after the bundled `.ini`, the current folder and
the rusEFI cache/download. It covers custom or locally built firmware that the
rusEFI server does not publish but that TunerStudio has already loaded. The file
is accepted only if the signature inside it matches the requested signature.
`RealIniFileProvider.importFromTunerStudioCache()` copies a match into rusEFI's
own cache (`~/.rusEFI/ini_database`, `SignatureHelper.importIntoCache`) so that
later connections do not depend on the TunerStudio folder. Covered by
`PrimeTunerStudioCacheTest`.

## 5. TunerStudio project auto-creation

`java_console/io/.../ts/TsProjectCreator.java`, called from
`StartupFrame.maybeAutoCreateTsProject()` after a successful connection with
calibrations. It is disabled by default: `auto_create_project=false` and
`default_project_name=rusEFI` in `shared_io.properties`.
`TsCreateProjectSandbox` (UI test sources) runs it manually.

`createIfMissing()` skips creation when `projectCfg/project.properties` already
exists under the target name, or when another project in
`~/TunerStudioProjects/` matches this ECU by `ecuUid`, `firmwareDescription`
(signature) or serial port, in that order. The `ecuUid` is a name-based UUID of
the `device_uid1` bytes, so firmware upgrades on the same hardware reuse the
project; firmware without that field gets a random UUID.

When it creates a project it writes:

| File | Content |
|---|---|
| `projectCfg/mainController.ini` | Copy of the ECU's `.ini` |
| `CurrentTune.msq` | Tune generated from the configuration image |
| `projectCfg/project.properties` and `.bkup` | Project name, `ecuUid`, signature, serial driver/port settings |
| `dashboard/dashboard.dash` | `DashboardFactory` output from `com/rusefi/ts/dashboard_template.dash` |
| Empty `dashboard/`, `DataLogs/`, `inc/`, `TuneView/`, `restorePoints/` | Folder scaffold |

It then merges into `~/.efiAnalytics/tsUser.properties`: `lastProjectPath`,
`lastConnectedFirmwareSignature`, `projectsDir`, `recentlyOpenedProject_0` and
`loadLastProjectOnStart2=true`, so TunerStudio opens the new project on its next
start. Other keys are kept, but `Properties.store` rewrites the file without
TunerStudio's comments or key order.

Constraints:

- The projects folder is always `user.home/TunerStudioProjects`. The creator
  does not read the existing `projectsDir`, and it overwrites that key. On
  Windows, TunerStudio's default is `Documents\TunerStudioProjects`, so a user
  with existing projects there has `projectsDir` redirected and no longer sees
  them in TunerStudio's default project list.
- The class comment refers to `/com/rusefi/ts/ts_project_format.md`, which is
  not in the repository.

## 6. Post-migration project check

`java_console/io/.../ts/TsProjectUpdater.java`, run as the last step of
`ComposedTuneMigrator.migrateTune()`. It reads `lastProjectPath` and
`lastConnectedFirmwareSignature` from `tsUser.properties`, parses
`<lastProjectPath>/projectCfg/mainController.ini` and logs whether its
signature differs from the pre-migration `.ini`. It writes nothing; the
intended update of the project's `.ini` (issue #8357) is not implemented. All
errors, including a missing TunerStudio installation, are logged and ignored.
`TsProjectUpdaterSandbox` runs it manually.

## 7. TS plugin: project tune and `.ini`

`java_tools/ts_plugin/.../TsTuneReader.java` resolves the projects folder from
`projectsDir` in `tsUser.properties`. If that file cannot be read, it falls back
to `<JFileChooser default directory>/TunerStudioProjects` (the Documents folder
on Windows), which can differ from the folder used by `TsProjectCreator`. The
plugin passes TunerStudio's current configuration (project) name as the folder
name.

| Consumer | Reads |
|---|---|
| `MetaDataCache.getModel()` | `projectCfg/mainController.ini` metadata (page size, signature), cached per project |
| `TuneUploadTab` | `projectCfg/mainController.ini` as an `IniFileModel` |
| `TuneUploder.grabTune()` | `CurrentTune.msq` values to decide which `ControllerAccess` parameters belong to the tune |
| `UploaderStatus` | Whether `CurrentTune.msq` exists, for the upload status text |

The tune the plugin uploads is written to `~/.rusEFI/output.msq`
(`Online.outputXmlFileName`) and queued as `.msq` files in `~/.rusEFI/outbox`
(`UploadQueue.OUTBOX_FOLDER`), not in the TunerStudio project.

## 8. TS plugin: data logs

`LogUploadSelector` lists the `.mlg` files in
`<projectsDir>/<project>/DataLogs/` for the first configuration name returned by
TunerStudio. Each file has an Upload button (`Online.uploadFile`, sent directly
without the outbox) and a Delete button, which removes the file from the
TunerStudio project after confirmation.
