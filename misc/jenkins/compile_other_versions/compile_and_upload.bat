@echo off
bash.exe misc\jenkins\compile_other_versions\compile.sh %1 %2 %3 %4
bash.exe misc\jenkins\compile_other_versions\prepare_bundle.sh %1 %2 %3 %4
