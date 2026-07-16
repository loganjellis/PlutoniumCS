cmake_minimum_required(VERSION 4.0)
project(PlutoniumCS VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER "C:/ProgramData/mingw64/mingw64/bin/gcc.exe")

include(FetchContent)
FetchContent_Declare(vibrant_logs GIT_REPOSITORY https://github.com/loganjellis/VibrantLogs.git GIT_TAG main)
FetchContent_Declare(dynamic_array_spellbook GIT_REPOSITORY https://github.com/loganjellis/DynamicArraySpellbook.git GIT_TAG main)
FetchContent_MakeAvailable(vibrant_logs dynamic_array_spellbook)

add_library(plutonium_cs "${CMAKE_CURRENT_SOURCE_DIR}/src/plutonium_cs.c")

option(BUILD_SHARED_LIBS "Build using a shared library." ON)
target_compile_definitions(plutonium_cs PRIVATE $<$<BOOL:${BUILD_SHARED_LIBS}>:PLUTONIUM_CS_EXPORTS>)

target_include_directories(plutonium_cs PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(plutonium_cs PUBLIC vibrant_logs dynamic_array_spellbook)
