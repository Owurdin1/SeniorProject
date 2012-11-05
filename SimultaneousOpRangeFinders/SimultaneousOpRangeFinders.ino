#define sensorPin1 9
#define sensorPin2 10
#define rxPin 7

long pwmRange1, inch1, cm1, inch2, cm2, pwmRange2, timeNow;

/*
 Have to kick start the range finders since they are
 set to run in a chained mode. It has a very specific
 process that must be followed:

	Second, then you will have to "kick start" them, (at least 250mS or more after power is applied to the sensors to give the 

	sensors this time to boot-up).  To do this, pull the RX pin high on the first sensor for at least 20uS.  Then the 
	micro controller will have to return it's pin to a high impedance state so that the next time around the TX output from the
	last sensor will make it's way to the RX of the first sensor.  Then all of the sensors in the chain will run in sequence.
	This "ring of sensors" will cycle around and around, constantly maintaining the validity of their analog values.


	You can then read the latest range reading (i.e. the analog value) at any time.  This is the easiest way to use them. After 		pulling the RX pin low,  you can read the analog pin about 50mS (100mS if this is the first time reading the sensor as it
	calibrates upon the first commanded range cycle after power up, i.e. the sensor must complete a range cycle). In addition, 
	the most recent range reading is always ready to be read on the analog voltage pin, so once you start the chain, and if you 
	are using it in continuous mode,  you can read the values at any time. 
*/
void SensorKickStart()
{
	delay(250);
	digitalWrite(rxPin, HIGH);
	delayMicroseconds(20);
	pinMode(rxPin, INPUT);
	digitalWrite(rxPin, HIGH);
	delayMicroseconds(20);
	pinMode(rxPin, OUTPUT);
	digitalWrite(rxPin, LOW);
	delay(50);
	digitalWrite(rxPin, HIGH);
}

// This is getting and calculating range finder data.
void RangeCheck()
{
	pwmRange1 = pulseIn(sensorPin1, HIGH);
	pwmRange2 = pulseIn(sensorPin2, HIGH);

	// 147uS per inch according to datasheet
	inch1 = pwmRange1 / 147;
	inch2 = pwmRange2 / 147;

	// convert inch to cm
	cm1 = inch1 * 2.54;
	cm2 = inch2 * 2.54;

	Serial.print(inch1);
	Serial.print(" inches Sensor 1 ");
	Serial.print(cm1);
	Serial.println(" cm Sensor 1");

	Serial.print("                                   ");
	Serial.print(inch2);
	Serial.print(" inches Sensor 2 ");
	Serial.print(cm2);
	Serial.println(" cm Sensor 2");
}

void setup() 
{
	// Start serial communications
	Serial.begin(115200);
	pinMode(sensorPin1, INPUT);
        pinMode(sensorPin2, INPUT);
	pinMode(rxPin, OUTPUT);
	SensorKickStart();
}


void loop() 
{
	RangeCheck();
}
