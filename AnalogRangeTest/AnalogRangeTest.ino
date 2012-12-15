// Sketch to test conversion rates for analog read
// Date: 20121214

#define rangePin A0
#define voltsPerInch 9.57 // this is millivolts
#define mVoltsPerUnit 4.9 // this is millivolts 1 unit = 4.9 mV

// do inches conversion and return range
long GetRangeInches(long readValue)
{
      long mVTotal = 0;
      long range = 0;
      
      // Conversion equations to get actual inches
      mVTotal = mVoltsPerUnit * readValue;
      range = mVTotal / voltsPerInch;
      
      return range;
}

// test reading from range finder
long GetRangeReadValue()
{
      long readValue = 0;
      
      readValue = analogRead(rangePin);
      
      return readValue;
}

// test equation with set values
void ConversionTest()
{
      long mVTotal = 0;
      long rangeInches = 0;
      long testReadValue = 60; // 15; // 7;
      
      Serial.print("mVTotal is ");
      Serial.print(mVoltsPerUnit);
      Serial.print(" * ");
      Serial.print(testReadValue);
      Serial.println(".");
      
      // Conversion Equation
      mVTotal = mVoltsPerUnit * testReadValue;
      
      Serial.print("mVTotal = ");
      Serial.println(mVTotal);
      
      Serial.print("rangeInches = ");
      Serial.print(mVTotal);
      Serial.print(" / ");
      Serial.print(voltsPerInch);
      Serial.print(" = ");
      
      // inches equation
      rangeInches = mVTotal / voltsPerInch;
      
      Serial.println(rangeInches);
}


void setup()
{
     Serial.begin(9600);
}

void loop()
{
/*  
     // Initial Values Test block
     //================================
     Serial.print("rangePin = ");
     Serial.println(rangePin);
     Serial.print("voltsPerInch = ");
     Serial.println(voltsPerInch);
     Serial.print("mVoltsPerUnit = ");
     Serial.println(mVoltsPerUnit);
     //================================
*/
     // Get value from range finder
     //================================
     long rangeReadValue = 0;
     rangeReadValue = GetRangeReadValue();
     Serial.print("rangeReadValue = ");
     Serial.println(rangeReadValue);
     //================================
     
     // Run conversion on rangeReadValue
     long rangeInchesFound = 0;
     rangeInchesFound = GetRangeInches(rangeReadValue);
     Serial.print("Range = ");
     Serial.println(rangeInchesFound);

/*    
     // Test function with all coded test values
     //================================
     ConversionTest();
     //================================
*/
}
