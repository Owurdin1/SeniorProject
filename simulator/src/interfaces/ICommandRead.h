/*
 * ICommandRead.h
 *
 *  Created on: Mar 6, 2013
 *      Author: Oscar Wurdinger
 */

#ifndef ICOMMANDREAD_H_
#define ICOMMANDREAD_H_

#include <iostream>
#include <fstream>
//#include <istream>
#include <string>

namespace droneSimulator
{

    class ICommandRead
    {
    private:
        // Class variables
        std::string fileName;                               /*  name of file or connection to the open port.  */
        std::string currentCommand;                         /*  the last command that was read is stored in this variable  */
    public:
        // Contractual Functions
        virtual int openPort() = 0;                         /*  Opens port to communication device for drone  */
        virtual int setPort(int) = 0;                       /*  Sets the options for the port so that communication is possible */
        virtual std::string readPort(std::string) = 0;      /*  Reads the data from open port. It takes a string value which is the port ID */
        virtual ~ICommandRead() {}                          /*  Virtual Destructor for implementation  */

        // Interface functions
        std::string getFileName();
        bool setFileName(std::string);
        std::string getCurrentCommand();
    };

} // namespace droneSimulator
#endif // ICOMMANDREAD_H_
