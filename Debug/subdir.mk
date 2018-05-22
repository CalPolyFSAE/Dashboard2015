################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ADCManager.cpp \
../CANLib.cpp \
../CPFERotarySwitch.cpp \
../Input.cpp \
../Screen.cpp \
../Subsystem.cpp \
../main.cpp 

OBJS += \
./ADCManager.o \
./CANLib.o \
./CPFERotarySwitch.o \
./Input.o \
./Screen.o \
./Subsystem.o \
./main.o 

CPP_DEPS += \
./ADCManager.d \
./CANLib.d \
./CPFERotarySwitch.d \
./Input.d \
./Screen.d \
./Subsystem.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -std=gnu++11 -fpermissive -I"/root/share/Code/AVRLibrary" -Wall -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -Wl,-u,vfprintf -lprintf_flt -lm -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


