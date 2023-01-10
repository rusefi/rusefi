# SD Tooth Log Converter

This program converts SD stored tooth logs to csv for analysis or replay as part of a unit test.

Create a log by setting "SD logger mode" to "trigger", then restart the ECU with an SD card present (but without USB connected), then crank/run/etc the engine to save a log. Restart again with USB to pull the log off.

# Usage

`./build.sh`

`./log_convert myToothLog.teeth convertedCsv.csv`
