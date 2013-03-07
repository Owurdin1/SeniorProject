/*
 * #DEFINE for all variables
 *
 *
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
#define RANGE_TIMER_MAX		3	/* rangeTimer maximum value is 3 */
#define MAX_SENSORS		3	/* maximum number of sensors on drone */
#define MILLISECOND_30		30	/* 30 millisecond variable */
#define MILLISECOND_100		100	/* 100 millisecond variable */
#define MILLISECOND_200		200	/* 200 millisecond variable */
#define MILLISECOND_300		300	/* 300 millisecond variable */
#define MILLISECOND_1000	1000	/* 1000 millisecond variable */
#define MILLISECOND_3000	3000	/* 3000 millisecond variable */
#define BAUD_RATE		9600	/* set baud rate value to 9600 */
#define MAX_TURNS		4	/* maximum number of turns */

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

//===========================================================================
int testing = 1; 		// Testing variable will not init WiFi if this is on
//===========================================================================

// Variables for drone states
char data[100];			// UDP Stream variable (extern char data[] in udpapp.c)
char yj_mac[17];		// RedBack mac address variable

// Sequence counters for drone commands
int seq = 1;			// First sequence command counter
int aeq = 1;			// Second sequence command counter
int turnCounter = 0;		// counter for number of turns while drone is in one place

// Timer variables for various applications
unsigned long time;		// Local time variable
unsigned long flightTime;	// Flight time variable for use in between states
unsigned long totalTime;	// total time drone has been flying
unsigned long reverseTime;	// reverse timer to stop drone from forward motion
unsigned long keepConn;		// keep connection method time variable
unsigned long turnTimer;	// turn timer to set desired angle per turn sequence
unsigned long verticalTimer;	// vertical timer to set desired height


// State variable
int droneState = 0;		// Drone state variable

// Range variables
int rangeStop = 15;		// Range value to stop drone
int leftSensor[3];		// Array containing 3 left sensor reads
int rightSensor[3];		// Array containing 3 right sensor reads
int verticalSensor[3];		// Array containing 3 vertical sensor reads
int leftRange = 0;		// largest value of leftSensor[3]
int rightRange = 0;		// largest value of rightSensor[3]
int verticalRange = 0;		// largest value of verticalSensor[3]
int downRange = 0;		// value from drone altitude sensor
int reverseTrigger = 0;		// reverse trigger, set to 1 if in reverseState
int turnTrigger = 0;		// turn trigger, set to 1 if in turnState
int verticalTrigger = 0;	// vertical trigger, set to 1 if in vertical state
int verticalUpDown = 0;		// vertical direction, 0 = up, 1 = down

// flight speed variables
float forwardSpeed = -0.1;	// forward speed state
float reverseSpeed = 0.2;	// reverse speed
float turnSpeed = -1;		// turn speed, turn left max speed
float downSpeed = -0.5;		// downward speed
float upSpeed = 0.5;		// upward speed

// Variables for range finders
MultiAnalogRange rangeClass;	// instance created of range finder library
int rangeTimer = 0;		// range timer counter

// Enumerator for drone States
enum droneFlightState
{
	LANDED,
	TAKEOFF,
	HOVERING,
	EMERGENCYLAND,
	FORWARDFLIGHT,
	TURN,
	REVERSE,
	VERTICAL
};

// Enumerator for vertical flight States
enum verticalDirection
{
	UP,
	DOWN
};

// ON or OFF enumerator
enum onOff
{
	OFF,
	ON
};


/*
* Initial state function. Sends drone preflight information
* and sets initial values to timers.
*/
void initialState()
{
        //Serial.println("initialState keep on ground until given command to fly");
	//Serial.println("sending flat trim command to drone to attempt to prevent flying into walls");
        time = millis();
	rangeTimer = time;
	flightTime = time;
	keepConn = time;
	totalTime = time;
        sprintf(data, "AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r", 1, 1);

	// Printing command structure to serial output
	//Serial.println("initialState command string: ");
	//Serial.println(data);
	//Serial.print(data);
	Serial.println("FirstCommand");

	reverseTime = 0;

	sprintf(data, "AT*FTRIM=%d\r", seq++);
	aeq = seq + 1;	// Make sure that aeq stays ahead of seq for sequencing

	// Printing command structure to serial output
	//Serial.println("flatTrim command string: ");
	//Serial.println(data);
	//Serial.print(data);
	Serial.println("Flattrimming");

	delay(MILLISECOND_200); //200);
}

void landingState()
{
        //Serial.println("landingState send land command");
        sprintf(data, "AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r", seq++, aeq++);

	// Printing command structure to serial output
	//Serial.println("landingState command string: ");
	//Serial.println(data);
	//Serial.print(data);
	Serial.println("Landing");
}

void takeOffState()
{
        //Serial.println("takeOffState, tell drone to take off");
        sprintf(data, "AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290718208\r", seq++, aeq++);

	// Printing command structure to serial output
	//Serial.println("takeOffState command string: ");
	//Serial.println(data);
	//Serial.print(data);
	Serial.println("TakeOff");
}

void hoverState()
{
        //Serial.println("hoverState, hovring with no directions");
        sprintf(data, "AT*PCMD=%d,0,0,0,0,0\r", seq++);
	aeq = seq + 1;

	// Printing command structure to serial output
	//Serial.println("hoverState command string: ");
	//Serial.println(data);
	//Serial.print(data);
	Serial.println("Hovering");
}

void emergencyLandState()
{
        //Serial.println("landingState send land command");
        sprintf(data, "AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r", seq++, aeq++);

	// Printing command structure to serial output
	//Serial.println("emergencyLandState command string: ");
	//Serial.println(data);
	//Serial.print(data);
	Serial.println("EmergencyLandState");
}

void forwardState()
{
	//Serial.println("forwardState, moving forward");
	// Entered a 1 for the second arg in the command to enable reading other values for control
	sprintf(data, "AT*PCMD=%d,1,0,%ld,0,0\r", seq++, forwardSpeed);
	aeq = seq + 1;

	// Printing command structure to serial output
	//Serial.println("forward command string: ");
	//Serial.println(data);
	//Serial.print(data);
	Serial.println("Forward");
}

void reverseState()
{
	//Serial.println("rangeStop has been breached, reverse drone then pass state back to run time.");

	if (reverseTime == 0)
	{
		reverseTime = millis();
	}

	if (millis() - reverseTime < MILLISECOND_200) //200)
	{
		sprintf(data, "AT*PCMD=%d,1,0,%ld,0,0\r", seq++, reverseSpeed);
		aeq = seq + 1;
	}
	else
	{
		reverseTime = 0;
		reverseTrigger = 0;
	}

	// Printing command structure to serial output
	//Serial.println("Reverse command string, send every 30ms for 200 ms: ");
	//Serial.println(data);
	//Serial.print(data);
	Serial.println("Reverse");
}

void turnState()
{
	//Serial.println("goForward has failed must turn drone");

	if (turnTimer == 0)
	{
		turnTimer = millis();
	}

	if (millis() - turnTimer < MILLISECOND_300) // 300)
	{
		sprintf(data, "AT*PCMD=%d,1,0,0,0,%ld\r", seq++, turnSpeed);
		aeq = seq + 1;
	}
	else
	{
		turnTimer = 0;
		turnTrigger = 0;
		turnCounter++;
	}

	// Printing command structure to serial output
	//Serial.println("Turn command string, send every 30ms for 300ms: ");
	//Serial.println(data);
	//Serial.print(data);
	Serial.println("Turning");
}

void verticalState()
{
	if (verticalTimer == OFF) // 0)
	{
		verticalTimer = millis();
	}

	if (millis() - verticalTimer < MILLISECOND_1000) // 1000)
	{
		switch(verticalUpDown)
		{
			case UP:
				flyUp();
				break;
			case DOWN:
				flyDown();
				break;
		}
	}
	else
	{
		verticalTrigger = 0;
		verticalTimer = 0;
		turnCounter = 0;
	}
}

void flyDown()
{
	sprintf(data, "AT*PCMD=%d,1,0,0,%ld,0\r", seq++, downSpeed);
	aeq = seq + 1;

	//Printing command structure to serial output
	//Serial.println("flyDown command:");
	//Serial.println(data);
	//Serial.print(data);
	Serial.println("Down");
}

void flyUp()
{
	sprintf(data, "AT*PCMD=%d,1,0,0,%ld,0\r", seq++, upSpeed);
	aeq = seq + 1;

	//Printing command structure to serial output
	//Serial.println("flyUp command:");
	//Serial.println(data);
	//Serial.print(data);
	Serial.println("Up");
}

void keepConnection()
{
	//Serial.print("keepConnection() Function: droneState = ");
	//Serial.println(droneState);

	// Check for 30 milliseconds rather than 300 for keep connection,
	// should enable smoother flight
	if (millis() - keepConn > MILLISECOND_30) // 30)
	{
		  keepConn = millis();

		  switch(droneState)
		  {
			case LANDED:
				landingState();
				break;
			case TAKEOFF:
				takeOffState();
				break;
			case HOVERING:
				hoverState();
				break;
			case EMERGENCYLAND:
				emergencyLandState();
				break;
			case FORWARDFLIGHT:
				forwardState();
				break;
			case REVERSE:
				reverseState();
				break;
			case TURN:
				turnState();
				break;
			case VERTICAL:
				verticalState();
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

	for (i = 0; i < MAX_SENSORS; i++) // 3; i++)
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
	//debugPrint();
}

void rangeCheck()
{
	//Serial.println("rangeCheck function");
	if (rangeTimer < RANGE_TIMER_MAX)
	{
		leftSensor[rangeTimer] = rangeClass.sensors[left];
		rightSensor[rangeTimer] = rangeClass.sensors[right];
		verticalSensor[rangeTimer] = rangeClass.sensors[vertical];
		rangeTimer++;
	}

	//Serial.println(rangeTimer);

	if (rangeTimer == RANGE_TIMER_MAX)
	{
		sensorCompare();
	}
}

int goForward()
{
	//debugPrint();

	if (leftRange > rangeStop && rightRange > rangeStop)
	{
		//Serial.println("goForward returning ON!");
		return ON; //1;
	}

	//Serial.println("goForward returning OFF");
	return OFF; //0;
}

int goVertical()
{
	if (verticalRange > rangeStop)
	{
		// if drone can go up then return UP
		return UP;
	}

	// If no room to move up then drone must go down
	return DOWN;
}

void stateSetter()
{
	switch (droneState)
	{
		case LANDED:
		{
			droneState = TAKEOFF;
			flightTime = millis();
			break;
		}
		case TAKEOFF:
		{
			// send take off for at least 3 seconds to be sure drone
			// is ready to enter hover state
			if (millis() - flightTime > MILLISECOND_3000) //3000)
			{
				droneState = HOVERING;
				flightTime = millis();
			}

			break;
		}
		case HOVERING:
		{
			// Make sure forward range is clear
			if (goForward())
			{
				droneState = FORWARDFLIGHT;
				flightTime = millis();
			}
			// If drone has gone through turn sequence
			// more than 4 times it will change to vertical flight
			else if (turnCounter >= MAX_TURNS && verticalTrigger == OFF) // 4 && verticalTrigger == 0)
			{
				droneState = VERTICAL;
				verticalUpDown = goVertical();
				verticalTrigger = ON; //1;
			}
			else if (turnTrigger == OFF && verticalTrigger == OFF) //0)
			{
				/*
				Serial.println("******* Switching to turn state *******");
				Serial.println("Turning Variables:");
				Serial.print("		turnTrigger = ");
				Serial.println(turnTrigger);
				*/

				droneState = TURN;
				flightTime = millis();
				turnTrigger = ON; //1;
			}

			break;
		}
		case FORWARDFLIGHT:
		{
			if (goForward())
			{
				//Serial.println("Flying forward!!!!");
				droneState = FORWARDFLIGHT;
			}
			else
			{
				//Serial.println("rangeStop has been breached, STOP the drone!!!");
				droneState = REVERSE;
				reverseTrigger = ON; //1;
			}

			break;
		}
		case REVERSE:
		{
			if (reverseTrigger == OFF) //0)
			{
				droneState = HOVERING;
			}

			break;
		}
		case VERTICAL:
		{
			if (verticalTrigger == OFF) //0)
			{
				droneState = HOVERING;
			}

			break;
		}
		case TURN:
		{
			if (turnTrigger == OFF)
			{
				droneState = HOVERING;
			}

			break;
		}
	}
}

void flyDrone()
{
	if (time == 0)
	{
		initialState();
	}

	// populate range arrays and get range values
	if (millis() - time > MILLISECOND_100)
	{
		rangeClass.GetRanges();
		rangeCheck();
		time = millis();
	}

	if (rangeTimer >= RANGE_TIMER_MAX) //3)
	{
		rangeTimer = 0;
		stateSetter();
	}

	keepConnection();
}

void debugPrint()
{

	// Range finder print Statments
	Serial.print("RangeTimer; ");
	Serial.print(rangeTimer);
	Serial.print("; DroneState; ");
	Serial.print(droneState);
	Serial.print("; Left Sensor; ");
	Serial.print(leftRange);
	Serial.print("; Right Sensor; ");
	Serial.print(rightRange);
	Serial.print("; Vertical Sensor; ");
	Serial.println(verticalRange);
}

void setup()
{
        Serial.begin(BAUD_RATE);
        Serial.println("");
        Serial.println("");
        Serial.println("Initialize Connection");
        
	if (!testing)
	{
		WiFi.init();
		Serial.println("Initialized");
        
		unsigned char *my_mac = zg_get_mac();
		sprintf(yj_mac,"%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",my_mac[0],my_mac[1],my_mac[2],my_mac[3],my_mac[4],my_mac[5]);
		
		Serial.print("MAC Address: ");
		Serial.println(yj_mac);
	}
	else
	{
		Serial.println("Testing program");
	}
}

void loop()
{
	if (!testing)
	{
		WiFi.run();
	}
	
        // Function to begin drone flight
        flyDrone();
}
