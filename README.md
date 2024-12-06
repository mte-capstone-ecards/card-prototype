## Building

``` zsh
C:\Strawberry\c\bin\cmake.exe -S . -B Debug/G431 -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake -DBOARD=G431
cmake --build Debug/G431
```

### Programming

https://www.st.com/resource/en/user_manual/um3088-stm32cube-commandline-toolset-quick-start-guide-stmicroelectronics.pdf

Erase flash
``` cmd
STM32_Programmer_CLI.exe -c port=SWD freq=4000 -e all
```

Upload to flash memory
``` cmd
STM32_Programmer_CLI.exe -c port=SWD freq=4000 -w .\Debug\YOUR_PROGRAM.elf 0x08000000
```

### Debugging

Start ST Link GDB server

``` cmd
ST-LINK_gdbserver.exe -d -v -t -cp C:\ST\STM32CubeCLT\STM32CubeProgrammer\bin
```

Open GDB

``` cmd
arm-none-eabi-gdb.exe
(gdb) target remote localhost:port
(gdb) load YOUR_PROGRAM.elf
```



