/*
 * Main.cpp
 *
 *  Created on: 31 џэт. 2015 у.
 *      Author: Admin
 */


#include "Graphics/Scene.h"
#include "ASRScreen.h"
#include "ASRSprite.h"
#include "cstdlib_ext.h"

#include <Arduino.h>


//static const byte sprite[] = { 0, 36, 36, 36, 0, 102, 60, 0 }; // =)


#define PIN_CLOCK 	13
#define PIN_LATCH 	12
#define PIN_DATA	 4

static asr::Screen screen;
static asr::Scene  scene	(8, 8);
static asr::Sprite smiley	(8, 8);


#ifdef DEBUG
static void toggleSlowDown() {
	screen.setScanLineDelay(screen.getScanLineDelay() > 0 ? 0 : 32);
}
#endif

void setup() {

#ifdef DEBUG
	attachInterrupt(1, toggleSlowDown, RISING);
	pinMode(3, INPUT_PULLUP);
#endif

	screen.init(PIN_CLOCK, PIN_LATCH, PIN_DATA);
	{
		uint8_t *spriteData;

		spriteData = smiley.getValueAt(0);
		spriteData[0] = 0x00;
		spriteData[1] = 0x24;
		spriteData[2] = 0x24;
		spriteData[3] = 0x24;
		spriteData[4] = 0x00;
		spriteData[5] = 0x66;
		spriteData[6] = 0x3c;
		spriteData[7] = 0x00;
	}
	scene.add(smiley);

	Serial.begin(57600);
}


static bool refresh = true;

void loop() {
	while (Serial.available()) {
		switch (Serial.read()) {
		case 'l':
			smiley.MoveBy(-1, 0);
			refresh = true;
			break;
		case 'u':
			smiley.MoveBy(0, -1);
			refresh = true;
			break;
		case 'r':
			smiley.MoveBy(1, 0);
			refresh = true;
			break;
		case 'd':
			smiley.MoveBy(0, 1);
			refresh = true;
			break;
		}
	}
	if (refresh) {
		refresh = false;

		screen.flush();
		scene.renderTo(screen);
	}
	screen.display();
}
