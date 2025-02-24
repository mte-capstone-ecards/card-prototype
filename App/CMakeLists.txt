# App CMakeLists

set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set (APP_SOURCES
	${APP_DIR}/Src/app.c
	${APP_DIR}/Src/led.c
	${APP_DIR}/Src/button.c
)

if (${TYPE} MATCHES "CONTROLLER")
	set (APP_SOURCES ${APP_SOURCES}
		${APP_DIR}/Src/st25r.c
		${APP_DIR}/Src/RTOSResources.c
		${APP_DIR}/Src/sender.c
		${APP_DIR}/Src/eink_e370_driver.c
		${APP_DIR}/Src/eink_lib.c
	)
elseif (${TYPE} MATCHES "CARD")
	set (APP_SOURCES ${APP_SOURCES}
		${APP_DIR}/Src/m24lr.c
		${APP_DIR}/Src/receiver.c
		${APP_DIR}/Src/card.c
		${APP_DIR}/Src/eink_e370_driver.c
		${APP_DIR}/Src/eink_lib.c
	)
endif()

set (APP_SOURCES ${APP_SOURCES} PARENT_SCOPE)

set (APP_INCLUDES
	${APP_DIR}/Inc
PARENT_SCOPE)
