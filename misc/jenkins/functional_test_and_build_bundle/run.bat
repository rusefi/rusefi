
call misc/jenkins/functional_test_and_build_bundle/build_current_bundle.bat
IF NOT ERRORLEVEL 0 echo build ERROR DETECTED
IF NOT ERRORLEVEL 0 EXIT /B 1


rem call misc/jenkins/functional_test_and_build_bundle/simulator_test.bat
rem IF NOT ERRORLEVEL 0 echo simulator test ERROR DETECTED
rem IF NOT ERRORLEVEL 0 EXIT /B 1


rem Here we use last version of firmware produced by 'clean_compile_two_versions.bat'

call misc/jenkins/functional_test_and_build_bundle/hw_test.bat
IF NOT ERRORLEVEL 0 echo real hardware test ERROR DETECTED
IF NOT ERRORLEVEL 0 EXIT /B 1

