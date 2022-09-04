# Add ${CMAKE_CURRENT_LIST_DIR}rary cpp files

if (NOT DEFINED COREMQTT_DIR)
    set(COREMQTT_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/coreMQTT")
endif()

add_library(coreMQTT STATIC)
include("${COREMQTT_DIR}/mqttFilePaths.cmake")

target_sources(coreMQTT PUBLIC
	${MQTT_SOURCES}
	${MQTT_SERIALIZER_SOURCES}
)

target_include_directories(coreMQTT PUBLIC 
	${MQTT_INCLUDE_PUBLIC_DIRS}
	${COREMQTT_PORT_PATH}
)


# Add the standard ${CMAKE_CURRENT_LIST_DIR}rary to the build
target_link_libraries(coreMQTT 
    CMSIS_FREERTOS_FILES
	FREERTOS_FILES 
	pico_stdlib
)