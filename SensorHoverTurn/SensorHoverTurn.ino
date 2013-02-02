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
int seq = 1;
unsigned long time;
unsigned long flightTime;
int droneState = 0;
int rangeStop = 14;
int leftSensor[3];
int rightSensor[3];
int verticalSensor[3];
int leftRange = 0;
int rightRange = 0;
int verticalRange = 0;
int turnCounter = 0;
int turnSpeed = -1;

// flightRange Test demo variables
int verticalTrigger = 0;
int forwardTrigger = 0;

// Hover Demo variable
int hoverCounter = 0;

// Variables for range finders
MultiAnalogRange rangeClass;
int rangeTimer = 0;

// Variables for Keep Connection
unsigned long keepConn;

void hoverState()
{
        //Serial.println("hoverState, hovring with no directions");
        droneState = 2;
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\r",seq++); 
}

void takeOffState()
{
        //Serial.println("takeOffState, tell drone to take off");
        droneState = 1;
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290718208\r",seq++,seq++);
}

void landingState()
{
        //Serial.println("landingState send land command");
        droneState = 0;
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r",seq++,seq++);
}

void emergencyLandState()
{
        //Serial.println("landingState send land command");
        droneState = 7;
	//sprintf(data,"AT*REF=%d,290717696\r", seq++);
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r",seq++,seq++);
}

void initialState()
{
         //Serial.println("initialState keep on ground until given command to fly");
	 //Serial.println("sending flat trim command to drone to attempt to prevent flying into walls");
         time = millis();
	 rangeTimer = time;
	 flightTime = time;
	 keepConn = time;
         sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r",1,1);
	 sprintf(data, "AT*FTRIM=%d\r", seq++);
}

void turnState()
{
	// This is for testing purposes so not using full
	// turnState function. This will tell to drone to turn
	// for each pass through with no interactiion with the
	// turn counter. This piece will be "dumb" so allow for
	// sensor/state behavior for this project
	Serial.println("turnState, Turning drone to the left");
	droneState = 3;

        sprintf(data,"AT*PCMD=%d,0,0,0,0,turnSpeed\r",seq++);
	/*
	//Serial.println("turnState, turning and incrementing turn counter");
	droneState = 3;

	if (turnCounter < 5)
	{
		turnCounter++;
	        sprintf(data,"AT*PCMD=%d,0,0,0,0,turnSpeed\r",seq++);
	}
	else
	{
		turnCounter = 0;

		//if (rangeClass.sensors[2] > 50)
		if (verticalRange > 50)
		{
			time = millis();
			verticalUpState();

			while (millis() - time < verticalUpTime)
			{
				keepConnection();
			}
		}
		else
		{
			time = millis();
			verticalDownState();

			while (millis() - time < verticalDownTime)
			{
				keepConnection();
			}
		}
	}
	*/
}

void keepConnection()
{
	if (millis() - keepConn > 300)
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
				turnState();
				break;
			case 7:
				emergencyLandState();
				break;
		  }
	}
}

void sensorCompare()
{
	//Serial.println("sensorCompare function");

	int i;
	int left = 0;
	int right = 0;
	int vertical = 0;

	for (i = 0; i < 3; i++)
	{
		if (left < leftSensor[i])
		{
			left = leftSensor[i];
		}
		if (right < rightSensor[i])
		{
			right = rightSensor[i];
		}
		if (vertical < verticalSensor[i])
		{
			vertical = verticalSensor[i];
		}
	}

	leftRange = left;
	rightRange = right;
	verticalRange = vertical;

	// DebugPrintFunction
	debugPrint();
}

void rangeCheck()
{
	//Serial.println("rangeCheck function");
	if (rangeTimer < 3)
	{
		leftSensor[rangeTimer] = rangeClass.sensors[0];
		rightSensor[rangeTimer] = rangeClass.sensors[1];
		verticalSensor[rangeTimer] = rangeClass.sensors[2];
		rangeTimer++;
	}

	//Serial.println(rangeTimer);

	if (rangeTimer == 3)
	{
		sensorCompare();
	}
}

//==============================

void hoverPrint()
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
		if (verticalRange < 10 && verticalRange != 0)
		{
			Serial.println("STOOOOPPPPPP!!!!");
			emergencyLandState();
			time = millis();
			while (millis() - time < 3000)
			{
				keepConnection();
				Serial.print("DroneState; ");
				Serial.print(droneState);
				Serial.println(";");
			}
		}
	}

	/*
	// Range finder print Statments
	Serial.print("Left Sensor; ");
	Serial.print(rangeClass.sensors[0]);
	Serial.print("; Right Sensor; ");
	Serial.print(rangeClass.sensors[1]);
	Serial.print("; Vertical Sensor; ");
	Serial.println(rangeClass.sensors[2]);
	*/

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
					hoverCounter++;
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
					hoverCounter++;
				}
				else
				{
					keepConnection();
				}

				break;

			default:
				keepConnection();
				break;
			
		}
	}
/*
	if (rangeClass.sensors[2] < 10)
	{
		landingState();
		Serial.println("STOOOOPPPPP!");
		delay(30000);
	}
*/
}

void sensorTurn()
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
		if (verticalRange < 10 && verticalRange != 0)
		{
			Serial.println("Turn Now!");
			turnState();
			time = millis();
			while (millis() - time < 3000)
			{
				keepConnection();
				Serial.print("DroneState; ");
				Serial.print(droneState);
				Serial.println(";");
			}
		}
	}

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
					hoverCounter++;
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
					hoverCounter++;
				}
				else
				{
					keepConnection();
				}

				break;

			default:
				keepConnection();
				break;
			
		}
	}

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

	// Function to tell drone to hover and when
	// range barrier is broken to turn the drone in place
	sensorTurn();
	
	// Function to run flight test using vertical range
	// finder to stop sequence
	//flightSequence();

	// Function to hover and print sensor readings
	//hoverPrint();

	// Function to run initial test hover sequence
        //hoverSequence();
}
