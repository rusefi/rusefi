rem make, gcc, Windows and Cygwin combined have some issue with spaces or colons in paths, that's a workaround
rem that's more or less 'make clean' 
rd /s /q .dep
rd /s /q build

# that's 'make' with some extra utilities
compile.bat