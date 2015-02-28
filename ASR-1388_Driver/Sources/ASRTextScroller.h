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

class TextScrollerBase : protected Scene {

protected:

	_2d::Viewport<uint8_t, uint8_t> * const viewport;

	uint8_t delay;


public:

	class Font {

	public:

		typedef TextScrollerBase::uunit_t 	 cpsize_t;
		typedef TextScrollerBase::unit_t	 cpoffset_t;
		typedef const asr::Sprite 			*cpunit_t;


		virtual ~Font() {
			/* Nothing to do */
		}

		virtual cpsize_t 	getMaxCodePoints()	 const = 0;
		virtual cpoffset_t  getCodePointOffset() const = 0;

		virtual const cpunit_t *operator* () const = 0;
	};


	TextScrollerBase(_2d::Viewport<uint8_t, uint8_t> *viewport)
		: viewport(viewport), delay(0xff) {

		/* Nothing to do */
	}

	virtual ~TextScrollerBase() = 0;


	uint8_t getDelay() const {
		return delay;
	}

	void setDelay(uint8_t delay) {
		this->delay = delay > 0 ? delay : 1;
	}


	virtual void updateText(unsigned long ticksCurrent) = 0;
};


class TextHScroller : public TextScrollerBase {

	uunit_t lineWidth;
	unit_t  direction;

	unit_t  startingPoint;
	uunit_t phaseShift;
	
	unsigned long ticksPassed;
//	bool needSync;


public:

	enum TextDirection {
		LeftToRight,
		RightToLeft
	};


	TextHScroller(_2d::Viewport<uint8_t, uint8_t> *viewport)
		: TextScrollerBase(viewport) {

		this->lineWidth = 0;
		this->direction = 0;

		this->startingPoint = 0;
		this->phaseShift 	= 0;

		this->ticksPassed = 0;
//		this->needSync = true;
	}

	virtual ~TextHScroller() {
		/* Nothing to do */
	}


	virtual void clear();


	unit_t getRow() {
		return getY();
	}

	void placeAtRow(unit_t row) {
		this->moveTo(getX(), row);
	}

	void offsetByRow(unit_t row) {
		this->moveBy(0, row);
	}


	uunit_t getLineWidth() {
		return lineWidth;
	}


	uunit_t setText(const char *text, uunit_t length, uunit_t phaseBias,
			const Font &font, uunit_t whitespaceWidth, TextDirection textDirection) {

		return setText(text, length, phaseBias, *font, font.getMaxCodePoints(), font.getCodePointOffset(), whitespaceWidth, textDirection);
	}

	template <size_t length>
	uunit_t setText(const char (&text)[length], uunit_t phaseBias, const Sprite * const font[],
			uunit_t maxCodePoints, unit_t codePointOffset, uunit_t whitespaceWidth, TextDirection textDirection) {

		return setText(text, length, phaseBias, font, maxCodePoints, codePointOffset, whitespaceWidth, textDirection);
	}

	uunit_t setText(const char *text, uunit_t length, uunit_t phaseBias, const Sprite * const font[],
			uunit_t maxCodePoints, unit_t codePointOffset, uunit_t whitespaceWidth, TextDirection textDirection);

	virtual void updateText(unsigned long ticksCurrent);
};

} /* namespace asr */

#endif /* SOURCES_ASRTEXTSCROLLER_H_ */
