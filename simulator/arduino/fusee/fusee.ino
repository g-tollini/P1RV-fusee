
#ifdef SIM
#include "../myarduino.hpp"
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
  double h = heightSensor.MeasureHeight();
  Serial.print("Height : ");
  Serial.print(h);
  // put your main code here, to run repeatedly:
  if (h < 10.0)
  {
    idealBooster.SetThrust(100.0);
  }
  else
  {
    idealBooster.SetThrust(0.0);
  }
}
