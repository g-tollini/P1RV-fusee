cd /home/guillaume/centrale/P1RV-fusee/simulator/arduino
echo "Building the simulator"
make
cd ../../
./build/simulator/P1RV-fusee-simulator -gui &
echo "Simulator started"
