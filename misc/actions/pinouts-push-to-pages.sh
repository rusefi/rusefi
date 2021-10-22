#!/bin/bash

rm -rf workarea
mkdir workarea
git clone https://github.com/rusefi/rusefi.github.io.git workarea/pages


#
# drama: we have two kinds of index files:
#  1) we have proper pinouts
#  2) we need to explicitly generate directory index files since github actions does not provide directory listing
# at the moment 'gen_index.py' overrides pinouts files
# TODO: make gen_index.py skip existing file? or create pinouts, then gen_index.py, and then create pinouts AGAIN to save those while enjoying additions to index files?!
#

rsync -a -v pinouts/ workarea/pages/pinouts/


cd workarea/pages/
git add *
git status

        git config --local user.email "action@github.com"
        git config --local user.name "GitHub build-firmware Action"

git commit -m 'pinouts'
git push
