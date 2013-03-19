/*
 * Drone.cpp
 *
 *  Created on: Mar 8, 2013
 *      Author: Oscar Wurdinger
 */

#include "DroneFileRun.h"

namespace droneSimulator
{

    /*
     * Accessor for fileName variable
     */
    DroneFileRun::DroneFileRun()
    {
        fileName = FILENAME;
    }

    /*
     * Destructor for DroneFileRun
     */
    DroneFileRun::~DroneFileRun()
    {

    }

    /*
     * Retrieves command from the file and returns
     * that value in a string
     */
    std::string DroneFileRun::retrieveCommand()
    {
        usleep( waitTime );

        std::string nextCommand;

        nextCommand = commandRead.readPort( fileName );

        return nextCommand;
    }

    /*
     * Initializes the drone to run.
     */
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
