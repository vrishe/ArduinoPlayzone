/*
 * Main.cpp
 *
 *  Created on: 31 џэт. 2015 у.
 *      Author: Admin
 */

#include "Main.h"

#include <Arduino.h>
#include "cstdlib_ext.h"

#include "Driver.h"
#include "error.h"


extern "C" void __cxa_pure_virtual() {
  for(;;);
}

static const byte sprite[] = { 0, 36, 36, 36, 0, 102, 60, 0 }; // =)


#define PIN_CLOCK 	13
#define PIN_LATCH 	12
#define PIN_DATA	 4

static asr::Driver matrix;

#ifdef DEBUG
static void toggleSlowDown() {
	matrix.setScanLineDelay(matrix.getScanLineDelay() > 0 ? 0 : 32);
}
#endif

void setup() {
	matrix.Initialize(PIN_CLOCK, PIN_LATCH, PIN_DATA);

#ifdef DEBUG
	pinMode(3, INPUT_PULLUP);

	attachInterrupt(1, toggleSlowDown, RISING);
#endif
}

void loop() {
	matrix.DrawSprite(0, 0, sprite, _countof(sprite));
}


int main(void) {
	init();
	setup();

	for(;;) {
		loop();
	}
	return 0; // Never reached.
}
