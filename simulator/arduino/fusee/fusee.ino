
#ifdef SIM
#include "../my_arduino_layer/myarduino.hpp"
#endif

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);
  Serial.print("Hello RV !");
}

void loop()
{
  delay(111); // arbitrary delay
  // put your main code here, to run repeatedly:
}


