#############################################################################################################################
# file:  CMakeLists.txt
# brief: Template "CMakeLists.txt" for building of executables and static libraries.
#
# usage: Edit "VARIABLES"-section to suit project requirements.
#        For debug build:
#          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
#          make -C Debug VERBOSE=1 -j
#        For release build:
#          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
#          make -C Release VERBOSE=1 -j
#############################################################################################################################
cmake_minimum_required(VERSION 3.20)

###################### CONSTANTS ######################################
set (PROJECT_NAME             "ecards")
set (PROJECT_TYPE             "exe")

###################### LIBRARIES ######################################
# Common libraries
set(SELECTED_LIBS
	DataProtocol
)

# Controller Libraries
if (${TYPE} MATCHES "CONTROLLER")
	set(SELECTED_LIBS ${SELECTED_LIBS}
		# RFAL
		# ST25R_Common
		ugui
	)
endif()

# Card Libraries
if (${TYPE} MATCHES "CARD")
	set(SELECTED_LIBS ${SELECTED_LIBS}
		M24LR
	)
endif()


add_subdirectory(Libs)

################## PROJECT SETUP ######################################
project(${PROJECT_NAME})
enable_language(ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include board files
add_subdirectory(Board)
add_subdirectory(App)

add_executable (${PROJECT_NAME}
	${BOARD_SOURCES}
	${APP_SOURCES}
	${COMBINED_LIB_SOURCES}
)

# Copy SVD to output
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy ${SVD_FILE} ${CMAKE_BINARY_DIR}/device.svd
)

target_compile_definitions (${PROJECT_NAME} PRIVATE
	${BOARD_DEFINES}
	${COMBINED_LIB_DEFINES}
	"TYPE_${TYPE}"
)

target_include_directories (${PROJECT_NAME} PRIVATE
	${BOARD_INCLUDES}
	${APP_INCLUDES}
	${COMBINED_LIB_INCLUDES}
)

set (CMAKE_EXECUTABLE_SUFFIX ".elf")
set (CMAKE_C_FLAGS "${MCPU} -std=gnu11 ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb -Wall -Wno-unused -Werror -fmax-errors=5")
set (CMAKE_EXE_LINKER_FLAGS "-T\"${LINKER_SCRIPT}\" ${RUNTIME_LIBRARY_SYSCALLS} -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group -Wl,-Map=${CMAKE_PROJECT_NAME}.map")
set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")

# Sizing
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)

