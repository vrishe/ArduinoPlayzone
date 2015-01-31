/*
 * Main.h
 *
 *  Created on: 31 џэт. 2015 у.
 *      Author: Admin
 */

#ifndef ASR_1388_DRIVER_SOURCES_MAIN_H_
#define ASR_1388_DRIVER_SOURCES_MAIN_H_

#include <Arduino.h>
#include <stddef.h>


template<typename T, size_t S>
char (*countof_func(T (&)[S])) [S];

#define _countof(array) (sizeof(*countof_func(array)) + 0)


int main(void);

#endif /* ASR_1388_DRIVER_SOURCES_MAIN_H_ */
