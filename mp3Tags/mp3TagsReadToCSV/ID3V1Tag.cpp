#include <fstream>
#include "ID3V1Tag.h"


bool getID3v1Header(std::filesystem::path filePath, ID3V1HDR* hdr) {
	bool retVal = false;
	std::ifstream mp3File;
	mp3File.open(filePath, std::ios::binary);
	if (mp3File.is_open()) {
		
		mp3File.seekg(ID3V1_TAG_OFFSET, std::ios::end);
		mp3File.read((char*)hdr, sizeof(ID3V1HDR));	//only read header first for effectiveness		
		retVal = true;
		mp3File.close();
	}
	else {
		retVal = false;
	}
	return retVal;
}