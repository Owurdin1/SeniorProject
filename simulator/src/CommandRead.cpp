/*
 * CommandRead.cpp
 *
 *  Created on: Mar 6, 2013
 *      Author: Oscar Wurdinger
 */

#include "CommandRead.h"

namespace droneSimulator
{

    /*
     * constructor for CommandRead, this is
     * a default constructor. It does not do anything
     * at the moment.
     */
    CommandRead::CommandRead()
    {
        // TODO Auto-generated constructor stub
        std::cout << "Hi from CommandRead implementation!" << std::endl;
    }

    /*
     * opens a a file using the fileName variable.
     */
    int droneSimulator::CommandRead::openPort()
    {
        const char* file;
        file = getFileName().c_str();
        droneFlightSequence.open( file );

        std::cout << "file name opening = " << file << std::endl;
        return droneFlightSequence.is_open();
    }

    /*
     * reads the open file and returns the next line in the command
     * sequence text file.
     */
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
                /*
                 * command string is a boolean trigger
                 * to make sure that the new command is not
                 * a blank line.
                 */
                commandString = true;
            }
        }
//        std::cout << "Gotten lines returning name: " << name << std::endl;

        return name;
    }

    /*
     * sets the port that needs to be used, since
     * using a file to read the command text for this
     * concrete class this function isn't being used.
     */
    int droneSimulator::CommandRead::setPort(int fd)
    {
        std::cout << "This function isn't needed for this implementation" << std::endl;
        return 0;
    }

    /*
     * destructor for CommandRead class
     */
    CommandRead::~CommandRead()
    {
        /* Close the file when finished with this class.  */
//        std::cout << "Closing droneFlightSequence file" << std::endl;
        droneFlightSequence.close();
    }
} // namespace droneSimulator
