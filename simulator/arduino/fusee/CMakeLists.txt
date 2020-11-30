set(LIB_NAME, P1RV-fusee-arduino-code)

include(CTest)
enable_testing()

add_definitions(-DSIM)

add_library(P1RV-fusee-arduino-code P1RV-fusee-arduino-code.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
