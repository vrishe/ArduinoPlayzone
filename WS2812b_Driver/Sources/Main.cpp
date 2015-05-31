/*
 * Main.cpp
 *
 *  Created on: 31 џэт. 2015 у.
 *      Author: Admin
 */

#include <Arduino.h>
#include <stdint.h>


#define BLINK_TIMES(n) ((n) + 1)

static uint8_t blinkTimes = 0;

void loop() {
	if (++blinkTimes % BLINK_TIMES(4) > 0) {
		digitalWrite(13, HIGH);
		delay(196);
		digitalWrite(13, LOW);
		delay(128);
	} else {
		blinkTimes = 0;

		delay(1024);
	}
}

void setup() {
	pinMode(13, OUTPUT);
}
