cmake_minimum_required(VERSION 3.11)
project(FireCloud)

MESSAGE(STATUS "begin configure project")
option(INNER_MATH "use self math library" OFF)

set(CORE
	core/fc_image.h
	core/fc_image.cpp
	core/fc_common.h
	core/model.h
	core/model.cpp
	core/geometry.h
)
source_group("core" FILES ${CORE})

set (PLATFORM
		crossplatform/window.h
		)
source_group("platform" FILES ${PLATFORM})

if(MSVC)
	set(SOURCES ${PLATFORM} win32.cpp)
endif()


set (SRC_LIST  ${PLATFORM} ${CORE} main.cpp)
add_executable(render ${SRC_LIST} )