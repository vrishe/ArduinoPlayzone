#pragma once

#include <stdint.h>


namespace audio {

union label_t {
	int8_t		letters[4];
	uint32_t	value;
};


#pragma pack(push, 1)

struct wav_chunk {
	label_t  chunkId;
	uint32_t chunkSize;
};

struct wav_header_chunk : public wav_chunk {
	// Title
	label_t  format;

	// Sub chunk WAV
	label_t  subchunk1Id;
	uint32_t subchunk1Size;
	uint16_t audioFormat;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
};

struct wav_list_chunk : public wav_chunk {
	// Title
	label_t	 listTypeId;

	/* Read the list sub-chunk data below */
};

struct wav_label_chunk : public wav_chunk {
	uint32_t cuePointId;
	int8_t *data; // Some ASCII text, null-terminated and padded
};

#pragma pack(pop)

} /* namespace audio */
