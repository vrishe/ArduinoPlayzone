/*
 * ASRScreen.h
 *
 *  Created on: 21 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_ASRSCREEN_H_
#define SOURCES_ASRSCREEN_H_


#include "Graphics/Screen.h"
#include "Graphics/Scene.h"

#include <stdint.h>

namespace asr {

class Screen : public _2d::Screen<uint8_t, uint8_t, 8, 8> {

	uint32_t pinClock;
	uint32_t pinLatch;
	uint32_t pinData;

#ifdef DEBUG
	volatile unsigned long scanLineDelay = 0;
#endif

	uint8_t data[8];

public:

#ifdef DEBUG
	void setScanLineDelay(unsigned long scanLineDelay) {
		this->scanLineDelay = scanLineDelay;
	}
	unsigned long getScanLineDelay() const {
		return scanLineDelay;
	}
#endif


	virtual ~Screen() {
		/* Nothing to do */
	}

	void init(uint8_t pinClock, uint8_t pinLatch, uint8_t pinData);
	void display() const;

	virtual uint8_t *getLine(uunit_t lineIndex);
	virtual void flush();

	virtual Screen &getViewport(unit_t x, unit_t y, uunit_t w, uunit_t h);
};

typedef _2d::Scene<uint8_t, uint8_t> Scene;


} /* namespace asr */

#endif /* SOURCES_ASRSCREEN_H_ */
