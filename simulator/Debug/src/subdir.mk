################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CommandRead.cpp \
../src/DroneSimulator.cpp \
../src/ICommandRead.cpp 

OBJS += \
./src/CommandRead.o \
./src/DroneSimulator.o \
./src/ICommandRead.o 

CPP_DEPS += \
./src/CommandRead.d \
./src/DroneSimulator.d \
./src/ICommandRead.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


