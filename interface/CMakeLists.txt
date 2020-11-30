set(EXEC_NAME P1RV-fusee-interface)

add_executable(
  ${EXEC_NAME}
  interface.cpp
)

target_link_libraries(${EXEC_NAME} 
  P1RV-fusee-threading
  pthread
  rt
  GL
  glut)
