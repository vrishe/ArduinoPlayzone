/*
 * Driver.h
 *
 *  Created on: 31 џэт. 2015 у.
 *      Author: Admin
 */

#ifndef ASR_1388_DRIVER_SOURCES_DRIVER_H_
#define ASR_1388_DRIVER_SOURCES_DRIVER_H_

#include <Arduino.h>
#include "cstdlib_ext.h"

#include "error.h"


namespace asr {

class Driver {

	uint32_t pinClock;
	uint32_t pinLatch;
	uint32_t pinData;

#ifdef DEBUG
	volatile unsigned long scanLineDelay;
#endif

public:

	void Initialize(uint32_t pinClock, uint32_t pinLatch, uint32_t pinData);

	void DrawSprite(int8_t x, int8_t, const byte rawBytes[], byte height);

#ifdef DEBUG
	void setScanLineDelay(unsigned long scanLineDelay) {
		this->scanLineDelay = scanLineDelay;
	}

	unsigned long getScanLineDelay() {
		return this->scanLineDelay;
	}
#endif
};

} /* namespace asr */

#endif /* ASR_1388_DRIVER_SOURCES_DRIVER_H_ */
