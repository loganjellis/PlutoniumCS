cmake_minimum_required(VERSION 4.0)
project(PlutoniumCS VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER "C:/ProgramData/mingw64/mingw64/bin/gcc.exe")

option(BUILD_SHARED_LIBS "Build using a shared library." ON)

add_library(plutonium_cs "${CMAKE_CURRENT_SOURCE_DIR}/src/plutonium_cs.c")

target_compile_definitions(plutonium_cs PRIVATE $<$<BOOL:${BUILD_SHARED_LIBS}>:PLUTONIUM_CS_EXPORTS>)

target_include_directories(plutonium_cs PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(plutonium_cs PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/libtimey.dll.a" "${CMAKE_CURRENT_SOURCE_DIR}/lib/libvibrant_logs.dll.a")

add_executable(plutonium_cs_test "${CMAKE_CURRENT_SOURCE_DIR}/example.c")
target_link_libraries(plutonium_cs_test PRIVATE plutonium_cs)
