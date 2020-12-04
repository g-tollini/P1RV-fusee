set(LIB_NAME P1RV-fusee-simulation)

set (EIGEN3_INCLUDE_DIR /usr/share/eigen-3.3.8)
set (Eigen3_DIR /usr/share/eigen-3.3.8)
set (INCLUDE_DIRECTORIES /usr/share/eigen-3.3.8)

# Trying to locate the Eigen3 lib thanks to the file FindEigen3.cmake in the subdirectory (of the directory of this file) CMake. If it fails we will do it manually
#find_package (Eigen3 3.3 NO_MODULE)
#
#if (Eigen3_FOUND)
#
#else (Eigen3_FOUND)
# set (Eigen3_DIR /usr/share/eigen-3.3.8)
#endif (Eigen3_FOUND)

add_library(
  ${LIB_NAME}
  simulation.cpp
  dynamic-model/solver.cpp
  dynamic-model/simple-model.cpp
  dynamic-model/cardan-model.cpp
  dynamic-model/quaternions-model.cpp)


target_link_libraries(${LIB_NAME} P1RV-fusee-threading)
  
