#!/bin/sh

# https://superuser.com/questions/611538/is-there-a-way-to-display-a-countdown-or-stopwatch-timer-in-a-terminal

while true; do echo -ne "`date +%H:%M:%S:%N`\r"; done