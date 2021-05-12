rm -f build/logicdata_sandbox.exe

x86_64-w64-mingw32-g++ logicdata.cpp logicdata_sandbox.cpp -o build/logicdata_sandbox.exe -Wall

build\logicdata_sandbox.exe
