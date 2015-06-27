/*
 * ASRTextScroller.cpp
 *
 *  Created on: 23 февр. 2015 г.
 *      Author: Admin
 */

#include "ASRTextScroller.h"

#include <cmath>


namespace asr {

TextScrollerBase::~TextScrollerBase() {
	/* Nothing to do */
}


void TextHScroller::clear() {
	for (sprite_iterator sprite = sprites.begin(),
			spriteEnd = sprites.end(); sprite != spriteEnd; ++sprite) {
		delete *sprite;
	}
	Scene::clear();
}

TextHScroller::uunit_t TextHScroller::setText(const char *text, uunit_t length, uunit_t phaseBias, const Sprite * const font[],
		uunit_t maxCodePoints, unit_t codePointOffset, uunit_t whitespaceWidth, TextDirection textDirection) {

	clear();

	lineWidth = 0;
	if (text != NULL && length > 0) {
		if (whitespaceWidth < 1) {
			whitespaceWidth = 1;
		}
		sprites.reserve(length);

		for (uunit_t i = 0; i < length; ++i, ++text) {
			const unsigned char character = *text - codePointOffset;

			const Sprite *unit;
			if (character < maxCodePoints
				&& ((unit = font[(size_t)character]) != NULL)) {

				asr::Sprite *unitCopy = new asr::Sprite(*unit);
				unitCopy->moveTo(lineWidth, unitCopy->getY());
				add(*unitCopy);

				lineWidth += unit->getWidth();
			} else {
				lineWidth += whitespaceWidth;
			}
		}
#if defined(__cplusplus) && __cplusplus >= 201103L
		sprite_vector(sprites).swap(sprites);
#endif

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

		moveTo(startingPoint, y);
	}
	return lineWidth;
}

void TextHScroller::updateText(unsigned long ticksCurrent) {
//	if (!needSync) {
		unit_t offset = direction * ((ticksCurrent - ticksPassed) / delay);

		if (offset != 0) {
			moveTo(startingPoint + ((x - startingPoint) % phaseShift) + offset, y);
			ticksPassed = ticksCurrent;
		}
//	} else {
//		needSync ^= needSync;
//		ticksPassed = ticksCurrent;
//	}
	renderTo(*viewport);
}

} /* namespace asr */
