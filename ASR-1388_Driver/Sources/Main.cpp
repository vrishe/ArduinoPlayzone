/*
 * Main.cpp
 *
 *  Created on: 31 џэт. 2015 у.
 *      Author: Admin
 */

#include "Graphics/Sprite.h"
#include "ASRScreen.h"
#include "ASRSprite.h"
#include "cstdlib_ext.h"

#include <Arduino.h>


//static const byte sprite[] = { 0, 36, 36, 36, 0, 102, 60, 0 }; // =)


#define PIN_CLOCK 	13
#define PIN_LATCH 	12
#define PIN_DATA	 4

static asr::Screen screen;
static _2d::Object<uint8_t> s(8, 8);

#ifdef DEBUG
static void toggleSlowDown() {
	screen.setScanLineDelay(screen.getScanLineDelay() > 0 ? 0 : 32);
}
#endif

#include "array_t.h"


void setup() {
	screen.init(PIN_CLOCK, PIN_LATCH, PIN_DATA);

#ifdef DEBUG
	pinMode(3, INPUT_PULLUP);

	attachInterrupt(1, toggleSlowDown, RISING);
#endif
}

void loop() {
//	screen.display();
}
