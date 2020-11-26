#include "../../threading.hpp"

#include "dynamic-model/parameters.hpp"
#include "dynamic-model/solver.hpp"
#include "dynamic-model/dynamic-model.hpp"

void *simulationMainLoop(void *pData);

/**
 * @brief Set this to true and unlock the simulationMutex to end the simulation infinite loop
 * 
 */
extern bool stop;