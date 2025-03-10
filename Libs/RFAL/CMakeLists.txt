set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})

IF     (TYPE MATCHES "CONTROLLER" AND REV EQUAL 0)
    set(CHIP ST25R3911)

    set(LIB_INCLUDES ${LIB_INCLUDES} ${LIB_DIR}/${CHIP}/source/st25r3911)

    set(LIB_SOURCES ${LIB_SOURCES}
        ${LIB_DIR}/${CHIP}/source/st25r3911/rfal_rfst25r3911.c
        ${LIB_DIR}/${CHIP}/source/st25r3911/st25r3911_com.c
        ${LIB_DIR}/${CHIP}/source/st25r3911/st25r3911_interrupt.c
        ${LIB_DIR}/${CHIP}/source/st25r3911/st25r3911.c
    )
ELSEIF (TYPE MATCHES "CONTROLLER" AND REV EQUAL 1)
    set(CHIP ST25R3916B)
    set(LIB_DEFINES ${LIB_DEFINES} RFAL_USE_I2C)

    set(LIB_INCLUDES ${LIB_INCLUDES} ${LIB_DIR}/${CHIP}/source/st25r3916)

    set(LIB_SOURCES ${LIB_SOURCES}
        ${LIB_DIR}/${CHIP}/source/st25r3916/rfal_rfst25r3916.c
        ${LIB_DIR}/${CHIP}/source/st25r3916/st25r3916_aat.c
        ${LIB_DIR}/${CHIP}/source/st25r3916/st25r3916_com.c
        ${LIB_DIR}/${CHIP}/source/st25r3916/st25r3916_irq.c
        ${LIB_DIR}/${CHIP}/source/st25r3916/st25r3916_led.c
        ${LIB_DIR}/${CHIP}/source/st25r3916/st25r3916.c
    )

ELSE()
    MESSAGE(FATAL_ERROR "no ST25R reader chip defined")
ENDIF()

set(LIB_SOURCES ${LIB_SOURCES}
    ${LIB_DIR}/${CHIP}/source/rfal_analogConfig.c
    ${LIB_DIR}/${CHIP}/source/rfal_cd.c
    ${LIB_DIR}/${CHIP}/source/rfal_crc.c
    ${LIB_DIR}/${CHIP}/source/rfal_dpo.c
    ${LIB_DIR}/${CHIP}/source/rfal_iso15693_2.c
    ${LIB_DIR}/${CHIP}/source/rfal_isoDep.c
    ${LIB_DIR}/${CHIP}/source/rfal_nfc.c
    ${LIB_DIR}/${CHIP}/source/rfal_nfca.c
    ${LIB_DIR}/${CHIP}/source/rfal_nfcb.c
    ${LIB_DIR}/${CHIP}/source/rfal_nfcDep.c
    ${LIB_DIR}/${CHIP}/source/rfal_nfcf.c
    ${LIB_DIR}/${CHIP}/source/rfal_nfcv.c
    ${LIB_DIR}/${CHIP}/source/rfal_st25tb.c
    ${LIB_DIR}/${CHIP}/source/rfal_st25xv.c
    ${LIB_DIR}/${CHIP}/source/rfal_t1t.c
    ${LIB_DIR}/${CHIP}/source/rfal_t2t.c
    ${LIB_DIR}/${CHIP}/source/rfal_t4t.c
)

set(LIB_DEFINES ${LIB_DEFINES} ${CHIP})


# Forward the lib sources
set(LIB_SOURCES ${LIB_SOURCES} PARENT_SCOPE)

set(LIB_INCLUDES
    ${LIB_INCLUDES}
    ${LIB_DIR}/${CHIP}/include
    ${LIB_DIR}/${CHIP}/source
PARENT_SCOPE)

set(LIB_DEFINES ${LIB_DEFINES}

PARENT_SCOPE)


