BUILDPATH = build/
SIMPATH = simulator/
SIMNPATH = simulator/simulation/
GUIPATH = interface/
ARDUINOPATH = simulator/arduino/fusee/
MYARDUINOPATH = simulator/arduino/my_arduino_layer/
USRLIBPATH = /usr/lib/x86_64-linux-gnu/

all : $(BUILDPATH)interface.out $(BUILDPATH)simulator.out

sim : $(BUILDPATH)arduinolib.so

# L'interface

$(BUILDPATH)interface.out : $(BUILDPATH)interface.o
	g++ -o $(BUILDPATH)interface.out $(BUILDPATH)interface.o $(USRLIBPATH)libGL.so $(USRLIBPATH)libglut.so

$(BUILDPATH)interface.o : $(GUIPATH)interface.cpp
	g++ -c $(GUIPATH)interface.cpp -o $(BUILDPATH)interface.o

# Le simulateur

$(BUILDPATH)simulator.out : $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o $(BUILDPATH)arduinolib.so $(SIMPATH)threading.hpp $(BUILDPATH)arduinocode.so
	g++ -o $(BUILDPATH)simulator.out $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o $(BUILDPATH)arduinolib.so $(BUILDPATH)arduinocode.so $(USRLIBPATH)libpthread.so

$(BUILDPATH)simulator.o : $(SIMPATH)simulator.cpp
	g++ -c $(SIMPATH)simulator.cpp -o $(BUILDPATH)simulator.o
	
$(BUILDPATH)simulation.o : $(SIMNPATH)simulation.cpp $(SIMNPATH)simulation.hpp
	g++ -c  $(SIMNPATH)simulation.cpp -o $(BUILDPATH)simulation.o

$(BUILDPATH)arduinolib.so : $(BUILDPATH)arduinomain.o $(BUILDPATH)fusee.o $(BUILDPATH)serial.o
	g++ -o $@ -shared $(BUILDPATH)arduinomain.o $(BUILDPATH)fusee.o $(BUILDPATH)serial.o -fPIC

$(BUILDPATH)arduinomain.o : $(MYARDUINOPATH)arduinomain.cpp $(MYARDUINOPATH)myarduino.hpp
	g++ -c $(MYARDUINOPATH)arduinomain.cpp -o $(BUILDPATH)arduinomain.o -fPIC

$(BUILDPATH)arduinocode.so : $(BUILDPATH)fusee.o $(BUILDPATH)serial.o
	g++ -o $@ -shared $(BUILDPATH)fusee.o $(BUILDPATH)serial.o -fPIC


$(BUILDPATH)fusee.o : $(ARDUINOPATH)fusee.ino $(MYARDUINOPATH)myarduino.hpp
	cp $(ARDUINOPATH)fusee.ino $(ARDUINOPATH)fusee.cpp
	g++ -c $(ARDUINOPATH)fusee.cpp -D SIM -o $(BUILDPATH)fusee.o -fPIC
	rm $(ARDUINOPATH)fusee.cpp

$(BUILDPATH)serial.o : $(MYARDUINOPATH)serial/myserial.cpp $(MYARDUINOPATH)serial/myserial.hpp
	g++ -c $(MYARDUINOPATH)serial/myserial.cpp -o $(BUILDPATH)serial.o -fPIC

clean :
	rm build/*.o
	rm build/*.so
	rm build/*.out
