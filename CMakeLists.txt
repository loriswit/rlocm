cmake_minimum_required(VERSION 2.8)
project(seed_manager)

set(CMAKE_CXX_STANDARD 11)

include_directories(third_party)

file(GLOB_RECURSE SOURCES src/* third_party/conio/* third_party/jos/*)
set(RESOURCES res/rscr.rc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_executable(seed_manager ${SOURCES} ${RESOURCES})
target_link_libraries(seed_manager wininet)

add_custom_command(TARGET seed_manager POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/readme.txt
            ${PROJECT_BINARY_DIR}/bin)
