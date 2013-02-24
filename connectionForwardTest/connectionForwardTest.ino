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

//===========================================================================
int testing = 1;
//===========================================================================

// Variables for drone states
char data[100];
char yj_mac[17];
int seq = 1;	// First sequence command counter
unsigned long time;
unsigned long flightTime;
unsigned long totalTime;
int droneState = 0;	// Drone state variable
float forwardSpeed = -0.1; // forward speed state

int init_drone = 1;	// Initialize drone trigger
int init_counter = 0;	// counter for initializing the drone connection
int flightTrigger = 0;	// Flight trigger if 0 drone is to stay on the ground else flying

// Enumerator for drone States
enum droneFlightState
{
	LANDED,
	TAKEOFF,
	HOVERING,
	EMERGENCYLAND,
	FORWARDFLIGHT,
	TURN,
	FLATTRIMING,
	RESETWATCHDOG,
	REVERSE,
	GROUNDED
};

// Variables for Keep Connection
unsigned long keepConn;

void initialState()
{
	Serial.println("Initializing drone");

	if (init_counter < 7)
	{
		sprintf(data,"Dummy to init network");
		Serial.println("Sending dummy to init network");
	}

	switch(init_counter)
	{
		case 7:
		{
			sprintf(data,"AT*CONFIG=%d,\"network:owner_mac\",\"%s\"\r",1,yj_mac);
			Serial.print("Setting network owner mac_addr: ");
			Serial.println(yj_mac);
			break;
		}
		case 20:
		{
			sprintf(data,"AT*CONFIG=%d,\"control:control_yaw\",\"%d\"\r",seq++,YAW_RATE);
			Serial.print("Setting YAW_RATE: ");
			Serial.println(YAW_RATE);
			break;
		}
		case 21:
		{
			sprintf(data,"AT*CONFIG=%d,\"control:control_vz_max\",\"%d\"\r",seq++,THROTTLE_RATE);
			Serial.print("Setting THROTTLE_RATE: ");
			Serial.println(THROTTLE_RATE);
			break;
		}
		case 22:
		{
			sprintf(data,"AT*CONFIG=%d,\"control:euler_angle_max\",\"0.%d\"\r",seq++,PITCH_ROLL_RATE);
			Serial.print("Setting PITCH_ROLL_RATE: ");
			Serial.println(PITCH_ROLL_RATE);
			break;
		}
		case 23:
		{
			sprintf(data,"AT*CONFIG=%d,\"control::altitude_max\",\"%d\"\r",seq++,ALTITUDE_MAX);
			Serial.print("Setting ALTITUDE_MAX: ");
			Serial.println(ALTITUDE_MAX);
			break;
		}
		case 24:
		{
			sprintf(data,"AT*CONFIG=%d,\"control::outdoor\",\"%s\"\r",seq++,FLY_OUTDOOR);
			Serial.print("Setting indoor outdoor FLY_OUTDOOR: ");
			Serial.println(FLY_OUTDOOR);
			break;
		}
		case 25:
		{
			sprintf(data,"AT*LED=%d,2,1073741824,5\r",seq++);
			Serial.println("Flashing LED Sequence");
			break;
		}
		case 27:
		{
			sprintf(data,"AT*LED=%d,4,1073741824,120\r",seq++);
			Serial.println("Flashing LED Sequence");
			break;
		}
		case 28:
		{
			sprintf(data,"AT*LED=%d,4,1073741824,120\r",seq++);
			Serial.println("Flashing LED Sequence");
			break;
		}
		case 30:
		{
			init_drone = 0;
			time = millis();
			flightTime = 0;
			totalTime = time;
			break;
		}
	}

	init_counter++;
}

void groundedState()
{
	Serial.println ("Grounded State");

	sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r",1,1);

	// if flying switch goes high then take off
	if (flightTrigger == 1) // reset watchdog, flat trim, take off and hover
	{
		droneState = RESETWATCHDOG;
		flightTime = 0;
	}
}

void resetWatchDog()
{
	Serial.println ("Resetting watchdog");

	sprintf(data,"AT*COMWDG=%d\r",1);

	// give WiFi some time to send the command
	if (flightTime == 0)
	{
		flightTime = millis();
	}

	if (millis() - flightTime > 400) // make sure the watch dog command is sent
	{
		droneState = FLATTRIMING;
		seq = 1;
		flightTime = 0;
	}

	if (flightTrigger == 0)
	{
		droneState = GROUNDED;
	}
}

void flatTrimState()
{
	Serial.println ("Flat triming");

	sprintf(data,"AT*FTRIM=%d,\rAT*LED=%d,2,1073741824,1\r",seq++,seq++);

	// give WiFi some time to send the flat trim command
	if (flightTime == 0)
	{
		flightTime = millis();
	}

	if (millis() - flightTime > 200)
	{
		droneState = TAKEOFF;
		flightTime = 0;
	}

	if (flightTrigger == 0)
	{
		droneState = GROUNDED;
	}
}

void landingState()
{
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r",seq++,seq++);
	Serial.println("landingState command string: ");
	Serial.println(data);
}

void takeOffState()
{
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290718208\r",seq++,seq++);
	Serial.println("takeOffState command string: ");
	Serial.println(data);

	// give WiFi some time to send the flat trim command

	if (flightTime == 0)
	{
		flightTime = millis();
	}

	if (millis() - flightTime > 300)
	{
		droneState = HOVERING;
	}

	if (flightTrigger == 0)
	{
		droneState = GROUNDED;
	}
}

void hoverState()
{
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\r",seq++);
	Serial.println("hoverState command string: ");
	Serial.println(data);
}

void emergencyLandState()
{
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r",seq++, seq++);
	Serial.println("emergencyLandState command string: ");
	Serial.println(data);
}

void forwardState()
{
	sprintf(data, "AT*PCMD=%d,1,0,%ld,0,0\r", seq++, forwardSpeed);
	Serial.println("forward command string: ");
	Serial.println(data);
}

void keepConnection()
{
	if (millis() - keepConn > 30)
	{
		  keepConn = millis();

		  switch(droneState)
		  {
			case GROUNDED:
				groundedState();
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
			case RESETWATCHDOG:
				resetWatchDog();
				break;
			case FLATTRIMING:
				flatTrimState();
				break;
		  }
	}
}

void connectionForwardTest()
{
	if (flightTrigger == 0)
	{
		flightTrigger = 1;
	}

	switch(droneState)
	{
		case GROUNDED:
			droneState = TAKEOFF;
			break;
		case LANDED:
			droneState = TAKEOFF;
			break;
		case HOVERING:
			if (millis() - flightTime > 3000)
			{
				droneState = EMERGENCYLAND;
			}

			if (flightTime == 0)
			{
				droneState = FORWARDFLIGHT;
				flightTime = millis();
			}
			break;
		case FORWARDFLIGHT:
			if (millis() - flightTime > 1000)
			{
				droneState = HOVERING;
			}
			break;
		case TAKEOFF:
			droneState = HOVERING;
			flightTime = 0;
	}

	keepConnection();
}

void setup()
{
        Serial.begin(9600);
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

	if (init_drone == 1)
	{
		initialState();
	}
	
        // Function to begin test
	if (init_drone == 0)
	{
		connectionForwardTest();
	}
}
