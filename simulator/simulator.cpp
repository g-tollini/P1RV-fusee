#include <iostream>

#include "../threading.hpp"
#include "simulation/simulation.hpp"

using namespace std;

int SimulationDataInit(SimulationData *_pSd);

bool interfaceOn = false;

/**
 * @brief Creates 2 threads and runs them : 1 for the computation of the state, 1 for the execution of the arduino code
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
    SimulationData *pSd = new SimulationData;
    SimulationDataInit(pSd);

    SharedMemory *pShm = new SharedMemory;

    bool interfaceOn = false;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-gui") == 0)
        {
            interfaceOn = true;
        }
    }

    if (interfaceOn)
    {
        //Creates a file descriptor
        int segmentFileDescriptor = shm_open(SHM_FILE_NAME, O_RDWR, S_IRUSR | S_IWUSR);
        cout << "Code shm_open : " << segmentFileDescriptor << endl;
        if (segmentFileDescriptor == -1)
        {
            exit(1);
        }

        // Opens posix shared memory
        off_t memOffset = 0;
        void *shmPtr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, segmentFileDescriptor, memOffset);
        pSd->pShm = (SharedMemory *)shmPtr;
    }
    else
    {
        cout << "Interface off" << endl;
        pSd->pShm->step_ms = 20;
    }

    pthread_mutex_lock(pSd->arduinoMutex);
    pthread_mutex_lock(pSd->simulationMutex);
    pthread_t *simulationThread = new pthread_t;
    pthread_t *arduinoThread = new pthread_t;

    if (pthread_create(simulationThread, NULL, simulationMainLoop, pSd) != 0)
    {
        cout << "Echec de la création du thread simulation" << endl;
        exit(1);
    }
    cout << "Thread simulation créé" << endl;
    if (pthread_create(arduinoThread, NULL, arduinoMain, pSd) != 0)
    {
        cout << "Echec de la création du thread arduino" << endl;
        exit(1);
    }
    cout << "Thread arduino créé" << endl;
    if (pthread_join(*arduinoThread, NULL) != 0)
    {
        cout << "Echec de la terminaison du thread simulation" << endl;
        exit(1);
    }
    cout << "Thread arduino terminé" << endl;
    if (pthread_join(*simulationThread, NULL) != 0)
    {
        cout << "Echec de la terminaison du thread arduino" << endl;
        exit(1);
    }
    cout << "Simulation succesfully terminated" << endl;

    // PUT SIMULATION LOOP HERE

    return 0;
}

int SimulationDataInit(SimulationData *_pSd)
{

    _pSd->sharedBuffer = new list<string>;

    _pSd->simulationMutex = new pthread_mutex_t;
    _pSd->arduinoMutex = new pthread_mutex_t;

    pthread_mutexattr_t *pmat = new pthread_mutexattr_t;
    pthread_mutexattr_init(pmat);

    pthread_mutex_init(_pSd->simulationMutex, pmat);
    pthread_mutex_init(_pSd->arduinoMutex, pmat);

    return 0;
}