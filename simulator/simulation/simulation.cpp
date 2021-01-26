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
    }

    DynamicModel *pDynMod;
    switch (pSd->pShm->model)
    {
    case Cardan:
        cout << "Dynamic model is : Cardan model" << endl;
        pDynMod = new CardanModel(pSd->pShm);
        break;
    case Quaternions:
        cout << "Dynamic model is : Quaternions model" << endl;
        pDynMod = new QuaternionsModel();
        break;
    default:
        cout << "Dynamic model is : Simple model" << endl;
        pDynMod = new SimpleModel();
        break;
    }

    Solver *pSolver;
    switch (pSd->pShm->method)
    {
    case methodEuler:
        cout << "Solver is : Euler" << endl;
        pSolver = new Euler(pDynMod);
        break;
    case methodRK4:
        cout << "Solver is : RK4" << endl;
        pSolver = new RungeKutta4(pDynMod);
        break;
    default:
        cout << "Solver is : Euler" << endl;
        pSolver = new Euler(pDynMod);
        break;
    }

    // Initialise the state
    pDynMod->SetPosition(pSd->pShm->position);
    pDynMod->SetAttitude(pSd->pShm->attitude);

    while (true)
    {
        pSd->pShm->simulationTerminated |= pSd->pShm->t_ms > 60000;
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
            if (pSd->sim_untill_ms == 0)
            {
                pthread_mutex_unlock(pSd->arduinoMutex);
                pthread_mutex_lock(pSd->simulationMutex);
            }
            else if (pSd->pShm->interfaceOn && (pSd->pShm->next_frame_ms == 0))
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
            else
            {
                // Be careful about thread calls like cout

                int step_ms = min(pSd->pShm->step_ms, pSd->sim_untill_ms);
                if (pSd->pShm->interfaceOn)
                {
                    step_ms = min(step_ms, pSd->pShm->next_frame_ms);
                }

                pSd->pShm->next_frame_ms -= step_ms;
                pSd->sim_untill_ms -= step_ms;

                pSolver->UpdateCommand(pSd);
                pSolver->ComputeNextState(step_ms);

                // Update the output
                pSd->pShm->position = pDynMod->getPosition();
                pSd->pShm->attitude = pDynMod->getAttitude();

                pSd->pShm->t_ms += step_ms;

                string s = to_string(pSd->pShm->t_ms);
                s.append(" ms total \n");
                pSd->sharedBuffer->push_back(s);
            }
        }
    }
    return NULL;
}