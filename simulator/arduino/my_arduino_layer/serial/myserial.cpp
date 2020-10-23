#include "myserial.hpp"

SerialObj::SerialObj(ostream &_os)
{
    os = &_os;
}

void SerialObj::begin(int _baud)
{
    if (_baud != 0)
    {
        baud = _baud;
    }
    else
    {
        baud = 9600;
    }
    return;
}

void SerialObj::print(string const &s) const
{
    sim_untill_ms = int((double)s.size() / 9.6);
    pthread_mutex_unlock(simulationMutex);
    pthread_mutex_lock(arduinoMutex);
    sharedBuffer->push_back(s);
}

void SerialObj::print(char const *s) const
{
    string ss = s;
    print(ss);
}
