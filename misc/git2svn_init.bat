rd /s /q  c:\stuff\rusefi.git2svn


mkdir -p c:\stuff\rusefi.git2svn\rusefi.svn
cd       c:\stuff\rusefi.git2svn\rusefi.svn
svn co https://svn.code.sf.net/p/rusefi/code/trunk/ .


mkdir -p c:\stuff\rusefi.git2svn\rusefi.github
cd       c:\stuff\rusefi.git2svn\rusefi.github
git clone https://github.com/rusefi/rusefi.git
