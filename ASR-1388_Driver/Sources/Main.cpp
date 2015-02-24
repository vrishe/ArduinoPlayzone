/*
 * Main.cpp
 *
 *  Created on: 31 џэт. 2015 у.
 *      Author: Admin
 */


#include "ASRScreen.h"
#include "ASRSprite.h"
#include "ASRTextScroller.h"
#include "cstdlib_ext.h"

#include <Arduino.h>


#define PIN_CLOCK 	13
#define PIN_LATCH 	12
#define PIN_DATA	 4

static asr::Screen screen;

#ifdef DEBUG
static void toggleSlowDown() {
	static bool ignore = true;

	if (ignore) {
		ignore = false;

		return;
	}
	screen.setScanLineDelay(screen.getScanLineDelay() > 0 ? 0 : 32);
}
#endif


static asr::Scene  scene	(8, 8);
//static asr::Sprite smiley	(8, 8);

//static void initScene() {
//
//	uint8_t *spriteData;
//
//	spriteData = smiley.getValueAt(0);
//	spriteData[0] = 0x00;
//	spriteData[1] = 0x24;
//	spriteData[2] = 0x24;
//	spriteData[3] = 0x24;
//	spriteData[4] = 0x00;
//	spriteData[5] = 0x66;
//	spriteData[6] = 0x3c;
//	spriteData[7] = 0x00;
//
//	scene.add(smiley);
//}

static asr::TextScroller textScroller(&screen);
static asr::Sprite *font[20] = { /* zero */ };

static void initTextScroller() {
	uint8_t *spriteData;

	if ((font[0] = new (std::nothrow) asr::Sprite(5, 5)) == NULL)
		return;

	spriteData = font[0]->getValueAt(0); // D
	spriteData[0] = 0xe0;
	spriteData[1] = 0x90;
	spriteData[2] = 0x90;
	spriteData[3] = 0x90;
	spriteData[4] = 0xe0;


	if ((font[1] = new (std::nothrow) asr::Sprite(5, 5)) == NULL)
		return;

	spriteData = font[1]->getValueAt(0); // E
	spriteData[0] = 0xf0;
	spriteData[1] = 0x80;
	spriteData[2] = 0xe0;
	spriteData[3] = 0x80;
	spriteData[4] = 0xf0;


	if ((font[4] = new (std::nothrow) asr::Sprite(5, 5)) == NULL)
		return;

	spriteData = font[4]->getValueAt(0); // H
	spriteData[0] = 0x90;
	spriteData[1] = 0x90;
	spriteData[2] = 0xf0;
	spriteData[3] = 0x90;
	spriteData[4] = 0x90;


	if ((font[8] = new (std::nothrow) asr::Sprite(5, 5)) == NULL)
		return;

	spriteData = font[8]->getValueAt(0); // L
	spriteData[0] = 0x80;
	spriteData[1] = 0x80;
	spriteData[2] = 0x80;
	spriteData[3] = 0x80;
	spriteData[4] = 0xf0;


	if ((font[11] = new (std::nothrow) asr::Sprite(5, 5)) == NULL)
		return;

	spriteData = font[11]->getValueAt(0); // O
	spriteData[0] = 0x60;
	spriteData[1] = 0x90;
	spriteData[2] = 0x90;
	spriteData[3] = 0x90;
	spriteData[4] = 0x60;


	if ((font[14] = new (std::nothrow) asr::Sprite(5, 5)) == NULL)
		return;

	spriteData = font[14]->getValueAt(0); // R
	spriteData[0] = 0xe0;
	spriteData[1] = 0x90;
	spriteData[2] = 0xe0;
	spriteData[3] = 0x90;
	spriteData[4] = 0x90;


	if ((font[19] = new (std::nothrow) asr::Sprite(6, 5)) == NULL)
		return;

	spriteData = font[19]->getValueAt(0); // W
	spriteData[0] = 0x88;
	spriteData[1] = 0x88;
	spriteData[2] = 0xa8;
	spriteData[3] = 0xa8;
	spriteData[4] = 0x50;


	const char text[] = "HELLO WORLD";
	textScroller.setText(text, _countof(text) - 1, 0, 5, font, _countof(font), 'D', asr::LeftToRight);
	textScroller.placeAtRow(2);
	textScroller.setDelay(64);
}


void setup() {
	screen.init(PIN_CLOCK, PIN_LATCH, PIN_DATA);

	initTextScroller();


#ifdef DEBUG
	pinMode(3, INPUT_PULLUP);
	attachInterrupt(1, toggleSlowDown, RISING);
#endif

//	Serial.begin(57600);
}


//static bool refresh = true;

void loop() {
//	while (Serial.available()) {
//		switch (Serial.read()) {
//		case 'l':
//			smiley.MoveBy(-1, 0);
//			refresh = true;
//			break;
//		case 'u':
//			smiley.MoveBy(0, -1);
//			refresh = true;
//			break;
//		case 'r':
//			smiley.MoveBy(1, 0);
//			refresh = true;
//			break;
//		case 'd':
//			smiley.MoveBy(0, 1);
//			refresh = true;
//			break;
//		}
//	}
//	if (refresh) {
//		refresh = false;
//
//		screen.flush();
//		scene.renderTo(screen);
//	}
	textScroller.updateText(millis());
	screen.display();
}
