/*
 * bitutility.cpp
 *
 *  Created on: 18 февр. 2015 г.
 *      Author: Admin
 */

#include "bitutility.h"


namespace asr {
namespace util {

namespace {

union bitcopy_register {
	struct {
		uint8_t lo;
		uint8_t hi;
	} segment;

	uint16_t value;
};

} /* namespace */

uint8_t bitcopy(uint8_t *dst, const uint8_t *src, uint16_t count, uint8_t shiftIn, uint8_t shiftOut, bitwise_op op) {
	uint16_t length = (shiftOut + count + 7) / 8 - 1;

	if (shiftOut > shiftIn) { // Shift bits right
		uint8_t shift = shiftOut - shiftIn;
		uint8_t mask = 0xff >> shiftOut;
		uint8_t value = dst[0];

		value = (value & ~mask) | (op(src[0] >> shift, value) & mask);

		if (length > 1) {
			dst[0] = value;

			bitcopy_register reg;
			reg.segment.hi = src[0];
			for (uint16_t i = 1; i < length; ++i) {
				value = src[i];

				reg.segment.lo = src[i];
				reg.value >>= shift;

				dst[i] = op(reg.segment.lo, dst[i]);

				reg.segment.hi = value;
			}
			reg.segment.lo = src[length];
			reg.value >>= shift;

			value = op(reg.segment.lo, dst[length]);
		}
		mask = 0xff >> ((shiftOut + count - 1) % 8 + 1);
		dst[length] = (dst[length] & mask) | (value & ~mask);
	} else if (shiftOut < shiftIn) { // Shift bits left
		uint8_t shift = shiftIn - shiftOut;
		uint8_t mask = 0xff >> ((shiftOut + count - 1) % 8 + 1);
		uint8_t value = dst[length];

		value = (value & mask) | (op(src[length] << shift, value) & ~mask);

		if (length > 1) {
			dst[length] = value;

			bitcopy_register reg;
			reg.segment.lo = src[length];
			for (uint16_t i = length - 1; i > 0; --i) {
				value = src[i];

				reg.segment.hi = value;
				reg.value <<= shift;

				dst[i] = op(reg.segment.hi, dst[i]);

				reg.segment.lo = value;
			}
			reg.segment.hi = src[0];
			reg.value <<= shift;

			value = op(reg.segment.hi, dst[0]);
		}
		mask = 0xff >> shiftOut;
		dst[0] = (dst[0] & ~mask) | (value & mask);
	} else /* shiftOut == shiftIn */ { // No need to shift, just mask and then copy as is.
		uint8_t mask = 0xff >> shiftOut;
		uint8_t value = dst[0];

		value = (value & ~mask) | (op(src[0], value) & mask);

		if (length > 1) {
			dst[0] = value;

			for (uint16_t i = 1; i < length; ++i) {
				dst[i] = op(src[i], dst[i]);
			}
			value = op(src[length], dst[length]);
		}
		mask = 0xff >> ((shiftOut + count - 1) % 8 + 1);
		dst[length] = (dst[length] & mask) | (value & ~mask);
	}
	return dst[length];
}

} /* namespace util */
} /* namespace asr */


