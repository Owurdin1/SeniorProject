/*
 * ICommandRead.cpp
 *
 *  Created on: Mar 6, 2013
 *      Author: Oscar Wurdinger
 */

#include "ICommandRead.h"

namespace droneSimulator
{
    /*
     * Accessor for the fileName variable.
     */
    std::string ICommandRead::getFileName()
    {
        return fileName;
    }

    /*
     * Mutator for fileName variable. Returns a boolean
     * based on success of change.
     */
    bool ICommandRead::setFileName(std::string newFileName)
    {
        fileName = newFileName;

        // make sure that newFileName has been set properly
        if (fileName.compare(newFileName) == 0)
        {
            return true;
        }

        return false;
    }

    /*
     * Accessor for the currentCommand variable
     */
    std::string ICommandRead::getCurrentCommand()
    {
        return currentCommand;
    }
} // namespace droneSimulator
