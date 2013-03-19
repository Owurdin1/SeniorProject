/*
 * CommandRead.h
 *
 *  Created on: Mar 6, 2013
 *      Author: Oscar Wurdinger
 */

#ifndef COMMANDREAD_H_
#define COMMANDREAD_H_

#define MINIMUM_COMMAND_SIZE 1                      /*  shortest command is Up therefore must be at least 1  */

#include "interfaces/ICommandRead.h"

namespace droneSimulator
{

    class CommandRead : public ICommandRead
    {
    private:
        /*  Private Class Variables  */
        char* commandRetrieval;
        std::string comCall;                            /*  command called from next line  */
        std::string fileName;                           /*  Name of the file that was opened  */
        std::ifstream droneFlightSequence;              /*  File stream to the command input file.  */

    public:
        CommandRead();                                  /*  Constructor  */
        ~CommandRead();                                 /*  Destructor  */
        int openPort();                                 /*  opens port to file for reading  */
        std::string readPort(std::string);              /*  reads file and returns line back to caller  */
        int setPort(int);                               /*  sets file or port name to be used for read  */
    };

} /* namespace droneSimulator */
#endif /* COMMANDREAD_H_ */
