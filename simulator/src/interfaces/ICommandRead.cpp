/*
 * ICommandRead.cpp
 *
 *  Created on: Mar 6, 2013
 *      Author: Oscar Wurdinger
 */

#include "ICommandRead.h"

namespace droneSimulator
{
    std::string ICommandRead::getFileName()
    {
        return fileName;
    }

    bool ICommandRead::setFileName(std::string newFileName)
    {
        fileName = newFileName;

        // make sure that newFileName has been set properly
        if ( fileName.compare( newFileName ) == 0 )
        {
            return true;
        }

        return false;
    }

    std::string ICommandRead::getCurrentCommand()
    {
        return currentCommand;
    }
} // namespace droneSimulator
