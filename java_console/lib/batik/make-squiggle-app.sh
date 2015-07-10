#!/bin/bash
# -----------------------------------------------------------------------------
#
#   Licensed to the Apache Software Foundation (ASF) under one or more
#   contributor license agreements.  See the NOTICE file distributed with
#   this work for additional information regarding copyright ownership.
#   The ASF licenses this file to You under the Apache License, Version 2.0
#   (the "License"); you may not use this file except in compliance with
#   the License.  You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
# make-squiggle-app.sh
#
# Creates a Mac OS X application bundle for Squiggle, then opens a Finder
# window for the current directory so that the user can drag the icon
# into their desired installation location (probably /Applications).
#
# $Id$
# -----------------------------------------------------------------------------

trap 'echo Error creating application bundle.; exit 1' ERR

cd `dirname "$0"`/..

APP=Squiggle.app
CON=$APP/Contents
MAC=$CON/MacOS
RES=$CON/Resources

[ -e $APP ] && echo $APP already exists: please move it out of the way before running this script. && exit 1

echo Creating $APP in `pwd`...

mkdir $APP $CON $MAC $RES

cat >$CON/Info.plist <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>CFBundleExecutable</key>
  <string>Squiggle</string>
  <key>CFBundleVersion</key>
  <string>1.7+r608262</string>
  <key>CFBundleShortVersionString</key>
  <string>1.7+r608262</string>
  <key>CFBundleIconFile</key>
  <string>Squiggle.icns</string>
  <key>CFBundleIdentifier</key>
  <string>org.apache.batik</string>
  <key>CFBundleInfoDictionaryVersion</key>
  <string>6.0</string>
  <key>CFBundleName</key>
  <string>Squiggle</string>
  <key>CFBundlePackageType</key>
  <string>APPL</string>
  <key>CFBundleSignature</key>
  <string>????</string>
  <key>NSHumanReadableCopyright</key>
  <string>Copyright © 2008 Apache Software Foundation. All Rights Reserved.</string>
</dict>
</plist>
EOF

cat >$MAC/Squiggle <<EOF
#!/bin/sh

JAVA=/usr/bin/java
[ -e "$JAVA_HOME/bin/java" ] && JAVA="$JAVA_HOME/bin/java"

"$JAVA" -Dcom.apple.mrj.application.apple.menu.about.name=Squiggle -jar "\`dirname \$0\`/../Resources/batik-squiggle.jar"
EOF
chmod a+x $MAC/Squiggle

cp -r *.jar lib extensions $RES/
mv $RES/lib/Squiggle.icns $RES/

echo Done.

open .
