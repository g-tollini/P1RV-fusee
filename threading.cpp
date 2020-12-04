#include <iostream>

#include "threading.hpp"

void errnoPrint(void)
{
    switch (errno)
    {
    case EACCES:
        cout << "EACCESS" << endl;
        break;
    case EINVAL:
        cout << "EINVAL" << endl;
        break;
    case EMFILE:
        cout << "EMFILE" << endl;
        break;
    case ENOENT:
        cout << "ENOENT" << endl;
        break;
    default:
        cout << "Unknown errno : " << errno << endl;
        break;
    }
}

void SharedMemoryInit(SharedMemory *shm)
{
    shm->simulationTerminated = false;
    shm->t_ms = 0;
}