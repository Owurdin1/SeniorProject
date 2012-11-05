// From Beginning Arduino, Michael McRoberts
// Project 38
#define sensorPin1 9
#define sensorPin2 10

long pwmRange1
, inch1, cm1, inch2, cm2, pwmRange2;

void setup() 
{
	// Start serial communications
	Serial.begin(115200);
	pinMode(sensorPin1, INPUT);
        pinMode(sensorPin2, INPUT);
}


void loop() 
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

