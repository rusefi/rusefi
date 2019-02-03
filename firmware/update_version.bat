rem Let's regemerate 'svnversion.h'
rem TODO: handle std err - for example, in case svn needs upgrade

for /f %%i in ('git rev-parse HEAD') do set git_hash=%%i
echo Git hash=%git_hash%
java -jar ../java_tools/version2header.jar 10000 https://github.com/rusefi/rusefi %git_hash%