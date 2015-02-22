/*
 * ASRSprite.cpp
 *
 *  Created on: 16 ����. 2015 �.
 *      Author: Admin
 */

#include "ASRSprite.h"
#include "bitutility.h"

namespace asr {

namespace {

	uint8_t bitwise_xor(uint8_t a, uint8_t b) {
		return a ^ b;
	}

} /* namespace */

void Sprite::render(_2d::Viewport<uunit_t, uint8_t> &viewport,
		const _2d::rect_t<unit_t> &rect, const _2d::point_t<unit_t> &origin) const {

	unit_t colSrc = rect.left - origin.x;
	unit_t colDst = rect.left - viewport.getX();
	uint16_t colDstOffset = ELEMENT_SIZE(colDst, uint8_t) - 1;
	uint16_t colSrcOffset = ELEMENT_SIZE(rect.left - origin.x, uint8_t) - 1;

	for (unit_t row = rect.top - viewport.getY(),
			rowmax = rect.bottom - viewport.getY(); row < rowmax; ++row) {

		util::bitcopy(
			viewport.getLine(row) + colDstOffset,
			// This has to be a non-const call, but it is guaranteed that sprite data won't be modified here.
			const_cast<Sprite *>(this)->getValueAt(rect.top - origin.y, colSrcOffset),
			rect.right - rect.left,
			colSrc % 8,
			colDst % 8,
			bitwise_xor
		);
	}
}

} /* namespace asr */
