cmake_minimum_required(VERSION 3.0)

# Project main parameters
project(P1RV-fusee VERSION 0.8.0)
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

# Enable tests in project
find_package(GTest REQUIRED)
enable_testing()
add_test(UnitTests tests/unit_tests)

#Modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

#Eigen
find_package(Eigen3)

# Include all needed stuff
add_subdirectory(interface)
add_subdirectory(simulator)
add_subdirectory(tests)

# What to build
set (LIB_NAME P1RV-fusee-threading)

add_library(${LIB_NAME}
    threading.cpp)

