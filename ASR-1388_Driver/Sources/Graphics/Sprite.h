/*
 * Sprite.h
 *
 *  Created on: 15 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_GRAPHICS_SPRITE_H_
#define SOURCES_GRAPHICS_SPRITE_H_


#include <stdlib.h>

#include "Object.h"
#include "Viewport.h"

namespace _2d {


template <typename TUnit, typename TCarrier>
class Sprite : public Object<TUnit> {

	float rotation;

public:

	using typename Object<TUnit>::unit_t;
	using typename Object<TUnit>::uunit_t;


protected:

	Sprite(unit_t x, unit_t y, uunit_t w, uunit_t h)
		: Object<TUnit>(x, y, w, h), rotation(0) {
		/* Nothing to do */
	}

	Sprite(uunit_t w, uunit_t h)
		: Object<TUnit>(w, h), rotation(0) {
		/* Nothing to do */
	}


public:

#ifdef GRAPHICS_FULL

	float getRotation() const { return rotation; }

	void rotateTo(float angle) {
		rotation = angle;
	}
	void rotateBy(float angle) {
		rotation += angle;
	}


	virtual void getBoundingRect(rect_t<unit_t> &rect) const {
		this->Object<uunit_t>::getBoundingRect(rect);

		unsigned test = abs((int)(rotation - rotation / 360));
		if (test != 0 && test != 180) {
			if (test == 90) {
				unit_t temp;

				temp = rect.left;
				rect.left = rect.top;
				rect.top = temp;

				temp = rect.right;
				rect.right = rect.bottom;
				rect.bottom = temp;

				return;
			}
			const double c = cos(rotation);

			point_t<float> center;
			getRectCenter(rect, center);

			rect.left  	= (unit_t)round((rect.left   - center.x) * c + center.x);
			rect.right 	= (unit_t)round((rect.right  - center.x) * c + center.x);
			rect.top   	= (unit_t)round((rect.top    - center.y) * c + center.y);
			rect.bottom = (unit_t)round((rect.bottom - center.y) * c + center.y);
		}
	}

#endif /* ifdef GRAPHICS_FULL */

	virtual ~Sprite() {
		/* Nothing to do */
	}
	virtual void render(Viewport<uunit_t, TCarrier> &viewport,
			const rect_t<unit_t> &rect, const point_t<unit_t> &origin) const = 0;
};


} /* namespace _2d */

#endif /* SOURCES_GRAPHICS_SPRITE_H_ */
