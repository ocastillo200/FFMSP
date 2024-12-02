cmake_minimum_required(VERSION 3.10)
project(FFMSP)
set(CMAKE_CXX_STANDARD 11)

set(GREEDY_SRC
    src/main.cpp
    src/libs/greedy.cpp
    src/libs/utils.cpp
    src/libs/grasp.cpp
    src/libs/genetic.cpp
    src/libs/hybrid.cpp
)

set(TESTS_SRC
    src/tests.cpp
    src/libs/greedy.cpp
    src/libs/utils.cpp
    src/libs/grasp.cpp
    src/libs/genetic.cpp
    src/libs/hybrid.cpp
)

add_executable(hybrid ${GREEDY_SRC})
add_executable(tests ${TESTS_SRC})
include_directories(include)