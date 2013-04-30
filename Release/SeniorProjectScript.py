#!/usr/bin/python

import subprocess

def RunSimulator():
    returnCode = 0
    while returnCode == 0:
        returnCode = subprocess.call("./DroneSimulator")

def main():
    print "Running main!"
    RunSimulator()

if __name__ == "__main__":
    main()
