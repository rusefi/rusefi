
TL,DR: look at https://github.com/rusefi/technical-and-legacy/tree/main/screenshots/uaefi-screenshots-and-XML-2025-09 sample output

Here we have a tool which does the following:

* automatically saves a screenshot of each TS dialog
* slices dialogs into individual settings
* exports an XML file of TS dialogs structure with corresponding images meta info
* there is also java code to convert XML into .md - but that's definitely not the way to render documentation from such XML

step 0:

In TunerStudio, set "Options->Preferences->Load Last Project on startup".

In TunerStudio, uncheck "Help->Automatic Update Check".

Exit TunerStudio.

step 1:

./gradlew :ts_screenshots:shadowJar

Expected output: ScreenShots.jar

step 2:

cp ts_screenshots\build\libs\ts_screenshots-all.jar "C:\Program Files (x86)\EFIAnalytics\TunerStudioMS"

Copy ScreenShots.jar inside TS install folder

java -agentlib:jdwp=transport=dt_socket,server=y,suspend=y,address=*:5005 -cp lib/*;ts_screenshots-all.jar ScreenCaptureLauncher C:\stuff\rusefi\firmware\tunerstudio\generated\rusefi_uaefi.ini

java -cp lib/*;ts_screenshots-all.jar ScreenCaptureLauncher C:\stuff\rusefi\firmware\tunerstudio\generated\rusefi_uaefi.ini
