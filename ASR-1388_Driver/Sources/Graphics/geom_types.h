/*
 * geom_types.h
 *
 *  Created on: 15 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_GRAPHICS_GEOM_TYPES_H_
#define SOURCES_GRAPHICS_GEOM_TYPES_H_


#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif


#include <cmath>

#ifndef ASR_TEST

inline double round(double x) {
	if (x != 0) {
		return x > 0 ? floor(x + .5) : ceil(x - .5);
	}
	return 0;
}

inline float round(float x) {
	if (x != 0) {
		return x > 0 ? floor(x + .5f) : ceil(x - .5f);
	}
	return 0;
}

#endif /* ifndef ASR_TEST */


template <typename TUnit>
TUnit minmax(TUnit a, TUnit b, TUnit &min, TUnit &max) {
	if (a <= b) {
		min = a;
		max = b;

		return a;
	}
	min = b;
	max = a;

	return b;
}


namespace _2d {


template <typename TUnit>
struct point_t {

	typedef TUnit unit_t;

	TUnit x;
	TUnit y;
};

template <typename TUnit>
struct rect_t {

	typedef TUnit unit_t;

	TUnit left;
	TUnit top;
	TUnit right;
	TUnit bottom;

	operator point_t<TUnit>() const {
		point_t<TUnit> pt;

		pt.x = left;
		pt.y = top;

		return pt;
	}
};


template <typename TUnit>
void offset(point_t<TUnit> &rc, TUnit dx, TUnit dy) {
	rc.x += dx;
	rc.y += dx;
}
template <typename TUnit>
void offset(rect_t<TUnit> &rc, TUnit dx, TUnit dy) {
	rc.left 	+= dx;
	rc.right 	+= dx;
	rc.top 		+= dy;
	rc.bottom 	+= dy;
}

template <typename TUnit>
bool intersect(const rect_t<TUnit> &a, const rect_t<TUnit> &b, rect_t<TUnit> &intersection) {
	TUnit la, lb, ra, rb, ta, tb, ba, bb;

	minmax(a.left, a.right, la, ra);
	minmax(b.left, b.right, lb, rb);
	minmax(a.top, a.bottom, ta, ba);
	minmax(b.top, b.bottom, tb, bb);

	intersection.left   = max(la, lb);
	intersection.top    = max(ta, tb);
	intersection.right  = min(ra, rb);
	intersection.bottom = min(ba, bb);

	return (intersection.left < intersection.right
			&& intersection.top < intersection.bottom);
}

template <typename TUnitRect, typename TUnitPoint>
void getRectCenter(const rect_t<TUnitRect> &r, point_t<TUnitPoint> &c) {
	c.x = (r.left + r.right) / TUnitPoint(2);
	c.y = (r.top + r.bottom) / TUnitPoint(2);
}


} /* namespace _2d */

#endif /* SOURCES_GRAPHICS_GEOM_TYPES_H_ */
