BUILDPATH = build/
SIMPATH = simulator/
SIMNPATH = simulator/simulation/
GUIPATH = interface/
ARDUINOPATH = simulator/arduino/fusee/
MYARDUINOPATH = simulator/arduino/my_arduino_layer/
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

$(BUILDPATH)simulator.out : $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o $(BUILDPATH)arduinolib.so threading.hpp $(BUILDPATH)arduinocode.so $(BUILDPATH)threading.o
	g++ -o $(BUILDPATH)simulator.out $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o $(BUILDPATH)arduinolib.so $(BUILDPATH)arduinocode.so $(BUILDPATH)threading.o -lpthread -lrt
	#g++ -o $(BUILDPATH)simulator.out $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o $(BUILDPATH)arduinolib.so $(BUILDPATH)arduinocode.so $(BUILDPATH)threading.o $(APPLEPTHREADPATH) -lpthread

$(BUILDPATH)simulator.o : $(SIMPATH)simulator.cpp
	g++ -c $(SIMPATH)simulator.cpp -o $(BUILDPATH)simulator.o -fPIC
	
$(BUILDPATH)simulation.o : $(SIMNPATH)simulation.cpp $(SIMNPATH)simulation.hpp threading.hpp
	g++ -c  $(SIMNPATH)simulation.cpp -o $(BUILDPATH)simulation.o -fPIC


$(BUILDPATH)arduinomain.o : $(MYARDUINOPATH)arduinomain.cpp $(MYARDUINOPATH)myarduino.hpp $(BUILDPATH)serial.o
	g++ -c $(MYARDUINOPATH)arduinomain.cpp -o $(BUILDPATH)arduinomain.o $(BUILDPATH)serial.o -fPIC

$(BUILDPATH)arduinocode.so : $(BUILDPATH)fusee.o $(BUILDPATH)serial.o $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o $(BUILDPATH)arduinomain.o 
	g++ -o $@ -shared $(BUILDPATH)fusee.o $(BUILDPATH)serial.o $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o $(BUILDPATH)arduinomain.o  -fPIC

$(BUILDPATH)arduinolib.so : $(BUILDPATH)arduinomain.o $(BUILDPATH)fusee.o $(BUILDPATH)serial.o $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o 
	g++ -o $@ -shared $(BUILDPATH)arduinomain.o $(BUILDPATH)fusee.o $(BUILDPATH)serial.o $(BUILDPATH)simulator.o $(BUILDPATH)simulation.o -fPIC


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
