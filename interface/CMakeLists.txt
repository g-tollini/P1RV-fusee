set(EXEC_NAME P1RV-fusee-interface)

cmake_minimum_required (VERSION 3.8) 

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(OpenThreads)
find_package(osg)

find_package(OpenSceneGraph REQUIRED OpenThreads osg osgGA osgDB osgText osgViewer)

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

set(BKG_LOC "\"${PROJECT_SOURCE_DIR}/ressources/background.stl\"")
set(E_SH_LOC "\"${PROJECT_SOURCE_DIR}/e.sh\"")
message("Background file located here : " ${BKG_LOC})
message("e.sh located here : " ${E_SH_LOC})
add_definitions(-DBACKGROUND_LOC=${BKG_LOC} -DE_SH=${E_SH_LOC})

if (${APPLE})
add_definitions(-D__APPLE__)
endif()

add_executable(
  ${EXEC_NAME}
  interface.cpp
)

add_library(testlib SHARED IMPORTED)
set_property(TARGET testlib PROPERTY IMPORTED_LOCATION "~/developpement/OpenSceneGraph/build/lib/osgPlugins-3.7.0/osgdb_stld.so")


if (${APPLE})
  target_link_libraries(${EXEC_NAME} 
    P1RV-fusee-threading
    pthread
    rt
    GL
    glut
    ${OPENSCENEGRAPH_LIBRARIES}
    osgdb_stld)
elseif (${UNIX})
  target_link_libraries(${EXEC_NAME} 
    P1RV-fusee-threading
    pthread
    rt
    GL
    glut
    ${OPENSCENEGRAPH_LIBRARIES}
    osgdb_stld)
endif()
