set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LIB_SOURCES
    ${LIB_DIR}/source/rfal_analogConfig.c
    ${LIB_DIR}/source/rfal_cd.c
    ${LIB_DIR}/source/rfal_crc.c
    ${LIB_DIR}/source/rfal_dpo.c
    ${LIB_DIR}/source/rfal_iso15693_2.c
    ${LIB_DIR}/source/rfal_isoDep.c
    ${LIB_DIR}/source/rfal_nfc.c
    ${LIB_DIR}/source/rfal_nfca.c
    ${LIB_DIR}/source/rfal_nfcb.c
    ${LIB_DIR}/source/rfal_nfcDep.c
    ${LIB_DIR}/source/rfal_nfcf.c
    ${LIB_DIR}/source/rfal_nfcv.c
    ${LIB_DIR}/source/rfal_st25tb.c
    ${LIB_DIR}/source/rfal_st25xv.c
    ${LIB_DIR}/source/rfal_t1t.c
    ${LIB_DIR}/source/rfal_t2t.c
    ${LIB_DIR}/source/rfal_t4t.c
)

# Select the correct ST25R chip
set(LIB_SOURCES ${LIB_SOURCES}
    ${LIB_DIR}/source/st25r3911/rfal_rfst25r3911.c
    ${LIB_DIR}/source/st25r3911/st25r3911_com.c
    ${LIB_DIR}/source/st25r3911/st25r3911_interrupt.c
    ${LIB_DIR}/source/st25r3911/st25r3911.c
)

# Forward the lib sources
set(LIB_SOURCES ${LIB_SOURCES} PARENT_SCOPE)

set(LIB_INCLUDES
    ${LIB_DIR}/include
    ${LIB_DIR}/source
    ${LIB_DIR}/source/st25r3911
PARENT_SCOPE)

set(LIB_DEFINES

PARENT_SCOPE)


