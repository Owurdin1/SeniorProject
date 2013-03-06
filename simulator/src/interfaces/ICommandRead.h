/*
 * ICommandRead.h
 *
 *  Created on: Mar 6, 2013
 *      Author: Oscar Wurdinger
 */

#ifndef ICOMMANDREAD_H_
#define ICOMMANDREAD_H_

namespace droneSimulator
{

class ICommandRead
{
public:
    ICommandRead();
    int portOpen();
    bool readPort(int);
};

} // namespace droneSimulator
#endif // ICOMMANDREAD_H_
