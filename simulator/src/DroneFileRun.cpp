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
        fileName = FILENAME;
    }

    DroneFileRun::~DroneFileRun()
    {

    }

    std::string DroneFileRun::retrieveCommand()
    {
        usleep( waitTime );

        std::string nextCommand;

        nextCommand = commandRead.readPort( fileName );

        return nextCommand;
    }

    void DroneFileRun::initializeDrone()
    {
        commandRead.setFileName( fileName );

        if ( commandRead.openPort() )
        {
            std::cout << "File has been opened!" << std::endl;
        }
        else
        {
            std::cout << "File was not opened!" << std::endl;
        }
    }

} /* namespace droneSimulator */
