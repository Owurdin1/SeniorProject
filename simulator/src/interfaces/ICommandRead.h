/*
 * ICommandRead.h
 *
 *  Created on: Mar 6, 2013
 *      Author: Oscar Wurdinger
 */

#ifndef ICOMMANDREAD_H_
#define ICOMMANDREAD_H_

#include <iostream>
#include <string>

namespace droneSimulator
{

    class ICommandRead
    {
    public:
//        virtual void ICommandRead();
        virtual int openPort() = 0;
        virtual int setPort(int) = 0;
        virtual char readPort(int) = 0;
        virtual ~ICommandRead() {};
    };

} // namespace droneSimulator
#endif // ICOMMANDREAD_H_
