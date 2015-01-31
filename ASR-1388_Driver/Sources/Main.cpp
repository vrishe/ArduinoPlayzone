/*
 * Main.cpp
 *
 *  Created on: 31 џэт. 2015 у.
 *      Author: Admin
 */

#include "Main.h"

#include "Driver.h"
#include "error.h"


extern "C" void __cxa_pure_virtual() {
  for(;;);
}

static const byte sprite[] = { 0, 36, 36, 36, 0, 102, 60, 0 }; // =)


#define PIN_CLOCK 	12
#define PIN_LATCH 	 8
#define PIN_DATA	11

static asr::Driver matrix;

void setup() {
	matrix.Initialize(PIN_CLOCK, PIN_LATCH, PIN_DATA);

	pinMode(13, OUTPUT);
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
