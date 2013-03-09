/*
 * CommandRead.cpp
 *
 *  Created on: Mar 6, 2013
 *      Author: Oscar Wurdinger
 */

#include "CommandRead.h"

namespace droneSimulator
{
    CommandRead::CommandRead()
    {
        // TODO Auto-generated constructor stub
        std::cout << "Hi from CommandRead implementation!" << std::endl;
    }

    int droneSimulator::CommandRead::openPort()
    {
        droneFlightSequence.open( "droneFlightSequence.txt" );

        if ( droneFlightSequence.is_open() )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    std::string droneSimulator::CommandRead::readPort(std::string name)
    {

        // TODO Finish this function!
//        droneFlightSequence.open
        droneFlightSequence.getline( commandRetrieval, 100 );


        return name;
    }

    int droneSimulator::CommandRead::setPort(int fd)
    {
        // TODO Generate setting to set up the port
        return 0;
    }

    CommandRead::~CommandRead()
    {

    }
} // namespace droneSimulator
