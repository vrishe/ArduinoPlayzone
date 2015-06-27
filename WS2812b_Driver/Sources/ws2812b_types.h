/*
 * ws2812b_types.h
 *
 *  Created on: 22 θώνÿ 2015 γ.
 *      Author: Admin
 */

#ifndef SOURCES_WS2812B_TYPES_H_
#define SOURCES_WS2812B_TYPES_H_


#include <stdint.h>


namespace ws2812b {

typedef union tagColor {
	struct {
		uint8_t r, g, b;
	};
	uint8_t value[3];
} color_rgb;

} /* namespace ws2812b */


#endif /* SOURCES_WS2812B_TYPES_H_ */
