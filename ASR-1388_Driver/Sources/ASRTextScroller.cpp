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

TextScroller::uunit_t TextScroller::setText(const char *text, uunit_t length,
		const Sprite * const font[], uunit_t maxCodePoints, unit_t codePointOffset, TextDirection textDirection) {

	clear();

	lineWidth = 0;
	if (text != NULL && length > 0) {
		for (uunit_t i = 0; i < length; ++i, ++text) {
			const char character = *text - codePointOffset;

			if (character < maxCodePoints) {
				const Sprite *unit = font[(size_t)character];

				lineWidth += unit->getWidth();
				add(*unit);
			} else {
				lineWidth += bitsizeof(uunit_t);
			}
		}
		switch(textDirection) {
		case LeftToRight:
			moveTo(-viewport->getWidth(), getY());
			direction = +1;
			break;
		case RightToLeft:
			moveTo(lineWidth + viewport->getWidth(), getY());
			direction = -1;
		}
		ticksPassed = 0;
	}
	return lineWidth;
}

void TextScroller::updateText(unsigned long ticksCurrent) {
	unit_t offset;

	if (ticksPassed > 0 && (offset = direction * ((ticksCurrent - ticksPassed) / delay)) > 0) {
		moveBy(offset, 0);

		viewport->flush();
		renderTo(*viewport);
	}
	ticksPassed = ticksCurrent;
}

} /* namespace asr */
