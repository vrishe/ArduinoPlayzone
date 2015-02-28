/*
 * ASRFontASCII.cpp
 *
 *  Created on: 28 февр. 2015 г.
 *      Author: Admin
 */

#include "ASRFontASCII.h"

#include <initializer_list>

namespace asr {

namespace {

	typedef std::initializer_list<uint8_t> sprite_initializer;

	static const Sprite CharA = Sprite(5, 5, sprite_initializer({ 0x60, 0x90, 0xf0, 0x90, 0x90 })		.begin());
	static const Sprite CharB = Sprite(5, 5, sprite_initializer({ 0xe0, 0xa0, 0xe0, 0x90, 0xe0 })		.begin());
	static const Sprite CharC = Sprite(5, 5, sprite_initializer({ 0x60, 0x80, 0x80, 0x80, 0x60 })		.begin());
	static const Sprite CharD = Sprite(5, 5, sprite_initializer({ 0xe0, 0x90, 0x90, 0x90, 0xe0 })		.begin());
	static const Sprite CharE = Sprite(5, 5, sprite_initializer({ 0xf0, 0x80, 0xe0, 0x80, 0xf0 })		.begin());
	static const Sprite CharF = Sprite(5, 5, sprite_initializer({ 0xf0, 0x80, 0xe0, 0x80, 0x80 })		.begin());
	static const Sprite CharG = Sprite(5, 5, sprite_initializer({ 0x60, 0x80, 0xb0, 0x90, 0x60 })		.begin());
	static const Sprite CharH = Sprite(5, 5, sprite_initializer({ 0x90, 0x90, 0xf0, 0x90, 0x90 })		.begin());
	static const Sprite CharI = Sprite(4, 5, sprite_initializer({ 0xe0, 0x40, 0x40, 0x40, 0xe0 })		.begin());
	static const Sprite CharJ = Sprite(5, 5, sprite_initializer({ 0x30, 0x10, 0x10, 0x90, 0x60 })		.begin());
	static const Sprite CharK = Sprite(5, 5, sprite_initializer({ 0x90, 0xa0, 0xe0, 0x90, 0x90 })		.begin());
	static const Sprite CharL = Sprite(5, 5, sprite_initializer({ 0x80, 0x80, 0x80, 0x80, 0xf0 })		.begin());
	static const Sprite CharM = Sprite(6, 5, sprite_initializer({ 0x88, 0xd8, 0xa8, 0x88, 0x88 })		.begin());
	static const Sprite CharN = Sprite(5, 5, sprite_initializer({ 0x90, 0xd0, 0xb0, 0xb0, 0x90 })		.begin());
	static const Sprite CharO = Sprite(5, 5, sprite_initializer({ 0x60, 0x90, 0x90, 0x90, 0x60 })		.begin());
	static const Sprite CharP = Sprite(5, 5, sprite_initializer({ 0xe0, 0x90, 0xe0, 0x80, 0x80 })		.begin());
	static const Sprite CharQ = Sprite(5, 6, sprite_initializer({ 0x60, 0x90, 0x90, 0x90, 0x60, 0x30 })	.begin());
	static const Sprite CharR = Sprite(5, 5, sprite_initializer({ 0xe0, 0x90, 0xe0, 0x90, 0x90 })		.begin());
	static const Sprite CharS = Sprite(5, 5, sprite_initializer({ 0x70, 0x80, 0x60, 0x10, 0xe0 })		.begin());
	static const Sprite CharT = Sprite(6, 5, sprite_initializer({ 0xf8, 0x20, 0x20, 0x20, 0x20 })		.begin());
	static const Sprite CharU = Sprite(5, 5, sprite_initializer({ 0x90, 0x90, 0x90, 0x90, 0x60 })		.begin());
	static const Sprite CharV = Sprite(5, 5, sprite_initializer({ 0x90, 0x90, 0x90, 0x50, 0x70 })		.begin());
	static const Sprite CharW = Sprite(6, 5, sprite_initializer({ 0x88, 0x88, 0xa8, 0xa8, 0x50 })		.begin());
	static const Sprite CharX = Sprite(6, 5, sprite_initializer({ 0xd8, 0x50, 0x20, 0x50, 0xd8 })		.begin());
	static const Sprite CharY = Sprite(5, 5, sprite_initializer({ 0x90, 0x90, 0x70, 0x20, 0x40 })		.begin());
	static const Sprite CharZ = Sprite(5, 5, sprite_initializer({ 0xf0, 0x20, 0x40, 0xc0, 0xf0 })		.begin());

} /* namespace */

	FontASCII::FontASCII() {
		storage[ 0] = &CharA; storage[ 1] = &CharB; storage[ 2] = &CharC; storage[ 3] = &CharD;
		storage[ 4] = &CharE; storage[ 5] = &CharF; storage[ 6] = &CharG; storage[ 7] = &CharH;
		storage[ 8] = &CharI; storage[ 9] = &CharJ; storage[10] = &CharK; storage[11] = &CharL;
		storage[12] = &CharM; storage[13] = &CharN; storage[14] = &CharO; storage[15] = &CharP;
		storage[16] = &CharQ; storage[17] = &CharR; storage[18] = &CharS; storage[19] = &CharT;
		storage[20] = &CharU; storage[21] = &CharV; storage[22] = &CharW; storage[23] = &CharX;
		storage[24] = &CharY; storage[25] = &CharZ;
	}

	const FontASCII FontASCII::Instance = FontASCII();

} /* namespace asr */





