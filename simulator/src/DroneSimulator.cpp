//============================================================================
// Name        : DroneSimulator.cpp
// Author      : Oscar Wurdinger
// Version     :
// Copyright   : Open Source ask if you would like source files
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "CommandRead.h"

using namespace std;

int main()
{
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

//	droneSimulator::ICommandRead* cmdRead = new droneSimulator::ICommandRead();
//	droneSimulator::CommandRead* cmdRead = new droneSimulator::CommandRead();
//	int fd = cmdRead->openPort();
//
//	cout << "fd var is: " << fd << endl;

	droneSimulator::ICommandRead *cmdRead = new droneSimulator::CommandRead();

//	int fd = cmdRead->openPort();
//	cout << "FD = " << fd << endl;

    string fileName = cmdRead->getFileName();
    fileName = "src/droneFlightSequence.txt";
    cmdRead->setFileName( fileName );

	if ( cmdRead->openPort() )
	{
	    cout << "File has been opened let's do some stuff with it!" << endl;
	}
	else
	{
	    cout << "File wasn't opened, it said \"FUCK OFF!\" " << endl;
	}

	delete cmdRead;
	cout << "Have \"Deleted\" cmdRead" << endl;

	return 0;
}
