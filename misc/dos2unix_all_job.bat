find_cygwin . -type f -print0 | xargs -0 dos2unix
git pull
call dos2unix_all.bat
git commit -m "dos2unix nightly"
git push