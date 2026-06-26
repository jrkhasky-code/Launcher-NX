cmake_minimum_required(VERSION 3.10)

# 1. Force CMake to use the correct Nintendo Switch ARM64 cross-compilers
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_COMPILER /opt/devkitpro/devkitA64/bin/aarch64-none-elf-gcc)
set(CMAKE_CXX_COMPILER /opt/devkitpro/devkitA64/bin/aarch64-none-elf-g++)

project(Launcher-NX CXX)

set(CMAKE_CXX_STANDARD 17)

# 2. Add vital compilation flags for the Switch architecture
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a+crc+crypto -mtune=cortex-a57 -O2 -ffunction-sections -fdata-sections -fPIE")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -specs=/opt/devkitpro/libnx/switch.specs -pie")

# 3. Include and Link paths to eliminate any undefined references
include_directories(/opt/devkitpro/libnx/include)
link_directories(/opt/devkitpro/libnx/lib)

add_executable(Launcher-NX main.cpp)
target_link_libraries(Launcher-NX nx)
