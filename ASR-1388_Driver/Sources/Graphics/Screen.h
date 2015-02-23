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


template <typename TUnit, typename TCarrier, typename TViewport, TUnit width, TUnit height, bool isViewportBase>
class ScreenBase;

template <typename TUnit, typename TCarrier, typename TViewport, TUnit width, TUnit height>
class ScreenBase<TUnit, TCarrier, TViewport, width, height, true> : public Viewport<TUnit, TCarrier> {

public:

	using typename Viewport<TUnit, TCarrier>::unit_t;
	using typename Viewport<TUnit, TCarrier>::uunit_t;


	ScreenBase() : Viewport<TUnit, TCarrier>(0, 0, width, height) {
		/* Nothing to do */
	}

	virtual ~ScreenBase() {
		/* Nothing to do */
	}


	virtual TViewport getViewport(unit_t x, unit_t y, uunit_t w, uunit_t h) const = 0;
};


template <typename TUnit, typename TCarrier, typename TViewport, TUnit width, TUnit height>
class Screen : public ScreenBase<TUnit, TCarrier, TViewport, width, height,
	true/*std::is_base_of<Viewport<TUnit, TCarrier>, TViewport>::value*/> {

public:

	using typename ScreenBase<TUnit, TCarrier, TViewport, width, height,
			true/*std::is_base_of<Viewport<TUnit, TCarrier>, TViewport>::value*/>::unit_t;
	using typename ScreenBase<TUnit, TCarrier, TViewport, width, height,
			true/*std::is_base_of<Viewport<TUnit, TCarrier>, TViewport>::value*/>::uunit_t;


	Screen() : ScreenBase<TUnit, TCarrier, TViewport, width, height,
		true/*std::is_base_of<Viewport<TUnit, TCarrier>, TViewport>::value*/>() {

		/* Nothing to do */
	}
};


} /* namespace _2d */


#endif /* SOURCES_GRAPHICS_SCREEN_H_ */
