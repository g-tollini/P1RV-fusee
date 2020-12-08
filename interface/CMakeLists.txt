set(EXEC_NAME P1RV-fusee-interface)

cmake_minimum_required (VERSION 3.8) 

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MODULE_PATH /usr/share/cmake-3.13/Modules)

find_package(OpenSceneGraph REQUIRED COMPONENTS osg OpenThreads osgDB osgUtil osgGA osgViewer)

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

add_executable(
  ${EXEC_NAME}
  interface.cpp
)

link_directories("~/developpement/OpenSceneGraph/build/lib/osgPlugins-3.7.0")

add_library(testlib SHARED IMPORTED)
 set_property(TARGET testlib PROPERTY IMPORTED_LOCATION "~/developpement/OpenSceneGraph/build/lib/osgPlugins-3.7.0/osgdb_stld.so")


target_link_libraries(${EXEC_NAME} 
  P1RV-fusee-threading
  pthread
  rt
  GL
  glut
  ${OPENSCENEGRAPH_LIBRARIES}
  osgdb_stld
  osgText)
