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
        std::string fileName;
        std::string currentCommand;
    public:
        // Contractual Functions
        virtual int openPort() = 0;
        virtual int setPort(int) = 0;
        virtual std::string readPort(std::string) = 0;
        virtual ~ICommandRead() {}

        // Interface functions
        std::string getFileName();
        bool setFileName(std::string);
        std::string getCurrentCommand();
    };

} // namespace droneSimulator
#endif // ICOMMANDREAD_H_
