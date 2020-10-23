#include "../threading.hpp"

void *simulationMainLoop(void *pData);

/**
 * @brief Set this to true and unlock the simulationMutex to end the simulation infinite loop
 * 
 */
extern bool stop;