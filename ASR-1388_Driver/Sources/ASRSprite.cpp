/*
 * ASRSprite.cpp
 *
 *  Created on: 16 февр. 2015 г.
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

	uunit_t colDstShift;
	uunit_t colSrcShift;
	uint16_t colDstOffset;
	uint16_t colSrcOffset;
	uint16_t count = rect.right - rect.left;
	{
		unit_t colDst = rect.left - viewport.getX();
		unit_t colSrc = rect.left - origin.x - getX();

		colDstShift = colDst % 8;
		colSrcShift = colSrc % 8;
		colDstOffset = colDst / 8;
		colSrcOffset = colSrc / 8;
	}
	Sprite *thiz = const_cast<Sprite *>(this);

	for (unit_t rowDst = rect.top - viewport.getY(), rowSrc = rect.top - origin.y - getY(),
			rowSrcMax = rect.bottom - origin.y - getY(); rowSrc < rowSrcMax; ++rowSrc, ++rowDst) {

		util::bitcopy(
			viewport.getLine(rowDst) + colDstOffset,
			// This has to be a non-const call, but it is guaranteed that sprite data won't be modified here.
			thiz->getValueAt(rowSrc, colSrcOffset),
			count,
			colSrcShift,
			colDstShift,
			bitwise_xor
		);
	}
}

} /* namespace asr */
