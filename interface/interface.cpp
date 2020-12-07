#include <iostream>

#include "GL/freeglut.h"
#include "GL/glut.h"
#include "GL/gl.h"

#include <errno.h>

#include "../threading.hpp"

using namespace std;

SharedMemory *shm;
sem_t *semInterface;
sem_t *semSimulator;

int nb_echanges;

void *printSimulationData(void *pData);

void KeyboardHandler(unsigned char key, int xpix, int ypix)
{
    switch (key)
    {
    case 'a':
        nb_echanges = 0;
        SharedMemoryInit(shm);
        system("cd ../../simulator && cmake . && ./../build/simulator/P1RV-fusee-simulator -gui &");
        cout << "Simulation lancée" << endl;
        sleep(1);
        if (sem_post(semSimulator) != 0)
        {
            cout << "semSimulator V error" << endl;
        }
        break;
    case 'e':
        if (int e = sem_wait(semInterface) != 0)
        {
            cout << " semInterface P error code : " << e << endl;
        }
        // Manipulate the shared memory (only) here. Be careful about thread calls

        if (int e = sem_post(semSimulator) != 0)
        {
            cout << " semSimulator V error code : " << e << endl;
        }
        break;
    default:
        break;
    }
    return;
}

void HandleDisplay()
{
    if (int e = sem_wait(semInterface) != 0)
    {
        cout << " semInterface P error code : " << e << endl;
    }
    // Manipulate the shared memory (only) here. Be careful about thread calls

    if (int e = sem_post(semSimulator) != 0)
    {
        cout << " semSimulator V error code : " << e << endl;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("P1RV - Fusee");
    glutDisplayFunc(HandleDisplay);
    glutKeyboardFunc(KeyboardHandler);

    // Creates a file descriptor
    shm_unlink(SHM_FILE_NAME);
    int segmentFileDescriptor = shm_open(SHM_FILE_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (segmentFileDescriptor == -1)
    {
        cout << "Segment non créé" << endl;
        return 1;
    }

    // Posix function that sets the size of the shared memory
    int outTruncate = ftruncate(segmentFileDescriptor, SHM_SIZE);

    // Creates posix shared memory
    off_t memOffset = 0;
    void *shmPtr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, segmentFileDescriptor, memOffset);
    shm = (SharedMemory *)shmPtr;

    shm->interfaceOn = true;
    shm->step_ms = 15;

    shm->model = Model::Simple;
    shm->method = Method::methodEuler;

    // Creates posix semaphores
    sem_unlink(SEM_INTERFACE_FILE_NAME);
    semInterface = sem_open(SEM_INTERFACE_FILE_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
    if (semInterface == SEM_FAILED)
    {
        cout << "Semaphore opening failed" << endl;
        errnoPrint();
        exit(1);
    }
    sem_unlink(SEM_SIMULATOR_FILE_NAME);
    semSimulator = sem_open(SEM_SIMULATOR_FILE_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
    if (semSimulator == SEM_FAILED)
    {
        cout << "Semaphore opening failed" << endl;

        errnoPrint();
        exit(1);
    }

    glutMainLoop();
    sem_close(semInterface);
    sem_close(semSimulator);
    sem_unlink(SEM_INTERFACE_FILE_NAME);
    sem_unlink(SEM_SIMULATOR_FILE_NAME);
    return 0;
}

void *printSimulationData(void *pData)
{
    static int t_ms_1 = 0;
    static int t_ms_2 = 0;
    shm = (SharedMemory *)pData;
    while (true)
    {

        if (int e = sem_wait(semInterface) != 0)
        {
            cout << " semInterface P error code : " << e << endl;
        }
        shm->t_ms = 100;
        cout << shm->position.z;
        sleep(0.1);

        if (t_ms_1 == t_ms_2)
        {
            cout << "Egalite t_ms probleme !" << endl;
        }
        nb_echanges++;
        cout << "nb echanges : " << nb_echanges << endl;
        string *s = new string;
        s->append("echo \" t_ms = ");
        s->append(to_string(t_ms_1));
        s->append("\"");
        system(s->c_str());
        // alors que :
        // cout << s->c_str(); //casse tout !

        if (int e = sem_post(semSimulator) != 0)
        {
            cout << " semSimulator V error code : " << e << endl;
        }
    }
    return NULL;
}