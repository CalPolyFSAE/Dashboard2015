################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DashPages/FE/Charging.cpp \
../DashPages/FE/FEDash.cpp \
../DashPages/FE/FEDashStr.cpp 

OBJS += \
./DashPages/FE/Charging.o \
./DashPages/FE/FEDash.o \
./DashPages/FE/FEDashStr.o 

CPP_DEPS += \
./DashPages/FE/Charging.d \
./DashPages/FE/FEDash.d \
./DashPages/FE/FEDashStr.d 


# Each subdirectory must supply rules for building sources it contributes
DashPages/FE/%.o: ../DashPages/FE/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -std=gnu++11 -fpermissive -I"/root/share/Code/AVRLibrary" -Wall -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -Wl,-u,vfprintf -lprintf_flt -lm -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


