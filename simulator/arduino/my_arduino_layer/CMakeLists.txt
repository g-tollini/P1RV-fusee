set(LIB_NAME, P1RV-fusee-arduino-layer)

include(CTest)
enable_testing()

add_library(P1RV-fusee-arduino-layer 
  serial/myserial.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


