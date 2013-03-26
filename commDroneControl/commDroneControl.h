// Header file for commDroneControl
// Date: 20130325

#ifndef COMM_DRONE_CONTROL
#define COMM_DRONE_CONTROL

#include <WiShield.h>
#include "MultiAnalogRangeFinder.h"
#include "user_config.h"

//#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2
#define RANGE_TIMER_MAX		3	    /* rangeTimer maximum value is 3 */
#define MAX_SENSORS		3	        /* maximum number of sensors on drone */
#define MILLISECOND_30		30	    /* 30 millisecond variable */
#define MILLISECOND_100		100	    /* 100 millisecond variable */
#define MILLISECOND_200		200	    /* 200 millisecond variable */
#define MILLISECOND_300		300	    /* 300 millisecond variable */
#define MILLISECOND_1000	1000	/* 1000 millisecond variable */
#define MILLISECOND_3000	3000	/* 3000 millisecond variable */
#define BAUD_RATE		9600	    /* set baud rate value to 9600 */
#define MAX_TURNS		4	        /* maximum number of turns */

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

#endif
