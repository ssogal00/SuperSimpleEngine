cmake_minimum_required(VERSION 3.5.0)
project(Core_CMAKE)

add_compile_options(/wd4251)

set(CMAKE_EXE_LINKER_FLAGS "/machine:x64")
set(CMAKE_MODULE_LINKER_FLAGS "/machine:x64")
set(CMAKE_SHARED_LINKER_FLAGS "/machine:x64")
set(CMAKE_STATIC_LINKER_FLAGS "/machine:x64")
set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "./*.cpp")
file(GLOB HEADERS "./*.h")

include_directories("../HeaderOnly")
include_directories("../lib/simdjson/include")

add_library(Core SHARED ${SOURCES} ${HEADERS})


target_link_libraries(Core 
$<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/lib/bin/simdjson/Debug/simdjson.lib>
$<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/lib/bin/simdjson/Release/simdjson.lib>
$<$<CONFIG:RelWithDebInfo>:${CMAKE_SOURCE_DIR}/lib/bin/simdjson/RelWithDebInfo/simdjson.lib>)

target_compile_definitions(Core PRIVATE CORE_EXPORTS)
target_compile_definitions(Core PUBLIC CORE_IMPORTS)

target_precompile_headers(Core PRIVATE SSCore.h)

target_compile_options(Core PRIVATE /GR-)



###########################################################


