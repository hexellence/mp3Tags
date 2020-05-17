#include <fstream>
#include "ID3V1Tag.h"
#include "filesys.h"


bool getID3v1Header(std::filesystem::path filePath, ID3V1HDR* hdr) 
{
	bool retVal = readFile(filePath, (char*)hdr, ID3V1_TAG_OFFSET);
	
	return retVal;
}

bool isID3v1(std::filesystem::path filePath)
{
	char tag[4]{};
	bool retVal = false;
	std::ifstream mp3File;
	mp3File.open(filePath, std::ios::binary);
	if (mp3File.is_open()) {

		mp3File.seekg(ID3V1_TAG_OFFSET, std::ios::end);
		mp3File.read(tag, 3);	//only read the tag info
		mp3File.close();
		if ((tag[0] == 'T') && (tag[1] == 'A') && (tag[2] == 'G'))
		{
			retVal = true;
		}
		
	}
	else {
		retVal = false;
	}
	return retVal;

}