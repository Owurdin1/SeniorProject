/*
 * IDrone.h
 *
 *  Created on: Mar 8, 2013
 *      Author: Oscar Wurdinger
 */

#ifndef IDRONE_H_
#define IDRONE_H_

#include <iostream>
#include <string>
#include "ICommandRead.h"

namespace droneSimulator
{
    class IDrone
    {
    protected:
        /*  Protected variables  */
        std::string currentState;
        std::string fileName;

    public:
        /*  Virtual functions  */
        virtual void initializeDrone() = 0;             /*  Initialization connects to command module and asks for commands  */
        virtual std::string retrieveCommand() = 0;      /*  Retrieves command from the command interface and returns as a string  */
        virtual ~IDrone() {}                            /*  Virtual Destructor  */

        /*  Interface functions  */
        std::string getState();                         /*  currentState accessor  */
        void changeState( std::string );                /*  Drone changes state of drone  */
        void processCommand( std::string );             /*  Processes command and calls changeState if new state is needed  */
        void flyDrone();                                /*  Drone flight function will run in loop until drone enters EmergencyLandingState  */
    };
} /* namespace droneSimulator */
#endif /* IDRONE_H_ */
