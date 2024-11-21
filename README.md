## Building

``` zsh
C:\Strawberry\c\bin\cmake.exe -S . -B Debug/G431 -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake -DBOARD=G431
cmake --build Debug/G431
```