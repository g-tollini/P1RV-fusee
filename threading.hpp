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

#include </usr/share/eigen-3.3.8/Eigen/Eigen>

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

struct Vector3d
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
    bool interfaceOn = false;
    Model model;
    Method method;
    bool simulationTerminated;
    int step_ms;
    int t_ms;
    Vector3d position;
    Vector3d attitude;
    double booster_thrust;
};

/**
 * @brief Sets the content of the shared memory segment to the initial values
 * 
 * @param shm 
 */
void SharedMemoryInit(SharedMemory *shm);

struct SimulationData
{
    int sim_untill_ms;
    list<string> *sharedBuffer;
    pthread_mutex_t *simulationMutex;
    pthread_mutex_t *arduinoMutex;
    SharedMemory *pShm;
};

void *arduinoMain(void *pData);

void errnoPrint(void);