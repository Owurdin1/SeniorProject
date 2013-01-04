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

// 0 = landingState, 1 = takeOffState, 2 = hoverState, 3 = turnState,
// 4 = forwardState, 5 = verticalUpState, 6 = verticalDownState
int droneState = 0;

int rangeStop = 15;
int forwardSpeed = -0.3;
int turnSpeed = -1;
int turnCounter = 0;
int verticalSpeed = 0; // vertical speed is set by calling function

// flightRange variables
int verticalTrigger = 0;
int forwardTrigger = 0;

// Variables for range finders
MultiAnalogRange rangeClass;
int rangeTimer = 0;

void verticalDownState()
{
	//Serial.println("verticalDownState, decreasing height");
	droneState = 6;
	sprintf(data,"AT*PCMD=%d,0,0,0,verticalSpeed,0\r",seq++);
}

void verticalUpState()
{
	//Serial.println("verticalState, increasing height");
	droneState = 5;
        sprintf(data,"AT*PCMD=%d,0,0,0,verticalSpeed,0\r",seq++);
}

void forwardState()
{
	//Serial.println("forwardState, moving forward");
	droneState = 4;
        sprintf(data,"AT*PCMD=%d,0,0,forwardSpeed,0,0\r",seq++); 
}

void turnState()
{
	//Serial.println("turnState, turning and incrementing turn counter");
	droneState = 3;
        sprintf(data,"AT*PCMD=%d,0,0,0,0,turnSpeed\r",seq++);
}

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

void initialState()
{
         //Serial.println("initialState keep on ground until given command to fly");
         time = millis();
	 rangeTimer = time;
         sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r",1,1);
}

void keepConnection()
{
	// 0 = landingState, 1 = takeOffState, 2 = hoverState, 3 = turnState,
	// 4 = forwardState, 5 = verticalUpState, 6 = verticalDownState

	Serial.println("Keeping drone connection alive");
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
		case 4:
			forwardState();
			break;
		case 5:
			verticalUpState();
			break;
		case 6:
			verticalDownState();
			break;	
	}
}

void printTestValues()
{
	Serial.print("DroneState = ");
	Serial.print(droneState);
	Serial.print(", Left Range = ");
	Serial.print(rangeClass.sensors[0]);
	Serial.print(", Right Range = ");
	Serial.print(rangeClass.sensors[1]);
	Serial.print(", Vertical Range = ");
	Serial.print(rangeClass.sensors[2]);
	Serial.println(",");
}

void stateChooser()
{
	
}

void flightSequence()
{
	if (time == 0)
	{
		initialState();
	}

	rangeClass.GetRanges();
	rangeTimer = millis();

	// Print debug values
	printTestValues();

	// If verticalTrigger or forwardTrigger is on then change
	// drone state to hover and transition to turnState or
	// verticalUpState/verticalDownState and continue to
	// forwardState when possible
	if ((rangeClass.sensors[0] > rangeStop) && (rangeClass.sensors[1] > rangeStop) && (rangeClass.sensors[2] > rangeStop))
	{
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
			case 4:
				forwardState();
				break;
			case 5:
				verticalUpState();
				break;
			case 6:
				verticalDownState();
				break;
			default:
				landingState();
				break;
		}
	}
	else
	{
		
	}
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
	
	// Function to run flight test using vertical range
	// finder to stop sequence
	flightSequence();
}
