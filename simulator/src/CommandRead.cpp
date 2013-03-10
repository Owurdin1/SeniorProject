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
        const char* file;
        file = getFileName().c_str();
        droneFlightSequence.open( file );

        std::cout << "file name opening = " << file << std::endl;
        return droneFlightSequence.is_open();
    }

    std::string droneSimulator::CommandRead::readPort(std::string name)
    {
        std::string newCommand;
        bool commandString = false;

        while ( !commandString )
        {
            std::getline( droneFlightSequence, newCommand );
            name = newCommand;
            if ( name.length() > MINIMUM_COMMAND_SIZE )
            {
                commandString = true;
            }
        }
//        std::cout << "Gotten lines returning name: " << name << std::endl;

        return name;
    }

    int droneSimulator::CommandRead::setPort(int fd)
    {
        std::cout << "This function isn't needed for this implementation" << std::endl;
        return 0;
    }

    CommandRead::~CommandRead()
    {
        /* Close the file when finished with this class.  */
        std::cout << "Closing droneFlightSequence file" << std::endl;
        droneFlightSequence.close();
    }
} // namespace droneSimulator
