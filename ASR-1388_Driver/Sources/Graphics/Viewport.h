/*
 * Viewport.h
 *
 *  Created on: 15 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_GRAPHICS_IVIEWPORT_H_
#define SOURCES_GRAPHICS_IVIEWPORT_H_


#include "Object.h"

namespace _2d {


template <typename TUnit, typename TCarrier>
class Viewport : public Object<TUnit> {

public:

	using typename Object<TUnit>::unit_t;
	using typename Object<TUnit>::uunit_t;

	virtual ~Viewport() {
		/* Nothing to do */
	}


	virtual TCarrier *getLine(uunit_t lineIndex) = 0;
	virtual void flush() = 0;

protected:

	Viewport(unit_t x, unit_t y, uunit_t w, uunit_t h)
		: Object<TUnit>(x, y, w, h) {
		/* Nothing to do */
	}
};


} /* namespace _2d */

#endif /* SOURCES_GRAPHICS_IVIEWPORT_H_ */
