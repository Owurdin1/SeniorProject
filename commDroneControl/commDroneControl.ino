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

#include "commDroneControl.h"
#include <WiShield.h>

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
	Serial.println("initialState command string: ");
	Serial.println(data);
	//Serial.print(data);
	//Serial.println("FirstCommand");

	reverseTime = 0;

// ============================Test Config Commands=====================================
    sprintf(data, "AT*PCMODE=%d,%d\r", seq++, 2);
    
    Serial.println("Test new command one:");
    Serial.println(data);
    
    delay(MILLISECOND_100);
    
    sprintf(data, "AT*MISC=%d,%d,%d,%d,%d\r", seq++, 2, 20, 2000, 3000);
    
    Serial.println("Test new command two:");
    Serial.println(data);
    
    delay(MILLISECOND_100);
// =====================================================================================

	sprintf(data, "AT*FTRIM=%d\r", seq++);
	aeq = seq + 1;	// Make sure that aeq stays ahead of seq for sequencing

	// Printing command structure to serial output
	Serial.println("flatTrim command string: ");
	Serial.println(data);
	//Serial.print(data);
	//Serial.println("Flattrimming");

	delay(MILLISECOND_200);

// ============================Test Config Commands=====================================

    /*
    *    Send config ID's to the drone
    *    send session id to the drone
    */
    sprintf(data, "AT*CONFIG_IDS= %d, \"%s\", \"%s\", \"%s\"\r", seq++, DRONE_SESSION_ID, DRONE_PROFILE_ID, DRONE_APP_ID);
    sprintf(data, "AT*CONFIG=%d, \"custom: sesion_id\", \"%s\"\r", seq++, DRONE_SESSION_ID);
    delay(MILLISECOND_100);
    
    /*
    *    Send config ID's to the drone
    *    send profile id to the drone
    */
    sprintf(data, "AT*CONFIG_IDS=%d, \"%s\", \"%s\", \"%s\"\r", seq++, DRONE_SESSION_ID, DRONE_PROFILE_ID, DRONE_APP_ID);
    sprintf(data, "AT*CONFIG=%d, \"custom: profile_id \", \"%s\"\r", seq++, DRONE_PROFILE_ID);
    delay(MILLISECOND_100);
    
    /*
    *    Send config ID's to the drone
    *    send application id to the drone
    */
    sprintf(data, "AT*CONFIG_IDS=%d, \"%s\", \"%s\", \"%s\"\r", seq++, DRONE_SESSION_ID, DRONE_PROFILE_ID, DRONE_APP_ID);
    sprintf(data, "AT*CONFIG=%d, \"custom: application_id\", \"%s\"\r", seq++, DRONE_APP_ID);
    delay(MILLISECOND_100);
// =====================================================================================
}

void landingState()
{
    //Serial.println("landingState send land command");
    sprintf(data, "AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r", seq++, aeq++);

	// Printing command structure to serial output
	Serial.println("landingState command string: ");
	Serial.println(data);
	//Serial.print(data);
	//Serial.println("Landing");
}

void takeOffState()
{
    //Serial.println("takeOffState, tell drone to take off");
    sprintf(data, "AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290718208\r", seq++, aeq++);

	// Printing command structure to serial output
	Serial.println("takeOffState command string: ");
	Serial.println(data);
	//Serial.print(data);
	//Serial.println("TakeOff");
}

void hoverState()
{
    //Serial.println("hoverState, hovring with no directions");
    sprintf(data, "AT*PCMD=%d,0,0,0,0,0\r", seq++);
	aeq = seq + 1;

	// Printing command structure to serial output
	Serial.println("hoverState command string: ");
	Serial.println(data);
	//Serial.print(data);
	//Serial.println("Hovering");
}

void emergencyLandState()
{
    //Serial.println("landingState send land command");
    sprintf(data, "AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r", seq++, aeq++);

	// Printing command structure to serial output
	Serial.println("emergencyLandState command string: ");
	Serial.println(data);
	//Serial.print(data);
	//Serial.println("EmergencyLandState");
}

void forwardState()
{
	//Serial.println("forwardState, moving forward");
	// Entered a 1 for the second arg in the command to enable reading other values for control
	sprintf(data, "AT*PCMD=%d,1,0,%ld,0,0\r", seq++, forwardSpeed);
	aeq = seq + 1;

	// Printing command structure to serial output
	Serial.println("forward command string: ");
	Serial.println(data);
	//Serial.print(data);
	//Serial.println("Forward");
}

void reverseState()
{
	//Serial.println("rangeStop has been breached, reverse drone then pass state back to run time.");

	if (reverseTime == 0)
	{
		reverseTime = millis();
	}

	if (millis() - reverseTime < MILLISECOND_200)
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
	Serial.println("Reverse command string, send every 30ms for 200 ms: ");
	Serial.println(data);
	//Serial.print(data);
	//Serial.println("Reverse");
}

void turnState()
{
	//Serial.println("goForward has failed must turn drone");

	if (turnTimer == 0)
	{
		turnTimer = millis();
	}

	if (millis() - turnTimer < MILLISECOND_300)
	{
		sprintf(data, "AT*PCMD=%d,1,0,0,0,%ld\r", seq++, turnSpeed);
		aeq = seq + 1;
	}
	else
	{
		turnTimer = OFF;
		turnTrigger = OFF;
		turnCounter++;
	}

	// Printing command structure to serial output
	Serial.println("Turn command string, send every 30ms for 300ms: ");
	Serial.println(data);
	//Serial.print(data);
	//Serial.println("Turning");
}

void verticalState()
{
	if (verticalTimer == OFF)
	{
		verticalTimer = millis();
	}

	if (millis() - verticalTimer < MILLISECOND_1000)
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
	Serial.println("flyDown command:");
	Serial.println(data);
	//Serial.print(data);
	//Serial.println("Down");
}

void flyUp()
{
	sprintf(data, "AT*PCMD=%d,1,0,0,%ld,0\r", seq++, upSpeed);
	aeq = seq + 1;

	//Printing command structure to serial output
	Serial.println("flyUp command:");
	Serial.println(data);
	//Serial.print(data);
	//Serial.println("Up");
}

void keepConnection()
{
	//Serial.print("keepConnection() Function: droneState = ");
	//Serial.println(droneState);

	// Check for 30 milliseconds rather than 300 for keep connection,
	// should enable smoother flight
	if (millis() - keepConn > MILLISECOND_30)
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

	for (i = 0; i < MAX_SENSORS; i++)
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
		return ON;
	}

	//Serial.println("goForward returning OFF");
	return OFF;
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
    if (millis() - totalTime < MILLISECOND_120000)
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
			    if (millis() - flightTime > MILLISECOND_3000)
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
			    else if (turnCounter >= MAX_TURNS && verticalTrigger == OFF)
			    {
				    droneState = VERTICAL;
				    verticalUpDown = goVertical();
				    verticalTrigger = ON;
			    }
			    else if (turnTrigger == OFF && verticalTrigger == OFF)
			    {
				    /*
				    Serial.println("******* Switching to turn state *******");
				    Serial.println("Turning Variables:");
				    Serial.print("		turnTrigger = ");
				    Serial.println(turnTrigger);
				    */

				    droneState = TURN;
				    flightTime = millis();
				    turnTrigger = ON;
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
				    reverseTrigger = ON;
			    }

			    break;
		    }
		    case REVERSE:
		    {
			    if (reverseTrigger == OFF)
			    {
				    droneState = HOVERING;
			    }

			    break;
		    }
		    case VERTICAL:
		    {
			    if (verticalTrigger == OFF)
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
    else
    {
        droneState = EMERGENCYLAND;
    }
}

/*
void stateSetter()
{
    if (millis() - totalTime < MILLISECOND_60000)
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
                if (millis() - flightTime > MILLISECOND_3000)
			    {
				    droneState = HOVERING;
				    flightTime = millis();
			    }

			    break;
            }
            case HOVERING:
            {
                if (millis() - flightTime > MILLISECOND_30)
                {
                    droneState = TURN;
                    turnTrigger = ON;
                    flightTime = millis();
                }
            }
            case TURN:
            {
                if (turnTrigger == OFF)
                {
                    droneState = HOVERING;
                }
                
                break;
            }
            case EMERGENCYLAND:
            {
                while (1)
                {
                    emergencyLandState();
                }
            }
        }
    }
    else
    {
        droneState = EMERGENCYLAND;
    }
}
*/

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

	if (rangeTimer >= RANGE_TIMER_MAX)
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
