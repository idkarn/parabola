cmake_minimum_required(VERSION 3.12)
project(CParabola C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    mir
    GIT_REPOSITORY https://github.com/vnmakarov/mir.git
)

FetchContent_MakeAvailable(mir)

include_directories(inc)

file(GLOB SOURCES "src/*.c")

add_executable(pb ${SOURCES})

target_link_libraries(pb PRIVATE mir m)
