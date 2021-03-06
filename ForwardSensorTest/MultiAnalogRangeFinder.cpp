#include "MultiAnalogRangeFinder.h"

// Converts the analog read values to inches and stores
// those values in the public container for later use
void MultiAnalogRange::ConvertRangeValues()
{
      long totalLeft, totalRight, totalVertical;
      long rangeLeft, rangeRight, rangeVertical;
      
      // Equation to convert analog units to MilliVolts for each sensor
      totalLeft = voltsPerUnit * sensors[left];
      totalRight = voltsPerUnit * sensors[right];
      totalVertical = voltsPerUnit * sensors[vertical];
      
      /*
      totalLeft = voltsPerUnit * sensors[0];
      totalRight = voltsPerUnit * sensors[1];
      totalVertical = voltsPerUnit * sensors[2];
      */
      
      // Equation to convert MilliVolts to inches for each sensor
      sensors[left] = totalLeft / voltsPerInch;
      sensors[right] = totalRight / voltsPerInch;
      sensors[vertical] = totalVertical / voltsPerInch;
      
      /*
      sensors[0] = totalLeft / voltsPerInch;
      sensors[1] = totalRight / voltsPerInch;
      sensors[2] = totalVertical / voltsPerInch;
      */
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
      
      /*
      // Test print calls
      Serial.print("RangeFinder Class SENSORS: ");
      Serial.print("Left sensor: ");
      Serial.print(sensors[0]);
      Serial.print(" Right sensor: ");
      Serial.print(sensors[1]);
      Serial.print(" Vertical sensor: ");
      Serial.println(sensors[2]);
      */
      
      // Call ConvertRangeValues to convert the analog
      // input values to inches for use in project
      ConvertRangeValues();
      
}
