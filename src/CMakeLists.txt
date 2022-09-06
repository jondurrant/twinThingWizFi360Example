add_executable(${NAME}
        main.cpp
        StateExample.cpp
		ExampleAgentObserver.cpp
		BlinkAgent.cpp
        Agent.cpp
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(${NAME} 
	 pico_stdlib
     CMSIS_FREERTOS_FILES
     WIZFI360_DRIVER_FILES
     WIZFI360_FILES
     FREERTOS_FILES
     IOT_SOCKET_FILES
     hardware_rtc
     twinThingWizFi360
)

target_compile_definitions(${NAME} PRIVATE
    configNUM_CORES=2
    WIFI_SSID=\"$ENV{WIFI_SSID}\"
    WIFI_PASSWORD=\"$ENV{WIFI_PASSWORD}\"
)

# enable usb output, disable uart output
pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})