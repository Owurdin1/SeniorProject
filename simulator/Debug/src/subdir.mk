################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CommandRead.cpp \
../src/Drone.cpp \
../src/DroneSimulator.cpp 

OBJS += \
./src/CommandRead.o \
./src/Drone.o \
./src/DroneSimulator.o 

CPP_DEPS += \
./src/CommandRead.d \
./src/Drone.d \
./src/DroneSimulator.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


