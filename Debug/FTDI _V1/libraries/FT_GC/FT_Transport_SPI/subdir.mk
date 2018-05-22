################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../FTDI\ _V1/libraries/FT_GC/FT_Transport_SPI/SPI.cpp 

OBJS += \
./FTDI\ _V1/libraries/FT_GC/FT_Transport_SPI/SPI.o 

CPP_DEPS += \
./FTDI\ _V1/libraries/FT_GC/FT_Transport_SPI/SPI.d 


# Each subdirectory must supply rules for building sources it contributes
FTDI\ _V1/libraries/FT_GC/FT_Transport_SPI/SPI.o: ../FTDI\ _V1/libraries/FT_GC/FT_Transport_SPI/SPI.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -std=gnu++11 -fpermissive -I"/root/share/Code/AVRLibrary" -Wall -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -Wl,-u,vfprintf -lprintf_flt -lm -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"FTDI _V1/libraries/FT_GC/FT_Transport_SPI/SPI.d" -MT"FTDI\ _V1/libraries/FT_GC/FT_Transport_SPI/SPI.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


