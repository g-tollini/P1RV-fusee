<h1>P1RV-fusee</h1>
3D rocket simulator  

Simulateur de fusée en 3D, club de robotique NantRobot, Centrale Nantes  

In the coming versions, I will be switching to a WebGL interface for easier deployment.

As soon as this is done I will set up a docker so that anyone can try out this project in a few clicks.

<h2>Install</h2>
How to run the executables :

Go in the folder of your choice and in the console, type :

`$ git clone https://github.com/g-tollini/P1RV-fusee`  
`$ cd P1RV-fusee`  
`$ mkdir build && cd build`    
`$ cmake ../ && make`  
In order to start the graphic user iterface, type :  
`$ ./interface/P1RV-interface`  
You can start simulations without the gui, to do so type :  
`$ ./simulator/P1RV-simulator`  

If something goes wrong, I list below some solutions to problems that might have happened. I hope that you will find the solution that suits your situation.

<h2>Compilation</h2>
Required tools :  
make, g++  

You can install everything you need to build this code with the build-essentials package :  
`$ sudo apt update`  
`$ sudo apt upgrade`  
`$ sudo apt install build-essentials`  

Required libraries :  
+ OpenGL that you should alredy have on your computer  
+ Freeglut `$ sudo apt install freeglut3-dev`  
+ OpenSceneGraph to be found here : https://github.com/openscenegraph/OpenSceneGraph  
+ pthread an rt should be on your computer  

To find any library, you can type `$ ld -l <your_lib_name> --verbose`. Type whatever as a name to see all the folders where the linker is searching.

<h2>If needed, cpoy OSG's .stl loader compiled library file in a folder where the linker is searching</h2>
When installing OpenSceneGraph, the .stl loader is not necessarily placed inside a folder where the linker will search. You will need to do it manually.

The static library file that we need to load .stl files is located in *OpenSceneGraph/build/lib/osgPlugins-3.7.0/* (*OpenSceneGraph/* is where you did your `$ git clone https://github.com/openscenegraph/OpenSceneGraph`). The static library is named **osgdb_stld.so**. 

If you tyupe the command `$ ld -l whatever --verbose`, unless the library whatever actually exists which I suppose it doesn't, you get all the repositories in which your linker is searching when you ask him to link. You can choose which. I choose */usr/lib* so I will place **osgdb_stld.so** there while renaming it : sudo cp *<yours_to_fill>/OpenSceneGraph/build/lib/osgPlugins-3.7.0/***osgdb_stld.so**** */usr/lib/***libosgdb_stld.so****.
You could also create a symbolic link instead :
*<yours_to_fill>/OpenSceneGraph/build/lib/osgPlugins-3.7.0/***osgdb_stld.so**** */usr/lib/***libosgdb_stld.so****

You should be good. You can try typing `$ ld -l osgdb_stld` and `ld` should find it (it can still return an error even if it has found the file).