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

#define ELEMENT_SIZE(bits, type)  \
	(((((bits) + 7) >> 3) + (sizeof(type) - 1)) / sizeof(type))

namespace asr {

class Sprite : public matrix_t<uint16_t, uint8_t>, public _2d::Sprite<uint8_t, uint8_t> {

public:

	Sprite(uunit_t w, uunit_t h, const uint8_t *data)
		: matrix_t<uint16_t, uint8_t>(h, ELEMENT_SIZE(w, uunit_t), 1),
			_2d::Sprite<uint8_t, uint8_t>(w, h) {
		memcpy(this->data, data, getRows() * getCols());
	}

	Sprite(uunit_t w, uunit_t h)
		: matrix_t<uint16_t, uint8_t>(h, ELEMENT_SIZE(w, uunit_t), 1),
			_2d::Sprite<uint8_t, uint8_t>(w, h) {

		/* Nothing to do */
	}

	virtual ~Sprite() {
		/* Nothing to do */
	}


	virtual void render(_2d::Viewport<uunit_t, uint8_t> &viewport,
			const _2d::rect_t<unit_t> &rect, const _2d::point_t<unit_t> &origin) const;
};


} /* namespace asr */

#endif /* SOURCES_ASRSPRITE_H_ */
