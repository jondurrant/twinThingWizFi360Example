#CMSIS_FreeRTOS_include.cmake
# Created on: 27 Aug 2022
#     Author: jondurrant

if (NOT DEFINED CMSIS_FREERTOS_DIR)
    set(CMSIS_FREERTOS_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/CMSIS-FreeRTOS")
endif ()


set(CMSIS_FREERTOS_SRC "${CMSIS_FREERTOS_DIR}/CMSIS/RTOS2/FreeRTOS/Source")

if (NOT DEFINED FREERTOS_DIR)
    set(FREERTOS_DIR "${CMSIS_FREERTOS_DIR}/Source")
endif()

add_library(FREERTOS_FILES STATIC)

target_sources(FREERTOS_FILES PUBLIC
        ${FREERTOS_DIR}/event_groups.c
        ${FREERTOS_DIR}/list.c
        ${FREERTOS_DIR}/queue.c
        ${FREERTOS_DIR}/stream_buffer.c
        ${FREERTOS_DIR}/tasks.c
        ${FREERTOS_DIR}/timers.c
#        ${FREERTOS_DIR}/portable/GCC/ARM_CM0/port.c 
		${CMSIS_FREERTOS_DIR}/Source/portable/ThirdParty/GCC/RP2040/port.c 		
        ${FREERTOS_DIR}/portable/MemMang/heap_4.c
)

target_include_directories(FREERTOS_FILES PUBLIC
        ${PORT_DIR}/FreeRTOS-Kernel/inc
        ${PORT_DIR}/RTE/inc
        ${FREERTOS_DIR}/include
#        ${FREERTOS_DIR}/portable/GCC/ARM_CM0
	    ${CMSIS_FREERTOS_DIR}/include
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Device/RaspberryPi/RP2040/Include
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Core/Include
        ${PICO_SDK_PATH}/src/rp2_common/hardware_exception/include
)

target_link_libraries(FREERTOS_FILES PRIVATE
        pico_stdlib
        cmsis_core
        CMSIS_FREERTOS_FILES
        hardware_exception
)

# CMSIS-FreeRTOS
add_library(CMSIS_FREERTOS_FILES STATIC)

target_sources(CMSIS_FREERTOS_FILES PUBLIC
        ${CMSIS_FREERTOS_SRC}/ARM/clib_arm.c        
        ${CMSIS_FREERTOS_SRC}/cmsis_os2.c        
        ${CMSIS_FREERTOS_SRC}/freertos_evr.c
        ${CMSIS_FREERTOS_SRC}/os_systick.c
#        ${CMSIS_FREERTOS_DIR}/Source/portable/GCC/ARM_CM0/port.c  	
		${CMSIS_FREERTOS_DIR}/Source/portable/ThirdParty/GCC/RP2040/port.c	
        ${CMSIS_FREERTOS_DIR}/Source/portable/MemMang/heap_4.c
)

target_include_directories(CMSIS_FREERTOS_FILES PUBLIC
        ${CMSIS_FREERTOS_DIR}/Source/include
        ${CMSIS_FREERTOS_DIR}/CMSIS/RTOS2/FreeRTOS/Include
#        ${CMSIS_FREERTOS_DIR}/Source/portable/GCC/ARM_CM0
        ${CMSIS_FREERTOS_DIR}/Source/portable/ThirdParty/GCC/RP2040/include/
        ${CMSIS_5_DIR}/CMSIS/RTOS2/Include
        ${CMSIS_5_DIR}/CMSIS/Core/Include
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Device/RaspberryPi/RP2040/Include
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Core/Include
        ${PICO_SDK_PATH}/src/rp2_common/hardware_exception/include
        ${PORT_DIR}/FreeRTOS-Kernel
        ${PORT_DIR}/RTE/inc
)

target_link_libraries(CMSIS_FREERTOS_FILES PRIVATE
        pico_stdlib
        cmsis_core
        FREERTOS_FILES  
        hardware_exception
)