#pragma once

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string>
#include <list>
#include <cstring>

#include "simulator/simulation/dynamic-model/parameters.hpp"

using namespace std;

#define SHM_FILE_NAME "/shm-fusee"
#define SEM_INTERFACE_FILE_NAME "/sem-interface"
#define SEM_SIMULATOR_FILE_NAME "/sem-simulator"
#define SHM_SIZE sizeof(SharedMemory)

/* General remark about posix :
 * In order to synchronize 2 processes, posix needs 2 semaphores. Theoretically, 1 is sufficient but the way posix works,
 * we annot ensure the execution time is handed to the other process each time the semaphore in common is posted. With 2
 * different semaphore this problem si solved and both processes alternate
*/

struct Vec3d
{
    double x;
    double y;
    double z;
};

/**
 * @brief Structure of the shared memory segment
 * 
 */
struct SharedMemory
{
    Model model;
    Method method;
    bool simulationTerminated;
    int step_ms;
    int t_ms;
    Vec3d position;
    Vec3d attitude;
};

/**
 * @brief Sets the content of the shared memory segment to the initial values
 * 
 * @param shm 
 */
void SharedMemoryInit(SharedMemory *shm);

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

void errnoPrint(void);