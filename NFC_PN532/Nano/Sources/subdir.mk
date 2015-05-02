################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
D:/Documents/Arduino/libraries/PN532/PN532/PN532.cpp \
D:/Documents/Arduino/libraries/PN532/PN532/emulatetag.cpp \
D:/Documents/Arduino/libraries/PN532/PN532/llcp.cpp \
D:/Documents/Arduino/libraries/PN532/PN532/mac_link.cpp \
D:/Documents/Arduino/libraries/PN532/PN532/snep.cpp 

OBJS += \
./Sources/PN532.o \
./Sources/emulatetag.o \
./Sources/llcp.o \
./Sources/mac_link.o \
./Sources/snep.o 

CPP_DEPS += \
./Sources/PN532.d \
./Sources/emulatetag.d \
./Sources/llcp.d \
./Sources/mac_link.d \
./Sources/snep.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/PN532.o: D:/Documents/Arduino/libraries/PN532/PN532/PN532.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Soft\dev\uClibc++" -I"D:\Soft\dev\arduino\cores\arduino" -I"D:\Soft\dev\arduino\variants\eightanaloginputs" -I"D:\Documents\Arduino\libraries\PN532\PN532" -DARDUINO -Wall -g2 -gstabs -Os -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/emulatetag.o: D:/Documents/Arduino/libraries/PN532/PN532/emulatetag.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Soft\dev\uClibc++" -I"D:\Soft\dev\arduino\cores\arduino" -I"D:\Soft\dev\arduino\variants\eightanaloginputs" -I"D:\Documents\Arduino\libraries\PN532\PN532" -DARDUINO -Wall -g2 -gstabs -Os -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/llcp.o: D:/Documents/Arduino/libraries/PN532/PN532/llcp.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Soft\dev\uClibc++" -I"D:\Soft\dev\arduino\cores\arduino" -I"D:\Soft\dev\arduino\variants\eightanaloginputs" -I"D:\Documents\Arduino\libraries\PN532\PN532" -DARDUINO -Wall -g2 -gstabs -Os -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/mac_link.o: D:/Documents/Arduino/libraries/PN532/PN532/mac_link.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Soft\dev\uClibc++" -I"D:\Soft\dev\arduino\cores\arduino" -I"D:\Soft\dev\arduino\variants\eightanaloginputs" -I"D:\Documents\Arduino\libraries\PN532\PN532" -DARDUINO -Wall -g2 -gstabs -Os -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/snep.o: D:/Documents/Arduino/libraries/PN532/PN532/snep.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Soft\dev\uClibc++" -I"D:\Soft\dev\arduino\cores\arduino" -I"D:\Soft\dev\arduino\variants\eightanaloginputs" -I"D:\Documents\Arduino\libraries\PN532\PN532" -DARDUINO -Wall -g2 -gstabs -Os -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


