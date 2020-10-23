#include <iostream>

#include "threading.hpp"
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

int main(int argc, char *argv[])
{
    pthread_mutex_init(simulationMutex, NULL);
    pthread_mutex_init(arduinoMutex, NULL);
    pthread_mutex_lock(simulationMutex);
    pthread_t simulationThread;
    pthread_mutex_lock(arduinoMutex);
    pthread_t arduinoThread;
    if (pthread_create(&simulationThread, NULL, simulationMainLoop, NULL) == 0)
    {
        cout << "Thread simulation créé" << endl;
    }
    if (pthread_create(&arduinoThread, NULL, arduinoMain, NULL) == 0)
    {
        cout << "Thread arduino créé" << endl;
    }

    while (sim_since_ms < 500)
    {
        ;
    }
    stop = true;
    if (pthread_join(arduinoThread, NULL) == 0)
    {
        cout << "Thread arduino terminé" << endl;
    }
    if (pthread_join(simulationThread, NULL) == 0)
    {
        cout << "Thread simulation terminé" << endl;
    }

    return 0;
}