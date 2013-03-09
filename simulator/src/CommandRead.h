/*
 * CommandRead.h
 *
 *  Created on: Mar 6, 2013
 *      Author: Oscar Wurdinger
 */

#ifndef COMMANDREAD_H_
#define COMMANDREAD_H_

#include "interfaces/ICommandRead.h"

namespace droneSimulator
{

class CommandRead: public virtual droneSimulator::ICommandRead
{
private:
    std::string comCall;                            /*  command called from next line  */
    std::string fileName;                           /*  Name of the file that was opened  */
    std::ifstream droneFlightSequence;              /*  File stream to the command input file.  */
//    std::istream droneFlightSequence;
    char* commandRetrieval;
public:
    CommandRead();                                  /*  Constructor  */
    ~CommandRead();                                 /*  Destructor  */
    int openPort();                                 /*  opens port to file for reading  */
    std::string readPort(std::string);              /*  reads file and returns line back to caller  */
    int setPort(int);                               /*  sets file or port name to be used for read  */
};

} /* namespace droneSimulator */
#endif /* COMMANDREAD_H_ */
