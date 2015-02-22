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

#ifndef ASR_TEST
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


typedef char(&_positive)[1];
typedef char(&_negative)[2];

template<typename B, typename D>
struct is_base_of {
	template<typename T>
	static _positive check(D*, T);
	static _negative check(B*, int);

	struct _host {
		operator B*() const;
		operator D*();
	};
	static const bool value = (sizeof(check(_host(), 0)) == sizeof(_positive));
};

#endif /* ifndef ASR_TEST */


template <typename TInt>
struct _is_integral_unsigned_base : false_type {
	/* not integral type */
};

template <>
struct _is_integral_unsigned_base <unsigned char> : true_type {};

template <>
struct _is_integral_unsigned_base <unsigned short> : true_type {};

template <>
struct _is_integral_unsigned_base <unsigned int> : true_type {};

template <>
struct _is_integral_unsigned_base <unsigned long> : true_type {};

template <>
struct _is_integral_unsigned_base <unsigned long long> : true_type {};

template <typename TInt>
struct is_integral_unsigned : _is_integral_unsigned_base<typename remove_cv<TInt>::type> {
	/* Determines whether TInt is of unsigned integral type */
};


template <typename TInt, bool isIntegral>
struct _integral_signed;

template <typename TInt>
struct _integral_signed<TInt, true> {
	typedef TInt type;
};

template <>
struct _integral_signed<bool, true>;

template <>
struct _integral_signed<unsigned char, true> {
	typedef signed char type;
};

template <>
struct _integral_signed<unsigned short, true> {
	typedef signed short type;
};

template <>
struct _integral_signed<unsigned int, true> {
	typedef signed int type;
};

template <>
struct _integral_signed<unsigned long, true> {
	typedef signed long type;
};

template <>
struct _integral_signed<unsigned long long, true> {
	typedef signed long long type;
};

template <typename TInt>
struct integral_signed : _integral_signed<TInt, is_integral<TInt>::value> {
	/* Removes 'unsigned' quality from an integral type. */
};


} /* namespace std */

#endif /* SOURCES_TYPE_TRAITS_EXT_H_ */
