set(LIB_NAME P1RV-fusee-arduino-main)

execute_process(COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/fusee/fusee.ino ${CMAKE_CURRENT_SOURCE_DIR}/P1RV-fusee-arduino-code.cpp)

add_definitions(${ARDUINO_CODE_LIB} -DSIM)

add_library(
  ${LIB_NAME}
  arduinomain.cpp
  P1RV-fusee-arduino-code.cpp)

target_link_libraries(${LIB_NAME})

add_subdirectory(my_arduino_layer)
