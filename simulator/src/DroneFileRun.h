/*
 * Drone.h
 *
 *  Created on: Mar 8, 2013
 *      Author: Oscar Wurdinger
 */

#ifndef DRONE_H_
#define DRONE_H_

#include "interfaces/IDrone.h"

namespace droneSimulator
{

class DroneFileRun: public droneSimulator::IDrone // virtual droneSimulator::IDrone
{
private:
    std::string fileName;
public:
    DroneFileRun();
    ~DroneFileRun();

    // Virtual functions
    void initializeDrone();                         /*  Initializes the drone connecting to the command interface.  */
    std::string retrieveCommand();                  /*  retrieves command from command interface  */
};

} /* namespace droneSimulator */
#endif /* DRONE_H_ */
