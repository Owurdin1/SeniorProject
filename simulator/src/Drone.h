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

class Drone: public droneSimulator::IDrone // virtual droneSimulator::IDrone
{
public:
    Drone();
    ~Drone();

    // Functions to be overridden
//    void processCommand( std::string );
    void initializeDrone();
};

} /* namespace droneSimulator */
#endif /* DRONE_H_ */
