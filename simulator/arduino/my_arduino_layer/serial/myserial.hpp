#include <iostream>

#include "../../../threading.hpp"

using namespace std;

class SerialObj
{
public:
    SerialObj(ostream &_os);
    void begin(int _baud);

    void print(string const &s) const;
    void print(char const *s) const;

private:
    ostream *os;
    int baud;
};