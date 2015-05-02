///*
// * Main.cpp
// *
// *  Created on: 31 џэт. 2015 у.
// *      Author: Admin
// */
//
//#include <Arduino.h>
//#include <stdint.h>
//
//#include <PN532.h>
//#include "PN532_HSU/PN532_HSU.h"
//#include <NfcAdapter.h>
//
//
//#define LED_GREEN 	12
//#define LED_RED		11
//
//String const myUID = "1B B3 C6 EF";
//
//
//PN532_HSU interface(Serial);
//NfcAdapter nfc(interface);
//
//void setup() {
//	Serial.begin(115200);
//    nfc.begin(); // begin NFC comm
//
//	pinMode(13, OUTPUT);
//	pinMode(LED_GREEN, OUTPUT);
//	pinMode(LED_RED, OUTPUT);
//
//	digitalWrite(LED_GREEN, HIGH);
//	delay(500);
//	digitalWrite(LED_GREEN, LOW);
//	digitalWrite(LED_RED, HIGH);
//	delay(500);
//	digitalWrite(LED_RED, LOW);
//	digitalWrite(13, LOW);
//}
//
//void loop() {
//	if (nfc.tagPresent()) // check if an NFC tag is present on the antenna area
//	{
//		NfcTag tag = nfc.read(); // read the NFC tag
//		String scannedUID = tag.getUidString(); // get the NFC tag's UID
//
//		if( myUID.compareTo(scannedUID) == 0) // compare the NFC tag's UID with the correct tag's UID (a match exists when compareTo returns 0)
//		{
//			// The correct NFC tag was used
//			Serial.println("Correct Key");
//			// Blink the green LED and make sure the RED led is off
//			digitalWrite(LED_GREEN, HIGH);
//			digitalWrite(LED_RED, LOW);
//
//			delay(500);
//			digitalWrite(LED_GREEN, LOW);
//			delay(500);
//			digitalWrite(LED_GREEN, HIGH);
//			delay(500);
//			digitalWrite(LED_GREEN, LOW);
//			// put your here to trigger the unlocking mechanism (e.g. motor, transducer)
//		}else{
//			// an incorrect NFC tag was used
//			Serial.println("Incorrect key");
//			// blink the red LED and make sure the green LED is off
//			digitalWrite(LED_GREEN, LOW);
//			digitalWrite(LED_RED, HIGH);
//
//			delay(500);
//			digitalWrite(LED_RED, LOW);
//			delay(500);
//			digitalWrite(LED_RED, HIGH);
//			delay(500);
//			digitalWrite(LED_RED, LOW);
//			// DO NOT UNLOCK! an incorrect NFC tag was used.
//			// put your code here to trigger an alarm (e.g. buzzard, speaker) or do something else
//		}
//	}
//	delay(2000);
//}

#include <Arduino.h>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <stdint.h>
#include <stdlib.h>

#include <exception>
#include <limits>

#include <llcp.h>
#include <snep.h>
#include <NdefMessage.h>
#include <PN532_Debug.h>

#include "AltSoftSerial/AltSoftSerial.h"
#include "PN532_HSU/PN532_HSU.h"


static AltSoftSerial serialDbg;

PN532_LOG(serialDbg)

static PN532_HSU nfcComm(Serial);
static LLCP llcp(nfcComm);
static SNEP snep(nfcComm);

void setup()
{
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);

	serialDbg.begin(9600);
	serialDbg.println("Android Beam to PN532 communication program. Welcome!");
	serialDbg.flush();

	int8_t activation_result;
	if ((activation_result = llcp.activate()) <= 0) {
    	serialDbg.print("Can't activate by LLCP (");
    	serialDbg.print(activation_result);
    	serialDbg.print(").");

    	do {
    		digitalWrite(13, HIGH);
    		delay(1000);

    		digitalWrite(13, LOW);
    		delay(1000);
    	} while(1);
	}
	serialDbg.println("PN532 is waiting for an LLCP connection from now.");
}


static uint8_t payloadBuf[1024];

void loop()
{
	serialDbg.flush();
	if (llcp.waitForConnection() > 0
			&& llcp.connect() > 0) {

		serialDbg.println("Connection established. Reading message by SNEP...");
		serialDbg.flush();
//
//		int messageSize = snep.read(payloadBuf, _countof(payloadBuf));
//
//		if (messageSize > 0) {
//			NdefMessage message  = NdefMessage(payloadBuf, messageSize);
//			{
//				message.print(serialDbg);
//			}
//			serialDbg.println("\nSuccess");
//		} else {
//			serialDbg.println("failed");
//		}
//		serialDbg.flush();
//		llcp.disconnect();
//
//		serialDbg.println("Disconnected.");
	}
	delay(1000);
}
