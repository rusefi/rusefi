@echo off
ST-LINK_CLI.exe -c SWD HOTPLUG -P %1 -Rst
pause