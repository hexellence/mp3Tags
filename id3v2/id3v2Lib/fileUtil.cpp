#include <string>
#include <fstream>
#include "fileUtil.h"
#include "Id3v2Tag.h"


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


int getAudioOffset(std::ifstream& mp3File, int size)
{
	int offset = 0;

	if (mp3File.is_open()) 
	{
		uint8_t bytes[2]{};

		while (((bytes[0] != mp3AudioHeader[0]) || (bytes[1] != mp3AudioHeader[1])) && (offset < size)) {
			mp3File.read((char*)&bytes[0], 1);	//only read header first for effectiveness
			offset++;
			if (bytes[0] == mp3AudioHeader[0]) {
				mp3File.read((char*)&bytes[1], 1);	//only read header first for effectiveness				
				offset++;
			}
		}
	}
	if (offset >= 2) {
		offset = offset - 2;
	}
	return offset;
}

int getAudioOffset(std::filesystem::path filePath)
{
	std::ifstream mp3File;
	int offset = 0;
	
	int fileSize = (int)getFileSize(filePath);
	int Id3v2Tagsize = getID3v2TagSize(filePath);
	
	mp3File.open(filePath, std::ios::binary);
	if (mp3File.is_open())
	{
		offset = Id3v2Tagsize - 1;
		mp3File.seekg(offset, std::ios::beg);

		uint8_t bytes[2]{};

		while (((bytes[0] != mp3AudioHeader[0]) || (bytes[1] != mp3AudioHeader[1])) && (offset < fileSize)) {
			mp3File.read((char*)&bytes[0], 1);	//only read header first for effectiveness
			offset++;
			if (bytes[0] == mp3AudioHeader[0]) {
				mp3File.read((char*)&bytes[1], 1);	//only read header first for effectiveness				
				offset++;
			}
		}
	}
	if (offset >= 2) {
		offset = offset - 2;
	}

	mp3File.close();
	return offset;
}


int getID3v2TagSize(std::filesystem::path filePath)
{
	int tagSize = 0;
	if (isMp3(filePath))
	{
		char* buffer = new char[sizeof(TagHdr)];

		if (readFile(filePath, buffer, sizeof(TagHdr)))
		{
			TagHdr* hdr = (TagHdr*)buffer;
			if (hdr->valid())
			{
				tagSize = hdr->bytes();
			}
		}
		delete[] buffer;
	}
	return tagSize;
}