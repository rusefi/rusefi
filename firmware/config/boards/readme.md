# Boards Directory

See [misc/jenkins/compile_other_versions/run.bat](misc/jenkins/compile_other_versions/run.bat) which is executed by build server.


See https://rusefi.com/build_server/

By definition, BOARD_NAME is a folder in firmware\config\boards

One BOARD_NAME could be producing a number of artifacts via compile_$BUNDLE_NAME.sh scripts

Work in progress: SHORT_BOARDNAME becomes BUNDLE_NAME

New board procedure

1) manually add four new files to git see gen_config.h comment