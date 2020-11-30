set(LIB_NAME P1RV-fusee-simulation)

add_library(
  ${LIB_NAME}
  simulation.cpp
  dynamic-model/solver.cpp
  dynamic-model/dynamic-model.cpp
)

target_link_libraries(${LIB_NAME} P1RV-fusee-threading)
