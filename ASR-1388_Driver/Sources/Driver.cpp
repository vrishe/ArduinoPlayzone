/*
 * Driver.cpp
 *
 *  Created on: 31 џэт. 2015 у.
 *      Author: Admin
 */

#include "Driver.h"


namespace asr {

void Driver::Initialize(uint32_t pinClock, uint32_t pinLatch, uint32_t pinData) {
	pinMode(this->pinClock = pinClock, OUTPUT);
	pinMode(this->pinLatch = pinLatch, OUTPUT);
	pinMode(this->pinData = pinData, OUTPUT);
}

void Driver::DrawSprite(int8_t x, int8_t y, const byte rawBytes[], byte height) {
	for (int8_t col = 0; col < 8; ++col) {

		uint16_t line = (y <= col && col < y + height ?
				~(((uint16_t)rawBytes[col - y]) >> x) << 8 | ((uint16_t)_BV(col)) : 0x00);

		digitalWrite(pinLatch, LOW);
		digitalWrite(pinData, LOW);

		for (int8_t row = 0; row < 16; ++row) {
			digitalWrite(pinClock, LOW);
			digitalWrite(pinData, !!((line >> row) & 0x01) ? HIGH : LOW);
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

} /* namespace asr */
