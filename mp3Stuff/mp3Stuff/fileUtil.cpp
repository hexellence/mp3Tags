#include <string>
#include "fileUtil.h"


bool isMp3(std::filesystem::path filePath) {

	std::string input_file_ext = filePath.extension().string();
	bool retVal = (input_file_ext == ".mp3") || (input_file_ext == ".MP3") || (input_file_ext == ".Mp3");
	return retVal;
}

bool isWma(std::filesystem::path filePath) {

	std::string input_file_ext = filePath.extension().string();
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

int getFileSize(std::filesystem::path filename)
{
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	return (int)in.tellg();
}

bool readFile(std::filesystem::path filePath, char* data, int startOffset, int endOffset) {
	bool retVal = false;
	std::ifstream file;

	if ((data != nullptr) && (startOffset < endOffset))
	{
		if (filePath.has_filename())
		{
			int fileSize = getFileSize(filePath);
			if (endOffset - startOffset <= fileSize)
			{
				file.open(filePath, std::ios::binary);
				if (file.is_open()) {
					file.seekg(startOffset, std::ios::beg);
					file.read(data, endOffset - startOffset);
					retVal = true;
					file.close();
				}
			}
		}
	}
	return retVal;
}


bool readFile(std::filesystem::path filePath, char* data, int sizeDirectionToRead) {
	bool retVal = false;
	std::ifstream file;

	if (data != nullptr)
	{
		if (filePath.has_filename())
		{
			file.open(filePath, std::ios::binary);
			if (file.is_open()) {

				std::ios_base::seekdir dir = std::ios::beg;
				if (sizeDirectionToRead < 0)
				{
					dir = std::ios::end;
					//use it as offset to read until the end 
					file.seekg(sizeDirectionToRead, dir);
				}
				else
				{
					dir = std::ios::beg;
					file.seekg(0, dir);
				}

				file.read((char*)data, abs(sizeDirectionToRead));	//only read header first for effectiveness				
				retVal = true;
				file.close();
			}
		}
	}
	return retVal;
}