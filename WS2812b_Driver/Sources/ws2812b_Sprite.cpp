/*
 * Sprite.cpp
 *
 *  Created on: 22 θώνÿ 2015 γ.
 *      Author: Admin
 */

#include "ws2812b_Sprite.h"

#include <bitutility.h>


namespace ws2812b {

namespace {

	uint8_t bitwise_xor(uint8_t a, uint8_t b) {
		return a ^ b;
	}

} /* namespace */

void Sprite::render(_2d::Viewport<uunit_t, color_rgb> &viewport,
		const _2d::rect_t<unit_t> &rect, const _2d::point_t<unit_t> &origin) const {

	unit_t colDst = rect.left - viewport.getX();
	unit_t colSrc = rect.left - origin.x - getX();
	size_t count  = (rect.right - rect.left) * elementSize;

	Sprite *thiz = const_cast<Sprite *>(this);
	for (unit_t rowDst = rect.top - viewport.getY(), rowSrc = rect.top - origin.y - getY(),
			rowDstMax = rect.bottom - viewport.getY(); rowDst < rowDstMax; ++rowDst, ++rowSrc) {

		memcpy(viewport.getLine(rowDst) + colDst, thiz->getValueAt(rowSrc, colSrc), count);
	}
}

} /* namespace ws2812b */
