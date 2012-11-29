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
#include "user_config.h"

#define WIRELESS_MODE_INFRA	1
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

char data[100];
char yj_mac[17];
int seq = 1;
unsigned long time;
int hoverCounter = 0;
int droneState = 0;

void hoverState()
{
        Serial.println("hoverState, hovring with no directions");
        droneState = 2;
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\r",seq++); 
}

void takeOffState()
{
        Serial.println("takeOffState, tell drone to take off");
        droneState = 1;
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290718208\r",seq++,seq++);
}

void landingState()
{
        Serial.println("landingState send land command");
        droneState = 0;
        sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r",seq++,seq++);
}

void initialState()
{
         Serial.println("initialState keep on ground until given command to fly");
         time = millis();
         sprintf(data,"AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r",1,1);
}

void keepConnection()
{
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
          }
}

void hoverSequence()
{
         if (time == 0)
         {
                 initialState();
         }
         
         Serial.print("hoverCounter = ");
         Serial.print (hoverCounter);
         Serial.print("  droneState = ");
         Serial.println(droneState);
         
         switch(hoverCounter)
         {
                 case 0:
                        if ((millis() - time) > 10000)
                        {
                                 takeOffState();
                                 time = millis();
                                 hoverCounter++;
                        }
                        else
                        {
                                 keepConnection();
                        }
                        break;
                 case 1:
                        if ((millis() - time) > 10000)
                        {
                                 landingState();
                                 time = millis();
                                 hoverCounter++;
                        }
                        else
                        {
                                 keepConnection();
                        }
                        break;
                 case 2:
                        if ((millis() - time) > 10000)
                        {
                                 takeOffState();
                                 time = millis();
                                 hoverCounter++;
                        }
                        else
                        {
                                 keepConnection();
                        }
                        break;
                 case 3:
                        if ((millis() - time) > 10000)
                        {
                                 landingState();
                                 time = millis();
                                 hoverCounter++;
                        }
                        else
                        {
                                 keepConnection();
                        }
                        break;
                 case 4:
                        if ((millis() - time) > 10000)
                        {
                                 takeOffState();
                                 time = millis();
                                 hoverCounter++;
                        }
                        else
                        {
                                 keepConnection();
                        }
                        break;
                 case 5:
                        if ((millis() - time) > 10000)
                        {
                                 landingState();
                                 time = millis();
                                 hoverCounter++;
                        }
                        else
                        {
                                 keepConnection();
                        }
                        break;
                 default:
                         keepConnection();
                         Serial.print("Defualt case hoverCounter = ");
                         Serial.println(hoverCounter);
         }
}

void setup()
{
        Serial.begin(38400);
        Serial.println("");
        Serial.println("");
        Serial.println("Initialize Cconnection");
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

        hoverSequence();
}
