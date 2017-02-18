svn up
call generate_docs

rem http://www.ncftp.com/download/
cd ../doxygen
ncftpput -u u71977750-docs -p %RUSEFI_DOXYGEN_FTP_PASS% rusefi.com /html html/*

