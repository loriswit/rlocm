cmake_minimum_required(VERSION 2.8)
project(seed_manager)

set(CMAKE_CXX_STANDARD 11)

include_directories(third_party)

file(GLOB_RECURSE SOURCES src/* third_party/*)
set(RESOURCES res/rscr.rc)

add_executable(seed_manager ${SOURCES} ${RESOURCES})
target_link_libraries(seed_manager wininet)
