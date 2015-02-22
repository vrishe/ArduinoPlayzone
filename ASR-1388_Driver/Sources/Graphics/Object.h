/*
 * Object.h
 *
 *  Created on: 15 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_GRAPHICS_OBJECT_H_
#define SOURCES_GRAPHICS_OBJECT_H_


#include "geom_types.h"
#include "../type_traits_ext.h"


namespace _2d {


template<typename TUnit, bool isIntegralUnsignedUnit>
class ObjectBase;

template<typename TUnit>
class ObjectBase<TUnit, true> {

public:

	typedef typename std::integral_signed<TUnit>::type unit_t;
	typedef TUnit uunit_t;


private:

	unit_t  x, y;
	uunit_t w, h;

	void Initialize(unit_t x, unit_t y, uunit_t w, uunit_t h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}


protected:

	ObjectBase(unit_t x, unit_t y, uunit_t w, uunit_t h) {
		Initialize(x, y, w, h);
	}

	ObjectBase(uunit_t w, uunit_t h) {
		Initialize(0, 0, w, h);
	}


public:

	virtual ~ObjectBase() = 0;

	unit_t getX() const { return x; }
	unit_t getY() const { return y;  }

	uunit_t getWidth () const { return w; }
	uunit_t getHeight() const { return h; }

	void MoveTo(unit_t ax, unit_t ay) {
		x = ax;
		y = ay;
	}
	void MoveBy(unit_t dx, unit_t dy) {
		x += dx;
		y += dy;
	}


	virtual void getBoundingRect(rect_t<unit_t> &rect) const {
		rect.left 	= x;
		rect.top 	= y;
		rect.right 	= x + w;
		rect.bottom = y + h;
	}
};

template<typename TUnit>
ObjectBase<TUnit, true>::~ObjectBase() {
	/* Nothing to do */
}

template <typename TUnit>
class Object : public ObjectBase<TUnit, std::is_integral_unsigned<TUnit>::value> {
	/* rotation 2d object representation. */

public:

	using typename ObjectBase<TUnit, std::is_integral_unsigned<TUnit>::value>::unit_t;
	using typename ObjectBase<TUnit, std::is_integral_unsigned<TUnit>::value>::uunit_t;


	Object(unit_t x, unit_t y, uunit_t w, uunit_t h)
		: ObjectBase<TUnit, std::is_integral_unsigned<TUnit>::value>(x, y, w, h) {
		/* Nothing to do */
	}

	Object(uunit_t w, uunit_t h)
		: ObjectBase<TUnit, std::is_integral_unsigned<TUnit>::value>(w, h) {
		/* Nothing to do */
	}

	virtual ~Object() {
		/* Nothing to do */
	}
};


} /* namespace _2d */

#endif /* SOURCES_GRAPHICS_OBJECT_H_ */
