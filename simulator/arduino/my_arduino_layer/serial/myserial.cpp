#include "myserial.hpp"
#include "../../myarduino.hpp"

SerialObj::SerialObj(ostream &_os)
{
    os = &_os;
}

void SerialObj::begin(int _baud)
{
    if (_baud > 0)
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
    silent_delay((double)s.size() / 9.6);
    pSd->sharedBuffer->push_back(s);
}

void SerialObj::print(char const *s) const
{
    string ss = s;
    print(ss);
}

void SerialObj::print(double d) const
{
    print(to_string(d));
}

void SerialObj::SetSimulationData(SimulationData *_pSd)
{
    pSd = _pSd;
}