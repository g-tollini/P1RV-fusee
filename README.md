# P1RV-fusee
Simulateur de fusée en 3D

# IMPORTANT
Je vais rendre tout propre le plus vite possible mais ce n'étais pas ma priorité jusqu'ici... notamment sur la procédure d'installation/de configuration.

# Installation, compilation
Logiciels requis :
make, g++

Bibliothèques requises :
OpenGL, freeglut

Fichiers à modifier :
- Le Makefile :
	* Les chemins

- interface.cpp
	* Le chemin vers e.sh codé en dur ligne 19

To make this project work I added to my PATH variable the absolute path to my osg shared library binary files (for me it is ~/developpement/OpenSceneGraph/build/lib). Also, since osg needs the plugin to open .stl objects, we need to give the specific shared object for that. I used the command : 

$sudo ln -s ~/developpement/OpenSceneGraph/build/lib/osgPlugins-3.7.0/osgdb_stld.so /usr/lib/x86_64-linux-gnu/libosgdb_stld.so

and linked the interface executable to the library osgdb_stld (see CMakeLists.txt in the subrepo /interface)

Ne pas oublier de créer si nécessaire les variables d'environnement : 
GTEST_ROOT (dans mon cas : $export GTEST_ROOT=~/developpement/googletest/googletest)
