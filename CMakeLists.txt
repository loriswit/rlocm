cmake_minimum_required(VERSION 2.8)
project(rlocm)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -std=c++11)

add_definitions(-DWIN32)
add_definitions(-D_hypot=hypot)

file(GLOB_RECURSE RLOCM_SRC src/*)
set(RLOCM_RES res/rscr.rc)

file(GLOB_RECURSE CONIO_SRC third_party/conio/*)
include_directories(third_party/conio)

find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init)
endif ()

if (NOT EXISTS "${PROJECT_SOURCE_DIR}/third_party/fltk/CMakeLists.txt")
    message(FATAL_ERROR "Cannot find FLTK submodule")
endif ()

add_subdirectory(third_party/fltk)
include_directories(third_party/fltk)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_executable(rlocm ${RLOCM_SRC} ${CONIO_SRC} ${RLOCM_RES})
target_link_libraries(rlocm fltk wininet psapi)

add_custom_command(TARGET rlocm POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/readme.txt
            ${PROJECT_BINARY_DIR}/bin)
