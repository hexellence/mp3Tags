
#include "fileSys.h"
#include "hxlstr.h"

using namespace hxl;

bool isMp3(std::filesystem::path filePath) {

	hxlstr input_file_ext = filePath.extension().string().c_str();
	bool retVal = (input_file_ext == ".mp3") || (input_file_ext == ".MP3") || (input_file_ext == ".Mp3");
	return retVal;
}

bool isWma(std::filesystem::path filePath) {

	hxlstr input_file_ext = filePath.extension().string().c_str();
	bool retVal = (input_file_ext == ".wma") || (input_file_ext == ".WMA") || (input_file_ext == ".Wma");
	return retVal;
}

void convertEndianness(uint8_t* inout, int size) {

	int loopCount = size / 2;
	for (int i = 0; i < loopCount; ++i) {
		uint8_t temp = inout[size - 1 - i];
		inout[size - 1 - i] = inout[i];
		inout[i] = temp;
	}	
}