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
        // TODO open port to file
        return 1001;
    }

    char droneSimulator::CommandRead::readPort(int fd)
    {
        // TODO Generate read for opened file
        return (char)fd;
    }

    int droneSimulator::CommandRead::setPort(int fd)
    {
        // TODO Generate setting to set up the port
        return 0;
    }
} // namespace droneSimulator
