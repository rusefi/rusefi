# Jenkins setup

### Install Cygwin
https://cygwin.com/install.html

Cygwin packages that need to be installed:
- make
- gcc-core
- mingw64-i686-gcc-core
- mingw64-i686-gcc-g++
- mingw64-x86_64-gcc-core
- mingw64-x86_64-gcc-g++
- python3
- git
- subversion
- doxygen
- zip
- sshpass
- wget
- ccache
- graphviz

### Install Java JDK 8
https://www.oracle.com/java/technologies/javase/javase-jdk8-downloads.html

### Install Ant:
https://ant.apache.org/

### Install Jenkins
https://jenkins.io/download/

### Install Kicad
http://www.kicad-pcb.org/download/
for iBom automation

### Install for Hardware Testing
#### ST Link Utility  
http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/utility/stsw-link004.zip
#### ST Virtual Serial Driver
http://www.st.com/web/en/catalog/tools/PF257938

### System Environment Variables:
|Variable|Example|
|--|--|
|CCACHE_DIR|C:\ccache  |
|ANT_HOME|C:\Program Files\apache-ant-1.10.8|
|JAVA_HOME|C:\Program Files\Java\jdk1.8.0_251|
|RUSEFI_FTP_SERVER|ftp.yourdomain.com|
|RUSEFI_BUILD_FTP_USER|rusefi_firmware|
|RUSEFI_BUILD_FTP_PASS|secretpass|
|RUSEFI_DOXYGEN_FTP_USER|rusefi_doxygen|
|RUSEFI_DOXYGEN_FTP_PASS|secretpass|

### Add to Path:
|Tool|Example|
|--|--|
|Java|C:\Program Files (x86)\Common Files\Oracle\Java\javapath|
|Java|%JAVA_HOME%\bin|
|Ant|%ANT_HOME%\bin|
|Cygwin|C:\cygwin64\bin|
|GCC|C:\Program Files (x86)\GNU Arm Embedded Toolchain\9 2020-q2-update\bin|

### Additional Jenkins set-up
#### Additional plugins:
- Pipeline GitHub Notify Step

https://stackoverflow.com/questions/14274293/show-current-state-of-jenkins-build-on-github-repo

https://github.com/settings/tokens

Discard Old Build

smtphost: smtp.gmail.com
r***@gmail.com
useSsl=yes

### Jobs:
1) unit_tests
2) integration_and_primary_bundle
3) documentation
4) build_extra_bundles
5) git2svn_sync
