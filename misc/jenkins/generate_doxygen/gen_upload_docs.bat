set RUSEFI_FTP_SERVER=home451478433.1and1-data.host
set RUSEFI_DOXYGEN_FTP_USER=u71977750-docs


echo Should be executed from project root folder. Will try to upload to %RUSEFI_FTP_SERVER%

rem ibom is part of Doxygen job simply in order to reduce workspace HDD usage on my tiny build server
misc\jenkins\InteractiveHtmlBom\run.bat
ncftpput -R -v -u %RUSEFI_DOXYGEN_FTP_USER% -p %RUSEFI_DOXYGEN_FTP_PASS% %RUSEFI_FTP_SERVER% /ibom hardware/ibom/*

pwd
cd firmware

doxygen
IF NOT ERRORLEVEL echo doxygen run FAILED
IF NOT ERRORLEVEL 0 EXIT /B 1


rem http://www.ncftp.com/download/
rem actually Cygwin http://rusefi.com/wiki/index.php?title=Internal:Software:Build_Server
cd ../doxygen
ncftpput -R -v -u %RUSEFI_DOXYGEN_FTP_USER% -p %RUSEFI_DOXYGEN_FTP_PASS% %RUSEFI_FTP_SERVER% /html html/*
