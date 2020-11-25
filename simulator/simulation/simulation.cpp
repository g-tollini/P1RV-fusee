#include <iostream>

#include "simulation.hpp"

using namespace std;

/**
 * @brief Computes the state each time the posix semaphore semSimulator is posted.
 * 
 * @param pData 
 * @return void* 
 */
void *simulationMainLoop(void *pData)
{
    SharedMemory *shm = (SharedMemory *)pData;

    // Opens posix semaphores
    sem_t *semInterface = sem_open(SEM_INTERFACE_FILE_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    if (semInterface == SEM_FAILED)
    {
        cout << "Semaphore opening failed" << endl;

        errnoPrint();
        exit(1);
    }

    sem_t *semSimulator = sem_open(SEM_SIMULATOR_FILE_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    if (semSimulator == SEM_FAILED)
    {
        cout << "Semaphore opening failed" << endl;

        errnoPrint();
        exit(1);
    }

    // if (sem_wait(semSimulator) != 0)
    // {
    //     cout << " semSImulator P error" << endl;
    // }

    while (true)
    {
        shm->simulationTerminated |= shm->t_ms > 500;
        if (shm->simulationTerminated)
        {
            pthread_mutex_unlock(arduinoMutex);
            if (int e = sem_post(semSimulator) != 0)
            {
                cout << " semSimulator V error code : " << e << endl;
            }
            sem_close(semInterface);
            sem_close(semSimulator);
            return NULL;
        }
        else
        {
            while (sharedBuffer->size() != 0)
            {
                cout << sharedBuffer->front() << endl;
                sharedBuffer->pop_front();
            }
            if (sim_untill_ms > 0)
            {
                if (int e = sem_wait(semSimulator) != 0)
                {
                    cout << " semSimulator P error code : " << e << endl;
                }
                // Manipulate the shared memory (only) here. Be careful about thread calls

                int dt = sim_step_ms < sim_untill_ms ? sim_step_ms : sim_untill_ms;
                sim_untill_ms -= dt;
                shm->t_ms += dt;

                if (int e = sem_post(semInterface) != 0)
                {
                    cout << " semInterface V error code : " << e << endl;
                }
            }
            else
            {
                pthread_mutex_unlock(arduinoMutex);
                pthread_mutex_lock(simulationMutex);
            }
        }
    }
    return NULL;
}