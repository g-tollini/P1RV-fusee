#include <iostream>
#include <chrono>

#include "GL/freeglut.h"
#include "GL/glut.h"
#include "GL/gl.h"

// base
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>

// Keyboard input
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>

#include <osgDB/ReadFile>

#include <errno.h>

#include "../threading.hpp"

using namespace std;
using namespace osg;

SharedMemory *shm;
sem_t *semInterface;
sem_t *semSimulator;

int nb_echanges;

bool simulationInProcess = false;

ref_ptr<PositionAttitudeTransform> pat(new PositionAttitudeTransform());

void *printSimulationData(void *pData);
void SimulationHandler(int value);
bool StartDisplay(void);
void StartSimulation(void);

class MyUpdateCallback : public NodeCallback
{
public:
    virtual void operator()(Node *n, NodeVisitor *nv)
    {
        SimulationHandler(0);
    }
};

class MyTextCallback : public NodeCallback
{
public:
    virtual void operator()(Node *n, NodeVisitor *nv)
    {
        osgText::Text *pTextNode;
        pTextNode = (osgText::Text *)n;
        pTextNode->setText(to_string(shm->t_ms));
    }
};

void KeyboardHandler(unsigned char key, int xpix, int ypix)
{
    switch (key)
    {
    case 'a':
        StartSimulation();
        break;
    case 'e':
        if (!shm->simulationTerminated)
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
        break;
    default:
        break;
    }
    return;
}

void HandleDisplay()
{
}

void SimulationHandler(int value)
{
    static auto start = std::chrono::steady_clock::now();
    static auto stop = std::chrono::steady_clock::now();
    static int previous_step_ms = shm->step_ms;
    static int time_to_wait = 0;
    std::chrono::duration<double> elapsed_seconds;
    if (simulationInProcess)
    {
        if (int e = sem_wait(semInterface) != 0)
        {
            cout << " semInterface P error code : " << e << endl;
        }
        stop = std::chrono::steady_clock::now();
        // Manipulate the shared memory (only) here. Be careful about thread calls

        cout << "t_ms = " << shm->t_ms << " ms" << endl;

        elapsed_seconds = stop - start;
        previous_step_ms = shm->step_ms;
        shm->step_ms = max(6, (int)(1000 * elapsed_seconds.count()) - 1);

        pat->setPosition(Vec3d(shm->position.x, shm->position.y, shm->position.z));

        if (shm->simulationTerminated)
        {
            simulationInProcess = false;
        }

        start = std::chrono::steady_clock::now();
        if (int e = sem_post(semSimulator) != 0)
        {
            cout << " semSimulator V error code : " << e << endl;
        }
    }
    else
    {
        shm->step_ms = 500;
    }
    glutPostRedisplay();
    cout << "step : " << 1000 * elapsed_seconds.count() << " ms" << endl;
    time_to_wait = max(0, previous_step_ms - (int)(1000 * elapsed_seconds.count()));
    glutTimerFunc(shm->step_ms, SimulationHandler, 0);
    return;
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

    StartSimulation();
    StartDisplay();

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

bool StartDisplay(void)
{
    /* OBJECTS CREATION */

    //Creating the viewer
    osgViewer::Viewer viewer;

    //Creating the root node
    osg::ref_ptr<osg::Group> root(new osg::Group);

    //The geode containing our shape
    osg::ref_ptr<osg::Geode> myshapegeode(new osg::Geode);
    osg::ref_ptr<osg::Geode> mystextgeode(new osg::Geode);

    //Our shape: a capsule, it could have been any other geometry (a box, plane, cylinder etc.)
    osg::ref_ptr<osg::Capsule> myCapsule(new osg::Capsule(osg::Vec3f(), 1, 2));

    //Our shape drawable
    osg::ref_ptr<osg::ShapeDrawable> capsuledrawable(new osg::ShapeDrawable(myCapsule.get()));
    // osg::ref_ptr<osgText::Text> text(new osgText::Text());
    // text->setText("YO MEK");
    // text->setUpdateCallback(new MyTextCallback());
    // root->addChild(text.get());

    /* SCENE GRAPH*/

    // Add the shape drawable to the geode
    myshapegeode->addDrawable(capsuledrawable.get());

    /* POSITION OF THE CAPSULE */
    // Rocket position

    root->addChild(pat.get());
    pat->addChild(myshapegeode.get());

    // Set the scene data
    viewer.setSceneData(root.get());

    /* KEYBOARD INPUT */

    //Stats Event Handler s key
    viewer.addEventHandler(new osgViewer::StatsHandler);

    //Windows size handler
    viewer.addEventHandler(new osgViewer::WindowSizeHandler);

    // add the state manipulator
    viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));

    /* TERRAIN */
    // Create transformation node
    ref_ptr<MatrixTransform> terrainScaleMAT(new MatrixTransform);

    // Scale matrix
    Matrix terrainScaleMatrix;
    terrainScaleMatrix.makeScale(osg::Vec3d(1, 1, 1));

    //Loading the terrain node
    ref_ptr<Node> terrainnode(osgDB::readNodeFile("/home/guillaume/centrale/P1RV-fusee/ressources/background.stl"));

    //Set transformation node parameters
    terrainScaleMAT->addChild(terrainnode.get());
    terrainScaleMAT->setMatrix(terrainScaleMatrix);

    root->addChild(terrainnode.get());

    root->setUpdateCallback(new MyUpdateCallback());

    /* START VIEWER */
    //The viewer.run() method starts the threads and the traversals.
    return (viewer.run());
}

void StartSimulation(void)
{
    nb_echanges = 0;
    SharedMemoryInit(shm);
    system("cd ../../simulator && cmake . && ./../build/simulator/P1RV-fusee-simulator -gui &");
    cout << "Simulation lancée" << endl;
    sleep(1);
    glutTimerFunc(0, SimulationHandler, 0);
    if (sem_post(semSimulator) != 0)
    {
        cout << "semSimulator V error" << endl;
    }
    simulationInProcess = true;

    return;
}