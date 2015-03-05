#pragma once

#include <stdint.h>


namespace audio {

union label_t {
	int8_t		letters[4];
	uint32_t	value;
};

#pragma pack(push, 1)
struct wav_header_chunk {
	// Title
	label_t  chunkId;
	uint32_t chunkSize;
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

struct wav_data_chunk {
	// Sub chunk DATA
	label_t  subchunk2Id;
	uint32_t subchunk2Size;

	/* Read the audio data below */
};
#pragma pack(pop)

} /* namespace audio */
