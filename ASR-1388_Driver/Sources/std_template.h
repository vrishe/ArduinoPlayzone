/*
 * std_template.h
 *
 *  Created on: 08 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_STD_TEMPLATE_H_
#define SOURCES_STD_TEMPLATE_H_


namespace std {

template<class _Ty>
struct remove_const
{
	typedef _Ty type;
};

template<class _Ty>
struct remove_const<const _Ty>
{
	typedef _Ty type;
};

template<class _Ty>
struct remove_const<const _Ty[]>
{
	typedef _Ty type[];
};

template<class _Ty, unsigned int _Nx>
struct remove_const<const _Ty[_Nx]>
{
	typedef _Ty type[_Nx];
};


template<class _Ty>
struct remove_volatile
{
	typedef _Ty type;
};

template<class _Ty>
struct remove_volatile<volatile _Ty>
{
	typedef _Ty type;
};

template<class _Ty>
struct remove_volatile<volatile _Ty[]>
{
	typedef _Ty type[];
};

template<class _Ty, unsigned int _Nx>
struct remove_volatile<volatile _Ty[_Nx]>
{
	typedef _Ty type[_Nx];
};


template<class _Ty>
struct remove_cv
{
	typedef typename remove_const<
			typename remove_volatile<_Ty>::type
			>::type type;
};


template<typename TValue, typename TValue val>
struct integral_constant {
	typedef TValue type;

	static const TValue value = val;
};

typedef integral_constant<bool, true>  true_type;
typedef integral_constant<bool, false> false_type;


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

#endif /* SOURCES_STD_TEMPLATE_H_ */
