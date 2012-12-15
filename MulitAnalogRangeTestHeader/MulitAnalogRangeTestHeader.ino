#include "MultiAnalogRangeFinder.h"

MultiAnalogRange analogRange;



void setup()
{
     Serial.begin(9600); 
}

void loop()
{
     analogRange.GetRanges();
     
     Serial.print("Left = ");
     Serial.print(analogRange.sensors[0]);
     Serial.print(" Right = ");
     Serial.print(analogRange.sensors[1]);
     Serial.print(" Vertical = ");
     Serial.print(analogRange.sensors[2]);
     Serial.println(".");
}
