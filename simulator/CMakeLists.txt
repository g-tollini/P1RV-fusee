set(EXEC_NAME P1RV-fusee-simulator)

add_executable(
  ${EXEC_NAME}
  simulator.cpp
)

add_subdirectory(arduino)
add_subdirectory(simulation)

target_link_libraries(${EXEC_NAME} 
  P1RV-fusee-simulation
  P1RV-fusee-arduino-main
  P1RV-fusee-arduino-code
  P1RV-fusee-arduino-layer
  rt
  pthread)


