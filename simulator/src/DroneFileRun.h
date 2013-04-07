/*
 * Drone.h
 *
 *  Created on: Mar 8, 2013
 *      Author: Oscar Wurdinger
 */

#ifndef DRONE_H_
#define DRONE_H_

#define FILENAME "src/droneFlightSequence.txt"
//#define FILENAME "../src/droneFlightSequence.txt"

#include "interfaces/IDrone.h"
#include "CommandRead.h"
#include <unistd.h>

namespace droneSimulator
{

class DroneFileRun: public droneSimulator::IDrone // virtual droneSimulator::IDrone
{
private:
    static const unsigned int waitTime = 30000;       /*  30 millisecond wait time to mimic drone  */
//    static const unsigned int waitTime = 0;     /*  0 wait time for testing purposes  */
    CommandRead commandRead;
public:
    DroneFileRun();
    ~DroneFileRun();

    // Virtual functions
    void initializeDrone();                         /*  Initializes the drone connecting to the command interface.  */
    std::string retrieveCommand();                  /*  retrieves command from command interface  */
};

} /* namespace droneSimulator */
#endif /* DRONE_H_ */
