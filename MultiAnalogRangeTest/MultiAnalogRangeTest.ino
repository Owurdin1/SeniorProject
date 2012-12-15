// Sketch to test reading from 3 analog range finders
// using the chain read function
// Date: 20121214

// Define analog pins for range finders
#define rangePinLeft A0
#define rangePinRight A1
#define rangePinVertical A2

// Define pins for control of sensors
#define bwPin 6 // set BW Pin to high for chained sensors
#define rxPin 7 // strobe to tell sensors to read values

// Define values for conversions in millivolts
#define voltsPerInch 9.57
#define voltsPerUnit 4.9

// Global Value declarations
long rangeSensorLeft = 0;
long rangeSensorRight = 0;
long rangeSensorVertical = 0;

// Converts the range values to inches
void ConvertRangeValues()
{
      long vTotalLeft, vTotalRight, vTotalVertical;
      long rangeLeft, rangeRight, rangeVertical;
      
      vTotalLeft = voltsPerUnit * rangeSensorLeft;
      vTotalRight = voltsPerUnit * rangeSensorRight;
      vTotalVertical = voltsPerUnit * rangeSensorVertical;
      
      rangeSensorLeft = vTotalLeft / voltsPerInch;
      rangeSensorRight = vTotalRight / voltsPerInch;
      rangeSensorVertical = vTotalVertical / voltsPerInch;
}

// Gets the range values
void GetRanges()
{
     // strobe the rx pin to begin read sequence
     digitalWrite(rxPin, HIGH);
     delay(10);
     digitalWrite(rxPin, LOW);
     
     // reads each sensor and sets values to global vars
     rangeSensorLeft = analogRead(rangePinLeft);
     rangeSensorRight = analogRead(rangePinRight);
     rangeSensorVertical = analogRead(rangePinVertical);
    
     // call function to run conversion on read values
     ConvertRangeValues();
}

void setup()
{
     Serial.begin(9600); 
     pinMode(rxPin, OUTPUT);
     pinMode(bwPin, OUTPUT);
     
     digitalWrite(bwPin, HIGH);
}

void loop()
{
     Serial.println(" ");
     Serial.println(" ");
     Serial.println("Begin multi sensor test");
     
     // Call function to get ranges from sensors
     GetRanges();
     
     Serial.print("rangeLeft = ");
     Serial.print(rangeSensorLeft);
     Serial.print(" rangeRight = ");
     Serial.print(rangeSensorRight);
     Serial.print(" rangeVertical = ");
     Serial.print(rangeSensorVertical);
     Serial.println(".");
}
