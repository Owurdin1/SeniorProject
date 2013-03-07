/*
 * CommandRead.h
 *
 *  Created on: Mar 6, 2013
 *      Author: postholes
 */

#ifndef COMMANDREAD_H_
#define COMMANDREAD_H_

#include "interfaces/ICommandRead.h"

namespace droneSimulator
{

class CommandRead: public virtual droneSimulator::ICommandRead
{
public:
    CommandRead();
    ~CommandRead();
    int openPort();
    char readPort(int);
    int setPort(int);
};

} /* namespace droneSimulator */
#endif /* COMMANDREAD_H_ */
