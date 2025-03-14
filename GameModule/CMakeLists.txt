cmake_minimum_required(VERSION 3.5.0)
project(DXEngine)

add_compile_options(/wd4251)

set(CMAKE_EXE_LINKER_FLAGS "/machine:x64")
set(CMAKE_MODULE_LINKER_FLAGS "/machine:x64")
set(CMAKE_SHARED_LINKER_FLAGS "/machine:x64")
set(CMAKE_STATIC_LINKER_FLAGS "/machine:x64")
set(CMAKE_CXX_STANDARD 17)

###########################################################
################ Engine					 ################
file(GLOB SOURCES "./*.cpp")
file(GLOB HEADERS "./*.h")

include_directories("../lib/DirectXMath/Include")
include_directories("../lib/freetype/include")
include_directories("../Shared")
include_directories("../lib/simdjson/include")
include_directories("../Core")
include_directories("../HeaderOnly")

link_directories("../lib/bin")

add_library(GameModule SHARED ${SOURCES} ${HEADERS})

target_precompile_headers(GameModule PRIVATE "SSGameModule.h")

target_link_libraries(GameModule d3d11.lib d3d12.lib d3dcompiler.lib dxguid.lib dxgi.lib Core)

target_compile_definitions(GameModule PRIVATE GAMEMODULE_EXPORTS)
target_compile_definitions(GameModule INTERFACE GAMEMODULE_IMPORTS)


target_compile_options(GameModule PRIVATE /GR-)


###########################################################


