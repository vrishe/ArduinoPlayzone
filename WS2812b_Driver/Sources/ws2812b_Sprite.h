/*
 * Sprite.h
 *
 *  Created on: 22 θώνÿ 2015 γ.
 *      Author: Admin
 */

#ifndef SOURCES_SPRITE_H_
#define SOURCES_SPRITE_H_


#include "ws2812b_types.h"

#include <Graphics/Sprite.h>
#include <Graphics/Viewport.h>

#include <stdint.h>

#include <array_t.h>


namespace ws2812b {

class Sprite : public matrix_t<uint32_t, uint8_t>, public _2d::Sprite<uint32_t, color_rgb> {
public:
	Sprite(uunit_t w, uunit_t h, const color_rgb *data)
		: matrix_t<uint32_t, uint8_t>(h, w, sizeof(color_rgb)),
			_2d::Sprite<uint32_t, color_rgb>(w, h) {
		memcpy(this->data, reinterpret_cast<const uint8_t*>(data), getRows() * getCols() * sizeof(color_rgb));
	}

	Sprite(uunit_t w, uunit_t h)
		: matrix_t<uint32_t, uint8_t>(h, w, sizeof(color_rgb)),
			_2d::Sprite<uint32_t, color_rgb>(w, h) {

		/* Nothing to do */
	}

	virtual ~Sprite() {
		/* Nothing to do */
	}


	virtual void render(_2d::Viewport<uunit_t, color_rgb> &viewport,
			const _2d::rect_t<unit_t> &rect, const _2d::point_t<unit_t> &origin) const;
};

} /* namespace ws2812b */

#endif /* SOURCES_SPRITE_H_ */
