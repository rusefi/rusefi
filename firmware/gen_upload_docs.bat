svn up
doxygen
IF NOT ERRORLEVEL echo doxygen run FAILED
IF NOT ERRORLEVEL 0 EXIT /B 1


rem http://www.ncftp.com/download/
rem actually Cygwin http://rusefi.com/wiki/index.php?title=Internal:Software:Build_Server
cd ../doxygen
ncftpput -R -v -u u71977750-docs -p %RUSEFI_DOXYGEN_FTP_PASS% rusefi.com /html html/*

