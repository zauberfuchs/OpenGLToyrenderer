#!/bin/bash

mkdir buildLinux
cd buildLinux

cmake .. -DCMAKE_TOOLCHAIN_FILE=/opt/GitHub/vcpkg/scripts/buildsystems/vcpkg.cmake

cd ..

cmake --build buildLinux --config Release

