cmake_minimum_required(VERSION 3.0)

# Project main parameters
project(P1RV-fusee VERSION 0.7.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#add_compile_options(-Wall -Wextra -Werror)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    message(STATUS "ccache found")
else()
    message(STATUS "ccache not found")
endif(CCACHE_FOUND)

# Options
#option(ENABLE_CLANG_TIDY
#    "If enabled, compile with clang-tidy checks"
#    OFF)

# Clang-Tidy configuration
#if ("${ENABLE_CLANG_TIDY}")...

# Enable tests in project
find_package(GTest REQUIRED)
enable_testing()
add_test(UnitTests tests/unit_tests)

# Python venv related
#set(Python3_VENV_DIR "${PROJECT_SOURCE_DIR}/.venv")

#if(EXISTS "${Python3_VENV_DIR}")
#    message(STATUS "Found Python3_VENV_DIR: ${Python3_VENV_DIR}")
#    set(Python3_ROOT_DIR "${Python3_VENV_DIR}")
#else()
#    message(FATAL_ERROR "No venv found in .venv")
#endif()

#find_package(Python3 REQUIRED COMPONENTS Interpreter)
#get_filename_component(Python3_STDLIB_PATH "${Python3_STDLIB}" DIRECTORY)
#get_filename_component(Python3_STDLIB_NAME "${Python3_STDLIB}" NAME)
#set(PYTHON_EXECUTABLE "${Python3_EXECUTABLE}")


# Include all needed stuff
#set(LIB_SEABER_INCLUDE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib/")
add_subdirectory(interface)
add_subdirectory(simulator)
add_subdirectory(tests)
# What to build
add_library(P1RV-fusee-threading
    threading.cpp)
