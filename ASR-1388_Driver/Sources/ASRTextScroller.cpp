/*
 * ASRTextScroller.cpp
 *
 *  Created on: 23 февр. 2015 г.
 *      Author: Admin
 */

#include "ASRTextScroller.h"

#include <cmath>
#include "bitutility.h"


namespace asr {

	TextScroller::uunit_t TextScroller::setText(const char *text, uunit_t length, uunit_t phaseBias, uunit_t whitespaceWidth,
		const Sprite * const font[], uunit_t maxCodePoints, unit_t codePointOffset, TextDirection textDirection) {

	clear();

	lineWidth = 0;
	if (text != NULL && length > 0) {
		if (whitespaceWidth < 1) {
			whitespaceWidth = 1;
		}
		for (uunit_t i = 0; i < length; ++i, ++text) {
			const char character = *text - codePointOffset;

			const Sprite *unit;
			if (character < maxCodePoints
				&& (unit = font[(size_t)character]) != NULL) {

				asr::Sprite *unitCopy = new asr::Sprite(*unit);
				unitCopy->moveTo(lineWidth, unitCopy->getY());
				add(*unitCopy);

				lineWidth += unit->getWidth();
			} else {
				lineWidth += whitespaceWidth;
			}
		}
		switch(textDirection) {
		case LeftToRight:
			startingPoint = viewport->getWidth();
			direction = -1;
			break;
		case RightToLeft:
			startingPoint = -lineWidth - viewport->getWidth();
			direction = +1;
		}
		phaseShift = lineWidth + viewport->getWidth() + phaseBias;
		ticksPassed = 0;

		moveTo(startingPoint, y);
	}
	return lineWidth;
}

void TextScroller::updateText(unsigned long ticksCurrent) {
	unit_t offset = direction * ((ticksCurrent - ticksPassed) / delay);

	if (ticksPassed == 0 || offset != 0) {	
		viewport->flush();

		moveTo(startingPoint + (x - startingPoint) % phaseShift + offset, y);
		renderTo(*viewport);

		ticksPassed = ticksCurrent;
	}
}

} /* namespace asr */
