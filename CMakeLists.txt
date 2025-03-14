cmake_minimum_required(VERSION 3.5.0)
project(SuperSimpleEngine)

add_compile_options(/wd4251)
string(REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

set(CMAKE_EXE_LINKER_FLAGS "/machine:x64")
set(CMAKE_MODULE_LINKER_FLAGS "/machine:x64")
set(CMAKE_SHARED_LINKER_FLAGS "/machine:x64")
set(CMAKE_STATIC_LINKER_FLAGS "/machine:x64")
set(CMAKE_CXX_STANDARD 17)

set(SIMDJSON_JUST_LIBRARY 1)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


message(STATUS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_subdirectory("Renderer")
add_subdirectory("Core")
#add_subdirectory("lib/simdjson")
add_subdirectory("Engine")
add_subdirectory("GameModule")



include_directories("HeaderOnly")
include_directories("Engine")
include_directories("Renderer")
include_directories("GameModule")
include_directories("Core")



add_executable(SuperSimpleEngine resource.h targetver.h SSAppWindow.h SSAppWindow.cpp SSApplication.cpp DXWindow.rc "Console/main.cpp")

if(WIN32)
	set_target_properties(SuperSimpleEngine PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")	
	set_target_properties(SuperSimpleEngine PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")	
endif(WIN32)


target_link_libraries(SuperSimpleEngine PRIVATE Engine
$<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/lib/bin/simdjson/Debug/simdjson.lib>
$<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/lib/bin/simdjson/Release/simdjson.lib>
$<$<CONFIG:RelWithDebInfo>:${CMAKE_SOURCE_DIR}/lib/bin/simdjson/RelWithDebInfo/simdjson.lib>)

target_compile_options(SuperSimpleEngine PRIVATE /GR-)

target_compile_options(SuperSimpleEngine PRIVATE /EHs-c-)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lib/bin DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


## console app ##
file(GLOB SOURCES "Console/*.cpp")
file(GLOB HEADERS "Console/*.h")

add_executable(ConsoleApp Console/main.cpp)

target_include_directories(ConsoleApp PUBLIC "Core" "Engine")

target_link_libraries(ConsoleApp PRIVATE Core)

target_compile_options(ConsoleApp PRIVATE /GR-)

set_target_properties(ConsoleApp PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:console")	
set_target_properties(ConsoleApp PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:console")

###########################################################
