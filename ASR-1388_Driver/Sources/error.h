/*
 * error.h
 *
 *  Created on: 31 џэт. 2015 у.
 *      Author: Admin
 */

#ifndef SOURCES_ERROR_H_
#define SOURCES_ERROR_H_


#define MAKE_ERROR_CODE(facility, error) \
	(((int)((short)(facility)) << (sizeof(int) / 2)) | ((short)(error)))


#endif /* SOURCES_ERROR_H_ */
