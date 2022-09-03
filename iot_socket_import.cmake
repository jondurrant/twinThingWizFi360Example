#iot_socket_import.cmake
# Created on: 2 Sept 2022
#     Author: jondurrant


if (NOT DEFINED IOT_SOCKET_DIR)
	set(IOT_SOCKET_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/IoT_Socket")
endif()


# IoT_Socket
add_library(IOT_SOCKET_FILES STATIC)

target_sources(IOT_SOCKET_FILES PUBLIC
        ${IOT_SOCKET_DIR}/source/wifi/iot_socket.c
        )

target_include_directories(IOT_SOCKET_FILES PUBLIC
        ${IOT_SOCKET_DIR}/include
        )

target_link_libraries(IOT_SOCKET_FILES PRIVATE
        pico_stdlib
        cmsis_core
        FREERTOS_FILES
        CMSIS_FREERTOS_FILES
        WIZFI360_FILES
)