#include "myarduino.hpp"
#include "../../../threading.hpp"

SerialObj Serial(cout);

void delay(int ms)
{
    sim_untill_ms = ms;
    pthread_mutex_unlock(simulationMutex);
    pthread_mutex_lock(arduinoMutex);
    string s = "Attendre ";
    s.append(to_string(ms));
    s.append(" ms");
    sharedBuffer->push_back(s);

    return;
}

void *arduinoMain(void *pData)
{
    SharedMemory *shm = (SharedMemory *)pData;
    pthread_mutex_lock(arduinoMutex);
    setup();

    while (1)
    {
        if (shm->simulationTerminated)
        {
            pthread_mutex_unlock(simulationMutex);
            pthread_exit(NULL);
        }
        else
        {
            loop();
        }
    }
    return NULL;
}
