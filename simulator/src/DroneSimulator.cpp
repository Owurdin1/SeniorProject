//============================================================================
// Name        : DroneSimulator.cpp
// Author      : Oscar Wurdinger
// Version     :
// Copyright   : Open Source ask if you would like source files
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "DroneFileRun.h"

using namespace std;

int main()
{
    droneSimulator::DroneFileRun dfRun;
    dfRun.initializeDrone();
    dfRun.flyDrone();

	return 0;
}
