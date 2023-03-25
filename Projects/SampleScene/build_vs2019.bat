
SET CMAKEEXE=%~dp0"../../Tools/CMake/bin/cmake.exe"

mkdir build64 & pushd build64
%CMAKEEXE% -G "Visual Studio 16 2019" Win64 .. -DCMAKE_TOOLCHAIN_FILE=../../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_INSTALL_PREFIX=../../..
popd

%CMAKEEXE% --build build64 --config Release --target INSTALL