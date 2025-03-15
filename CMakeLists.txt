cmake_minimum_required(VERSION 3.20)

############ Include Project Features ####################
include("features.cmake")

###################### CONSTANTS ######################################
STRING(TOLOWER ${TYPE} TYPE_LOWER)
IF (NOT FTR_SIM)
	set (PROJECT_NAME             	${TYPE_LOWER}_Rev${REV})
ELSE()
	set (PROJECT_NAME				${TYPE_LOWER}_sim)
ENDIF()

###################### LIBRARIES ######################################
# Common libraries
IF (NOT FTR_SIM)
	set(SELECTED_LIBS
		DataProtocol
	)

	IF (FTR_GUI)
		set(SELECTED_LIBS ${SELECTED_LIBS}
			ugui
		)
	ENDIF()

	IF (${FTR_NFCREADER})
		set(SELECTED_LIBS ${SELECTED_LIBS}
			RFAL
			ST25R_Common
		)
	ENDIF()

	IF (${FTR_NFCTAG})
		set(SELECTED_LIBS ${SELECTED_LIBS}
			# M24LR
		)
	ENDIF()
ENDIF()

add_subdirectory(Libs)

################## PROJECT SETUP ######################################
project(${PROJECT_NAME})
enable_language(ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include board files
add_subdirectory(Board)

IF (FTR_SIM)
	add_subdirectory(Sim)
ELSE()
	add_subdirectory(App)
ENDIF()

add_executable (${PROJECT_NAME}
	${BOARD_SOURCES}
	${APP_SOURCES}
	${COMBINED_LIB_SOURCES}
	${SIM_SOURCES}
)

target_compile_definitions (${PROJECT_NAME} PRIVATE
	${FEATURE_DEFINES}
	${BOARD_DEFINES}
	${COMBINED_LIB_DEFINES}
	${SIM_DEFINES}
)

target_include_directories (${PROJECT_NAME} PRIVATE
	${BOARD_INCLUDES}
	${APP_INCLUDES}
	${COMBINED_LIB_INCLUDES}
	${SIM_INCLUDES}
)

IF (NOT FTR_SIM)
	set (CMAKE_EXECUTABLE_SUFFIX ".elf")
	set (CMAKE_C_FLAGS "${MCPU} ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb")
	set (CMAKE_EXE_LINKER_FLAGS "-T\"${LINKER_SCRIPT}\" ${RUNTIME_LIBRARY_SYSCALLS} -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group -Wl,-Map=${CMAKE_PROJECT_NAME}.map")
	set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")
ELSE()
	set (CMAKE_EXECUTABLE_SUFFIX ".out")
	set (CMAKE_C_FLAGS "")
	set (CMAKE_EXE_LINKER_FLAGS "")
	set (CMAKE_ASM_FLAGS "")
ENDIF()

set (CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=gnu11 -Wall -Wno-unused -Werror -fmax-errors=5")

set(DEBUG_OPTIONS "-Og")
set(RELEASE_OPTIONS "-O3")

target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>")
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")

# Sizing
IF (NOT FTR_SIM)
	# Copy SVD to output
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND}
		ARGS -E copy ${SVD_FILE} ${CMAKE_BINARY_DIR}/device.svd
	)

	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)
ENDIF()

