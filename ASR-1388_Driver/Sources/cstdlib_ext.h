/*
 * cstdlib_ext.h
 *
 *  Created on: 07 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_CSTDLIB_EXT_H_
#define SOURCES_CSTDLIB_EXT_H_


/* _countof helper */
#if !defined (_countof)
#if !defined (__cplusplus)
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#else  /* !defined (__cplusplus) */
extern "C++"
{
	template<typename T, size_t S>
	char (*countof_func(T (&)[S])) [S];

	#define _countof(array) (sizeof(*countof_func(array)) + 0)
}
#endif  /* !defined (__cplusplus) */
#endif  /* !defined (_countof) */

#define bitsizeof(type) (sizeof(type) << 3)

#endif /* SOURCES_CSTDLIB_EXT_H_ */
