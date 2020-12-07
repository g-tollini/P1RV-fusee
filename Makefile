BUILDPATH = build/
SIMPATH = simulator/
SIMNPATH = simulator/simulation/
MODELPATH = simulator/simulation/dynamic-model/
GUIPATH = interface/
ARDUINOPATH = simulator/arduino/
ARDUINOCODEPATH = simulator/arduino/fusee/
MYARDUINOLAYERPATH = simulator/arduino/my_arduino_layer/
USRLIBPATH = /usr/lib/x86_64-linux-gnu/

APPLEGLPATH = -framework GLUT -framework OpenGL
APPLEPTHREADPATH = -L/usr/lib/

OS =

all : $(BUILDPATH)interface.out $(BUILDPATH)simulator.out 

sim : $(BUILDPATH)simulator.out

$(BUILDPATH)threading.o : threading.cpp threading.hpp
	g++ -c threading.cpp -o $(BUILDPATH)threading.o -fPIC

# L'interface

$(BUILDPATH)interface.out : $(BUILDPATH)interface.o $(BUILDPATH)threading.o
	g++ -o $(BUILDPATH)interface.out $(BUILDPATH)interface.o $(BUILDPATH)threading.o -lGL -lglut -lpthread -lrt
	#g++ -o $(BUILDPATH)interface.out $(BUILDPATH)interface.o $(APPLEGLPATH)

$(BUILDPATH)interface.o : $(GUIPATH)interface.cpp threading.hpp
	g++ -c $(GUIPATH)interface.cpp -o $(BUILDPATH)interface.o -fPIC

# Le simulateur

$(BUILDPATH)simulator.out : $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o $(BUILDPATH)arduinolib.so threading.hpp $(BUILDPATH)arduinocode.so $(BUILDPATH)threading.o $(BUILDPATH)solver.o $(BUILDPATH)simple-model.o $(BUILDPATH)cardan-model.o $(BUILDPATH)quaternions-model.o
	g++ -o $(BUILDPATH)simulator.out $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o $(BUILDPATH)arduinolib.so $(BUILDPATH)arduinocode.so $(BUILDPATH)threading.o $(BUILDPATH)solver.o $(BUILDPATH)simple-model.o $(BUILDPATH)cardan-model.o $(BUILDPATH)quaternions-model.o -lpthread -lrt
	#g++ -o $(BUILDPATH)simulator.out $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o $(BUILDPATH)arduinolib.so $(BUILDPATH)arduinocode.so $(BUILDPATH)threading.o $(APPLEPTHREADPATH) -lpthread

$(BUILDPATH)simulator.o : $(SIMPATH)simulator.cpp
	g++ -c $(SIMPATH)simulator.cpp -o $(BUILDPATH)simulator.o -fPIC
	
$(BUILDPATH)simulation.o : $(SIMNPATH)simulation.cpp $(SIMNPATH)simulation.hpp threading.hpp $(BUILDPATH)solver.o $(BUILDPATH)simple-model.o $(BUILDPATH)cardan-model.o $(BUILDPATH)quaternions-model.o
	g++ -c  $(SIMNPATH)simulation.cpp -o $(BUILDPATH)simulation.o $(BUILDPATH)solver.o $(BUILDPATH)simple-model.o $(BUILDPATH)cardan-model.o $(BUILDPATH)quaternions-model.o -fPIC

# Modele dynamique

$(BUILDPATH)solver.o : $(MODELPATH)solver.cpp  $(MODELPATH)solver.hpp
	g++ -c $(MODELPATH)solver.cpp -o $(BUILDPATH)solver.o -fPIC

$(BUILDPATH)simple-model.o : $(MODELPATH)simple-model.cpp  $(MODELPATH)simple-model.hpp $(BUILDPATH)dynamic-model.o 
	g++ -c $(MODELPATH)simple-model.cpp -o $(BUILDPATH)simple-model.o $(BUILDPATH)dynamic-model.o -fPIC

$(BUILDPATH)cardan-model.o : $(MODELPATH)cardan-model.cpp  $(MODELPATH)cardan-model.hpp $(BUILDPATH)dynamic-model.o
	g++ -c $(MODELPATH)cardan-model.cpp -o $(BUILDPATH)cardan-model.o $(BUILDPATH)dynamic-model.o -fPIC

$(BUILDPATH)quaternions-model.o : $(MODELPATH)quaternions-model.cpp  $(MODELPATH)quaternions-model.hpp $(BUILDPATH)dynamic-model.o 
	g++ -c $(MODELPATH)quaternions-model.cpp -o $(BUILDPATH)quaternions-model.o $(BUILDPATH)dynamic-model.o -fPIC

$(BUILDPATH)dynamic-model.o : $(MODELPATH)dynamic-model.cpp  $(MODELPATH)dynamic-model.hpp
	g++ -c $(MODELPATH)dynamic-model.cpp -o $(BUILDPATH)dynamic-model.o -fPIC


# Arduino

$(BUILDPATH)arduinomain.o : $(ARDUINOPATH)arduinomain.cpp $(ARDUINOPATH)myarduino.hpp $(BUILDPATH)serial.o
	g++ -c $(ARDUINOPATH)arduinomain.cpp -o $(BUILDPATH)arduinomain.o $(BUILDPATH)serial.o -fPIC

$(BUILDPATH)arduinocode.so : $(BUILDPATH)fusee.o $(BUILDPATH)serial.o $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o $(BUILDPATH)arduinomain.o 
	g++ -o $@ -shared $(BUILDPATH)fusee.o $(BUILDPATH)serial.o $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o $(BUILDPATH)arduinomain.o  -fPIC

$(BUILDPATH)arduinolib.so : $(BUILDPATH)arduinomain.o $(BUILDPATH)fusee.o $(BUILDPATH)serial.o $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o 
	g++ -o $@ -shared $(BUILDPATH)arduinomain.o $(BUILDPATH)fusee.o $(BUILDPATH)serial.o $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o -fPIC

$(BUILDPATH)fusee.o : $(ARDUINOCODEPATH)fusee.ino $(ARDUINOPATH)myarduino.hpp
	cp $(ARDUINOCODEPATH)fusee.ino $(ARDUINOCODEPATH)fusee.cpp
	g++ -c $(ARDUINOCODEPATH)fusee.cpp -D SIM -o $(BUILDPATH)fusee.o -fPIC
	rm $(ARDUINOCODEPATH)fusee.cpp

$(BUILDPATH)serial.o : $(MYARDUINOLAYERPATH)serial/myserial.cpp $(MYARDUINOLAYERPATH)serial/myserial.hpp
	g++ -c $(MYARDUINOLAYERPATH)serial/myserial.cpp -o $(BUILDPATH)serial.o -fPIC

clean :
	rm build/*.o
	rm build/*.so
	rm build/*.out
