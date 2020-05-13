
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