
echo Should be executed from project root folder. Will try to upload to %RUSEFI_FTP_SERVER%

rem ibom is part of Doxygen job simply in order to reduce workspace HDD usage on my tiny build server
call misc\jenkins\InteractiveHtmlBom\run.bat
echo Uploading IBOMs
ncftpput -R -v -u %RUSEFI_DOXYGEN_FTP_USER% -p %RUSEFI_DOXYGEN_FTP_PASS% %RUSEFI_FTP_SERVER% /ibom hardware/ibom/*

pwd
cd firmware

doxygen
IF NOT ERRORLEVEL 0 echo doxygen run FAILED
IF NOT ERRORLEVEL 0 EXIT /B 1


rem http://www.ncftp.com/download/
rem actually Cygwin http://rusefi.com/wiki/index.php?title=Internal:Software:Build_Server
cd ../doxygen
echo Uploading Doxygen
ncftpput -R -v -u %RUSEFI_DOXYGEN_FTP_USER% -p %RUSEFI_DOXYGEN_FTP_PASS% %RUSEFI_FTP_SERVER% /html html/*
IF NOT ERRORLEVEL 0 echo upload FAILED
IF NOT ERRORLEVEL 0 EXIT /B 1
