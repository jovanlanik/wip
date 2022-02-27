# WIP
# Copyright (c) 2021 Jovan Lanik

# CMakeLists

cmake_minimum_required(VERSION 3.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(GAME "dungeon" CACHE STRING "Game")
set_property(CACHE GAME PROPERTY STRINGS "dungeon")

set(WINDOW_BACKEND "glfw" CACHE STRING "Window backend")
set_property(CACHE WINDOW_BACKEND PROPERTY STRINGS "glfw" "sdl2")

project(WIP LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

file(GLOB src "src/*.c")
file(GLOB src_external "src/external/*.c")
set(src_optional "src/optional/wip_window_${WINDOW_BACKEND}.c")
file(GLOB src_game "${GAME}.d/src/*.c")

add_library(external OBJECT ${src_external})
add_executable(${GAME} ${src} $<TARGET_OBJECTS:external> ${src_optional} ${src_game})

if(UNIX)
	target_link_libraries(${GAME} PUBLIC m)
	target_link_libraries(${GAME} PUBLIC dl)
endif()

if(WINDOW_BACKEND STREQUAL "glfw")
	find_package(glfw3 REQUIRED)
	target_link_libraries(${GAME} PUBLIC glfw)
elseif(WINDOW_BACKEND STREQUAL "sdl2")
	find_package(SDL2 REQUIRED)
	target_link_libraries(${GAME} PUBLIC SDL2::SDL2)
endif()

find_package(libconfig QUIET)
if(LIBCONFIG_FOUND)
	target_include_directories(${GAME} PUBLIC ${LIBCONFIG_INCLUDE_DIRS})
	target_link_libraries(${GAME} PUBLIC ${LIBCONFIG_LIBRARIES})
else()
	find_library(LIBCONFIG_LOCATION libconfig REQUIRED)
	target_link_libraries(${GAME} PUBLIC ${LIBCONFIG_LOCATION})
endif()

target_include_directories(external PUBLIC "include/external")
target_include_directories(${GAME} PUBLIC "include" "include/external" "${GAME}.d/include")

target_compile_definitions(${GAME} PUBLIC WIP_GAME=${GAME} WIP_WINDOW_BACKEND=${WINDOW_BACKEND})
