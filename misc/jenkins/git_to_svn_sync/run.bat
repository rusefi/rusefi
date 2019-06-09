rm -rf C:\stuff\rusefi.git2svn
mkdir C:\stuff\rusefi.git2svn
mkdir C:\stuff\rusefi.git2svn\git
cd C:\stuff\rusefi.git2svn\git
git clone https://github.com/rusefi/rusefi
cd C:\stuff\rusefi.git2svn\git\rusefi
call misc/jenkins/git_to_svn_sync/git2svn.bat