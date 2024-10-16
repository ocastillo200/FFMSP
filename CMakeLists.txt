cmake_minimum_required(VERSION 3.10)
project(FFMSP)
set(CMAKE_CXX_STANDARD 11)

if(NOT CPLEX_ROOT)
    message(FATAL_ERROR "CPLEX_ROOT not set. Please specify the CPLEX root directory.")
endif()

if(APPLE)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
        set(ARCHITECTURE "arm64_osx")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
        set(ARCHITECTURE "x86_osx")
    else()
        message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
    endif()
elseif(UNIX)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
        set(ARCHITECTURE "x86-64_linux")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
        set(ARCHITECTURE "arm64_linux")
    else()
        message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
    endif()
else()
    message(FATAL_ERROR "Unsupported platform. This script is intended for macOS and Unix.")
endif()

include_directories(
    ${CPLEX_ROOT}/cplex/include
    ${CPLEX_ROOT}/concert/include
)

link_directories(
    ${CPLEX_ROOT}/cplex/lib/${ARCHITECTURE}/static_pic 
    ${CPLEX_ROOT}/concert/lib/${ARCHITECTURE}/static_pic
)

set(CMSA_SRC
    src/main.cpp
    src/libs/greedy.cpp
    src/libs/utils.cpp
)

set(TESTS_SRC
    src/tests.cpp
    src/libs/greedy.cpp
    src/libs/utils.cpp
)

add_executable(cmsa ${CMSA_SRC})
add_executable(tests ${TESTS_SRC})

# Enlazar bibliotecas de CPLEX y Concert
target_link_libraries(cmsa
    cplex
    concert
)

target_link_libraries(tests
    cplex
    concert
)

# Incluir directorio de tus propias cabeceras
include_directories(include)