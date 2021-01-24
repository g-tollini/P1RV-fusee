#include "myarduino.hpp"
#include "../../threading.hpp"

SerialObj Serial(cout);
SimulationData *pSd;
IdealBooster idealBooster;
HeightSensor heightSensor;

void silent_delay(int delay_ms)
{
    if (pSd->pShm->simulationTerminated)
    {
        pthread_exit(nullptr);
        return; // skip this function
    }
    pSd->sim_untill_ms = delay_ms;
    pthread_mutex_unlock(pSd->simulationMutex);
    pthread_mutex_lock(pSd->arduinoMutex);

    return;
}

void delay(int delay_ms)
{
    string s = "Attendre ";
    s.append(to_string(delay_ms));
    s.append(" ms");
    pSd->sharedBuffer->push_back(s);
    silent_delay(delay_ms);

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
            pthread_exit(nullptr);
        }
        else
        {
            delay(1);
            loop();
        }
    }
    return nullptr;
}
