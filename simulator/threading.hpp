#include <pthread.h>
#include <string>
#include <list>

using namespace std;
/**
 * @brief The amount of time that is left to simulate. Is set by the arduino code to a positive value then decremented by the simulation
 * 
 */
extern volatile int sim_untill_ms;
/**
 * @brief Set this to true and unlock the simulationMutex to end the simulation infinite loop
 * 
 */

/**
 * @brief Current time of the simulation
 * 
 */
extern volatile int sim_since_ms;

/**
 * @brief Simulation time step
 * 
 */
extern volatile int sim_step_ms;

extern bool stop;

extern list<string> *sharedBuffer;

/**
 * @brief Is locked if and only if the simulation is running
 * 
 */
extern pthread_mutex_t *simulationMutex;
/**
 * @brief Is locked if and only if the arduino code is executing
 * 
 */
extern pthread_mutex_t *arduinoMutex;

void *arduinoMain(void *pData);