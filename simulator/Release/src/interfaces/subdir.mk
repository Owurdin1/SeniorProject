################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/interfaces/ICommandRead.cpp \
../src/interfaces/IDrone.cpp 

OBJS += \
./src/interfaces/ICommandRead.o \
./src/interfaces/IDrone.o 

CPP_DEPS += \
./src/interfaces/ICommandRead.d \
./src/interfaces/IDrone.d 


# Each subdirectory must supply rules for building sources it contributes
src/interfaces/%.o: ../src/interfaces/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


