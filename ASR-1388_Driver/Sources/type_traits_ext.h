/*
 * std_template.h
 *
 *  Created on: 08 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_TYPE_TRAITS_EXT_H_
#define SOURCES_TYPE_TRAITS_EXT_H_


#include <type_traits>


namespace std {

template <bool>
struct predicate_base : false_type {
	/* Negative predicate */
};

template <>
struct predicate_base<true> : true_type {
	/* Positive predicate */
};


template <typename TInt>
struct _is_integral_base : false_type {
	/* not integral type */
};

template <>
struct _is_integral_base <bool> : true_type {};

template <>
struct _is_integral_base <signed char> : true_type {};
template <>
struct _is_integral_base <unsigned char> : true_type {};

template <>
struct _is_integral_base <signed short> : true_type {};
template <>
struct _is_integral_base <unsigned short> : true_type {};

template <>
struct _is_integral_base <signed int> : true_type {};
template <>
struct _is_integral_base <unsigned int> : true_type {};

template <>
struct _is_integral_base <signed long> : true_type {};
template <>
struct _is_integral_base <unsigned long> : true_type {};

template <>
struct _is_integral_base <signed long long> : true_type {};
template <>
struct _is_integral_base <unsigned long long> : true_type {};

template <typename TInt>
struct is_integral : _is_integral_base<typename remove_cv<TInt>::type> {
	/* Determines whether TInt is of integral type */
};

} /* namespace std */

#endif /* SOURCES_TYPE_TRAITS_EXT_H_ */
