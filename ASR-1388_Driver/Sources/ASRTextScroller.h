/*
 * ASRTextScroller.h
 *
 *  Created on: 23 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_ASRTEXTSCROLLER_H_
#define SOURCES_ASRTEXTSCROLLER_H_


#include "Graphics/Viewport.h"
#include "ASRScreen.h"
#include "ASRSprite.h"

namespace asr {

enum TextDirection {
	LeftToRight,
	RightToLeft
};

class TextScroller : Scene {

	_2d::Viewport<uint8_t, uint8_t> * const viewport;

	uint8_t delay;
	uunit_t lineWidth;
	unit_t  direction;

	unit_t  startingPoint;
	uunit_t phaseShift;
	
	unsigned long ticksPassed;


public:

	TextScroller(_2d::Viewport<uint8_t, uint8_t> *viewport)
		: Scene(), viewport(viewport) {

		this->delay = 0xff;
		this->lineWidth = 0;
		this->direction = 0;
		this->startingPoint = 0;
		this->phaseShift = 0;
		this->ticksPassed = 0;
	}

	virtual ~TextScroller() {
		/* Nothing to do */
	}


	unit_t getRow() {
		return getY();
	}

	void placeAtRow(unit_t row) {
		this->moveTo(getX(), row);
	}

	void offsetByRow(unit_t row) {
		this->moveBy(0, row);
	}


	uint8_t getDelay() const {
		return delay;
	}

	void setDelay(uint8_t delay) {
		this->delay = delay;
	}


	uunit_t getLineWidth() {
		return lineWidth;
	}

	template <size_t length>
	uunit_t setText(const char (&text)[length], uunit_t phaseBias, const Sprite * const font[],
			uunit_t maxCodePoints, unit_t codePointOffset, uunit_t whitespaceWidth, TextDirection textDirection) {

		return setText(text, length, phaseBias, font, maxCodePoints, codePointOffset, whitespaceWidth, textDirection);
	}

	uunit_t setText(const char *text, uunit_t length, uunit_t phaseBias, const Sprite * const font[],
			uunit_t maxCodePoints, unit_t codePointOffset, uunit_t whitespaceWidth, TextDirection textDirection);

	void updateText(unsigned long ticksCurrent);
};

} /* namespace asr */

#endif /* SOURCES_ASRTEXTSCROLLER_H_ */
