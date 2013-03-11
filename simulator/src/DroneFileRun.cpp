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
        fileName = FILENAME;    // "src/droneFlightSequence.txt";
//        fileName = "src/droneFlightSequence.txt";
//        fileName = "../src/droneFlightSequence.txt";
    }

    DroneFileRun::~DroneFileRun()
    {
//        commandRead.~CommandRead();
    }

    std::string DroneFileRun::retrieveCommand()
    {
//        std::string cmdRead = commandRead.getFileName();
//        commandRead->setFileName( fileName );
        usleep( waitTime );

        std::string nextCommand;

        nextCommand = commandRead.readPort( fileName );

//        std::cout << "Retrieved nextCommand: " << nextCommand << std::endl;

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
