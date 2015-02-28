/*
 * Main.cpp
 *
 *  Created on: 31 џэт. 2015 у.
 *      Author: Admin
 */

#include <Arduino.h>
#include "cstdlib_ext.h"


#define PIN_CLOCK 	13
#define PIN_LATCH 	12
#define PIN_DATA	 4


#include "ASRScreen.h"

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


//#include "ASRSprite.h"
//
//static asr::Scene  scene	(8, 8);
//static asr::Sprite smiley	(8, 8);
//
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


#include "ASRFontASCII.h"
#include "ASRTextScroller.h"

static asr::TextHScroller textScroller(&screen);

static void initTextScroller() {
//	const char text[] = "HERE ARE SOME TEST";
//
//	textScroller.setText(
//			text, _countof(text) - 1, 0,
//		asr::FontASCII::Instance, 5,
//		asr::TextHScroller::LeftToRight
//	);
	textScroller.placeAtRow(2);
	textScroller.setDelay  (56);
}


void setup() {
	screen.init(PIN_CLOCK, PIN_LATCH, PIN_DATA);

	Serial.begin(57600);
//	initScene();
	initTextScroller();

#ifdef DEBUG
	pinMode(3, INPUT_PULLUP);
	attachInterrupt(1, toggleSlowDown, RISING);
#endif
}


void loop() {
//	while (Serial.available()) {
//		switch (Serial.read()) {
//		case 'l':
//			smiley.moveBy(-1, 0);
//			refresh = true;
//			break;
//		case 'u':
//			smiley.moveBy(0, -1);
//			refresh = true;
//			break;
//		case 'r':
//			smiley.moveBy(1, 0);
//			refresh = true;
//			break;
//		case 'd':
//			smiley.moveBy(0, 1);
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

	static bool refresh = false;

	while (Serial.available()) {
		static String buffer;

		auto character = (char)Serial.read();

		if (character == '@') {
			refresh = (buffer.length() > 0);

			if (refresh) {
				auto font = &asr::FontASCII::Instance;

				textScroller.setText(buffer.c_str(), (uint8_t)buffer.length(), 0,
						*font, font->operator [](0)->getWidth(), asr::TextHScroller::LeftToRight);

				buffer.remove(0);
			} else {
				textScroller.clear();
			}
			break;
		} else {
			buffer += character;
		}
	}
	if (refresh) {
		screen.flush();

		textScroller.updateText(millis());
	}
	screen.display();
}
