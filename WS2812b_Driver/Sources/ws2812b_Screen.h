/*
 * ws2812bScreen.h
 *
 *  Created on: 21 θώνÿ 2015 γ.
 *      Author: Admin
 */

#ifndef SOURCES_WS2812BSCREEN_H_
#define SOURCES_WS2812BSCREEN_H_


#include <Graphics/Screen.h>
#include <Graphics/Scene.h>

#include <stdint.h>

#include "ws2812b_types.h"
#include "ws2812b_write.h"

namespace ws2812b {

template<uint8_t pin>
class Screen : public _2d::Screen<uint32_t, color_rgb, 10, 6> {

	color_rgb data[SCREEN_HEIGHT][SCREEN_WIDTH];

public:

	virtual ~Screen() {
		/* Nothing to do */
	}

	void init()  {
		flush();
		display();
	}

	void display() const {
		write<pin>(data, 0, sizeof(data) / sizeof(color_rgb));
	}

	virtual color_rgb *getLine(uunit_t lineIndex) {
		return data[lineIndex];
	}

	virtual void flush() {
		memset(data, 0x00, sizeof(data));
	}

	virtual Screen &getViewport(unit_t x, unit_t y, uunit_t w, uunit_t h) {
		return *this;
	}
};

typedef _2d::Scene<uint32_t, color_rgb> Scene;


} /* namespace ws2812b */

#endif /* SOURCES_WS2812BSCREEN_H_ */
