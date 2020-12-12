set(EXEC_NAME P1RV-fusee-interface)

cmake_minimum_required (VERSION 3.8) 

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(OpenThreads)
find_package(osg)

find_package(OpenSceneGraph REQUIRED OpenThreads osg osgGA osgDB osgText osgViewer)

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

set(BKG_LOC "\"${PROJECT_SOURCE_DIR}/ressources/background.stl\"")
message("Background file located here : " ${BKG_LOC})
add_definitions(-DBACKGROUND_LOC=${BKG_LOC})

add_executable(
  ${EXEC_NAME}
  interface.cpp
)

add_library(testlib SHARED IMPORTED)
set_property(TARGET testlib PROPERTY IMPORTED_LOCATION "~/developpement/OpenSceneGraph/build/lib/osgPlugins-3.7.0/osgdb_stld.so")


target_link_libraries(${EXEC_NAME} 
  P1RV-fusee-threading
  pthread
  rt
  GL
  glut
  ${OPENSCENEGRAPH_LIBRARIES}
  osgdb_stld)
