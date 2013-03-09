/*
 * IDrone.h
 *
 *  Created on: Mar 8, 2013
 *      Author: Oscar Wurdinger
 */

#ifndef IDRONE_H_
#define IDRONE_H_

#include <iostream>
#include <string>

namespace droneSimulator
{
    class IDrone
    {
    private:
        std::string currentState;
    public:
        // Contractual functions
        virtual void ProcessCommand(std::string) = 0;
        virtual void InitializeDrone() = 0;
        virtual ~IDrone() {}

        // Interface functions
        std::string getState();
        void changeState(std::string);
    };

} /* namespace droneSimulator */
#endif /* IDRONE_H_ */
