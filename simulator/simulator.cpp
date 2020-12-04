#include <iostream>

#include "../threading.hpp"
#include "simulation/simulation.hpp"

using namespace std;

volatile int sim_untill_ms = 0;
volatile int sim_since_ms = 0;
volatile int sim_step_ms = 20;

/**
 * @brief Set this to true and unlock the simulationMutex to end the simulation infinite loop
 * 
 */
bool stop = false;

list<string> *sharedBuffer = new list<string>();

/**
 * @brief Is locked if and only if the simulation is running
 * 
 */
pthread_mutex_t *simulationMutex = new pthread_mutex_t();
/**
 * @brief Is locked if and only if the arduino code is executing
 * 
 */
pthread_mutex_t *arduinoMutex = new pthread_mutex_t();

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
    // Posix function that sets the size of the shared memory
    SharedMemory *shm;

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
        shm = (SharedMemory *)shmPtr;
        shm->interfaceOn = interfaceOn;
    }
    else
    {
        cout << "Interface off" << endl;
    }

    pthread_mutex_init(simulationMutex, NULL);
    pthread_mutex_init(arduinoMutex, NULL);
    pthread_mutex_lock(simulationMutex);
    pthread_t simulationThread;
    pthread_t arduinoThread;
    if (pthread_create(&simulationThread, NULL, simulationMainLoop, NULL) != 0)
    {
        cout << "Echec de la création du thread simulation" << endl;
        exit(1);
    }
    cout << "Thread simulation créé" << endl;
    if (pthread_create(&arduinoThread, NULL, arduinoMain, NULL) != 0)
    {
        cout << "Echec de la création du thread arduino" << endl;
        exit(1);
    }
    cout << "Thread arduino créé" << endl;
    if (pthread_join(arduinoThread, NULL) != 0)
    {
        cout << "Echec de la terminaison du thread simulation" << endl;
        exit(1);
    }
    cout << "Thread arduino terminé" << endl;
    if (pthread_join(simulationThread, NULL) != 0)
    {
        cout << "Echec de la terminaison du thread arduino" << endl;
        exit(1);
    }
    cout << "Simulation succesfully terminated" << endl;

    return 0;
}