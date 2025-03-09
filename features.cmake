
#######################################################################
#
#   Identifiers (XXXYY)
#       XXX - Board type
#           001 Controller
#           002 Card
#       YY - Revision
#
#   Board Definitions
#       (00100) Controller Rev0 - F446 w/ NFC EVK
#       (00101) Controller Rev1 - G0C1 w/ Controller PCBA1
#       (00200) Card Rev0       - G431 w/ Nucleo and NFC shield
#       (00201) Card Rev1       - L0x1 w/ Card PCBA1
#       (00202) Card Rev2       - L0x1 w/ Card PCBA2
#
#######################################################################

# Define the board identifiers
set(FEATURE_DEFINES ${FEATURE_DEFINES}
    TYPE_CONTROLLER=001
    TYPE_CARD=002

    BOARD_TYPE=TYPE_${TYPE}
    BOARD_REV=${REV}
)


#######################################
#            OS Selection             #
#######################################

if (${TYPE} STREQUAL "CONTROLLER")
    set(OS_FREERTOS 1)
    set(OS_BAREMETAL 0)
ELSE()
    set(OS_FREERTOS 0)
    set(OS_BAREMETAL 1)
ENDIF()

set(FEATURE_DEFINES ${FEATURE_DEFINES}
    OS_FREERTOS=${OS_FREERTOS}
    OS_BAREMETAL=${OS_BAREMETAL}
)

#######################################
#            Feature Set              #
#######################################

########### Eink ##############
# Feature for enabling EInk driver library
IF ((${TYPE} STREQUAL "CARD") OR ((${TYPE} STREQUAL "CONTROLLER") AND (${REV} EQUAL 1)))
    set(FTR_EINK 1)
ELSE()
    set(FTR_EINK 0)
ENDIF()

set(FEATURE_DEFINES ${FEATURE_DEFINES}
    FTR_EINK=${FTR_EINK}
)


########## NFC Tag ############
# Feature for enabling M24LR and tag related features
IF (${TYPE} MATCHES "CARD")
    set(FTR_NFCTAG 1)
ELSE()
    set(FTR_NFCTAG 0)
ENDIF()

set(FEATURE_DEFINES ${FEATURE_DEFINES}
    FTR_NFCTAG=${FTR_NFCTAG}
)


####### NFC Reader ############
# Feature for enabling ST25 and reader related features
IF (${TYPE} MATCHES "CONTROLLER")
    set(FTR_NFCREADER 1)
ELSE()
    set(FTR_NFCREADER 0)
ENDIF()

set(FEATURE_DEFINES ${FEATURE_DEFINES}
    FTR_NFCREADER=${FTR_NFCREADER}
)


############ LED ##############
# Feature for RTOS task controlling LED blinking
IF (${TYPE} MATCHES "CONTROLLER" AND ${REV} EQUAL 1)
    set(FTR_LED 1)
ELSE()
    set(FTR_LED 0)
ENDIF()

set(FEATURE_DEFINES ${FEATURE_DEFINES}
    FTR_LED=${FTR_LED}
)


########## Button ###############
# Feature for RTOS task managing button events
IF ((${TYPE} MATCHES "CONTROLLER") AND (${REV} EQUAL 1))
    set(FTR_BUTTON 1)
ELSE()
    set(FTR_BUTTON 0)
ENDIF()

set(FEATURE_DEFINES ${FEATURE_DEFINES}
    FTR_BUTTON=${FTR_BUTTON}
)


########## EEPROM ###############
# Feature for enabling shared EEPROM library
IF (${TYPE} STREQUAL "CARD" OR ${TYPE} STREQUAL "CONTROLLER")
    set(FTR_EEPROM 1)
ELSE()
    set(FTR_EEPROM 0)
ENDIF()

set(FEATURE_DEFINES ${FEATURE_DEFINES}
    FTR_EEPROM=${FTR_EEPROM}
)


########## Data Sender ###############
# Feature for enabling sending EEPROM node
IF (${TYPE} STREQUAL "CONTROLLER")
    set(FTR_DATASENDER 1)
ELSE()
    set(FTR_DATASENDER 0)
ENDIF()

set(FEATURE_DEFINES ${FEATURE_DEFINES}
    FTR_DATASENDER=${FTR_DATASENDER}
)


########## Data Receiver ###############
# Feature for enabling receiving EEPROM node
IF (${TYPE} STREQUAL "CARD")
    set(FTR_DATARECEIVER 1)
ELSE()
    set(FTR_DATARECEIVER 0)
ENDIF()

set(FEATURE_DEFINES ${FEATURE_DEFINES}
    FTR_DATARECEIVER=${FTR_DATARECEIVER}
)


########## GUI ###############
# Feature for enabling GUI library
IF (${TYPE} STREQUAL "CONTROLLER" AND ${REV} EQUAL 1)
    set(FTR_GUI 1)
ELSE()
    set(FTR_GUI 0)
ENDIF()

set(FEATURE_DEFINES ${FEATURE_DEFINES}
    FTR_GUI=${FTR_GUI}
)
