cmake_minimum_required(VERSION 3.16)
project(cstarter_gui C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CLAY_H "${CMAKE_CURRENT_SOURCE_DIR}/third_party/clay.h")
if(NOT EXISTS "${CLAY_H}")
	message(STATUS "Downloading clay.h")
	file(DOWNLOAD
		"https://raw.githubusercontent.com/nicbarker/clay/main/clay.h"
		"${CLAY_H}"
		SHOW_PROGRESS
		STATUS _clay_status
	)
	list(GET _clay_status 0 _clay_err)
	if(NOT _clay_err EQUAL 0)
		message(FATAL_ERROR "Could not download clay.h")
	endif()
endif()

set(CLAY_RENDERER "${CMAKE_CURRENT_SOURCE_DIR}/third_party/clay_renderer_raylib.c")
if(NOT EXISTS "${CLAY_RENDERER}")
	message(STATUS "Downloading clay_renderer_raylib.c")
	file(DOWNLOAD
		"https://raw.githubusercontent.com/nicbarker/clay/main/renderers/raylib/clay_renderer_raylib.c"
		"${CLAY_RENDERER}"
		SHOW_PROGRESS
		STATUS _ren_status
	)
	list(GET _ren_status 0 _ren_err)
	if(NOT _ren_err EQUAL 0)
		message(FATAL_ERROR "Could not download clay_renderer_raylib.c")
	endif()
endif()

find_package(raylib 5.0 QUIET)
if(NOT raylib_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		raylib
		GIT_REPOSITORY https://github.com/raysan5/raylib.git
		GIT_TAG 5.5
		GIT_SHALLOW TRUE
	)
	set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(raylib)
endif()

add_executable(cstarter-gui
	main.c
	app.c
	layout.c
)
target_include_directories(cstarter-gui PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/third_party
)
target_link_libraries(cstarter-gui PRIVATE raylib)

if(UNIX AND NOT APPLE)
	target_link_libraries(cstarter-gui PRIVATE m dl pthread)
endif()
