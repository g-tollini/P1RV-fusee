#include <iostream>

#include "../../threading.hpp"
#include "simulation.hpp"
#include "dynamic-model/dynamic-model.hpp"
#include "dynamic-model/simple-model.hpp"
#include "dynamic-model/cardan-model.hpp"
#include "dynamic-model/quaternions-model.hpp"

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
    sem_t *semInterface;
    sem_t *semSimulator;
    // Opens posix semaphores
    if (shm->interfaceOn)
    {
        semInterface = sem_open(SEM_INTERFACE_FILE_NAME, O_RDWR, S_IRUSR | S_IWUSR);
        if (semInterface == SEM_FAILED)
        {
            cout << "Semaphore opening failed" << endl;

            errnoPrint();
            exit(1);
        }

        semSimulator = sem_open(SEM_SIMULATOR_FILE_NAME, O_RDWR, S_IRUSR | S_IWUSR);
        if (semSimulator == SEM_FAILED)
        {
            cout << "Semaphore opening failed" << endl;

            errnoPrint();
            exit(1);
        }
    }

    DynamicModel *pDynMod;
    switch (shm->model)
    {
    case Cardan:
        pDynMod = new CardanModel();
        break;
    case Quaternions:
        pDynMod = new QuaternionsModel();
        break;
    default:
        pDynMod = new SimpleModel();
        break;
    }

    Solver *pSolver;
    switch (shm->method)
    {
    case methodEuler:
        pSolver = new Euler(shm);
        break;
    case methodRK4:
        pSolver = new RungeKutta4(shm);
        break;
    default:
        pSolver = new Euler(shm);
        break;
    }

    while (true)
    {
        cout << "ici" << endl;
        pthread_mutex_lock(simulationMutex);
        cout << "ici" << endl;
        shm->simulationTerminated |= shm->t_ms > 500;
        if (shm->simulationTerminated)
        {
            cout << "stop" << endl;
            if (shm->interfaceOn)
            {
                sem_close(semInterface);
                sem_close(semSimulator);
                if (int e = sem_post(semInterface) != 0)
                {
                    cout << " semInterface V error code : " << e << endl;
                }
            }
            pthread_mutex_unlock(arduinoMutex);
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
                // Be careful about thread calls like cout

                int step_ms = sim_step_ms < sim_untill_ms ? sim_step_ms : sim_untill_ms;
                sim_untill_ms -= step_ms;

                // pSolver->ComputeNextStep(step_ms);

                shm->t_ms += step_ms;
            }
            else
            {
                pthread_mutex_unlock(arduinoMutex);
                if (shm->interfaceOn)
                {
                    if (int e = sem_post(semInterface) != 0)
                    {
                        cout << " semInterface V error code : " << e << endl;
                    }
                    if (int e = sem_wait(semSimulator) != 0)
                    {
                        cout << " semSimulator P error code : " << e << endl;
                    }
                }
            }
        }
    }
    return NULL;
}