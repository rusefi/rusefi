#!/bin/bash

rm -rf workarea
mkdir workarea
git clone https://github.com/rusefi/rusefi.github.io.git workarea/pages

mv -u pinouts/ workarea/pages/
cd workarea/pages/
git add *
git status

        git config --local user.email "action@github.com"
        git config --local user.name "GitHub build-firmware Action"

git commit -m 'pinouts'
git push
