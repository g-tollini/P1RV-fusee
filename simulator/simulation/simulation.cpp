#include <iostream>

#include "simulation.hpp"

using namespace std;

void *simulationMainLoop(void *pData)
{
    while (true)
    {

        if (stop)
        {
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
                int dt = sim_step_ms < sim_untill_ms ? sim_step_ms : sim_untill_ms;
                sim_untill_ms -= dt;
                sim_since_ms += dt;
                string s = to_string(sim_since_ms);
                s.append(" ms");
                sharedBuffer->push_back(s);
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