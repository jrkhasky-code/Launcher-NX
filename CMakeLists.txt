cmake_minimum_required(VERSION 3.10)
project(Launcher-NX CXX)

# Force the compiler system to recognize native Nintendo Switch parameters
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -specs=/opt/devkitpro/libnx/switch.specs")

# Map our include search vectors and code files
include_directories(/opt/devkitpro/libnx/include)
link_directories(/opt/devkitpro/libnx/lib)

# Point directly to your code
add_executable(Launcher-NX main.cpp)

# Link your code directly with the official target operating system system library
target_link_libraries(Launcher-NX nx)
