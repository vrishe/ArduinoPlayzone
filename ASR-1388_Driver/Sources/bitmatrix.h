/*
 * bitmatrix.h
 *
 *  Created on: 07 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_BITMATRIX_H_
#define SOURCES_BITMATRIX_H_


template<typename TCarrier, bool>
class bitmatrix_trait;


#include <stdint.h>
#include <string.h>

#include "cstdlib_ext.h"


#define STRIDE(bits, type) ((((((bits) + 7) & ~7) >> 3) + (sizeof(type) - 1)) / sizeof(type))

template<typename TCarrier>
class bitmatrix_trait<TCarrier, true> {
	TCarrier *data;

	uint8_t bitDepth;

	size_t			dimensionsCount;
	const uint32_t *dimensions;

	void initStorage(const uint32_t *dimensions, size_t dimensionsCount, uint8_t bitDepth) {
		size_t volume = 1;

		for (size_t i = 0; i < dimensionsCount; ++i) {
			volume *= dimensions[i];
		}
		volume = STRIDE(volume * bitDepth, TCarrier);

		this->data = new TCarrier[volume];
		this->bitDepth = bitDepth;
		this->dimensions = dimensions;
		this->dimensionsCount = dimensionsCount;

		memset(reinterpret_cast<void *>(this->data), 0x00, volume * sizeof(TCarrier));
	}


protected:

	bitmatrix_trait(const uint32_t *dimensions, size_t dimensionsCount, uint8_t bitDepth) {
		initStorage(dimensions, dimensionsCount, bitDepth);
	}


public:

	typedef TCarrier carrier_type;


	~bitmatrix_trait() {
		delete[] data;
	}


	uint8_t getBitDepth() {
		return bitDepth;
	}

	uint32_t getDimensionsCount() {
		return dimensionsCount;
	}

	uint32_t getDimension(uint32_t index) {
		return index < dimensionsCount ? dimensions[index] : (uint32_t)-1;
	}


	void getValueAt(const uint32_t coords[], TCarrier *dataOut) {
		size_t index = 0;
		{
			size_t offset = 1;
			for (size_t i = 0; i < dimensionsCount; ++i) {
				index += coords[i] * offset;
				offset *= dimensions[i];
			}
			index *= bitDepth;
		}
		size_t shiftRight = bitsizeof(TCarrier) - index % bitsizeof(TCarrier) - bitDepth % bitsizeof(TCarrier);
		size_t shiftLeft = bitsizeof(TCarrier) - shiftRight;

		for (size_t i = 0, j = STRIDE(index, TCarrier),
			imax = STRIDE(bitDepth, TCarrier); i < imax; ++i, ++j) {

			dataOut[i] = data[j] << shiftLeft | data[j + 1] >> shiftRight;
		}
	}

	void setValueAt(const uint32_t coords[], const TCarrier *dataIn) {
		size_t index = 0;
		{
			size_t offset = 1;
			for (size_t i = 0; i < dimensionsCount; ++i) {
				index += coords[i] * offset;
				offset *= dimensions[i];
			}
			index *= bitDepth;
		}
		size_t shiftLeft = bitsizeof(TCarrier) - index % bitsizeof(TCarrier) - bitDepth % bitsizeof(TCarrier);
		size_t shiftRight = bitsizeof(TCarrier) - shiftLeft;

		for (size_t i = 0, j = STRIDE(index, TCarrier),
			imax = STRIDE(bitDepth, TCarrier); i < imax; ++i, ++j) {

			TCarrier value = dataIn[i];

			if (i == 0) {
				value &= ~(~TCarrier(0) << bitDepth % (sizeof(TCarrier) << 3));
			}
			data[j] |= value >> shiftRight;
			data[j + 1] |= value << shiftLeft;
		}
	}
};


#include "type_traits_ext.h"


template<typename TCarrier>
class bitmatrix_base : public bitmatrix_trait<
	typename std::remove_cv<TCarrier>::type,
				std::is_integral<TCarrier>::value> {

public:

	bitmatrix_base(const uint32_t *dimensions, size_t dimensionsCount, uint8_t bitDepth)
		: bitmatrix_trait<typename std::remove_cv<TCarrier>::type,
			std::is_integral<TCarrier>::value>(dimensions, dimensionsCount, bitDepth) {
		/* Nothing to do */
	}

	template<size_t dimensionsCount>
	bitmatrix_base(const uint32_t (&dimensions)[dimensionsCount], uint8_t bitDepth)
		: bitmatrix_trait<typename std::remove_cv<TCarrier>::type,
			std::is_integral<TCarrier>::value>(dimensions, dimensionsCount, bitDepth) {
		/* Nothing to do */
	}
};

typedef bitmatrix_base<uint8_t> bitmatrix;

#endif /* SOURCES_BITMATRIX_H_ */
