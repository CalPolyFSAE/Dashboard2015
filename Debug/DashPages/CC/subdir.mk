################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DashPages/CC/CCDash.cpp \
../DashPages/CC/CCDashStr.cpp \
../DashPages/CC/Driving.cpp \
../DashPages/CC/Error.cpp \
../DashPages/CC/ErrorConditions.cpp 

OBJS += \
./DashPages/CC/CCDash.o \
./DashPages/CC/CCDashStr.o \
./DashPages/CC/Driving.o \
./DashPages/CC/Error.o \
./DashPages/CC/ErrorConditions.o 

CPP_DEPS += \
./DashPages/CC/CCDash.d \
./DashPages/CC/CCDashStr.d \
./DashPages/CC/Driving.d \
./DashPages/CC/Error.d \
./DashPages/CC/ErrorConditions.d 


# Each subdirectory must supply rules for building sources it contributes
DashPages/CC/%.o: ../DashPages/CC/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -std=gnu++11 -fpermissive -I"/root/share/Code/AVRLibrary" -Wall -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -Wl,-u,vfprintf -lprintf_flt -lm -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


