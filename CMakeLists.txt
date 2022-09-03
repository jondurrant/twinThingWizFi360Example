cmake_minimum_required(VERSION 3.12)

# Change your executable name to something creative!
set(NAME NetConn) # <-- Name your project/executable here!

include(pico_sdk_import.cmake)
set (CMSIS_FREERTOS_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/CMSIS-FreeRTOS"  CACHE STRING "Common Lib")
include(wizfi360_evb_pico_c-patch.cmake)
include(wizfi360_evb_pico_c_sdk_version.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()


if(NOT DEFINED CMSIS_5_DIR)
    set(CMSIS_5_DIR ${CMAKE_SOURCE_DIR}/lib/CMSIS_5)
    message(STATUS "CMSIS_5_DIR = ${CMSIS_5_DIR}")
endif()

if(NOT DEFINED WIZFI360_DRIVER_DIR)
    set(WIZFI360_DRIVER_DIR ${CMAKE_SOURCE_DIR}/lib/CMSIS-Driver/WiFi/WizFi360)
    message(STATUS "WIZFI360_DRIVER_DIR = ${WIZFI360_DRIVER_DIR}")
endif()

if(NOT DEFINED PORT_DIR)
	set(PORT_DIR ${CMAKE_SOURCE_DIR}/port)
endif()

# Configure Wi-Fi for WizFi360
add_definitions(-DDRIVER_WIFI_NUM=1)

#add_definitions(-D"__WEAK"="")

include(WizFi360_import.cmake)

#set (CMSIS_FREERTOS_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/CMSIS-FreeRTOS"  CACHE STRING "Common Lib")
include(CMSIS_FreeRTOS_include.cmake)

set (IOT_SOCKET_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/IoT_Socket"  CACHE STRING "Common Lib")
include(iot_socket_import.cmake)


set (TWIN_THING_PICO_CONFIG_PATH "${CMAKE_CURRENT_LIST_DIR}/port/twinThing")
include(${CMAKE_CURRENT_LIST_DIR}/lib/twinThingWizFi360/twinThingWizFi360.cmake)

#Add main source directory
#add_subdirectory(src)
add_subdirectory(test)

#Set up files for the release packages
install(CODE "execute_process(COMMAND $ENV{HOME}/bin/picoDeploy.sh ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.elf)")

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.uf2
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
