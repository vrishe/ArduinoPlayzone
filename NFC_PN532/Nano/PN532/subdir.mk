################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../PN532/PN532.cpp \
../PN532/emulatetag.cpp \
../PN532/llcp.cpp \
../PN532/mac_link.cpp \
../PN532/snep.cpp 

OBJS += \
./PN532/PN532.o \
./PN532/emulatetag.o \
./PN532/llcp.o \
./PN532/mac_link.o \
./PN532/snep.o 

CPP_DEPS += \
./PN532/PN532.d \
./PN532/emulatetag.d \
./PN532/llcp.d \
./PN532/mac_link.d \
./PN532/snep.d 


# Each subdirectory must supply rules for building sources it contributes
PN532/%.o: ../PN532/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Soft\dev\uClibc++" -I"D:\Soft\dev\arduino\cores\arduino" -I"D:\Soft\dev\arduino\variants\eightanaloginputs" -I"D:\Documents\Arduino\libraries\PN532\PN532" -DARDUINO=163 -Wall -g2 -gstabs -Os -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


