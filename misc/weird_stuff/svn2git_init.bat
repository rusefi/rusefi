rd /s /q c:\stuff\rusefi.svn2git 

mkdir -p c:\stuff\rusefi.svn2git\rusefi.svn
cd       c:\stuff\rusefi.svn2git\rusefi.svn
svn co https://svn.code.sf.net/p/rusefi/code/trunk/ .

mkdir -p c:\stuff\rusefi.svn2git\rusefi.github
cd       c:\stuff\rusefi.svn2git\rusefi.github
git clone https://github.com/rusefi/rusefi.git
