cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_PATH ${PICO_SDK_PATH})

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico-autobixler C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_AUTOBIXLER_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

# Hardware-specific examples in subdirectories:
add_subdirectory(src)
add_subdirectory(lib)
