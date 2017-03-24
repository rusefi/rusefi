cd ..

rem manually copy find.exe from cygwin to find_cygwin to avoid conflict with systen32/find
find_cygwin . -type f -print0 | xargs -0 dos2unix