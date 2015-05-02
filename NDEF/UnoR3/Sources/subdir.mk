################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
D:/Documents/Arduino/libraries/PN532/NDEF/MifareClassic.cpp \
D:/Documents/Arduino/libraries/PN532/NDEF/MifareUltralight.cpp \
D:/Documents/Arduino/libraries/PN532/NDEF/Ndef.cpp \
D:/Documents/Arduino/libraries/PN532/NDEF/NdefMessage.cpp \
D:/Documents/Arduino/libraries/PN532/NDEF/NdefRecord.cpp \
D:/Documents/Arduino/libraries/PN532/NDEF/NfcAdapter.cpp \
D:/Documents/Arduino/libraries/PN532/NDEF/NfcTag.cpp 

OBJS += \
./Sources/MifareClassic.o \
./Sources/MifareUltralight.o \
./Sources/Ndef.o \
./Sources/NdefMessage.o \
./Sources/NdefRecord.o \
./Sources/NfcAdapter.o \
./Sources/NfcTag.o 

CPP_DEPS += \
./Sources/MifareClassic.d \
./Sources/MifareUltralight.d \
./Sources/Ndef.d \
./Sources/NdefMessage.d \
./Sources/NdefRecord.d \
./Sources/NfcAdapter.d \
./Sources/NfcTag.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/MifareClassic.o: D:/Documents/Arduino/libraries/PN532/NDEF/MifareClassic.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Soft\dev\uClibc++" -I"D:\Soft\dev\arduino\cores\arduino" -I"D:\Soft\dev\arduino\variants\eightanaloginputs" -I"D:\Documents\Arduino\libraries\PN532\PN532" -I"D:\Documents\Arduino\libraries\PN532\NDEF" -DARDUINO=163 -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/MifareUltralight.o: D:/Documents/Arduino/libraries/PN532/NDEF/MifareUltralight.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Soft\dev\uClibc++" -I"D:\Soft\dev\arduino\cores\arduino" -I"D:\Soft\dev\arduino\variants\eightanaloginputs" -I"D:\Documents\Arduino\libraries\PN532\PN532" -I"D:\Documents\Arduino\libraries\PN532\NDEF" -DARDUINO=163 -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Ndef.o: D:/Documents/Arduino/libraries/PN532/NDEF/Ndef.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Soft\dev\uClibc++" -I"D:\Soft\dev\arduino\cores\arduino" -I"D:\Soft\dev\arduino\variants\eightanaloginputs" -I"D:\Documents\Arduino\libraries\PN532\PN532" -I"D:\Documents\Arduino\libraries\PN532\NDEF" -DARDUINO=163 -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/NdefMessage.o: D:/Documents/Arduino/libraries/PN532/NDEF/NdefMessage.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Soft\dev\uClibc++" -I"D:\Soft\dev\arduino\cores\arduino" -I"D:\Soft\dev\arduino\variants\eightanaloginputs" -I"D:\Documents\Arduino\libraries\PN532\PN532" -I"D:\Documents\Arduino\libraries\PN532\NDEF" -DARDUINO=163 -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/NdefRecord.o: D:/Documents/Arduino/libraries/PN532/NDEF/NdefRecord.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Soft\dev\uClibc++" -I"D:\Soft\dev\arduino\cores\arduino" -I"D:\Soft\dev\arduino\variants\eightanaloginputs" -I"D:\Documents\Arduino\libraries\PN532\PN532" -I"D:\Documents\Arduino\libraries\PN532\NDEF" -DARDUINO=163 -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/NfcAdapter.o: D:/Documents/Arduino/libraries/PN532/NDEF/NfcAdapter.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Soft\dev\uClibc++" -I"D:\Soft\dev\arduino\cores\arduino" -I"D:\Soft\dev\arduino\variants\eightanaloginputs" -I"D:\Documents\Arduino\libraries\PN532\PN532" -I"D:\Documents\Arduino\libraries\PN532\NDEF" -DARDUINO=163 -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/NfcTag.o: D:/Documents/Arduino/libraries/PN532/NDEF/NfcTag.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\Soft\dev\uClibc++" -I"D:\Soft\dev\arduino\cores\arduino" -I"D:\Soft\dev\arduino\variants\eightanaloginputs" -I"D:\Documents\Arduino\libraries\PN532\PN532" -I"D:\Documents\Arduino\libraries\PN532\NDEF" -DARDUINO=163 -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -ffunction-sections -fdata-sections -std=gnu++11 -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


