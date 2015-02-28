/*
 * cmath_ext.h
 *
 *  Created on: 28 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_CMATH_EXT_H_
#define SOURCES_CMATH_EXT_H_


#include <cmath>

//#ifndef min
//#define min(a, b) ((a) < (b) ? (a) : (b))
//#endif
//
//#ifndef max
//#define max(a, b) ((a) > (b) ? (a) : (b))
//#endif
//
//
//#ifndef ASR_TEST
//
//inline double round(double x) {
//	if (x != 0) {
//		return x > 0 ? floor(x + .5) : ceil(x - .5);
//	}
//	return 0;
//}
//
//inline float round(float x) {
//	if (x != 0) {
//		return x > 0 ? floor(x + .5f) : ceil(x - .5f);
//	}
//	return 0;
//}
//
//#endif /* ifndef ASR_TEST */


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


#endif /* SOURCES_CMATH_EXT_H_ */
