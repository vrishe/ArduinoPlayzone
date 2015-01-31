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


class exception {
	const int code;
	const char* const msg;

public:
	exception(int code, const char* msg)
		: code(code), msg(msg) {
		/* Nothing to do */
	}


	int getCode() const {
		return code;
	}

	const char *getMessage() const {
		return msg;
	}
};

#endif /* SOURCES_ERROR_H_ */
