(follwing steps produce a DLL which fails in runtime see https://github.com/rusefi/rusefi/issues/6358)

add into path:

c:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin
c:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin

cmake .
msbuild OpenBLTJni.sln
