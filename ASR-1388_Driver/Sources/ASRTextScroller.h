/*
 * ASRTextScroller.h
 *
 *  Created on: 23 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_ASRTEXTSCROLLER_H_
#define SOURCES_ASRTEXTSCROLLER_H_


#include "ASRScreen.h"
#include "ASRSprite.h"

namespace asr {

enum TextDirection {
	LeftToRight,
	RightToLeft
};

class TextScroller : Scene {

	Screen::viewport_t * const viewport;

	uint8_t delay;
	uunit_t lineWidth;
	unit_t  direction;
	unit_t  ticksPassed;


public:

	TextScroller(Screen::viewport_t *viewport)
		: Scene(), viewport(viewport) {

		this->delay = 0xff;
		this->lineWidth = 0;
		this->direction = 0;
		this->ticksPassed = 0;
	}

	virtual ~TextScroller() {
		/* Nothing to do */
	}


	uunit_t getLineWidth() {
		return lineWidth;
	}


	uunit_t setText(const char *text, uunit_t length, const Sprite * const font[],
			uunit_t maxCodePoints, unit_t codePointOffset, TextDirection textDirection);

	void updateText(unsigned long ticksCurrent);
};

} /* namespace asr */

#endif /* SOURCES_ASRTEXTSCROLLER_H_ */
