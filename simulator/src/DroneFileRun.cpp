/*
 * Drone.cpp
 *
 *  Created on: Mar 8, 2013
 *      Author: Oscar Wurdinger
 */

#include "DroneFileRun.h"

namespace droneSimulator
{

    DroneFileRun::DroneFileRun()
    {
        // TODO Auto-generated constructor stub

    }

    DroneFileRun::~DroneFileRun()
    {
        // TODO Build the destructor
    }

//    void Drone::processCommand(std::string)
//    {
//        // TODO build process command for this concrete instance
//    }

    std::string DroneFileRun::retrieveCommand()
    {
        // TODO build function to retrieve command from ICommandRead interface
//        droneSimulator::ICommandRead *commandRead;
//        commandRead->setFileName( fileName );

        std::string nextCommand;

        return nextCommand;
    }

    void DroneFileRun::initializeDrone()
    {
        // TODO  Initialize drone for this concrete instance
    }

} /* namespace droneSimulator */
