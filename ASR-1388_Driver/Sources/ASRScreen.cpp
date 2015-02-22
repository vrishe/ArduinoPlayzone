/*
 * ASRScreen.cpp
 *
 *  Created on: 21 февр. 2015 г.
 *      Author: Admin
 */

#include "ASRScreen.h"

#include <Arduino.h>

namespace asr {

void Screen::init(uint8_t pinClock, uint8_t pinLatch, uint8_t pinData) {
	pinMode(this->pinClock = pinClock, OUTPUT);
	pinMode(this->pinLatch = pinLatch, OUTPUT);
	pinMode(this->pinData  = pinData,  OUTPUT);
}

void Screen::display() const {
	for (int8_t r = 0, rmax = getHeight(); r < rmax; ++r) {

		uint16_t line = (data[r] << 8 | ((uint16_t)_BV(r)));

		digitalWrite(pinLatch, LOW);
		digitalWrite(pinData, LOW);

		for (int8_t c = 0; c < 16; ++c) {
			digitalWrite(pinClock, LOW);
			digitalWrite(pinData, !!((line >> c) & 0x01) ? HIGH : LOW);
			digitalWrite(pinClock, HIGH);
			digitalWrite(pinData, LOW);
		}
		digitalWrite(pinClock, LOW);
		digitalWrite(pinLatch, HIGH);

#ifdef DEBUG
		unsigned long scanLineDelay = this->scanLineDelay;

		if (scanLineDelay > 0) {
			delay(scanLineDelay);
		}
#endif
	}
}

Screen::uunit_t *Screen::getLine(uunit_t lineIndex) {
	return this->data + lineIndex;
}

Screen Screen::getViewport(unit_t x, unit_t y, uunit_t w, uunit_t h) const {
	return Screen(*this);
}

} /* namespace asr */
