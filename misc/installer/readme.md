We have a license for https://www.ej-technologies.com/install4j

# install4j Windows Edition

key for version 11, not valid for version 12!

# Details

* For simplicity, we always use Windows Elevate UAC - i.e. Just write files wherever we want, without restrictions.

* On the other hand, it seems like jars are required in the root folder (maybe I just haven't figured out how to put jars in the console folder yet) - I could probably make a startup script.

One day we'll buy a command line license and install the internal Windows build on the Intel NUC.

## Details

We have an installer configuration file, ``rusefi-updater.install4j``.

The installer is created from a local copy of the bundle.

Unfortunately, they have relative paths; my sources are at the folder C:\stuff\rusefi

My bundle is unzipped in ``C:\stuff\bundles\TBD``
