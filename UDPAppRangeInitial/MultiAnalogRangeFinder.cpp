#include "MultiAnalogRangeFinder.h"

// Converts the analog read values to inches and stores
// those values in the public container for later use
void MultiAnalogRange::ConvertRangeValues()
{
      long totalLeft, totalRight, totalVertical;
      long rangeLeft, rangeRight, rangeVertical;
      
      // Equation to convert analog units to MilliVolts for each sensor
      totalLeft = voltsPerUnit * sensors[0];
      totalRight = voltsPerUnit * sensors[1];
      totalVertical = voltsPerUnit * sensors[2];
      
      // Equation to convert MilliVolts to inches for each sensor
      sensors[0] = totalLeft / voltsPerInch;
      sensors[1] = totalRight / voltsPerInch;
      sensors[2] = totalVertical / voltsPerInch;
}

// Reads the analog pins to find the values of the range finders
void MultiAnalogRange::GetRanges()
{
      // strobe the rx pin to begin read sequence
      digitalWrite(rxPin, HIGH);
      digitalWrite(rxPin, LOW);
      
      // reads each sensor and sets values to global vars
      sensors[0] = analogRead(rangePinLeft);
      sensors[1] = analogRead(rangePinRight);
      sensors[2] = analogRead(rangePinVertical);
      
      // Call ConvertRangeValues to convert the analog
      // input values to inches for use in project
      ConvertRangeValues();
}
