//============================================================================
// Name        : DroneSimulator.cpp
// Author      : Oscar Wurdinger
// Version     :
// Copyright   : Open Source ask if you would like source files
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "interfaces/ICommandRead.h"
using namespace std;

int main()
{
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	using droneSimulator;
	ICommandRead cmdRead = new ICommandRead();
	cmdRead.ICommandRead();
	return 0;
}
