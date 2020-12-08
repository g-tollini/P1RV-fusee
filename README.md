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

To make this project work I added to my PATH variable the absolute path to my osg static library binary files (for me it is ~/developpement/OpenSceneGraph/build/lib)
