# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
include(${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")
SET(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(AirFobulous C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(
  AirFobulous 
  src/air_fobulous.c
  src/neopixel_task.c
  src/blue_led_task.c
  src/print_task.c
  src/sdcard_log_task.c
  src/bme68x_task.c
  src/bme68x/common.c
)

# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(AirFobulous ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/src/include)

pico_set_program_name(AirFobulous "AirFobulous")
pico_set_program_version(AirFobulous "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(AirFobulous 0)
pico_enable_stdio_usb(AirFobulous 1)

# Add the standard include files to the build
target_include_directories(AirFobulous PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
  ${CMAKE_CURRENT_LIST_DIR}/src/include
  ${CMAKE_CURRENT_LIST_DIR}/src/bme68x/
)

add_library(bme68x STATIC
  src/bme68x/bme68x.c 
  src/bme68x/bsec_selectivity.c
  src/bme68x/bsec_integration.c
  #${CMAKE_CURRENT_LIST_DIR}/src/bme68x/bme68x.h 
  #${CMAKE_CURRENT_LIST_DIR}/src/bme68x/bme68x_defs.h
)

# Add any user requested libraries
target_link_libraries(AirFobulous PUBLIC
        hardware_i2c
        hardware_pio
        pico_stdlib
        FreeRTOS-Kernel
        bme68x
      )

pico_add_extra_outputs(AirFobulous)


add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/src/include/ws2812.py
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio
        COMMAND Pioasm -o python ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio ${CMAKE_CURRENT_LIST_DIR}/src/include/ws2812.py
        VERBATIM)
