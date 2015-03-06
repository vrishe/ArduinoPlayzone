// Experimental.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cstdlib>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>


#if defined(UNICODE) || defined(_UNICODE)
#define _tstring	wstring
#define _tcout		wcout
#else
#define _tstring	string
#define _tcout		cout
#endif // defined(UNICODE) || defined(_UNICODE)


#include "audio_wav.h"

#define WRITE_HEADER_FIELD1(HEADER, FIELD)		\
	std::cout << std::setw(20) << std::left << #FIELD ":  " << HEADER.FIELD << std::endl

#define WRITE_HEADER_FIELD2(HEADER, FIELD)					\
	std::cout << std::setw(20) << std::left << #FIELD ":  "	\
		<< HEADER.FIELD.letters[0]							\
		<< HEADER.FIELD.letters[1]							\
		<< HEADER.FIELD.letters[2]							\
		<< HEADER.FIELD.letters[3] << std::endl

#define LEAVE_WITH_ERROR(errno)		\
		{							\
			_set_errno(errno);		\
			goto EXECUTION_SINK;	\
		}


static errno_t processFile(const std::_tstring &path, bool verbose) {
	FILE *input;

	const TCHAR *ptrPath = path.c_str();
	if (_tfopen_s(&input, ptrPath, _T("rb")) == 0) {
		bool output = false;

		audio::label_t referenceHeader = { 'R', 'I', 'F', 'F' };
		audio::label_t referenceData = { 'd', 'a', 't', 'a' };

		audio::wav_header_chunk chunkHeader;
		audio::wav_data_chunk	chunkData;

		if (fread_s(&chunkHeader, sizeof(chunkHeader), sizeof(chunkHeader), 1, input) < 1) {
			LEAVE_WITH_ERROR(EFAULT);
		}
		if (chunkHeader.chunkId.value != referenceHeader.value) {
			LEAVE_WITH_ERROR(EBADF);
		}
		fseek(input, chunkHeader.subchunk1Size - 16, SEEK_CUR);

		if (fread_s(&chunkData, sizeof(chunkData), sizeof(chunkData), 1, input) < 1) {
			LEAVE_WITH_ERROR(EFAULT);
		}
		size_t curPosition = ftell(input);

		if (chunkData.subchunk2Id.value != referenceData.value
			|| fseek(input, 0, SEEK_END) - curPosition < chunkData.subchunk2Size) {
			LEAVE_WITH_ERROR(EFAULT);
		}
		fseek(input, curPosition, SEEK_SET);

		// Write file's name and location.
		LPTSTR filePath = new TCHAR[MAX_PATH + 1];
		{
			filePath[MAX_PATH] = 0x00;

			GetFullPathName(ptrPath, MAX_PATH, filePath, NULL);
			std::_tcout << "\nFile name: \n" << std::endl 
				<< " " << filePath << "\n\n";
		}
		delete[] filePath;

		// Write WAV header chunk.
		WRITE_HEADER_FIELD2(chunkHeader, chunkId);
		WRITE_HEADER_FIELD1(chunkHeader, chunkSize);
		WRITE_HEADER_FIELD2(chunkHeader, format);
		WRITE_HEADER_FIELD2(chunkHeader, subchunk1Id);
		WRITE_HEADER_FIELD1(chunkHeader, subchunk1Size);
		WRITE_HEADER_FIELD1(chunkHeader, audioFormat);
		WRITE_HEADER_FIELD1(chunkHeader, numChannels);
		WRITE_HEADER_FIELD1(chunkHeader, sampleRate);
		WRITE_HEADER_FIELD1(chunkHeader, byteRate);
		WRITE_HEADER_FIELD1(chunkHeader, blockAlign);
		WRITE_HEADER_FIELD1(chunkHeader, bitsPerSample);
		WRITE_HEADER_FIELD2(chunkData, subchunk2Id);
		WRITE_HEADER_FIELD1(chunkData, subchunk2Size);
		
		if (verbose) {
			size_t bufferLength = min(chunkData.subchunk2Size, 32767);
			uint8_t *buffer = new uint8_t[bufferLength];
			{
				std::stringstream ss;
				size_t readLength, readTotal = 0;

				std::cout << std::endl;
				std::cout << "Data:" << std::endl;

				while (!feof(input)) {
					readLength = fread_s(buffer, bufferLength, sizeof(uint8_t), bufferLength, input);
					for (size_t i = 0; i < readLength; ++i) {
						ss << std::setfill('0') << std::setw(2) << std::hex << (int)buffer[i] << " ";

						++readTotal;
						if (chunkHeader.blockAlign > 1) {
							if (readTotal % chunkHeader.blockAlign == 0) {
								ss << "  ";
							}
						}
						if (readTotal % 16 == 0) {
							ss << std::endl;

							if (readTotal % 800 == 0) {
								std::cout << ss.str();

								ss.str("");
								ss.clear();
							}
						}
					}
				}
				if (ss.tellp() > 0) {
					std::cout << ss.str();
				}
			}
			delete[] buffer;
		} 
		std::cout << std::endl;
		_set_errno(0);

EXECUTION_SINK:
		fclose(input);
	}
	return errno;
}


#define PARAM_BOUND		0
#define PARAM_UNBOUND	1
typedef std::vector<std::_tstring>(command_line)[2];

static inline void storeValue(std::vector<std::_tstring> &v, const std::_tstring &value) {
	v.reserve(v.size() + 4);
	v.emplace_back(value);
}

static void argvRead(int argc, _TCHAR *argv[], command_line &output) {
	while (--argc > 0) {
		_TCHAR *argument = argv[argc];

		size_t argLength;
		if (argument != NULL && (argLength = _tcslen(argument)) > 0 && --argLength > 0) {
			_TCHAR marker = *argument;

			if (marker == '-' || marker == '/') {
				std::_tstring boundParameter(argument);
				{
					boundParameter[0] = marker == '-' ? 'd' : 's';
				}
				storeValue(output[PARAM_BOUND], boundParameter);
			}
			else
			{
				storeValue(output[PARAM_UNBOUND], argument);
			}
		}
	}
}


typedef std::vector<std::_tstring>::const_iterator	cl_iterator;

static bool containsKey(command_line &cl, const TCHAR *key, std::_tstring *keyOut = NULL) {
	if (key != NULL) {
		size_t offset = 0;

		while (*key == '*')
		{
			++key;
			++offset;
		}
		size_t length = _tcslen(key);

		std::vector<std::_tstring> &keys = cl[PARAM_BOUND];
		for (cl_iterator cur = keys.begin(), end = keys.end(); cur != end; ++cur) {
			if (_tcsncmp(&cur->at(offset), key, length) == 0) {
				if (keyOut != NULL) {
					*keyOut = *cur;
				}
				return true;
			}
		}
	}
	return false;
}

int _tmain(int argc, _TCHAR* argv[]) {

	command_line commandLine;
	{
		argvRead(argc, argv, commandLine);
	}
	int result = 0;

	std::vector<std::_tstring> &paramUnbound = commandLine[PARAM_UNBOUND];
	if (!paramUnbound.empty()) {
		bool verbose = containsKey(commandLine, _T("*v"));

		errno_t error;
		if ((error = processFile(*paramUnbound.begin(), verbose)) != 0) {
			switch (error) {
			case EFAULT:
				std::cout << "File contents is corrupted or does not conform RIFF container format.\n";
				result = -2;
				break;

			case EBADF:
				std::cout << "File is not WAV-compliant.\n";
				result = -3;
				break;

			default:
				std::cout << "Can't access WAV data. Check the file exists and is available.\n";
				result = -1;
			}
		}
	} else {
		goto COMMAND_DIGEST;
	}
	return result;

COMMAND_DIGEST:
	std::cout << "\nwavdump [-v] <\\path\\to\\compliant_RIFF_file.wav>\n\n" 
		<< "    -v - enables verbose mode (include hexdump of data chunk)."<< std::endl << std::endl;
	return 0;
}
