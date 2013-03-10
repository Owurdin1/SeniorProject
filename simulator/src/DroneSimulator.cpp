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
//	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	droneSimulator::ICommandRead *cmdRead = new droneSimulator::CommandRead();

	string currentCommand;
    string fileName = cmdRead->getFileName();

//    ==============================================
//    fileName = "../src/droneFlightSequence.txt";                /*  Use this call to run in terminal  */
    fileName = "src/droneFlightSequence.txt";                   /*  Use this to run in Eclipse  */
//    ==============================================

    cmdRead->setFileName( fileName );

	if ( cmdRead->openPort() )
	{
	    cout << "File has been opened let's do some stuff with it!" << endl;

        for ( int i = 0; i < 10; i++ )
        {
            currentCommand = cmdRead->readPort( fileName );

            cout << "currentCommand = " << currentCommand << endl;
        }
	}
	else
	{
	    cout << "File wasn't opened" << endl;
	}


	delete cmdRead;
	cout << "Reached end of Main function" << endl;

	return 0;
}
