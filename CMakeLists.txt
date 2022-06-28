cmake_minimum_required(VERSION 3.22.1)

project(odyssey)

find_package(Vulkan REQUIRED)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/win32_odyssey.cpp

)

# -wd4100: Unreferenced format parameter.
# -wd4189: local variable is initialized but not referenced.
# -wd4505: Unreferenced function with internal linkage.
set(MSVC_COMPILER_FLAGS "-Zi -Od -Oi -WX -W4 -wd4100 -wd4189 -wd4505")
set(MSVC_LINK_FLAGS "-incremental:no")

set(CLANG_CL_COMPILER_FLAGS "-g -O0 -Wall -Wextra -Werror")
set(CLANG_CL_LINK_FLAGS "-incremental:no -opt:ref -machine:x64 -manifest:no -subsystem:WINDOWS")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_CL_COMPILER_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MSVC_COMPILER_FLAGS}")
    endif()
endif()

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

# $ENV{VULKAN_SDK}/include

target_include_directories(${PROJECT_NAME}  PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} kernel32.lib user32.lib gdi32.lib)
