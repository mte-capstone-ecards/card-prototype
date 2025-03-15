
set(SIM_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(APP_DIR ${CMAKE_SOURCE_DIR}/App)

set (SIM_SOURCES ${SIM_SOURCES}
    ${SIM_DIR}/Src/sim.c
    ${APP_DIR}/Src/Games/Hanabi.c
PARENT_SCOPE)

set (SIM_INCLUDES
    ${SIM_DIR}/Inc
    ${SIM_DIR}/Inc/Stubs
    ${APP_DIR}/Inc/Games
PARENT_SCOPE)

set (SIM_DEFINES ${SIM_DEFINES}

PARENT_SCOPE)
message(${APP_DIR})