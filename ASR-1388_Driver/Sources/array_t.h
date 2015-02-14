/*
* bitmatrix.h
*
*  Created on: 07 февр. 2015 г.
*      Author: Admin
*/

#ifndef SOURCES_ARRAY_T_H_
#define SOURCES_ARRAY_T_H_


#include <stdint.h>

template<typename TCarrier, uint8_t dims, bool isIntegral>
class array_trait;


#include <string.h>

#define NO_DIMENSION		uint32_t(-1)
#define NO_INDEX			size_t  (-1)

#define ELEMENT_SIZE(bits, type)  \
	(((((bits) + 7) >> 3) + (sizeof(type) - 1)) / sizeof(type))

template<typename TCarrier, uint8_t dims>
class array_base {

protected:

	uint32_t  dimensions[dims];
	uint8_t   elementSize;
	TCarrier *data;


	array_base() {
		this->elementSize = 0;
		this->data = NULL;
	}
	array_base(const array_base &source) {
		this->data = source.data;
		this->elementSize = source.elementSize;

		memcpy(this->dimensions, dimensions, sizeof(dimensions));
	}


	void getValueAt(size_t valueIndex, TCarrier *dataOut) const {
		if (valueIndex != NO_INDEX) {
			memcpy(dataOut, data + valueIndex, elementSize * sizeof(TCarrier));
		}
	}


	void setValueAt(size_t valueIndex, const TCarrier *dataIn) {
		if (valueIndex != NO_INDEX) {
			memcpy(data + valueIndex, dataIn, elementSize * sizeof(TCarrier));
		}
	}


public:

	typedef TCarrier carrier_t;


	virtual ~array_base() = 0;


	uint8_t getElementSize() const {
		return elementSize;
	}

	uint32_t getDimensionSize(uint8_t index) const {
		return index < dims ? dimensions[index] : NO_DIMENSION;
	}

	virtual size_t getValueIndex(const uint32_t(&coords)[dims]) const {
		size_t index = 0;
		{
			size_t offset = elementSize;

			for (size_t i = 1; i < dims; ++i) {
				if (coords[i] >= dimensions[i]) {
					return NO_INDEX;
				}
				index += coords[i] * offset;
				offset *= dimensions[i];
			}
		}
		return index;
	}


	TCarrier *getValueAt(size_t valueIndex) {
		return data + valueIndex;
	}
	TCarrier *getValueAt(const uint32_t(&coords)[dims]) {
		return getValueAt(getValueIndex(coords));
	}
	void getValueAt(const uint32_t(&coords)[dims], TCarrier *dataOut) const {
		getValueAt(getValueIndex(coords), dataOut);
	}


	void setValueAt(const uint32_t(&coords)[dims], const TCarrier *dataIn) {
		setValueAt(getValueIndex(coords), dataIn);
	}
};


template<typename TCarrier, uint8_t dims>
class array_trait<TCarrier, dims, true> : public array_base<TCarrier, dims> {

	void initialize(const uint32_t(&dimensions)[dims], uint8_t elementSize) {
		size_t volume = elementSize * dimensions[0];

		for (size_t i = 1; i < dims; ++i) {
			volume *= dimensions[i];
		}
		this->elementSize = elementSize;

		memset(this->data = new TCarrier[volume], 0x00, volume * sizeof(TCarrier));
		memcpy(this->dimensions, dimensions, sizeof(dimensions));
	}


protected:

	array_trait(const uint32_t(&dimensions)[dims], uint8_t elementSize) {
		initialize(dimensions, elementSize);
	}


public:

	virtual ~array_trait() {
		/* Nothing to do */
	}
};

template<typename TCarrier>
class array_trait <TCarrier, 2, true> : public array_base<TCarrier, 2> {

	void initialize(uint32_t rows, uint32_t cols, uint8_t elementSize) {
		size_t volume = elementSize * this->dimensions[0] * this->dimensions[1];

		this->dimensions[0] = cols;
		this->dimensions[1] = rows;
		this->elementSize = elementSize;

		memset(this->data = new TCarrier[volume], 0x00, volume * sizeof(TCarrier));
	}


protected:

	array_trait(const uint32_t(&dimensions)[2], uint8_t elementSize) {
		initialize(dimensions[1], dimensions[0], elementSize);
	}
	array_trait(uint32_t rows, uint32_t cols, uint8_t elementSize) {
		initialize(rows, cols, elementSize);
	}


public:

	virtual ~array_trait() {
		/* Nothing to do */
	}


	uint32_t getRows() const {
		return this->dimensions[1];
	}
	uint32_t getCols() const {
		return this->dimensions[0];
	}


	size_t getValueIndex(uint32_t r, uint32_t c) const {
		return c < this->dimensions[0] && r < this->dimensions[1] ?
			r * this->dimensions[0] + c * this->elementSize : NO_INDEX;
	}
	virtual size_t getValueIndex(const uint32_t(&coords)[2]) const {
		return getValueIndex(coords[1], coords[0]);
	}


	TCarrier *getValueAt(size_t valueIndex) {
		return array_base<TCarrier, 2>::getValueAt(valueIndex);
	}
	TCarrier *getValueAt(const uint32_t(&coords)[2]) {
		return array_base<TCarrier, 2>::getValueAt(coords);
	}
	TCarrier *getValueAt(uint32_t r, uint32_t c) {
		return array_base<TCarrier, 2>::getValueAt(getValueIndex(r, c));
	}
	TCarrier *operator()(uint32_t r, uint32_t c) {
		return array_base<TCarrier, 2>::getValueAt(getValueIndex(r, c));
	}
	void getValueAt(const uint32_t(&coords)[2], TCarrier *dataOut) const {
		array_base<TCarrier, 2>::getValueAt(coords, dataOut);
	}


	void setValueAt(const uint32_t(&coords)[2], const TCarrier *dataIn) {
		array_base<TCarrier, 2>::setValueAt(coords, dataIn);
	}
	void setValueAt(uint32_t r, uint32_t c, const TCarrier *dataIn) {
		array_base<TCarrier, 2>::setValueAt(getValueIndex(r, c), dataIn);
	}
};

template<typename TCarrier>
class array_trait <TCarrier, 1, true> : public array_base<TCarrier, 1>{

	void initialize(uint32_t length, uint8_t elementSize) {
		size_t volume = elementSize * this->dimensions[0];

		this->dimensions[0] = length;
		this->elementSize = elementSize;

		memset(this->data = new TCarrier[volume], 0x00, volume * sizeof(TCarrier));
	}


protected:

	array_trait(const uint32_t(&dimensions)[1], uint8_t elementSize) {
		initialize(dimensions[0], elementSize);
	}
	array_trait(uint32_t length, uint8_t elementSize) {
		initialize(length, elementSize);
	}


public:

	virtual ~array_trait() {
		/* Nothing to do */
	}


	uint32_t getLength() const {
		return this->dimensions[0];
	}


	size_t getValueIndex(uint32_t i) const {
		return i < this->dimensions[0] ? i * this->elementSize : NO_INDEX;
	}
	virtual size_t getValueIndex(const uint32_t(&coords)[1]) const {
		return getValueIndex(coords[0]);
	}


	TCarrier *operator[](uint32_t i) {
		return this->getValueAt(getValueIndex(i));
	}


	void setValueAt(const uint32_t(&coords)[1], const TCarrier *dataIn) {
		array_base<TCarrier, 2>::setValueAt(coords, dataIn);
	}
	void setValueAt(uint32_t i, const TCarrier *dataIn) {
		this->array_base<TCarrier, 1>::setValueAt(getValueIndex(i), dataIn);
	}
};

template<typename TCarrier>
class array_trait <TCarrier, 0, true>;


#include "type_traits_ext.h"


template<typename TCarrier, uint8_t dims>
class array_t : public array_trait<
	typename std::remove_cv<TCarrier>::type, dims,
		std::is_integral<TCarrier>::value> {

public:

	array_t(const uint32_t (&dimensions) [dims], uint8_t elementSize)
		: array_trait <typename std::remove_cv<TCarrier>::type, dims,
			std::is_integral<TCarrier>::value>(dimensions, elementSize) {
		/* Nothing to do */
	}

};

template<typename TCarrier>
class matrix_t : public array_t<TCarrier, 2> {

public:

	matrix_t(const uint32_t(&dimensions)[2], uint8_t elementSize)
		: array_t<TCarrier, 2>(dimensions, elementSize) {
		/* Nothing to do */
	}
	matrix_t(uint32_t rows, uint32_t cols, uint8_t elementSize)
		: array_t <TCarrier, 2>(rows, cols, elementSize) {
		/* Nothing to do */
	}
};

template<typename TCarrier>
class vector_t : public array_t<TCarrier, 1> {

public:

	vector_t(const uint32_t(&dimensions)[1], uint8_t elementSize)
		: array_t<TCarrier, 1>(dimensions, elementSize) {
		/* Nothing to do */
	}
	vector_t(uint32_t length, uint8_t elementSize)
		: array_t <TCarrier, 1>(length, elementSize) {
		/* Nothing to do */
	}
};

#endif /* SOURCES_ARRAY_T_H_ */

