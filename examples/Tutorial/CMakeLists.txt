cmake_minimum_required(VERSION 3.10)

project(Tutorial)

set(SOURCE_FILES
	Tutorial.cpp
	)

set(HEADER_FILES
	)

# "Sources" and "Headers" are the group names in Visual Studio.
source_group("Sources" FILES ${SOURCE_FILES})
source_group("Headers" FILES ${INC_FILES})

add_executable(TutorialExample ${SOURCE_FILES})

# Gosu's own examples have to be built into the parent directory so that they
# can find the shared 'media' directory. You do not have to do this in your own
# Gosu projects.
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} PATH CACHE)
set_target_properties(TutorialExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PARENT_DIR})

# Platform-dependent compiler flags.
if(MSVC)
	set_target_properties(TutorialExample PROPERTIES COMPILE_FLAGS "/W4 /wd4127")
else()
	set_target_properties(TutorialExample PROPERTIES COMPILE_FLAGS "-g -std=gnu++11")
endif()

find_package(Gosu REQUIRED development)
include_directories(${GOSU_INCLUDE_DIRS})
target_link_libraries(TutorialExample ${GOSU_LIBRARIES})
