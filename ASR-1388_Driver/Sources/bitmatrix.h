/*
 * bitmatrix.h
 *
 *  Created on: 07 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_BITMATRIX_H_
#define SOURCES_BITMATRIX_H_


#include "std_template.h"

template<typename TCarrier, bool isNum = std::is_integral<TCarrier>::value>
class basic_bitmatrix;


#include <stdint.h>
#include <string.h>

#include "cstdlib_ext.h"


#define STRIDE(bits, type) ((((((bits) + 7) & ~7) >> 3) + (sizeof(type) - 1)) / sizeof(type))

template<typename TCarrier>
class basic_bitmatrix<TCarrier, true> {
	TCarrier *data;

	uint8_t bitDepth;

	size_t			dimensionsCount;
	const uint32_t *dimensions;

	void initStorage(const uint32_t *dimensions, size_t dimensionsCount, uint8_t bitDepth) {
		size_t volume = 1;

		for (int i = 0; i < dimensionsCount; ++i) {
			volume *= dimensions[i];
		}
		volume = STRIDE(volume * bitDepth, TCarrier);

		this->data = new TCarrier[volume];
		this->bitDepth = bitDepth;
		this->dimensions = dimensions;
		this->dimensionsCount = dimensionsCount;

		memset(reinterpret_cast<void *>(this->data), 0x00, volume * sizeof(TCarrier));
	}

public:

	typedef TCarrier carrier_t;

	basic_bitmatrix(const uint32_t *dimensions, size_t dimensionsCount, uint8_t bitDepth) {
		initStorage(dimensions, dimensionsCount, bitDepth);
	}

	template<size_t dimensionsCount>
	basic_bitmatrix(const uint32_t (&dimensions)[dimensionsCount], uint8_t bitDepth) {
		initStorage(dimensions, dimensionsCount, bitDepth);
	}

	virtual ~basic_bitmatrix() {
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

typedef basic_bitmatrix<uint8_t> bitmatrix;

#endif /* SOURCES_BITMATRIX_H_ */
