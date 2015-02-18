/*
 * bitutility.h
 *
 *  Created on: 18 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_BITUTILITY_H_
#define SOURCES_BITUTILITY_H_


#include <stdint.h>

namespace asr {
namespace util {

typedef uint8_t (*bitwise_op)(uint8_t, uint8_t);

uint8_t bitcopy(uint8_t *dst, const uint8_t *src, uint16_t count, uint8_t shiftIn, uint8_t shiftOut, bitwise_op op);

} /* namespace util */
} /* namespace asr */

#define bitsizeof(type) (sizeof(type) << 3)

#endif /* SOURCES_BITUTILITY_H_ */
