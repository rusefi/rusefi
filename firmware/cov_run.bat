rem Download from https://scan.coverity.com/download?tab=cxx

call clean.bat
cov-build --dir cov-int make
zip -r cov-int.zip cov-int