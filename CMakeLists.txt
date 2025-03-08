cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME compilerFront)
project(${TARGET_NAME} VERSION 0.1 LANGUAGES CXX)

set(SRC_DIR "src")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")

add_executable(${TARGET_NAME} ${SOURCES})

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

target_include_directories(${TARGET_NAME} PRIVATE "${SRC_DIR}/include")

if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4 /permissive-)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic-errors)
endif()
