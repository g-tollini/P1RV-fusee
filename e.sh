cd ../../simulator/arduino/fusee
cp fusee.ino P1RV-fusee-arduino-code.cpp
cd ../../
cmake .
./../build/simulator/P1RV-fusee-simulator -gui &
