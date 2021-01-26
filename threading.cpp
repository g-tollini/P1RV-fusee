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
    shm->interfaceOn = true;
    shm->simulationTerminated = false;
    shm->step_ms = 10;
    shm->t_ms = 0;
    shm->method = Method::methodEuler;
    shm->model = Model::Cardan;
    shm->mass = 1.0;
    shm->position = Vector3d{0, 0, 0};
    shm->attitude = Vector3d{0, 0, 0};
    shm->tvc_angle_1 = 0.0;
    shm->tvc_angle_2 = 0.0;
}