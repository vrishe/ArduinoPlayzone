/*
 * ASRSprite.h
 *
 *  Created on: 16 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_ASRSPRITE_H_
#define SOURCES_ASRSPRITE_H_


#include <stdint.h>

#include "array_t.h"
#include "Graphics/Sprite.h"
#include "Graphics/Viewport.h"

namespace asr {

class ASRSprite : public matrix_t<uint8_t>, public _2d::Sprite<uint8_t, uint8_t> {

public:
	ASRSprite(uunit_t w, uunit_t h, uint8_t *data)
		: matrix_t<uint8_t>(h, w, ELEMENT_SIZE(1, uunit_t)),
		  	  _2d::Sprite<uint8_t, uint8_t>(w, h) {
		/* Nothing to do */
	}

	virtual ~ASRSprite() {
		delete[] data;
	}


	virtual void render(_2d::Viewport<uunit_t, uint8_t> &viewport, _2d::rect_t<unit_t> &rect) const;
};

} /* namespace asr */

#endif /* SOURCES_ASRSPRITE_H_ */
