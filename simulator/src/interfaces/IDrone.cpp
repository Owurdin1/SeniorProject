/*
 * IDrone.cpp
 *
 *  Created on: Mar 8, 2013
 *      Author: Oscar Wurdinger
 */

#include "IDrone.h"

namespace droneSimulator
{
    /*
     * getter for the currentState variable.
     */
    std::string IDrone::getState()
    {
        return currentState;
    }

    /*
     * changes the current state of the drone and
     * updates the terminal.
     */
    void IDrone::changeState(std::string newState)
    {
        if ( newState.compare( "Up" ) == 0 )
        {
            std::cout << "Turn counter has reached max Drone flying Up" << std::endl;
        }
        else if ( newState.compare( "Down" ) == 0 )
        {
            std::cout << "Drone cannot fly Up and turn counter has reached max. Drone flying Down" << std::endl;
        }
        else if ( newState.compare( "Landing" ) == 0 )
        {
            std::cout << "Drone is being sent Landing command to keep it on the ground" << std::endl;
        }
        else if ( newState.compare( "TakeOff" ) == 0 )
        {
            std::cout << "Ready for flight, Drone is being sent Take Off command." << std::endl;
        }
        else if ( newState.compare( "Hovering" ) == 0 )
        {
            std::cout << "Drone is Hovering and scanning." << std::endl;
        }
        else if ( newState.compare( "Turning" ) == 0 )
        {
            std::cout << "No room to fly Forward, Drone is Turning." << std::endl;
        }
        else if ( newState.compare( "Reverse" ) == 0 )
        {
            std::cout << "Drone has detected an object in Forward flight, using Reverse to stop Forward motion" << std::endl;
        }
        else if ( newState.compare( "Flattrimming" ) == 0 )
        {
            std::cout << "Drone is being sent Flat Trim command to give it a Horizontal reference" << std::endl;
        }
        else if ( newState.compare( "Initialize Connection" ) == 0 )
        {
            std::cout << "Initializing Connection with the Drone" << std::endl;
        }
        else if ( newState.compare( "Testing program" ) == 0 )
        {
            std::cout << "This program is running in Test mode so it is not starting the WiFi module" << std::endl;
        }
        else if ( newState.compare( "FirstCommand" ) == 0 )
        {
            std::cout << "Sending the First Command to the Drone starting sequence counter from this point" << std::endl;
        }
        else if ( newState.compare( "Forward" ) == 0 )
        {
            std::cout << "Drone is flying Forward." << std::endl;
        }
        else
        {
            std::cout << "Drone has been sent an invalid command, ignoring and continuing previous state" << std::endl;
        }
    }

    /*
     * Processes commands for the drone and calls changeState
     * function if there is a new state required to handle
     * the new command.
     */
    void IDrone::processCommand( std::string newState )
    {

        if ( newState.compare( currentState ) != 0 )
        {
            currentState = newState;
            changeState( currentState );
        }
    }

    /*
     * Since drone does this sequence in a loop until it runs into an emergency
     * of some sort, either battery, or crash has been detected. Behavior of
     * program will act in same manner.
     */
    void IDrone::flyDrone()
    {
        std::string nextCommand;

        while ( 1 )
        {
            // Retrieve command from the command source
            nextCommand = retrieveCommand();

            // Process command and carry out it's instructions
            processCommand( nextCommand );

            // If transitions to an emergency state then drone stops flying
            if ( nextCommand.compare( "EmergencyLandState" ) == 0 )
            {
                break;
            }
        }
    }
}; /*  end droneSimulator  */
