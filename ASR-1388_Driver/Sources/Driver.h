/*
 * Driver.h
 *
 *  Created on: 31 џэт. 2015 у.
 *      Author: Admin
 */

#ifndef ASR_1388_DRIVER_SOURCES_DRIVER_H_
#define ASR_1388_DRIVER_SOURCES_DRIVER_H_

#include "error.h"

#include <Arduino.h>
#include <stdint.h>


namespace asr {

class Driver {

	uint32_t pinClock;
	uint32_t pinLatch;
	uint32_t pinData;

public:

	void Initialize(uint32_t pinClock, uint32_t pinLatch, uint32_t pinData);

	void DrawSprite(int8_t x, int8_t, const byte rawBytes[], byte height);

};

} /* namespace asr */

#endif /* ASR_1388_DRIVER_SOURCES_DRIVER_H_ */
