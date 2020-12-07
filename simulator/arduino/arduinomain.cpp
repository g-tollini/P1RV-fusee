#include "myarduino.hpp"
#include "../../threading.hpp"

SerialObj Serial(cout);
SimulationData *pSd;

void delay(int delay_ms)
{
    pSd->sim_untill_ms = delay_ms;
    string s = "Attendre ";
    s.append(to_string(delay_ms));
    s.append(" ms");
    pSd->sharedBuffer->push_back(s);
    pthread_mutex_unlock(pSd->simulationMutex);
    pthread_mutex_lock(pSd->arduinoMutex);

    return;
}

void *arduinoMain(void *pData)
{
    pSd = (SimulationData *)pData;
    Serial.SetSimulationData(pSd);
    pthread_mutex_lock(pSd->arduinoMutex);
    setup();

    while (1)
    {
        if (pSd->pShm->simulationTerminated)
        {
            pthread_mutex_unlock(pSd->simulationMutex);
            pthread_exit(NULL);
        }
        else
        {
            loop();
        }
    }
    return NULL;
}
