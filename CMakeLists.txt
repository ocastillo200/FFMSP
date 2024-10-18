cmake_minimum_required(VERSION 3.10)
project(FFMSP)
set(CMAKE_CXX_STANDARD 11)

set(GREEDY_SRC
    src/main.cpp
    src/libs/greedy.cpp
    src/libs/utils.cpp
    src/libs/grasp.cpp
)

set(TESTS_SRC
    src/tests.cpp
    src/libs/greedy.cpp
    src/libs/utils.cpp
    src/libs/grasp.cpp
)

add_executable(grasp ${GREEDY_SRC})
add_executable(tests ${TESTS_SRC})
include_directories(include)