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
    SimulationData *pSd = (SimulationData *)pData;
    sem_t *semInterface;
    sem_t *semSimulator;
    // Opens posix semaphores
    if (pSd->pShm->interfaceOn)
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

        if (int e = sem_wait(semSimulator) != 0)
        {
            cout << " semSimulator P error code : " << e << endl;
        }

        cout << "salut" << endl;
    }
    DynamicModel *pDynMod;
    switch (pSd->pShm->model)
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
    switch (pSd->pShm->method)
    {
    case methodEuler:
        pSolver = new Euler(pSd->pShm, pDynMod);
        break;
    case methodRK4:
        pSolver = new RungeKutta4(pSd->pShm, pDynMod);
        break;
    default:
        pSolver = new Euler(pSd->pShm, pDynMod);
        break;
    }

    while (true)
    {
        pSd->pShm->simulationTerminated |= pSd->pShm->t_ms > 6000;
        if (pSd->pShm->simulationTerminated)
        {
            if (pSd->pShm->interfaceOn)
            {
                if (int e = sem_post(semInterface) != 0)
                {
                    cout << " semInterface V error code : " << e << endl;
                }
                sem_close(semInterface);
                sem_close(semSimulator);
            }
            pthread_mutex_unlock(pSd->arduinoMutex);
            return NULL;
        }
        else
        {
            while (pSd->sharedBuffer->size() != 0)
            {
                cout << pSd->sharedBuffer->front() << endl;
                pSd->sharedBuffer->pop_front();
            }
            if (pSd->sim_untill_ms > 0)
            {
                // Be careful about thread calls like cout

                int step_ms = pSd->pShm->step_ms < pSd->sim_untill_ms ? pSd->pShm->step_ms : pSd->sim_untill_ms;
                pSd->sim_untill_ms -= step_ms;
                // string s = "computed a step of " + to_string(pSd->pShm->step_ms) + " ms";
                // pSd->sharedBuffer->push_back(s);

                pSolver->ComputeNextStep(step_ms);

                pSd->pShm->t_ms += step_ms;
            }
            else
            {
                pthread_mutex_unlock(pSd->arduinoMutex);
                pthread_mutex_lock(pSd->simulationMutex);
            }

            if (pSd->pShm->interfaceOn)
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
    return NULL;
}