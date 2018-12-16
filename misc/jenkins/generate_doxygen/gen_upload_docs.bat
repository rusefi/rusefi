
echo Should be executed from root folder
pwd
cd firmware

doxygen
IF NOT ERRORLEVEL echo doxygen run FAILED
IF NOT ERRORLEVEL 0 EXIT /B 1

set FTP_SERVER=home451478433.1and1-data.host


rem http://www.ncftp.com/download/
rem actually Cygwin http://rusefi.com/wiki/index.php?title=Internal:Software:Build_Server
cd ../doxygen
ncftpput -R -v -u u71977750-docs -p %RUSEFI_DOXYGEN_FTP_PASS% %FTP_SERVER% /html html/*

