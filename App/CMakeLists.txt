# App CMakeLists

set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set (APP_SOURCES
	${APP_DIR}/Src/app.c
	${APP_DIR}/Src/eink_e370_driver.c
	${APP_DIR}/Src/eink_lib.c
	# ${APP_DIR}/Src/img.c
PARENT_SCOPE)

set (APP_INCLUDES
	${APP_DIR}/Inc
PARENT_SCOPE)
