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
//        fileName = "src/droneFlightSequence.txt";

        const char* file;
        file = getFileName().c_str(); // fileName.c_str();
        droneFlightSequence.open( file ); // fileName.c_str() ); // "droneFlightSequence.txt" );

        std::cout << "file name opening = " << file << std::endl;
        return droneFlightSequence.is_open();
//        if ( droneFlightSequence.is_open() )
//        {
//            /*  A file has been opened and is ready to be read.  */
//            std::cout << "File has been opened!!!!" << std::endl;
//            return 1;
//        }
//        else
//        {
//            std::cout << "File has not been opened!" << std::endl;
//            return 0;
//        }
    }

    std::string droneSimulator::CommandRead::readPort(std::string name)
    {

        // TODO Finish this function!
//        droneFlightSequence.open
        droneFlightSequence.getline( commandRetrieval, 100 );
        std::string test(commandRetrieval);
//        name = name( commandRetrieval );
        name = test;

        std::cout << "Gotten line returning name: " << name << std::endl;


        return name;
    }

    int droneSimulator::CommandRead::setPort(int fd)
    {
        // TODO Generate setting to set up the port
        return 0;
    }

    CommandRead::~CommandRead()
    {
        /* Close the file when finished with this class.  */
        droneFlightSequence.close();
    }
} // namespace droneSimulator
