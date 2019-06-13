rem
rem Looks like this script is not really needed?
rem
set GIT2SVN_ROOT=c:\stuff\rusefi.git2svn

set RUSEFI_SVN_PATH=%GIT2SVN_ROOT%\svn
set RUSEFI_GIT_PATH=%GIT2SVN_ROOT%\git

rd /s /q  %GIT2SVN_ROOT%


mkdir -p %RUSEFI_SVN_PATH%
cd       %RUSEFI_SVN_PATH%
svn co https://svn.code.sf.net/p/rusefi/code/trunk/ .


mkdir -p %RUSEFI_GIT_PATH%
cd       %RUSEFI_GIT_PATH%
git clone https://github.com/rusefi/rusefi.git
