# CMake toolchain definition for Strawberry gcc on windows

# Skip link step during toolchain validation.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Specify toolchain.
set(TOOLCHAIN_PREFIX   "")
set(CMAKE_C_COMPILER   "${TOOLCHAIN_PREFIX}gcc.exe")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}gcc.exe")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++.exe")
set(CMAKE_AR           "${TOOLCHAIN_PREFIX}ar.exe")
set(CMAKE_LINKER       "{TOOLCHAIN_PREFIX}ld.exe")
set(CMAKE_OBJCOPY      "${TOOLCHAIN_PREFIX}objcopy.exe")
set(CMAKE_RANLIB       "${TOOLCHAIN_PREFIX}ranlib.exe")
set(CMAKE_SIZE         "${TOOLCHAIN_PREFIX}size.exe")
set(CMAKE_STRIP        "${TOOLCHAIN_PREFIX}ld.exe")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)