set(LIB_NAME P1RV-fusee-arduino-main)

add_library(
  ${LIB_NAME}
arduinomain.cpp
)

add_subdirectory(fusee)
add_subdirectory(my_arduino_layer)


