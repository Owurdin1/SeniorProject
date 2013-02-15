/*
 * UDP endpoint
 *
 * A simple UDP endpoint example using the WiShield 1.0
 *
 * NOTE: You must edit the uip-conf.h file in the WiShield library directory
 * as follows (at about line 110):
 *         #define UIP_CONF_UDP             1
 *
 * Change this back to 0 for compiling the other examples.
 *
 */

#include <WiShield.h>
#include "MultiAnalogRangeFinder.h"
#include "user_config.h"

//#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2

// Wireless configuration parameters ----------------------------------------
unsigned char local_ip[] = {192,168,1,4};	// IP address of WiShield
unsigned char gateway_ip[] = {192,168,1,1};	// router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0};	// subnet mask for the local network
const prog_char ssid[] PROGMEM = {DRONE_NAME};		// max 32 bytes

unsigned char security_type = 0;	// 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {"12345678"};	// max 64 characters

// WEP 128-bit keys
// sample HEX keys
prog_uchar wep_keys[] PROGMEM = {	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,	// Key 0
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00,	// Key 1
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00,	// Key 2
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00	// Key 3
								};


// setup the wireless mode
// infrastructure - connect to AP
// adhoc - connect to another WiFi device
unsigned char wireless_mode = WIRELESS_MODE_ADHOC;

unsigned char ssid_len;
unsigned char security_passphrase_len;
//---------------------------------------------------------------------------

// Variables for drone states
char data[100];
char yj_mac[17];
int seq = 1;	// First sequence command counter
  //int aeq = 1;	// Second sequence command counter
unsigned long time;
unsigned long flightTime;
unsigned long totalTime;
int droneState = 0;	// Drone state variable
int rangeStop = 7;	// Range value to stop drone
int leftSensor[3];	// Array containing 3 left sensor reads
int rightSensor[3];	// Array containing 3 right sensor reads
int verticalSensor[3];	// Array containing 3 vertical sensor reads
int leftRange = 0;	// largest value of leftSensor[3]
int rightRange = 0;	// largest value of rightSensor[3]
int verticalRange = 0;	// largest value of verticalSensor[3]
int forwardSpeed = -0.2; // forward speed state


// flightRange Test demo variables
int verticalTrigger = 0;
int forwardTrigger = 0;
int rangeViolation = 0;

// Variables for range finders
MultiAnalogRange rangeClass;
int rangeTimer = 0;

// Variables for Keep Connection
unsigned long keepConn;

void initialState()
{
        //Serial.println("initialState keep on ground until given command to fly");
	//Serial.println("sending flat trim command to drone to attempt to prevent flying into walls");
        time = millis();
	rangeTimer = time;
	flightTime = time;
	keepConn = time;
	totalTime = time;
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r",1,1);
	sprintf(data, "AT*FTRIM=%d\r", seq++);

	delay(200);
//aeq++;
}

void landingState()
{
        //Serial.println("landingState send land command");
        droneState = 0;
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r",seq++,seq++);  //,aeq++); //seq++);
}

void takeOffState()
{
        //Serial.println("takeOffState, tell drone to take off");
        droneState = 1;
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290718208\r",seq++,seq++);  //,aeq++); //seq++);
}

void hoverState()
{
        //Serial.println("hoverState, hovring with no directions");
        droneState = 2;
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\r",seq++);
//aeq++;
}

void emergencyLandState()
{
        //Serial.println("landingState send land command");
        droneState = 3;
	//sprintf(data,"AT*REF=%d,290717696\r", seq++);
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r",seq++, seq++);  //,aeq++); //seq++);
}

void forwardState()
{
	//Serial.println("forwardState, moving forward");
	droneState = 4;

	// Entered a 1 for the second arg in the command to enable reading other values for control
	sprintf(data,"AT*PCMD=%d,1,0,%d,0,0\r",seq++,forwardSpeed);
        //sprintf(data,"AT*PCMD=%d,1,0,forwardSpeed,0,0\r",seq++);
        //sprintf(data,"AT*PCMD=%d,0,0,forwardSpeed,0,0\r",seq++);
}

void keepConnection()
{
	//if (millis() - keepConn > 300)
	// Check for 30 milliseconds rather than 300 for keep connection,
	// should enable smoother flight
	if (millis() - keepConn > 30)
	{
		  keepConn = millis();

		  Serial.print("Keeping drone connection alive, droneState = ");
		  Serial.println(droneState);

		  switch(droneState)
		  {
			case 0:
				landingState();
				break;
			case 1:
				takeOffState();
				break;
			case 2:
				hoverState();
				break;
			case 3:
				emergencyLandState();
				break;
			case 4:
				forwardState();
				break;
		  }
	}
}

void sensorCompare()
{
	//Serial.println("sensorCompare function");

	int i;
	int leftRead = 0;
	int rightRead = 0;
	int verticalRead = 0;

	for (i = 0; i < 3; i++)
	{
		if (leftRead < leftSensor[i])
		{
			leftRead = leftSensor[i];
		}
		if (rightRead < rightSensor[i])
		{
			rightRead = rightSensor[i];
		}
		if (verticalRead < verticalSensor[i])
		{
			verticalRead = verticalSensor[i];
		}
	}

	leftRange = leftRead;
	rightRange = rightRead;
	verticalRange = verticalRead;

	// DebugPrintFunction
	debugPrint();
}

void rangeCheck()
{
	//Serial.println("rangeCheck function");
	if (rangeTimer < 3)
	{
		leftSensor[rangeTimer] = rangeClass.sensors[left]; //0];
		rightSensor[rangeTimer] = rangeClass.sensors[right]; //1];
		verticalSensor[rangeTimer] = rangeClass.sensors[vertical]; //2];
		rangeTimer++;
	}

	//Serial.println(rangeTimer);

	if (rangeTimer == 3)
	{
		sensorCompare();
	}
}

void forwardSensorTest()
{
        if (time == 0)
        {
                initialState();
        }

	if (millis() - time > 100)
	{
		 // Range finders funciton
		rangeClass.GetRanges();
		rangeCheck();
		time = millis();

		//if (rangeClass.sensors[2] < 10)
//		if ((leftRange < rangeStop) && (leftRange != 0) || (rightRange < rangeStop) && (rightRange != 0))
		if ((leftRange > rangeStop) && (leftRange != 0) || (rightRange > rangeStop) && (rightRange != 0))
		{
			if (rangeTimer >= 3)
			{
				rangeTimer = 0;

				switch(droneState)
				{
					case 0:
						if (millis() - flightTime > 5000)
						{
							takeOffState();
							flightTime = millis();
						}
						else
						{
							keepConnection();
						}

						break;
					case 1:
						if (millis() - flightTime > 5000)
						{
							hoverState();
							flightTime = millis();
						}
						else
						{
							keepConnection();
						}

						break;
					case 2:
						forwardState();

						flightTime = millis();

						Serial.println("Flying forward now");

						break;

					case 4:
						keepConnection();
		/*
						if (millis() - flightTime > 3000)
						{
							landingState();
							Serial.println("Forward flight for 3 seconds complete");

							while (1)
							{
								keepConnection();
							}
						}
						else
						{
							keepConnection();
						}
		*/
						break;

					default:
						keepConnection();
						break;			
				}
			}
/*
			Serial.println("RangeViolation!!!! Land");

			rangeViolation++;
			Serial.println("Switch to hover!");
			hoverState();
			time = millis();
			while (millis() - time < 2000)
			{
				keepConnection();
				Serial.print("DroneState; ");
				Serial.print(droneState);
				Serial.println(";");
			}

			if (rangeViolation == 3)
			{
				hoverState();

				while (millis() - time < 2000)
				{
					keepConnection();
				}

				landingState();

				while (1)
				{
					keepConnection();
				}
			}
*/
		}
	}



	if (millis() - totalTime > 60000)
	{

		Serial.println("Flight done LAND and STAY DOWN!");
		flightTime = millis();
		hoverState();

		while (millis() - flightTime < 2000)
		{
			keepConnection();
		}

		landingState();

		while(1)
		{
			keepConnection();
		}
	}

	debugPrint();
}

void debugPrint()
{

	// Range finder print Statments
	Serial.print("RangeTimer; ");
	Serial.print(rangeTimer);
	Serial.print("; DroneState; ");
	Serial.print(droneState);
	Serial.print("; Left Sensor; ");
	Serial.print(leftRange); //rangeClass.sensors[0]);
	Serial.print("; Right Sensor; ");
	Serial.print(rightRange); //rangeClass.sensors[1]);
	Serial.print("; Vertical Sensor; ");
	Serial.println(verticalRange); //rangeClass.sensors[2]);
}

void setup()
{
        Serial.begin(9600);
        Serial.println("");
        Serial.println("");
        Serial.println("Initialize Connection");
        
	WiFi.init();
        Serial.println("Initialized");
        
        unsigned char *my_mac = zg_get_mac();
        sprintf(yj_mac,"%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",my_mac[0],my_mac[1],my_mac[2],my_mac[3],my_mac[4],my_mac[5]);
        
        Serial.print("MAC Address: ");
        Serial.println(yj_mac);
}

void loop()
{
	WiFi.run();
	
        // Function to begin forward flight test
        forwardSensorTest();
}
