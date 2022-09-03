#WizFi360_import.cmake
#     Author: jondurrant
# Created on: 26 Aug 2022


# WizFi360
add_library(WIZFI360_FILES STATIC)

target_sources(WIZFI360_FILES PUBLIC
        ${PORT_DIR}/WizFi360/src/UART_RP2040.c
        )

target_include_directories(WIZFI360_FILES PUBLIC
        ${CMSIS_5_DIR}/CMSIS/Driver/Include
        ${PORT_DIR}/WizFi360/inc
        ${PORT_DIR}/RTE/inc
        )

target_link_libraries(WIZFI360_FILES PRIVATE
        pico_stdlib
        cmsis_core
        hardware_uart
        WIZFI360_DRIVER_FILES
        CMSIS_FREERTOS_FILES
        )


# WizFi360 CMSIS-Driver
add_library(WIZFI360_DRIVER_FILES STATIC)

target_sources(WIZFI360_DRIVER_FILES PUBLIC
        ${WIZFI360_DRIVER_DIR}/BufList/BufList.c
        ${WIZFI360_DRIVER_DIR}/BufList/LinkList.c
        ${WIZFI360_DRIVER_DIR}/WiFi_WizFi360.c
        ${WIZFI360_DRIVER_DIR}/WiFi_WizFi360_Os.c
        ${WIZFI360_DRIVER_DIR}/WizFi360.c
        ${WIZFI360_DRIVER_DIR}/WizFi360_Serial.c
)

target_include_directories(WIZFI360_DRIVER_FILES PUBLIC
        ${WIZFI360_DRIVER_DIR}
        ${WIZFI360_DRIVER_DIR}/BufList
        ${CMSIS_5_DIR}/CMSIS/Driver/Include
        ${CMSIS_5_DIR}/CMSIS/RTOS2/Include
        ${CMSIS_5_DIR}/CMSIS/Core/Include
        ${FREERTOS_KERNEL_PATH}/include
        ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0
        ${PORT_DIR}/FreeRTOS-Kernel
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Device/RaspberryPi/RP2040/Include
)

target_link_libraries(WIZFI360_DRIVER_FILES PRIVATE
        pico_stdlib
        cmsis_core
        WIZFI360_FILES
        CMSIS_FREERTOS_FILES
)    


 

