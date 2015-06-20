/*
 * Main.cpp
 *
 *  Created on: 31 џэт. 2015 у.
 *      Author: Admin
 */

#include <Arduino.h>
#include <stdint.h>

#include "ws2812b.h"

typedef union tagColor {
	struct {
		uint8_t r, g, b;
	};
	uint8_t value[3];
} color_rgb;

static int phase;
static color_rgb colors[60];
static const color_rgb colors_reference[] = {
		{ 0xff, 0x00, 0x00 },
		{ 0x00, 0xff, 0x00 },
		{ 0x00, 0x00, 0xff }
};

void loop() {
	int phaseLocal = analogRead(0);

	if (abs(phaseLocal - phase) > 16) {
		phase = phaseLocal;

		float shift = phase / 1024.f;
		for (int i = 0; i < _countof(colors); ++i) {
			const color_rgb color_reference = colors_reference[i % 3];

			colors[i].r += round((color_reference.r - (0xff - color_reference.r)) * shift);
			colors[i].g += round((color_reference.g - (0xff - color_reference.g)) * shift);
			colors[i].b	+= round((color_reference.b - (0xff - color_reference.b)) * shift);
		}
		ws2812b::write<2, color_rgb>(colors, _countof(colors));
	}
}

void setup() {
	DDRB |= B00100000;
	DDRD |= B00000100;

	ws2812b::write<2, color_rgb>(colors, _countof(colors));

	PORTB = B00100000;
}
