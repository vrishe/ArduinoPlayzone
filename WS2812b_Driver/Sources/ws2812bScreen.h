/*
 * ws2812bScreen.h
 *
 *  Created on: 21 θώνÿ 2015 γ.
 *      Author: Admin
 */

#ifndef SOURCES_WS2812BSCREEN_H_
#define SOURCES_WS2812BSCREEN_H_


#include <Graphics/Screen.h>

#include <Arduino.h>

#include <stdint.h>


typedef union tagColor {
	struct {
		uint8_t r, g, b;
	};
	uint8_t value[3];
} color_rgb;


namespace ws2812b {

template<uint8_t pin>
class ws2812bScreen : public _2d::Screen<int32_t, color_rgb, ws2812bScreen, 10, 6> {

	color_rgb data[60];

public:

	using typename _2d::Screen<int32_t, color_rgb, ws2812bScreen, 10, 6>::unit_t;
	using typename _2d::Screen<int32_t, color_rgb, ws2812bScreen, 10, 6>::uunit_t;


	virtual ~ws2812bScreen() {
		/* Nothing to do */
	}


	void init();

	void display() const;


	virtual uunit_t *getLine(uunit_t lineIndex);

	virtual void flush() {
		memset(data, 0x00, sizeof(data));
	}


	virtual ws2812bScreen getViewport(unit_t x, unit_t y, uunit_t w, uunit_t h) const;
};


template<uint8_t pin>
void ws2812bScreen<pin>::init() {
	pinMode(pin, OUTPUT);
}

template<uint8_t pin>
void ws2812bScreen<pin>::display() const {
	write<pin>(data);
}

template<uint8_t pin>
ws2812bScreen::uunit_t *ws2812bScreen<pin>::getLine(uunit_t lineIndex) {
	return this->data + lineIndex;
}

template<uint8_t pin>
ws2812bScreen<pin> ws2812bScreen<pin>::getViewport(unit_t x, unit_t y, uunit_t w, uunit_t h) const {
	return ws2812bScreen(*this);
}

} /* namespace ws2812b */

#endif /* SOURCES_WS2812BSCREEN_H_ */
