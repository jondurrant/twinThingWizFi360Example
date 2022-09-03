# CMake minimum required version
cmake_minimum_required(VERSION 3.12)

# Find git
find_package(Git)

if(NOT Git_FOUND)
	message(FATAL_ERROR "Could not find 'git' tool for WIZFI360-EVB-PICO-C patching")
endif()


set(WIZFI360_EVB_PICO_C_PATCH_DIR "${CMAKE_CURRENT_LIST_DIR}/patches")




 
file(GLOB CMSIS_FREERTOS_PATCHES
	"${WIZFI360_EVB_PICO_C_PATCH_DIR}/01_cmsis_freertos_cmsis_os2.patch"
	)

foreach(CMSIS_FREERTOS_PATCH IN LISTS CMSIS_FREERTOS_PATCHES)
	message("Running patch ${CMSIS_FREERTOS_PATCH}")
	execute_process(
		COMMAND ${GIT_EXECUTABLE} apply ${CMSIS_FREERTOS_PATCH}
		WORKING_DIRECTORY ${CMSIS_FREERTOS_DIR}
	)
endforeach()
