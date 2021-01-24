#include <iostream>
#include <chrono>

#ifndef __APPLE__
#include "GL/freeglut.h"
#include "GL/glut.h"
#include "GL/gl.h"
#else
#include "OPENGL/gl.h"
#include "GLUT/glut.h"
#endif

// base
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>

// Keyboard input
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgGA/GUIEventHandler>

#include <osgDB/ReadFile>

#include <errno.h>

#include "../threading.hpp"

using namespace std;
using namespace osg;

SharedMemory *shm;
sem_t *semInterface;
sem_t *semSimulator;

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

        osgText::Text *txt = static_cast<osgText::Text *>(n);
        txt->setText(to_string(shm->t_ms).c_str());
    }
};

class KeyboardHandler : public osgGA::GUIEventHandler
{
public:
    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &)
    {
        switch (ea.getKey())
        {
        case 'a':
            StartSimulation();
            break;
        default:
            break;
        }
        return false;
    }
};

void SimulationHandler(int value)
{
    static auto tic = std::chrono::steady_clock::now();
    static auto toc = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds;
    if (simulationInProcess)
    {
        if (int e = sem_wait(semInterface) != 0)
        {
            cout << " semInterface P error code : " << e << endl;
        }
        // Manipulate the shared memory (only) here. Be careful about thread calls

        shm->next_frame_ms = 10;

        pat->setPosition(Vec3d(shm->position.x, shm->position.y, shm->position.z));

        if (shm->simulationTerminated)
        {
            simulationInProcess = false;
        }

        if (int e = sem_post(semSimulator) != 0)
        {
            cout << " semSimulator V error code : " << e << endl;
        }
    }

    return;
}

int main(int argc, char **argv)
{
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

    StartDisplay();

    sem_close(semInterface);
    sem_close(semSimulator);
    sem_unlink(SEM_INTERFACE_FILE_NAME);
    sem_unlink(SEM_SIMULATOR_FILE_NAME);
    return 0;
}

void StartSimulation(void)
{
    SharedMemoryInit(shm);
    system("/home/guillaume/centrale/P1RV-fusee/e.sh");
    if (sem_post(semSimulator) != 0)
    {
        cout << "semSimulator V error" << endl;
    }
    simulationInProcess = true;

    return;
}

osg::Camera *createHUD();

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

    ref_ptr<PositionAttitudeTransform> textPAT(new PositionAttitudeTransform());
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    viewer.getCamera()->getViewMatrix().getLookAt(eye, center, up);
    textPAT->setPosition(center);
    root->addChild(textPAT.get());

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
    viewer.addEventHandler(new KeyboardHandler);

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
#ifdef BACKGROUND_LOC
    cout << "backgrould.stl location : " << BACKGROUND_LOC << endl;
    ref_ptr<Node> terrainnode(osgDB::readNodeFile(BACKGROUND_LOC));
#else
    ref_ptr<Node> terrainnode(osgDB::readNodeFile("ressources/background.stl"));
    cout << "background.stl location not defined" << endl;
    exit(0);
#endif

    //Set transformation node parameters
    terrainScaleMAT->addChild(terrainnode.get());
    terrainScaleMAT->setMatrix(terrainScaleMatrix);

    root->addChild(terrainnode.get());

    root->setUpdateCallback(new MyUpdateCallback());

    // osg::Camera *hudCamera = createHUD();
    // viewer.setUpViewAcrossAllScreens();
    // osgViewer::Viewer::Windows windows;
    // viewer.getWindows(windows);
    // hudCamera->setGraphicsContext(windows[0]);
    // hudCamera->setViewport(0, 0, windows[0]->getTraits()->width, windows[0]->getTraits()->height);

    // viewer.addSlave(hudCamera, false);

    /* START VIEWER */
    //The viewer.run() method starts the threads and the traversals.
    return (viewer.run());
}

osg::Camera *createHUD()
{
    // create a camera to set up the projection and model view matrices, and the subgraph to draw in the HUD
    osg::Camera *camera = new osg::Camera;

    // set the projection matrix
    camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 1280, 0, 1024));

    // set the view matrix
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrix(osg::Matrix::identity());

    // only clear the depth buffer
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);

    // draw subgraph after main camera view.
    camera->setRenderOrder(osg::Camera::POST_RENDER);

    // we don't want the camera to grab event focus from the viewers main camera(s).
    camera->setAllowEventFocus(false);

    // add to this camera a subgraph to render
    {
        osg::Geode *geode = new osg::Geode();

        // turn lighting off for the text and disable depth test to ensure it's always ontop.
        osg::StateSet *stateset = geode->getOrCreateStateSet();
        stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

        osg::Vec3 position(150.0f, 800.0f, 0.0f);
        osg::Vec3 delta(0.0f, -120.0f, 0.0f);

        {
            osgText::Text *text = new osgText::Text;
            geode->addDrawable(text);

            // text->setFont(timesFont);
            text->setPosition(position);
            text->setText("0");
            text->addUpdateCallback(new MyTextCallback);

            position += delta;
        }

        camera->addChild(geode);
    }

    return camera;
}