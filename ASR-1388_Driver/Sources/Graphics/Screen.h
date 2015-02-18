/*
 * Screen.h
 *
 *  Created on: 15 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_GRAPHICS_SCREEN_H_
#define SOURCES_GRAPHICS_SCREEN_H_


#include "Object.h"
#include "Viewport.h"

namespace _2d {


template <typename TUnit, typename TCarrier, TUnit width, TUnit height>
class Screen : public Viewport<TUnit, TCarrier> {

public:

	Screen() : Viewport<TUnit, TCarrier>(width, height) {
		/* Nothing to do */
	}

	virtual ~Screen() = 0;
	virtual Viewport getViewport(unit_t x, unit_t y, uunit_t w, uunit_t h) = 0;
};


} /* namespace _2d */


#endif /* SOURCES_GRAPHICS_SCREEN_H_ */
