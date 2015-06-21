/*
 * ASRFontASCII.h
 *
 *  Created on: 28 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_ASRFONT_H_
#define SOURCES_ASRFONT_H_


#include "ASRTextScroller.h"

#include <cstdlib>

namespace asr {


class FontASCII : public TextScrollerBase::Font {

	cpunit_t storage[26];

	FontASCII();
	FontASCII(const FontASCII &);


public:

	static const FontASCII Instance;


	virtual cpsize_t getMaxCodePoints() const {
		return _countof(storage);
	}
	virtual cpoffset_t getCodePointOffset() const {
		return 'A';
	}


	const cpunit_t operator[] (size_t index) const {
		return FontASCII::storage[index];
	}
	virtual const cpunit_t *operator* () const {
		return storage;
	}
};


} /* namespace asr */

#endif /* SOURCES_ASRFONT_H_ */
