cmake_minimum_required(VERSION 3.0)
project(libhv-http)

set(SERVER_NAME "server")
set(CLIENT_NAME "client")

set(LIBHV_INCLUDE ${PROJECT_BINARY_DIR}/contrib/libhv/include/hv)
add_subdirectory(${PROJECT_SOURCE_DIR}/contrib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

file(GLOB_RECURSE SERVER_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/server/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/server/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/server/*.h )

file(GLOB_RECURSE CLIENT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/client/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/client/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/client/*.h )

set(SOURCE_DIR_HEADER "")
execute_process(
        COMMAND find src -type d
        OUTPUT_VARIABLE SOURCE_DIR_HEADER
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
string(REPLACE "\n" ";" SOURCE_DIR_HEADER ${SOURCE_DIR_HEADER} SOURCE_DIR_HEADER)

include_directories(${SOURCE_DIR_HEADER})
include_directories(${LIBHV_INCLUDE})

add_executable(${SERVER_NAME} ${SERVER_SOURCES})
add_executable(${CLIENT_NAME} ${CLIENT_SOURCES})

target_link_libraries(${SERVER_NAME} hv_static)
target_link_libraries(${CLIENT_NAME} hv_static)
