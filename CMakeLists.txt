cmake_minimum_required(VERSION 3.10)

find_program(CMAKE_C_COMPILER NAMES $ENV{CC} clang-cl PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} clang-cl PATHS ENV PATH NO_DEFAULT_PATH)

project(task-killer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")

add_executable(task-killer WIN32 src/main.cpp)

target_link_libraries(task-killer PRIVATE
    user32
    gdi32
    kernel32
)

set_target_properties(task-killer PROPERTIES
  LINK_FLAGS "/SUBSYSTEM:WINDOWS"
)




