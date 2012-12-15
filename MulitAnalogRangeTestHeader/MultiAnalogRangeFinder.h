// Header file for MultiAnalogRangeFinderTest
// using chain read function
// Date: 20121214

#ifndef MULTI_ANALOG_RANGE_FINDER
#define MULTI_ANALOG_RANGE_FINDER

// Arduino libraries, needed to use arduino hardware functions
#include <Arduino.h>

// Define analog pins for range finders
#define rangePinLeft A0
#define rangePinRight A1
#define rangePinVertical A2

// Define pins for control of sensors
#define bwPin 6
#define rxPin 7

// Define values for conversions in millivolts
#define voltsPerInch 9.57
#define voltsPerUnit 4.9

// Class for reading the range finders and setting a
// public container for easy access
class MultiAnalogRange
{
  private:
      // Private functions
      void ConvertRangeValues();
      
  public:
      // Public Functions
      void GetRanges();
      
      // Global Private Variables
      // public container for easy access to find range values
      long sensors[3];  // sensor values, left = 0, right = 1, vertical = 2  
};
#endif
