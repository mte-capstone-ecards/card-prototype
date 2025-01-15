set(COMBINED_LIB_SOURCES    "")
set(COMBINED_LIB_INCLUDES   "")
set(COMBINED_LIB_DEFINES    "")

foreach (LIB ${SELECTED_LIBS})
    set(LIB_SOURCES     "")
    set(LIB_INCLUDES    "")
    set(LIB_DEFINES     "")

    if (${LIB} MATCHES "RFAL")
        add_subdirectory(RFAL)
    endif()

    if (${LIB} MATCHES "ST25R_Common")
        add_subdirectory(ST25R_Common)
    endif()

    if (${LIB} MATCHES "M24LR")
        add_subdirectory(M24LR)
    endif()

    set(COMBINED_LIB_SOURCES    ${COMBINED_LIB_SOURCES}     ${LIB_SOURCES})
    set(COMBINED_LIB_INCLUDES   ${COMBINED_LIB_INCLUDES}    ${LIB_INCLUDES})
    set(COMBINED_LIB_DEFINES    ${COMBINED_LIB_DEFINES}     ${LIB_DEFINES})

    message("Including library " ${LIB})
endforeach()

set(COMBINED_LIB_SOURCES    ${COMBINED_LIB_SOURCES}     PARENT_SCOPE)
set(COMBINED_LIB_INCLUDES   ${COMBINED_LIB_INCLUDES}    PARENT_SCOPE)
set(COMBINED_LIB_DEFINES    ${COMBINED_LIB_DEFINES}     PARENT_SCOPE)

